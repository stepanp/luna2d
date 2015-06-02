#pragma once

#include <wrl.h>
#include <wrl/client.h>
#include <memory>
#include <agile.h>
#include <concrt.h>
#include <collection.h>
#include <ppltasks.h>
#include <math.h>
#include "lunagl.h"

namespace WpTestApp
{
    class HelloTriangleRenderer
    {
    public:
        HelloTriangleRenderer();
        ~HelloTriangleRenderer();
        void Draw(GLsizei width, GLsizei height);
    };
}