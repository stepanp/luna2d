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
#include "lunaparticlesystem.h"
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

	// Bind "luna.graphics" module
	LuaScript *lua = LUNAEngine::SharedLua();
	LuaTable tblLuna = lua->GetGlobalTable().GetTable("luna");
	LuaTable tblGraphics(lua);

	tblGraphics.SetField("getFps", LuaFunction(lua, this, &LUNAGraphics::GetFps));
	tblGraphics.SetField("getDeltaTime", LuaFunction(lua, this, &LUNAGraphics::GetDeltaTime));
	tblGraphics.SetField("getRenderCalls", LuaFunction(lua, this, &LUNAGraphics::GetRenderCalls));
	tblGraphics.SetField("setBackgroundColor", LuaFunction(lua, this, &LUNAGraphics::SetBackgroundColor));
	tblGraphics.SetField("enableDebugRender", LuaFunction(lua, renderer, &LUNARenderer::EnableDebugRender));
	tblGraphics.SetField("renderLine", LuaFunction(lua, renderer, &LUNARenderer::RenderLine));

	// Bind sprite
	LuaClass<LUNASprite> clsSprite(lua);
	clsSprite.SetConstructor<const LuaAny&>();
	clsSprite.SetMethod("setTexture", &LUNASprite::SetTexture);
	clsSprite.SetMethod("setTextureRegion", &LUNASprite::SetTextureRegion);
	clsSprite.SetMethod("getX", &LUNASprite::GetX);
	clsSprite.SetMethod("getY", &LUNASprite::GetY);
	clsSprite.SetMethod("setX", &LUNASprite::SetX);
	clsSprite.SetMethod("setY", &LUNASprite::SetY);
	clsSprite.SetMethod("getPos", &LUNASprite::GetPos);
	clsSprite.SetMethod("setPos", &LUNASprite::SetPos);
	clsSprite.SetMethod("getWidth", &LUNASprite::GetWidth);
	clsSprite.SetMethod("getHeight", &LUNASprite::GetHeight);
	clsSprite.SetMethod("setWidth", &LUNASprite::SetWidth);
	clsSprite.SetMethod("setHeight", &LUNASprite::SetHeight);
	clsSprite.SetMethod("setSize", &LUNASprite::SetSize);
	clsSprite.SetMethod("getOriginX", &LUNASprite::GetOriginX);
	clsSprite.SetMethod("getOriginY", &LUNASprite::GetOriginY);
	clsSprite.SetMethod("setOriginX", &LUNASprite::SetOriginX);
	clsSprite.SetMethod("setOriginY", &LUNASprite::SetOriginY);
	clsSprite.SetMethod("setOrigin", &LUNASprite::SetOrigin);
	clsSprite.SetMethod("setOriginToCenter", &LUNASprite::SetOriginToCenter);
	clsSprite.SetMethod("getScaleX", &LUNASprite::GetScaleX);
	clsSprite.SetMethod("getScaleY", &LUNASprite::GetScaleY);
	clsSprite.SetMethod("setScaleX", &LUNASprite::SetScaleX);
	clsSprite.SetMethod("setScaleY", &LUNASprite::SetScaleY);
	clsSprite.SetMethod("setScale", &LUNASprite::SetScale);
	clsSprite.SetMethod("getAngle", &LUNASprite::GetAngle);
	clsSprite.SetMethod("setAngle", &LUNASprite::SetAngle);
	clsSprite.SetMethod("setColor", &LUNASprite::SetColor);
	clsSprite.SetMethod("getColor", &LUNASprite::GetColor);
	clsSprite.SetMethod("setAlpha", &LUNASprite::SetAlpha);
	clsSprite.SetMethod("getAlpha", &LUNASprite::GetAlpha);
	clsSprite.SetMethod("render", &LUNASprite::Render);
	tblGraphics.SetField("Sprite", clsSprite);

	// Bind animation
	LuaClass<LUNAAnimation> clsAnimation(lua);
	clsAnimation.SetConstructor<const std::vector<std::weak_ptr<LUNATextureRegion>>&, float>();
	clsAnimation.SetMethod("getFramesCount", &LUNAAnimation::GetFramesCount);
	clsAnimation.SetMethod("getFrames", &LUNAAnimation::GetFrames);
	clsAnimation.SetMethod("setFrames", &LUNAAnimation::SetFrames);
	clsAnimation.SetMethod("getFrameTime", &LUNAAnimation::GetFrameTime);
	clsAnimation.SetMethod("setFrameTime", &LUNAAnimation::SetFrameTime);
	clsAnimation.SetMethod("getCurFrame", &LUNAAnimation::GetCurFrame);
	clsAnimation.SetMethod("setCurFrame", &LUNAAnimation::SetCurFrame);
	clsAnimation.SetMethod("setLoop", &LUNAAnimation::SetLoop);
	clsAnimation.SetMethod("isRunning", &LUNAAnimation::IsRunning);
	clsAnimation.SetMethod("start", &LUNAAnimation::Start);
	clsAnimation.SetMethod("pause", &LUNAAnimation::Pause);
	clsAnimation.SetMethod("stop", &LUNAAnimation::Stop);
	clsAnimation.SetMethod("update", &LUNAAnimation::Update);
	tblGraphics.SetField("Animation", clsAnimation);

	// Bind mesh
	LuaClass<LUNAMesh> clsMesh(lua);
	clsMesh.SetConstructor<const std::weak_ptr<LUNATexture>&>();
	clsMesh.SetMethod("clear", &LUNAMesh::Clear);
	clsMesh.SetMethod("setTexture", &LUNAMesh::SetTexture);
	clsMesh.SetMethod("addVertex", &LUNAMesh::AddVertex);
	clsMesh.SetMethod("render", &LUNAMesh::Render);
	tblGraphics.SetField("Mesh", clsMesh);

	// Bind text renderer
	LuaClass<LUNAText> clsText(lua);
	clsText.SetConstructor<const std::weak_ptr<LUNAFont>&>();
	clsText.SetMethod("getX", &LUNAText::GetX);
	clsText.SetMethod("getY", &LUNAText::GetY);
	clsText.SetMethod("setX", &LUNAText::SetX);
	clsText.SetMethod("setY", &LUNAText::SetY);
	clsText.SetMethod("getPos", &LUNAText::GetPos);
	clsText.SetMethod("setPos", &LUNAText::SetPos);
	clsText.SetMethod("setColor", &LUNAText::SetColor);
	clsText.SetMethod("getColor", &LUNAText::GetColor);
	clsText.SetMethod("setAlpha", &LUNAText::SetAlpha);
	clsText.SetMethod("getAlpha", &LUNAText::GetAlpha);
	clsText.SetMethod("getText", &LUNAText::GetText);
	clsText.SetMethod("setText", &LUNAText::SetText);
	clsText.SetMethod("render", &LUNAText::Render);
	tblGraphics.SetField("Text", clsText);

	// Bind particle system
	LuaClass<LUNAParticleSystem> clsParticleSystem(lua);
	clsParticleSystem.SetConstructor<const LuaTable&>();
	clsParticleSystem.SetMethod("update", &LUNAParticleSystem::Update);
	clsParticleSystem.SetMethod("render", &LUNAParticleSystem::Render);
	tblGraphics.SetField("ParticleSystem", clsParticleSystem);

	// Bind curve renderer
	LuaClass<LUNACurveRenderer> clsCurveRenderer(lua);
	clsCurveRenderer.SetConstructor<const LuaTable&>();
	clsCurveRenderer.SetMethod("getKnotsCount", &LUNACurveRenderer::GetKnotsCount);
	clsCurveRenderer.SetMethod("clearKnots", &LUNACurveRenderer::ClearKnots);
	clsCurveRenderer.SetMethod("addKnot", &LUNACurveRenderer::AddKnot);
	clsCurveRenderer.SetMethod("removeKnot", &LUNACurveRenderer::RemoveKnot);
	clsCurveRenderer.SetMethod("setKnot", &LUNACurveRenderer::SetKnot);
	clsCurveRenderer.SetMethod("setKnots", &LUNACurveRenderer::SetKnots);
	clsCurveRenderer.SetMethod("getColor", &LUNACurveRenderer::GetColor);
	clsCurveRenderer.SetMethod("setColor", &LUNACurveRenderer::SetColor);
	clsCurveRenderer.SetMethod("getAlpha", &LUNACurveRenderer::GetAlpha);
	clsCurveRenderer.SetMethod("setAlpha", &LUNACurveRenderer::SetAlpha);
	clsCurveRenderer.SetMethod("render", &LUNACurveRenderer::Render);
	tblGraphics.SetField("CurveRenderer", clsCurveRenderer);

	tblGraphics.MakeReadOnly();
	tblLuna.SetField("graphics", tblGraphics);

	// Bind graphics asset types
	LuaClass<LUNATexture> clsTexture(lua);
	clsTexture.SetMethod("getWidth", &LUNATexture::GetWidth);
	clsTexture.SetMethod("getHeight", &LUNATexture::GetHeight);
	clsTexture.SetMethod("getWidthPoints", &LUNATexture::GetWidthPoints);
	clsTexture.SetMethod("getHeightPoints", &LUNATexture::GetHeightPoints);

	LuaClass<LUNATextureRegion> clsTextureRegion(lua);
	clsTextureRegion.SetMethod("getTexture", &LUNATextureRegion::GetTexture);
	clsTextureRegion.SetMethod("getWidth", &LUNATextureRegion::GetWidth);
	clsTextureRegion.SetMethod("getHeight", &LUNATextureRegion::GetHeight);
	clsTextureRegion.SetMethod("getWidthPoints", &LUNATextureRegion::GetWidthPoints);
	clsTextureRegion.SetMethod("getHeightPoints", &LUNATextureRegion::GetHeightPoints);
	clsTextureRegion.SetMethod("getU1", &LUNATextureRegion::GetU1);
	clsTextureRegion.SetMethod("getV1", &LUNATextureRegion::GetV1);
	clsTextureRegion.SetMethod("getU2", &LUNATextureRegion::GetU2);
	clsTextureRegion.SetMethod("getV2", &LUNATextureRegion::GetV2);

	LuaClass<LUNAFont> clsFont(lua);
	clsFont.SetMethod("getSize", &LUNAFont::GetSize);
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

	if(deltaTime > MAX_DELTA) deltaTime = MAX_DELTA;

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
