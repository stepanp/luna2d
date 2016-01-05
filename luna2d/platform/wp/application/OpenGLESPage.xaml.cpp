
#include "OpenGLESPage.xaml.h"
#include "lunaengine.h"
#include "lunawpfiles.h"
#include "lunawplog.h"
#include "lunawputils.h"
#include "lunawpprefs.h"
#include "lunasizes.h"

using namespace Platform;
using namespace Concurrency;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::System::Threading;
using namespace Windows::UI::ViewManagement;
using namespace luna2d;

OpenGLESPage::OpenGLESPage() :
	mOpenGLES(&gles),
	mRenderSurface(EGL_NO_SURFACE),
	mCustomRenderSurfaceSize(0,0),
	mUseCustomRenderSurfaceSize(false),
	successEvent(nullptr),
	failEvent(nullptr)
{
	InitializeComponent();

	Windows::UI::Core::CoreWindow^ window = Windows::UI::Xaml::Window::Current->CoreWindow;

	// Set fullscreen mode on Windows Phone
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
	StatusBar::GetForCurrentView()->HideAsync(); // Hide status bar
	ApplicationView::GetForCurrentView()->SuppressSystemOverlays = true; // Hide navigation bar
	ApplicationView::GetForCurrentView()->SetDesiredBoundsMode(ApplicationViewBoundsMode::UseCoreWindow); // Disable stretching layout by navigation bar
#endif	

	window->VisibilityChanged +=
		ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::VisibilityChangedEventArgs^>(this, &OpenGLESPage::OnVisibilityChanged);

	swapChainPanel->SizeChanged +=
		ref new Windows::UI::Xaml::SizeChangedEventHandler(this, &OpenGLESPage::OnSwapChainPanelSizeChanged);

	this->Loaded +=
		ref new Windows::UI::Xaml::RoutedEventHandler(this, &OpenGLESPage::OnPageLoaded);

#if !(WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
	// Disable all pointer visual feedback for better performance when touching.
	// This is not supported on Windows Phone applications.
	auto pointerVisualizationSettings = Windows::UI::Input::PointerVisualizationSettings::GetForCurrentView();
	pointerVisualizationSettings->IsContactFeedbackEnabled = false;
	pointerVisualizationSettings->IsBarrelButtonFeedbackEnabled = false;
#endif

	// Register our SwapChainPanel to get independent input pointer events
	auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction ^)
	{
		// The CoreIndependentInputSource will raise pointer events for the specified device types on whichever thread it's created on.
		m_coreInput = swapChainPanel->CreateCoreIndependentInputSource(
			Windows::UI::Core::CoreInputDeviceTypes::Mouse |
			Windows::UI::Core::CoreInputDeviceTypes::Touch |
			Windows::UI::Core::CoreInputDeviceTypes::Pen
			);
		// Register for pointer events, which will be raised on the background thread.
		m_coreInput->PointerPressed += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &OpenGLESPage::OnPointerPressed);
		m_coreInput->PointerMoved += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &OpenGLESPage::OnPointerMoved);
		m_coreInput->PointerReleased += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &OpenGLESPage::OnPointerReleased);
		// Begin processing input messages as they're delivered.
		m_coreInput->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
	});
	// Run task on a dedicated high priority background thread.

	m_inputLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);

	mSwapChainPanelSize = { swapChainPanel->RenderSize.Width, swapChainPanel->RenderSize.Height };
}

OpenGLESPage::~OpenGLESPage()
{
	StopRenderLoop();
	DestroyRenderSurface();
}

void OpenGLESPage::SetDelegates(
		GetNameEvent^ getName,
		IsVideoSupportedEvent^ videoSupported,
		IsVideoReadyEvent^ videoReady,
		ShowVideoEvent^ showVideo,
		RequestRateAppEvent^ rateApp)
{
	getNameEvent = getName;
	videoSupportedEvent = videoSupported;
	videoReadyEvent = videoReady;
	showVideoEvent = showVideo;
	rateAppEvent = rateApp;
}

CallbackEvent^ OpenGLESPage::GetSuccessDelegate()
{
	return successEvent;
}

CallbackEvent^ OpenGLESPage::GetFailDelegate()
{
	return failEvent;
}

void OpenGLESPage::RunInUiThread(ThreadEvent^ handler)
{
	swapChainPanel->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, 
		ref new Windows::UI::Core::DispatchedHandler([handler]() { handler->Invoke(); }));
}

void OpenGLESPage::RunInGameThread(ThreadEvent^ handler)
{
	gameThreadEvents.push(Platform::Agile<ThreadEvent>(handler));
}

