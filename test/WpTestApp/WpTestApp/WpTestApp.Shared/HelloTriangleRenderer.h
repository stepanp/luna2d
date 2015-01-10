#pragma once

#include "pch.h"

namespace WpTestApp
{
    class HelloTriangleRenderer
    {
    public:
        HelloTriangleRenderer();
        ~HelloTriangleRenderer();
        void Draw(GLsizei width, GLsizei height);

    private:
        GLuint mProgram;
    };
}