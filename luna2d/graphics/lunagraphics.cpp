//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2017 Stepan Prokofjev
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
#include "lunaplatformutils.h"
#include "lunascenes.h"
#include "lunasizes.h"
#include "lunarenderer.h"
#include "lunaanimation.h"
#include "lunamesh.h"
#include "lunatext.h"
#include "lunaparticlesystem.h"
#include "lunacurve.h"
#include "lunaradialmesh.h"
#include "lunaframebuffer.h"
#include "lunapngformat.h"
#include "lunajpegformat.h"

using namespace luna2d;

LUNAGraphics::LUNAGraphics() :
	camera(LUNAEngine::SharedSizes()->MakeCamera())
{
	renderer.SetCamera(camera);

	lastTime = LUNAEngine::SharedPlatformUtils()->GetSystemTime();
	fpsTime = 0;
	framesCount = 0;
	lastFrames = 0;
	deltaTime = 0;
	movAvgDelta = TARGET_DELTA_TIME;

	// Bind "luna.graphics" module
	LuaScript *lua = LUNAEngine::SharedLua();
	LuaTable tblLuna = lua->GetGlobalTable().GetTable("luna");
	LuaTable tblGraphics(lua);

	tblGraphics.SetField("getFps", LuaFunction(lua, this, &LUNAGraphics::GetFps));
	tblGraphics.SetField("getDeltaTime", LuaFunction(lua, this, &LUNAGraphics::GetDeltaTime));
	tblGraphics.SetField("getRenderCalls", LuaFunction(lua, this, &LUNAGraphics::GetRenderCalls));
	tblGraphics.SetField("getRenderedVertexes", LuaFunction(lua, this, &LUNAGraphics::GetRenderedVertexes));
	tblGraphics.SetField("getCamera", LuaFunction(lua, this, &LUNAGraphics::GetCamera));
	tblGraphics.SetField("setBackgroundColor", LuaFunction(lua, this, &LUNAGraphics::SetBackgroundColor));
	tblGraphics.SetField("getDefaultShader", LuaFunction(lua, &renderer, &LUNARenderer::GetDefaultShader));
	tblGraphics.SetField("enableScissor", LuaFunction(lua, &renderer, &LUNARenderer::EnableScissor));
	tblGraphics.SetField("disableScissor", LuaFunction(lua, &renderer, &LUNARenderer::DisableScissor));
	tblGraphics.SetField("setFrameBuffer", LuaFunction(lua, &renderer, &LUNARenderer::SetFrameBuffer));
	tblGraphics.SetField("enableDebugRender", LuaFunction(lua, &renderer, &LUNARenderer::EnableDebugRender));
	tblGraphics.SetField("renderLine", LuaFunction(lua, &renderer, &LUNARenderer::RenderLine));

	// Bind camera
	LuaClass<LUNACamera> clsCamera(lua);
	clsCamera.SetMethod("getX", &LUNACamera::GetX);
	clsCamera.SetMethod("getY", &LUNACamera::GetY);
	clsCamera.SetMethod("setX", &LUNACamera::SetX);
	clsCamera.SetMethod("setY", &LUNACamera::SetY);
	clsCamera.SetMethod("getPos", &LUNACamera::GetPos);
	clsCamera.SetMethod("setPos", &LUNACamera::SetPos);
	clsCamera.SetMethod("getZoom", &LUNACamera::GetZoom);
	clsCamera.SetMethod("setZoom", &LUNACamera::SetZoom);

	// Bind sprite
	LuaClass<LUNASprite> clsSprite(lua);
	clsSprite.SetConstructor<const LuaAny&>();
	clsSprite.SetMethod("setTexture", &LUNASprite::SetTexture);
	clsSprite.SetMethod("setTextureRegion", &LUNASprite::SetTextureRegion);
	clsSprite.SetMethod("setShader", &LUNASprite::SetShader);
	clsSprite.SetMethod("getBlendingMode", &LUNASprite::GetBlendingMode);
	clsSprite.SetMethod("setBlendingMode", &LUNASprite::SetBlendingMode);
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
	clsAnimation.SetMethod("isLoop", &LUNAAnimation::IsLoop);
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

	// Bind text
	LuaClass<LUNAText> clsText(lua);
	clsText.SetConstructor<const std::weak_ptr<LUNAFont>&>();
	clsText.SetMethod("getX", &LUNAText::GetX);
	clsText.SetMethod("getY", &LUNAText::GetY);
	clsText.SetMethod("setX", &LUNAText::SetX);
	clsText.SetMethod("setY", &LUNAText::SetY);
	clsText.SetMethod("getPos", &LUNAText::GetPos);
	clsText.SetMethod("setPos", &LUNAText::SetPos);
	clsText.SetMethod("getScaleX", &LUNAText::GetScaleX);
	clsText.SetMethod("getScaleY", &LUNAText::GetScaleY);
	clsText.SetMethod("setScaleX", &LUNAText::SetScaleX);
	clsText.SetMethod("setScaleY", &LUNAText::SetScaleY);
	clsText.SetMethod("setScale", &LUNAText::SetScale);
	clsText.SetMethod("getWidth", &LUNAText::GetWidth);
	clsText.SetMethod("getHeight", &LUNAText::GetHeight);
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
	clsParticleSystem.SetMethod("getX", &LUNAParticleSystem::GetX);
	clsParticleSystem.SetMethod("getY", &LUNAParticleSystem::GetY);
	clsParticleSystem.SetMethod("setX", &LUNAParticleSystem::SetX);
	clsParticleSystem.SetMethod("setY", &LUNAParticleSystem::SetY);
	clsParticleSystem.SetMethod("getPos", &LUNAParticleSystem::GetPos);
	clsParticleSystem.SetMethod("setPos", &LUNAParticleSystem::SetPos);
	clsParticleSystem.SetMethod("isLoop", &LUNAParticleSystem::IsLoop);
	clsParticleSystem.SetMethod("setLoop", &LUNAParticleSystem::SetLoop);
	clsParticleSystem.SetMethod("isRunning", &LUNAParticleSystem::IsRunning);
	clsParticleSystem.SetMethod("start", &LUNAParticleSystem::Start);
	clsParticleSystem.SetMethod("pause", &LUNAParticleSystem::Pause);
	clsParticleSystem.SetMethod("stop", &LUNAParticleSystem::Stop);
	clsParticleSystem.SetMethod("update", &LUNAParticleSystem::Update);
	clsParticleSystem.SetMethod("render", &LUNAParticleSystem::Render);
	tblGraphics.SetField("ParticleSystem", clsParticleSystem);

	// Bind curve renderer
	LuaClass<LUNACurve> clsCurveRenderer(lua);

	std::function<void(const std::shared_ptr<LUNACurve>&, float, float, const std::string&)> fnSetCustomWidth =
		[](const std::shared_ptr<LUNACurve>& thisCurve, float percent, float scale, const std::string& easingName)
	{
		LUNAEasingFunc easing = easing::Linear;

		if(!easingName.empty())
		{
			if(EASINGS_MAP.count(easingName) == 1) easing = EASINGS_MAP.at(easingName);
			else LUNA_LOGE("Easing with name \"%s\" not found. Using default \"linear\" easing", easingName.c_str());
		}

		thisCurve->SetCustomWidth(percent, scale, easing);
	};

	clsCurveRenderer.SetConstructor<const LUNACurveParams&>();
	clsCurveRenderer.SetMethod("setShader", &LUNACurve::SetShader);
	clsCurveRenderer.SetMethod("clearCustomWidths", &LUNACurve::ClearCustomWidths);
	clsCurveRenderer.SetExtensionMethod("setCustomWidth", fnSetCustomWidth);
	clsCurveRenderer.SetMethod("getKnotsCount", &LUNACurve::GetKnotsCount);
	clsCurveRenderer.SetMethod("clearKnots", &LUNACurve::ClearKnots);
	clsCurveRenderer.SetMethod("getKnots", &LUNACurve::GetKnots);
	clsCurveRenderer.SetMethod("setKnots", &LUNACurve::SetKnots);
	clsCurveRenderer.SetMethod("addKnot", &LUNACurve::AddKnot);
	clsCurveRenderer.SetMethod("removeKnot", &LUNACurve::RemoveKnot);
	clsCurveRenderer.SetMethod("setKnot", &LUNACurve::SetKnot);
	clsCurveRenderer.SetMethod("getColor", &LUNACurve::GetColor);
	clsCurveRenderer.SetMethod("setColor", &LUNACurve::SetColor);
	clsCurveRenderer.SetMethod("getAlpha", &LUNACurve::GetAlpha);
	clsCurveRenderer.SetMethod("setAlpha", &LUNACurve::SetAlpha);
	clsCurveRenderer.SetMethod("render", &LUNACurve::Render);
	tblGraphics.SetField("Curve", clsCurveRenderer);

	// Bind radial mesh
	LuaClass<LUNARadialMesh> clsRadialMesh(lua);
	clsRadialMesh.SetConstructor<const std::weak_ptr<LUNATextureRegion>&>();
	clsRadialMesh.SetMethod("getX", &LUNARadialMesh::GetX);
	clsRadialMesh.SetMethod("getY", &LUNARadialMesh::GetY);
	clsRadialMesh.SetMethod("setX", &LUNARadialMesh::SetX);
	clsRadialMesh.SetMethod("setY", &LUNARadialMesh::SetY);
	clsRadialMesh.SetMethod("getPos", &LUNARadialMesh::GetPos);
	clsRadialMesh.SetMethod("setPos", &LUNARadialMesh::SetPos);
	clsRadialMesh.SetMethod("getWidth", &LUNARadialMesh::GetWidth);
	clsRadialMesh.SetMethod("getHeight", &LUNARadialMesh::GetHeight);
	clsRadialMesh.SetMethod("setWidth", &LUNARadialMesh::SetWidth);
	clsRadialMesh.SetMethod("setHeight", &LUNARadialMesh::SetHeight);
	clsRadialMesh.SetMethod("setSize", &LUNARadialMesh::SetSize);
	clsRadialMesh.SetMethod("getColor", &LUNARadialMesh::GetColor);
	clsRadialMesh.SetMethod("setColor", &LUNARadialMesh::SetColor);
	clsRadialMesh.SetMethod("getAlpha", &LUNARadialMesh::GetAlpha);
	clsRadialMesh.SetMethod("setAlpha", &LUNARadialMesh::SetAlpha);
	clsRadialMesh.SetMethod("getScale", &LUNARadialMesh::GetScale);
	clsRadialMesh.SetMethod("setScale", &LUNARadialMesh::SetScale);
	clsRadialMesh.SetMethod("getProgress", &LUNARadialMesh::GetProgress);
	clsRadialMesh.SetMethod("setProgress", &LUNARadialMesh::SetProgress);
	clsRadialMesh.SetMethod("render", &LUNARadialMesh::Render);
	tblGraphics.SetField("RadialMesh", clsRadialMesh);

	// Bind texture
	LuaClass<LUNATexture> clsTexture(lua);
	clsTexture.SetMethod("getWidth", &LUNATexture::GetWidth);
	clsTexture.SetMethod("getHeight", &LUNATexture::GetHeight);
	clsTexture.SetMethod("getWidthPoints", &LUNATexture::GetWidthPoints);
	clsTexture.SetMethod("getHeightPoints", &LUNATexture::GetHeightPoints);

	// Bind texture region
	LuaClass<LUNATextureRegion> clsTextureRegion(lua);
	clsTextureRegion.SetConstructor<const std::weak_ptr<LUNATexture>&,int,int,int,int>();
	clsTextureRegion.SetMethod("getTexture", &LUNATextureRegion::GetTexture);
	clsTextureRegion.SetMethod("getWidth", &LUNATextureRegion::GetWidth);
	clsTextureRegion.SetMethod("getHeight", &LUNATextureRegion::GetHeight);
	clsTextureRegion.SetMethod("getWidthPoints", &LUNATextureRegion::GetWidthPoints);
	clsTextureRegion.SetMethod("getHeightPoints", &LUNATextureRegion::GetHeightPoints);
	clsTextureRegion.SetMethod("getU1", &LUNATextureRegion::GetU1);
	clsTextureRegion.SetMethod("getV1", &LUNATextureRegion::GetV1);
	clsTextureRegion.SetMethod("getU2", &LUNATextureRegion::GetU2);
	clsTextureRegion.SetMethod("getV2", &LUNATextureRegion::GetV2);
	tblGraphics.SetField("TextureRegion", clsTextureRegion);

	// Bind shader
	LuaClass<LUNAShader> clsShader(lua);

	// Bind font
	LuaClass<LUNAFont> clsFont(lua);
	clsFont.SetMethod("getSize", &LUNAFont::GetSize);
	clsFont.SetMethod("getStringWidth", &LUNAFont::GetStringWidth);
	clsFont.SetMethod("getStringHeight", &LUNAFont::GetStringHeight);

	// Bind pixmap
	LuaClass<LUNAImage> clsImage(lua);
	clsImage.SetConstructor<int,int,LUNAColorType>();
	clsImage.SetMethod("getWidth", &LUNAImage::GetWidth);
	clsImage.SetMethod("getHeight", &LUNAImage::GetHeight);
	clsImage.SetMethod("getColorType", &LUNAImage::GetColorType);
	clsImage.SetMethod("setSize", &LUNAImage::SetSize);
	clsImage.SetMethod("getPixel", &LUNAImage::GetPixel);
	clsImage.SetMethod("setPixel", &LUNAImage::SetPixel);
	clsImage.SetMethod("getHeight", &LUNAImage::GetHeight);
	clsImage.SetMethod("flipVertically", &LUNAImage::FlipVertically);
	clsImage.SetMethod("flipHorizontally", &LUNAImage::FlipHorizontally);

	std::function<void(const std::shared_ptr<LUNAImage>&, const LUNAColor&, LuaAny)> fnFill =
		[](const std::shared_ptr<LUNAImage>& thisPixmap, const LUNAColor& color, LuaAny blendingMode)
	{
		LUNABlendingMode mode = blendingMode.GetType() != LUA_TSTRING ? LUNABlendingMode::NONE : blendingMode.To<LUNABlendingMode>();
		thisPixmap->Fill(color, mode);
	};
	clsImage.SetExtensionMethod("fill", fnFill);

	std::function<void(const std::shared_ptr<LUNAImage>&,int,int,int,int,const LUNAColor&, LuaAny)> fnFillRectangle =
		[](const std::shared_ptr<LUNAImage>& thisPixmap, int x, int y, int width, int height, const LUNAColor& color, LuaAny blendingMode)
	{
		LUNABlendingMode mode = blendingMode.GetType() != LUA_TSTRING ? LUNABlendingMode::NONE : blendingMode.To<LUNABlendingMode>();
		thisPixmap->FillRectangle(x, y, width, height, color, mode);
	};
	clsImage.SetExtensionMethod("fillRectangle", fnFillRectangle);

	std::function<void(const std::shared_ptr<LUNAImage>&,int,int,const std::shared_ptr<LUNAImage>&, LUNABlendingMode)> fnDrawPixmap =
		[](const std::shared_ptr<LUNAImage>& thisPixmap, int x, int y, const std::shared_ptr<LUNAImage>& pixmap, LUNABlendingMode mode)
	{
		if(!pixmap) LUNA_RETURN_ERR("Attempt to draw invalid pixmap");

		thisPixmap->DrawImage(x, y, *(pixmap.get()), mode);
	};
	clsImage.SetExtensionMethod("drawPixmap", fnDrawPixmap);

	std::function<void(const std::shared_ptr<LUNAImage>&,const std::string&)> fnSave =
		[](const std::shared_ptr<LUNAImage>& thisPixmap, const std::string& filename)
	{
		std::string ext = LUNAEngine::SharedFiles()->GetExtension(filename);
		std::unique_ptr<LUNAImageFormat> format;

		if(ext == "png") format = std::unique_ptr<LUNAPngFormat>(new LUNAPngFormat());
		else if(ext == "jpg" || ext == "jpeg") format = std::unique_ptr<LUNAJpegFormat>(new LUNAJpegFormat());
		else LUNA_LOG("Cannot save pixmap. Image format \"%s\" is unsupoorted");

		thisPixmap->Save(filename, *format, LUNAFileLocation::CACHE);
	};
	clsImage.SetExtensionMethod("save", fnSave);

	std::function<std::shared_ptr<LUNAImage>(const std::string&)> fnImageFromFile =
		[](const std::string& filename) -> std::shared_ptr<LUNAImage>
	{
		auto image = std::make_shared<LUNAImage>(filename, LUNAPngFormat(), LUNAFileLocation::APP_FOLDER);
		if(image->IsEmpty()) return nullptr;
		return image;
	};
	clsImage.SetField("fromFile", LuaFunction(lua, fnImageFromFile));
	tblGraphics.SetField("Pixmap", clsImage);

	// Bind framebuffer
	LuaClass<LUNAFrameBuffer> clsFrameBuffer(lua);
	clsFrameBuffer.SetConstructor<int,int,LUNAColorType>();
	clsFrameBuffer.SetMethod("getViewportWidth", &LUNAFrameBuffer::GetViewportWidth);
	clsFrameBuffer.SetMethod("getViewportHeight", &LUNAFrameBuffer::GetViewportHeight);
	clsFrameBuffer.SetMethod("getTexture", &LUNAFrameBuffer::GetTexture);
	clsFrameBuffer.SetMethod("getTextureRegion", &LUNAFrameBuffer::GetTextureRegion);
	clsFrameBuffer.SetMethod("readPixels", &LUNAFrameBuffer::ReadPixels);

	std::function<std::shared_ptr<LUNAFrameBuffer>(LUNAColorType)> fnFrameBufferConstruct = [](LUNAColorType colorType)
	{
		auto sizes = LUNAEngine::SharedSizes();
		return std::make_shared<LUNAFrameBuffer>(sizes->GetPhysicalScreenWidth(), sizes->GetPhysicalScreenHeight(), colorType);
	};
	clsFrameBuffer.SetField("fullscreen", LuaFunction(lua, fnFrameBufferConstruct));
	tblGraphics.SetField("FrameBuffer", clsFrameBuffer);

	// Bind color
	LuaTable tblColor(lua);

	std::function<LUNAColor(const std::string&)> fnHex = [](const std::string& hexStr)
	{
		return LUNAColor::HexString(hexStr, 1.0f);
	};

	std::function<LUNAColor(float, float, float)> fnRgb = [](float r, float g, float b)
	{
		return LUNAColor::Rgb(r, g, b, 1.0f);
	};

	std::function<LUNAColor(float, float, float)> fnRgbFloat = [](float r, float g, float b)
	{
		return LUNAColor::RgbFloat(r, g, b, 1.0f);
	};

	tblColor.SetField("hex", LuaFunction(lua, fnHex));
	tblColor.SetField("rgb", LuaFunction(lua, fnRgb));
	tblColor.SetField("rgbFloat", LuaFunction(lua, fnRgbFloat));

	std::function<LUNAColor(LuaNil, float, float, float)> fnColorConsruct = [](LuaNil, float r, float g, float b)
	{
		return LUNAColor::Rgb(r, g, b, 1.0f);
	};

	LuaTable metaColor(lua);
	metaColor.SetField("__call", LuaFunction(lua, fnColorConsruct));
	tblColor.SetMetatable(metaColor);

	tblGraphics.SetField("Color", tblColor);

	tblGraphics.MakeReadOnly();
	tblLuna.SetField("graphics", tblGraphics);
}

