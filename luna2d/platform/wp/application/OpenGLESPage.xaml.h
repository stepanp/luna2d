#pragma once

#include "OpenGLESPage.g.h"
#include "OpenGLES.h"
#include <wrl.h>
#include <wrl/client.h>
#include <memory>
#include <agile.h>
#include <concrt.h>
#include <collection.h>
#include <ppltasks.h>
#include <concurrent_queue.h>
#include <math.h>

#include "lunawpads.h"
#include "lunawpstore.h"

namespace luna2d
{
	public delegate void ThreadEvent();

	enum class TouchType
	{
		TOUCH_DOWN,
		TOUCH_MOVED,
		TOUCH_UP
	};

	class TouchEvent
	{
	public:
		TouchEvent(TouchType type, Windows::UI::Core::PointerEventArgs^ args)
		{
			this->type = type;
			this->x = args->CurrentPoint->Position.X;
			this->y = args->CurrentPoint->Position.Y;
			this->id = args->CurrentPoint->PointerId;
		}

		TouchType type;
		float x, y;
		int id;
	};

	public ref class OpenGLESPage sealed
	{
	public:
		OpenGLESPage();
		virtual ~OpenGLESPage();

	public:
		void SetDelegates(
			GetNameEvent^ getName,
			IsVideoSupportedEvent^ videoSupported,
			IsVideoReadyEvent^ videoReady,
			ShowVideoEvent^ showVideo,
			RequestRateAppEvent^ rateApp);

		CallbackEvent^ GetSuccessDelegate();
		CallbackEvent^ GetFailDelegate();

		void RunInUiThread(ThreadEvent^ handler);
		void RunInGameThread(ThreadEvent^ handler);

	private:
		GetNameEvent^ getNameEvent;
		IsVideoSupportedEvent^ videoSupportedEvent;
		IsVideoReadyEvent^ videoReadyEvent;
		ShowVideoEvent^ showVideoEvent;
		CallbackEvent^ successEvent;
		CallbackEvent^ failEvent;
		RequestRateAppEvent^ rateAppEvent;

		void OnPageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
		void OnSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void GetSwapChainPanelSize(GLsizei* width, GLsizei* height);
		void CreateRenderSurface();
		void DestroyRenderSurface();
		void RecoverFromLostDevice();
		void StartRenderLoop();
		void StopRenderLoop();

		OpenGLES gles;
		OpenGLES* mOpenGLES;

		Windows::Foundation::Size mSwapChainPanelSize;
		Concurrency::critical_section mSwapChainPanelSizeCriticalSection;

		Windows::Foundation::Size mCustomRenderSurfaceSize;
		bool mUseCustomRenderSurfaceSize;
		float scaleFactor;

		EGLSurface mRenderSurface;     // This surface is associated with a swapChainPanel on the page
		Concurrency::critical_section mRenderSurfaceCriticalSection;
		Windows::Foundation::IAsyncAction^ mRenderLoopWorker;
		
		// Track user input on a background worker thread.
		Windows::Foundation::IAsyncAction^ m_inputLoopWorker;
		Windows::UI::Core::CoreIndependentInputSource^ m_coreInput;

		void OnPointerPressed(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerMoved(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerReleased(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);

		
		Concurrency::concurrent_queue<Platform::Agile<ThreadEvent>> gameThreadEvents;
		void ProcessGameThreadEvents();

		Concurrency::concurrent_queue<std::shared_ptr<TouchEvent>> pointers;
		std::vector<int> touchIndexes;
		void ProcessPointers();
	};
}