void OpenGLESPage::OnPageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// The SwapChainPanel has been created and arranged in the page layout, so EGL can be initialized.
	CreateRenderSurface();
	StartRenderLoop();
}

void OpenGLESPage::OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args)
{
	if (args->Visible && mRenderSurface != EGL_NO_SURFACE)
	{
		StartRenderLoop();
	}
	else
	{
		StopRenderLoop();
	}
}

void OpenGLESPage::OnSwapChainPanelSizeChanged(Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	// Size change events occur outside of the render thread.  A lock is required when updating
	// the swapchainpanel size
	critical_section::scoped_lock lock(mSwapChainPanelSizeCriticalSection);
	mSwapChainPanelSize = { e->NewSize.Width, e->NewSize.Height };
}

void OpenGLESPage::GetSwapChainPanelSize(GLsizei* width, GLsizei* height)
{
	critical_section::scoped_lock lock(mSwapChainPanelSizeCriticalSection);
	// If a custom render surface size is specified, return its size instead of
	// the swapchain panel size.
	if (mUseCustomRenderSurfaceSize)
	{
		*width = static_cast<GLsizei>(mCustomRenderSurfaceSize.Width);
		*height = static_cast<GLsizei>(mCustomRenderSurfaceSize.Height);
	}
	else
	{
		*width = static_cast<GLsizei>(mSwapChainPanelSize.Width);
		*height = static_cast<GLsizei>(mSwapChainPanelSize.Height);
	}
}

void OpenGLESPage::CreateRenderSurface()
{
	if (mOpenGLES)
	{
		//
		// A Custom render surface size can be specified by uncommenting the following lines.
		// The render surface will be automatically scaled to fit the entire window.  Using a
		// smaller sized render surface can result in a performance gain.
		//
		//mCustomRenderSurfaceSize = Size(800, 600);
		//mUseCustomRenderSurfaceSize = true;

		// Create custom render surface with screen size
		// To avoid incorrect swap chain panel size in Windows Phone
		auto dispInfo = Windows::Graphics::Display::DisplayInformation::GetForCurrentView();
		auto bounds = Windows::UI::Xaml::Window::Current->Bounds;
		scaleFactor = (float)dispInfo->RawPixelsPerViewPixel;
		
		mCustomRenderSurfaceSize = Size(bounds.Width * scaleFactor, bounds.Height * scaleFactor);
		mUseCustomRenderSurfaceSize = true;

		mRenderSurface = mOpenGLES->CreateSurface(swapChainPanel, mUseCustomRenderSurfaceSize ? &mCustomRenderSurfaceSize : nullptr);
	}
}

void OpenGLESPage::DestroyRenderSurface()
{
	if (mOpenGLES)
	{
		mOpenGLES->DestroySurface(mRenderSurface);
	}
	mRenderSurface = EGL_NO_SURFACE;
}

void OpenGLESPage::RecoverFromLostDevice()
{
	// Stop the render loop, reset OpenGLES, recreate the render surface
	// and start the render loop again to recover from a lost device.

	StopRenderLoop();

	{
		critical_section::scoped_lock lock(mRenderSurfaceCriticalSection);

		DestroyRenderSurface();
		mOpenGLES->Reset();
		CreateRenderSurface();
	}

	StartRenderLoop();
}

