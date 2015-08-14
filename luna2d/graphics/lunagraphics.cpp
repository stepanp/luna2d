//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2015 Stepan Prokofjev
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

#include "lunagraphics.h"
#include "lunaengine.h"
#include "lunaplatformutils.h"
#include "lunascenes.h"
#include "lunaanimation.h"
#include "lunamesh.h"
#include "lunatext.h"
#include "lunacurverenderer.h"

using namespace luna2d;

LUNAGraphics::LUNAGraphics()
{
	renderer = new LUNARenderer();

	lastTime = LUNAEngine::SharedPlatformUtils()->GetSystemTime();
	fpsTime = 0;
	framesCount = 0;
	lastFrames = 0;
	deltaTime = 0;

	// Register "luna.graphics" module
	SqVm *sq = LUNAEngine::SharedSquirrel();
	SqTable tblLuna = sq->GetRootTable().GetTable("luna");
	SqTable tblGraphics(sq);

	tblGraphics.NewSlot("getFps", SqFunction(sq, this, &LUNAGraphics::GetFps));
	tblGraphics.NewSlot("getDeltaTime", SqFunction(sq, this, &LUNAGraphics::GetDeltaTime));
	tblGraphics.NewSlot("getRenderCalls", SqFunction(sq, this, &LUNAGraphics::GetRenderCalls));
	tblGraphics.NewSlot("setBackgroundColor", SqFunction(sq, this, &LUNAGraphics::SetBackgroundColor));
	tblGraphics.NewSlot("enableDebugRender", SqFunction(sq, renderer, &LUNARenderer::EnableDebugRender));
	tblGraphics.NewSlot("renderLine", SqFunction(sq, renderer, &LUNARenderer::RenderLine));

	// Register sprite
	SqClass<LUNASprite> clsSprite(sq);
	clsSprite.BindConstructor<const SqAny&>();
	clsSprite.BindMethod("setTexture", &LUNASprite::SetTexture);
	clsSprite.BindMethod("setTextureRegion", &LUNASprite::SetTextureRegion);
	clsSprite.BindMethod("getX", &LUNASprite::GetX);
	clsSprite.BindMethod("getY", &LUNASprite::GetY);
	clsSprite.BindMethod("setX", &LUNASprite::SetX);
	clsSprite.BindMethod("setY", &LUNASprite::SetY);
	clsSprite.BindMethod("getPos", &LUNASprite::GetPos);
	clsSprite.BindMethod("setPos", &LUNASprite::SetPos);
	clsSprite.BindMethod("getWidth", &LUNASprite::GetWidth);
	clsSprite.BindMethod("getHeight", &LUNASprite::GetHeight);
	clsSprite.BindMethod("setWidth", &LUNASprite::SetWidth);
	clsSprite.BindMethod("setHeight", &LUNASprite::SetHeight);
	clsSprite.BindMethod("setSize", &LUNASprite::SetSize);
	clsSprite.BindMethod("getOriginX", &LUNASprite::GetOriginX);
	clsSprite.BindMethod("getOriginY", &LUNASprite::GetOriginY);
	clsSprite.BindMethod("setOriginX", &LUNASprite::SetOriginX);
	clsSprite.BindMethod("setOriginY", &LUNASprite::SetOriginY);
	clsSprite.BindMethod("setOrigin", &LUNASprite::SetOrigin);
	clsSprite.BindMethod("setOriginToCenter", &LUNASprite::SetOriginToCenter);
	clsSprite.BindMethod("getScaleX", &LUNASprite::GetScaleX);
	clsSprite.BindMethod("getScaleY", &LUNASprite::GetScaleY);
	clsSprite.BindMethod("setScaleX", &LUNASprite::SetScaleX);
	clsSprite.BindMethod("setScaleY", &LUNASprite::SetScaleY);
	clsSprite.BindMethod("setScale", &LUNASprite::SetScale);
	clsSprite.BindMethod("getAngle", &LUNASprite::GetAngle);
	clsSprite.BindMethod("setAngle", &LUNASprite::SetAngle);
	clsSprite.BindMethod("setColor", &LUNASprite::SetColor);
	clsSprite.BindMethod("getColor", &LUNASprite::GetColor);
	clsSprite.BindMethod("setAlpha", &LUNASprite::SetAlpha);
	clsSprite.BindMethod("getAlpha", &LUNASprite::GetAlpha);
	clsSprite.BindMethod("render", &LUNASprite::Render);
	tblGraphics.NewSlot("Sprite", clsSprite);

	// Register animation
	SqClass<LUNAAnimation, LUNASprite> clsAnimation(sq);
	clsAnimation.BindConstructor<const std::vector<std::weak_ptr<LUNATextureRegion>>&, float>();
	clsAnimation.BindMethod("getFramesCount", &LUNAAnimation::GetFramesCount);
	clsAnimation.BindMethod("getFrames", &LUNAAnimation::GetFrames);
	clsAnimation.BindMethod("setFrames", &LUNAAnimation::SetFrames);
	clsAnimation.BindMethod("getFrameTime", &LUNAAnimation::GetFrameTime);
	clsAnimation.BindMethod("setFrameTime", &LUNAAnimation::SetFrameTime);
	clsAnimation.BindMethod("getCurFrame", &LUNAAnimation::GetCurFrame);
	clsAnimation.BindMethod("setCurFrame", &LUNAAnimation::SetCurFrame);
	clsAnimation.BindMethod("setLoop", &LUNAAnimation::SetLoop);
	clsAnimation.BindMethod("isRunning", &LUNAAnimation::IsRunning);
	clsAnimation.BindMethod("start", &LUNAAnimation::Start);
	clsAnimation.BindMethod("pause", &LUNAAnimation::Pause);
	clsAnimation.BindMethod("stop", &LUNAAnimation::Stop);
	clsAnimation.BindMethod("update", &LUNAAnimation::Update);
	tblGraphics.NewSlot("Animation", clsAnimation);

	// Register mesh
	SqClass<LUNAMesh> clsMesh(sq);
	clsMesh.BindConstructor<const std::weak_ptr<LUNATexture>&>();
	clsMesh.BindMethod("clear", &LUNAMesh::Clear);
	clsMesh.BindMethod("setTexture", &LUNAMesh::SetTexture);
	clsMesh.BindMethod("addVertex", &LUNAMesh::AddVertex);
	clsMesh.BindMethod("render", &LUNAMesh::Render);
	tblGraphics.NewSlot("Mesh", clsMesh);

	// Register text renderer
	SqClass<LUNAText> clsText(sq);
	clsText.BindConstructor<const std::weak_ptr<LUNAFont>&>();
	clsText.BindMethod("getX", &LUNAText::GetX);
	clsText.BindMethod("getY", &LUNAText::GetY);
	clsText.BindMethod("setX", &LUNAText::SetX);
	clsText.BindMethod("setY", &LUNAText::SetY);
	clsText.BindMethod("getPos", &LUNAText::GetPos);
	clsText.BindMethod("setPos", &LUNAText::SetPos);
	clsText.BindMethod("setColor", &LUNAText::SetColor);
	clsText.BindMethod("getColor", &LUNAText::GetColor);
	clsText.BindMethod("setAlpha", &LUNAText::SetAlpha);
	clsText.BindMethod("getAlpha", &LUNAText::GetAlpha);
	clsText.BindMethod("getText", &LUNAText::GetText);
	clsText.BindMethod("setText", &LUNAText::SetText);
	clsText.BindMethod("render", &LUNAText::Render);
	tblGraphics.NewSlot("Text", clsText);

	// Register curve renderer
	SqClass<LUNACurveRenderer> clsCurveRenderer(sq);
	clsCurveRenderer.BindConstructor<const SqTable&>();
	clsCurveRenderer.BindMethod("getKnotsCount", &LUNACurveRenderer::GetKnotsCount);
	clsCurveRenderer.BindMethod("clearKnots", &LUNACurveRenderer::ClearKnots);
	clsCurveRenderer.BindMethod("addKnot", &LUNACurveRenderer::AddKnot);
	clsCurveRenderer.BindMethod("removeKnot", &LUNACurveRenderer::RemoveKnot);
	clsCurveRenderer.BindMethod("setKnot", &LUNACurveRenderer::SetKnot);
	clsCurveRenderer.BindMethod("setKnots", &LUNACurveRenderer::SetKnots);
	clsCurveRenderer.BindMethod("getColor", &LUNACurveRenderer::GetColor);
	clsCurveRenderer.BindMethod("setColor", &LUNACurveRenderer::SetColor);
	clsCurveRenderer.BindMethod("getAlpha", &LUNACurveRenderer::GetAlpha);
	clsCurveRenderer.BindMethod("setAlpha", &LUNACurveRenderer::SetAlpha);
	clsCurveRenderer.BindMethod("render", &LUNACurveRenderer::Render);
	tblGraphics.NewSlot("CurveRenderer", clsCurveRenderer);

	tblLuna.NewSlot("graphics", tblGraphics);

	// Register graphics asset types
	SqClass<LUNATexture, LUNAAsset> clsTexture(sq);
	clsTexture.BindMethod("getWidth", &LUNATexture::GetWidth);
	clsTexture.BindMethod("getHeight", &LUNATexture::GetHeight);
	clsTexture.BindMethod("getWidthPoints", &LUNATexture::GetWidthPoints);
	clsTexture.BindMethod("getHeightPoints", &LUNATexture::GetHeightPoints);

	SqClass<LUNATextureRegion, LUNAAsset> clsTextureRegion(sq);
	clsTextureRegion.BindMethod("getTexture", &LUNATextureRegion::GetTexture);
	clsTextureRegion.BindMethod("getWidth", &LUNATextureRegion::GetWidth);
	clsTextureRegion.BindMethod("getHeight", &LUNATextureRegion::GetHeight);
	clsTextureRegion.BindMethod("getWidthPoints", &LUNATextureRegion::GetWidthPoints);
	clsTextureRegion.BindMethod("getHeightPoints", &LUNATextureRegion::GetHeightPoints);
	clsTextureRegion.BindMethod("getU1", &LUNATextureRegion::GetU1);
	clsTextureRegion.BindMethod("getV1", &LUNATextureRegion::GetV1);
	clsTextureRegion.BindMethod("getU2", &LUNATextureRegion::GetU2);
	clsTextureRegion.BindMethod("getV2", &LUNATextureRegion::GetV2);

	SqClass<LUNAFont, LUNAAsset> clsFont(sq);
	clsFont.BindMethod("getSize", &LUNAFont::GetSize);
}

