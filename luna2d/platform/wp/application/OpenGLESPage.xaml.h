#pragma once

#include "OpenGLESPage.g.h"
#include "OpenGLES.h"
#include "HelloTriangleRenderer.h"
#include <concurrent_queue.h>

namespace WpTestApp
{
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
		}

		TouchType type;
		float x, y;
	};

    public ref class OpenGLESPage sealed
    {
    public:
        OpenGLESPage();
        virtual ~OpenGLESPage();

    internal:
        OpenGLESPage(OpenGLES* openGLES);

    private:
        void OnPageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
        void OnSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
        void GetSwapChainPanelSize(GLsizei* width, GLsizei* height);
        void CreateRenderSurface();
        void DestroyRenderSurface();
        void RecoverFromLostDevice();
        void StartRenderLoop();
        void StopRenderLoop();

        OpenGLES* mOpenGLES;

        Windows::Foundation::Size mSwapChainPanelSize;
        Concurrency::critical_section mSwapChainPanelSizeCriticalSection;

        Windows::Foundation::Size mCustomRenderSurfaceSize;
        bool mUseCustomRenderSurfaceSize;

        EGLSurface mRenderSurface;     // This surface is associated with a swapChainPanel on the page
        Concurrency::critical_section mRenderSurfaceCriticalSection;
        Windows::Foundation::IAsyncAction^ mRenderLoopWorker;
		
		// Track user input on a background worker thread.
		Windows::Foundation::IAsyncAction^ m_inputLoopWorker;
		Windows::UI::Core::CoreIndependentInputSource^ m_coreInput;

		Concurrency::concurrent_queue<std::shared_ptr<TouchEvent>> pointers;
		void ProcessPointers();
		void OnPointerPressed(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerMoved(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerReleased(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
    };
}