void OpenGLESPage::StartRenderLoop()
{
	// If the render loop is already running then do not start another thread.
	if (mRenderLoopWorker != nullptr && mRenderLoopWorker->Status == Windows::Foundation::AsyncStatus::Started)
	{
		return;
	}

	// Create a task for rendering that will be run on a background thread.
	auto workItemHandler = ref new Windows::System::Threading::WorkItemHandler([this](Windows::Foundation::IAsyncAction ^ action)
	{
		critical_section::scoped_lock lock(mRenderSurfaceCriticalSection);

		mOpenGLES->MakeCurrent(mRenderSurface);

		while(action->Status == Windows::Foundation::AsyncStatus::Started)
		{
			GLsizei panelWidth = 0;
			GLsizei panelHeight = 0;
			GetSwapChainPanelSize(&panelWidth, &panelHeight);

			if(!LUNAEngine::Shared()->IsInitialized())
			{
				LUNAEngine::Shared()->Assemble(new LUNAWpFiles(), new LUNAWpLog(), 
					new LUNAWpUtils(swapChainPanel->Dispatcher), new LUNAWpPrefs());
				LUNAEngine::Shared()->Initialize(panelWidth, panelHeight);

				auto sdk = std::make_shared<LUNAWpAdsProxy>(swapChainPanel->Dispatcher);

				sdk->SetDelegates(getNameEvent, videoSupportedEvent, videoReadyEvent, showVideoEvent);
				successEvent = sdk->GetSuccessDelegate();
				failEvent = sdk->GetFailDelegate();

				LUNAEngine::SharedAds()->SetSdk(sdk);

				auto storeSdk = std::make_shared<LUNAWpStoreProxy>();
				storeSdk->SetDelegates(rateAppEvent);
				LUNAEngine::SharedStore()->SetSdk(storeSdk);
			}
			else 
			{
				ProcessGameThreadEvents();
				ProcessPointers();
				LUNAEngine::Shared()->MainLoop();
			}			

			// The call to eglSwapBuffers might not be successful (i.e. due to Device Lost)
			// If the call fails, then we must reinitialize EGL and the GL resources.
			if (mOpenGLES->SwapBuffers(mRenderSurface) != GL_TRUE)
			{
				// XAML objects like the SwapChainPanel must only be manipulated on the UI thread.
				swapChainPanel->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High, ref new Windows::UI::Core::DispatchedHandler([=]()
				{
					RecoverFromLostDevice();
				}, CallbackContext::Any));

				return;
			}
		}
	});

	if(LUNAEngine::Shared()->IsInitialized()) LUNAEngine::Shared()->OnResume();

	// Run task on a dedicated high priority background thread.
	mRenderLoopWorker = Windows::System::Threading::ThreadPool::RunAsync(workItemHandler, Windows::System::Threading::WorkItemPriority::High, Windows::System::Threading::WorkItemOptions::TimeSliced);
}

void OpenGLESPage::StopRenderLoop()
{
	if (mRenderLoopWorker)
	{
		mRenderLoopWorker->Cancel();
		mRenderLoopWorker = nullptr;
		if(LUNAEngine::Shared()->IsInitialized()) LUNAEngine::Shared()->OnPause();
	}
}

void OpenGLESPage::OnPointerPressed(Object^ sender, PointerEventArgs^ e)
{
	pointers.push(std::make_shared<TouchEvent>(TouchType::TOUCH_DOWN, e));
}

void OpenGLESPage::OnPointerMoved(Object^ sender, PointerEventArgs^ e)
{
	pointers.push(std::make_shared<TouchEvent>(TouchType::TOUCH_MOVED, e));
}

void OpenGLESPage::OnPointerReleased(Object^ sender, PointerEventArgs^ e)
{
	pointers.push(std::make_shared<TouchEvent>(TouchType::TOUCH_UP, e));
}

void OpenGLESPage::ProcessGameThreadEvents()
{
	Platform::Agile<ThreadEvent> threadEvent;
	while(gameThreadEvents.try_pop(threadEvent))
	{
		threadEvent->Invoke();
	}
}

void OpenGLESPage::ProcessPointers()
{
	if(!LUNAEngine::Shared()->IsInitialized()) return;

	std::shared_ptr<TouchEvent> touch;
	while(pointers.try_pop(touch))
	{
		float x = scaleFactor * touch->x;
		float y = LUNAEngine::SharedSizes()->GetPhysicalScreenHeight() - (scaleFactor * touch->y);
		int id = touch->id;

		switch (touch->type)
		{
		case TouchType::TOUCH_DOWN:
		{
			auto it = std::find(touchIndexes.begin(), touchIndexes.end(), -1);
			int touchIndex;

			if(it != touchIndexes.end())
			{
				touchIndex = it - touchIndexes.begin();
				touchIndexes[touchIndex] = id;
			}
			else
			{
				touchIndex = touchIndexes.size();
				touchIndexes.push_back(id);
			}

			LUNAEngine::Shared()->OnTouchDown(x, y, touchIndex);
			break;
		}
		case TouchType::TOUCH_MOVED:
		{
			auto it = std::find(touchIndexes.begin(), touchIndexes.end(), id);
			int touchIndex = it - touchIndexes.begin();
			LUNAEngine::Shared()->OnTouchMoved(x, y, touchIndex);
			break;
		}
		case TouchType::TOUCH_UP:
		{
			auto it = std::find(touchIndexes.begin(), touchIndexes.end(), id);
			int touchIndex = it - touchIndexes.begin();
			touchIndexes[touchIndex] = -1;
			LUNAEngine::Shared()->OnTouchUp(x, y, touchIndex);
			break;
		}
		}
	}
}