//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014 Stepan Prokofjev
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#include "lunaqtwidget.h"
#include "lunagraphics.h"
#include "lunasizes.h"
#include "lunaqtfiles.h"
#include "lunaqtlog.h"
#include "lunaqtutils.h"

using namespace luna2d;

LUNAQtWidget::LUNAQtWidget(QWidget* parent) :
	QOpenGLWidget(parent),
	paintDevice(nullptr),
	placeholderImage(nullptr),
	mouseDown(false)
{
}

LUNAQtWidget::~LUNAQtWidget()
{
	if(IsEngineInitialized()) DeinitializeEngine();
	delete paintDevice;
}

// Translate coordinates from QWindow coordinate system to engine coordinate system
float LUNAQtWidget::TranslateMouseX(int x)
{
	float graphicsWidth = LUNAEngine::SharedSizes()->GetPhysicalScreenWidth();
	int wndWidth = width();

	return x * (graphicsWidth / (float)wndWidth);
}

float LUNAQtWidget::TranslateMouseY(int y)
{
	float graphicsHeight = LUNAEngine::SharedSizes()->GetPhysicalScreenHeight();
	int wndHeight = height();

	return graphicsHeight - (y * (graphicsHeight / (float)wndHeight));
}

void LUNAQtWidget::initializeGL()
{
	QOpenGLContext* context = QOpenGLContext::currentContext();

	// Set OpenGL version to 2.1 to maximize compatibility with OpenGL ES 2.0
	QSurfaceFormat format = context->format();
	format.setVersion(2,1);
	context->setFormat(format);

	// Set methods from current QOpenGLFunctions object as usual GL functions
	// SEE "platform/qt/lunaqtgl.h"
	LUNAQtGl::InitFunctions();

	paintDevice = new QOpenGLPaintDevice();

	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(1000.0f / MAX_FPS);

	emit glSurfaceInitialized();
}

void LUNAQtWidget::paintGL()
{
	// Stretch rendered image to window size
	QSize wndSize = size();
	glViewport(0, 0, wndSize.width(), wndSize.height());

	if(LUNAEngine::Shared()->IsInitialized())
	{
		emit gameLoopIteration();

		// On Android "OnTouchMoved" event calls every time when finger is being pressed,
		// even if finger isn't moved
		// Emulate same behavior
		if(mouseDown)
		{
			QPoint mousePos = mapFromGlobal(QCursor::pos());
			LUNAEngine::Shared()->OnTouchMoved(TranslateMouseX(mousePos.x()), TranslateMouseY(mousePos.y()));
		}

		LUNAEngine::Shared()->MainLoop();
	}

	// Until engine isn't initialized, render placeholder image
	else
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		// Render image on center of window if image is specifed
		if(!placeholderImage.isNull())
		{
			paintDevice->setSize(wndSize);

			QPainter painter;
			QPoint pos((wndSize.width() - placeholderImage.width()) / 2,
				(wndSize.height() - placeholderImage.height()) / 2);

			painter.begin(paintDevice);
			painter.drawImage(pos, placeholderImage);
			painter.end();
		}
	}
}

void LUNAQtWidget::mousePressEvent(QMouseEvent *event)
{
	if(!LUNAEngine::Shared()->IsInitialized()) return;

	if(QApplication::mouseButtons() == Qt::LeftButton)
	{
		mouseDown = true;
		LUNAEngine::Shared()->OnTouchDown(TranslateMouseX(event->x()), TranslateMouseY(event->y()));
	}
}

void LUNAQtWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if(!LUNAEngine::Shared()->IsInitialized()) return;

	mouseDown = false;
	LUNAEngine::Shared()->OnTouchUp(TranslateMouseX(event->x()), TranslateMouseY(event->y()));
}

bool LUNAQtWidget::IsEngineInitialized()
{
	return LUNAEngine::Shared()->IsInitialized();
}

void LUNAQtWidget::InitializeEngine(const QString& assetsPath, int width, int height)
{
	LUNAEngine::Shared()->Assemble(new LUNAQtFiles(assetsPath), new LUNAQtLog(), new LUNAQtUtils());
	LUNAEngine::Shared()->Initialize(width, height);

	emit engineInitialized();
}

void LUNAQtWidget::InitializeEngine(const QString& assetsPath)
{
	QSize wndSize = size();
	InitializeEngine(assetsPath, wndSize.width(), wndSize.height());
}

void LUNAQtWidget::DeinitializeEngine()
{
	LUNAEngine::Shared()->Deinitialize();
}

LUNAEngine* LUNAQtWidget::GetEngine()
{
	return LUNAEngine::Shared();
}

void LUNAQtWidget::SetPlaceholderImage(const QImage &image)
{
	placeholderImage = image;
}

int LUNAQtWidget::GetFps()
{
	return LUNAEngine::SharedGraphics()->GetFps();
}