double LUNAGraphics::SmoothDeltaTime(double deltaTime)
{
	movAvgDelta = (deltaTime + movAvgDelta * (MOV_AVG_PERIOD - 1)) / MOV_AVG_PERIOD;

	return movAvgDelta;
}

LUNARenderer* LUNAGraphics::GetRenderer()
{
	return &renderer;
}

const std::shared_ptr<LUNACamera>& LUNAGraphics::GetCamera()
{
	return camera;
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
	return renderer.GetRenderCalls();
}

int LUNAGraphics::GetRenderedVertexes()
{
	return renderer.GetRenderedVertexes();
}

void LUNAGraphics::ResetLastTime()
{
	lastTime = LUNAEngine::SharedPlatformUtils()->GetSystemTime();
}

void LUNAGraphics::SetBackgroundColor(float r, float g, float b)
{	
	renderer.SetBackgroundColor(LUNAColor::RgbFloat(r / 255.0f, g / 255.0f, b / 255.0f));
}

// Run given action after render current frame
void LUNAGraphics::RunAfterRender(const std::function<void()>& action)
{
	afterRenderActions.push_back(action);
}

bool LUNAGraphics::IsPaused()
{
	return paused;
}

void LUNAGraphics::OnPause()
{
	paused = true;
	LUNAEngine::SharedScenes()->OnPause();
}

void LUNAGraphics::OnResume()
{
	paused = false;
	ResetLastTime();
	LUNAEngine::SharedScenes()->OnResume();
}

void LUNAGraphics::OnUpdate()
{
	if(paused) return;

	// Calculate delta time
	double curTime = LUNAEngine::SharedPlatformUtils()->GetSystemTime();
	deltaTime = curTime - lastTime;
	lastTime = curTime;

	if(deltaTime > MAX_DELTA) deltaTime = MAX_DELTA;

	deltaTime = SmoothDeltaTime(deltaTime);

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
	renderer.BeginRender();
	LUNAEngine::SharedScenes()->OnRender();
	renderer.EndRender();

	for(const auto& action : afterRenderActions) action();
	afterRenderActions.clear();
}
