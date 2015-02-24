//            Based on Hello_Triangle.c from
// Book:      OpenGL(R) ES 2.0 Programming Guide
// Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
// ISBN-10:   0321502795
// ISBN-13:   9780321502797
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780321563835
//            http://www.opengles-book.com

//
// This file is used by the template to render a basic scene using GL.
//

#include "pch.h"
#include "HelloTriangleRenderer.h"
#include "lunaengine.h"
#include "lunawpfiles.h"
#include "lunawplog.h"
#include "lunawputils.h"

using namespace Platform;
using namespace WpTestApp;
using namespace luna2d;

HelloTriangleRenderer::HelloTriangleRenderer()
{

}

HelloTriangleRenderer::~HelloTriangleRenderer()
{
	if(LUNAEngine::Shared()->IsInitialized()) LUNAEngine::Shared()->Deinitialize();
}

// Draws a basic triangle
void HelloTriangleRenderer::Draw(GLsizei width, GLsizei height)
{
	if(!LUNAEngine::Shared()->IsInitialized())
	{
		LUNAEngine::Shared()->Assemble(new LUNAWpFiles(), new LUNAWpLog(), new LUNAWpUtils());
		LUNAEngine::Shared()->Initialize(width, height);
	}

	LUNAEngine::Shared()->MainLoop();
}

