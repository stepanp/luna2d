//
// App.xaml.cpp
// Implementation of the App class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace WpTestApp2;
using namespace luna2d;

App::App()
{
	InitializeComponent();
}

void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e)
{
	if (mPage == nullptr)
	{
		mPage = ref new luna2d::OpenGLESPage();
	}

	// Place the page in the current window and ensure that it is active.
	Windows::UI::Xaml::Window::Current->Content = mPage;
	Windows::UI::Xaml::Window::Current->Activate();

	// Set fullscreen mode for Windows Phone
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
	Windows::UI::ViewManagement::StatusBar::GetForCurrentView()->HideAsync();
#endif
}