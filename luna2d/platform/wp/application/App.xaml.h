#pragma once

#include "app.g.h"
#include "OpenGLESPage.xaml.h"

namespace WpTestApp
{
    ref class App sealed
    {
    public:
        App();
        virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;

    private:
        OpenGLESPage^ mPage;
        OpenGLES mOpenGLES;
    };
}