LUNAGraphics::~LUNAGraphics()
{
	delete renderer;
}

LUNARenderer* LUNAGraphics::GetRenderer()
{
	return renderer;
}

int LUNAGraphics::GetFps()
{
	return lastFrames;
}

float LUNAGraphics::GetDeltaTime()
{
	return deltaTime;
}

int LUNAGraphics::GetRenderCalls()
{
	return renderer->GetRenderCalls();
}

void LUNAGraphics::SetBackgroundColor(float r, float g, float b)
{
	renderer->SetBackgroundColor(LUNAColor::RgbFloat(r / 255.0f, g / 255.0f, b / 255.0f));
}

void LUNAGraphics::OnUpdate()
{
	// Calculate delta time
	double curTime = LUNAEngine::SharedPlatformUtils()->GetSystemTime();
	deltaTime = curTime - lastTime;
	lastTime = curTime;

	// Calculate FPS
	fpsTime += deltaTime;
	framesCount++;
	if(fpsTime >= 1.0f)
	{
		lastFrames = framesCount;
		fpsTime = 0;
		framesCount = 0;
	}

	LUNAEngine::SharedScenes()->OnUpdate(deltaTime);

	// Render
	renderer->Begin();
	LUNAEngine::SharedScenes()->OnRender();
	renderer->End();
}
