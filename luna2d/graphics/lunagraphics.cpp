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
#include "lunaassets.h"
#include "lunascenes.h"
#include "lunatextureatlas.h"
#include "lunasprite.h"
#include "lunamesh.h"

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
	LuaScript *lua = LUNAEngine::SharedLua();
	LuaTable tblLuna = lua->GetGlobalTable().GetTable("luna");
	LuaTable tblGraphics(lua);

	tblGraphics.SetField("getFps", LuaFunction(lua, this, &LUNAGraphics::GetFps));
	tblGraphics.SetField("getDeltaTime", LuaFunction(lua, this, &LUNAGraphics::GetDeltaTime));
	tblGraphics.SetField("setBackgroundColor", LuaFunction(lua, this, &LUNAGraphics::SetBackgroundColor));
	tblGraphics.SetField("getTextureInfo", LuaFunction(lua, this, &LUNAGraphics::GetTextureInfo));
	tblGraphics.SetField("getAtlasTexture", LuaFunction(lua, this, &LUNAGraphics::GetAtlasTexture));
	tblGraphics.SetField("getTextureRegionInfo", LuaFunction(lua, this, &LUNAGraphics::GetTextureRegionInfo));
	tblGraphics.SetField("enableDebugRender", LuaFunction(lua, renderer, &LUNARenderer::EnableDebugRender));
	tblGraphics.SetField("renderLine", LuaFunction(lua, renderer, &LUNARenderer::RenderLine));

	// Register sprite
	LuaClass<LUNASprite> clsSprite(lua);
	clsSprite.SetConstructor<int>();
	clsSprite.SetMethod("render", &LUNASprite::Render);
	clsSprite.SetMethod("getX", &LUNASprite::GetX);
	clsSprite.SetMethod("getY", &LUNASprite::GetY);
	clsSprite.SetMethod("setX", &LUNASprite::SetX);
	clsSprite.SetMethod("setY", &LUNASprite::SetY);
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
	tblGraphics.SetField("Sprite", clsSprite);

	// Register mesh
	LuaClass<LUNAMesh> clsMesh(lua);
	clsMesh.SetConstructor<int>();
	clsMesh.SetMethod("clear", &LUNAMesh::Clear);
	clsMesh.SetMethod("setTexture", &LUNAMesh::SetTexture);
	clsMesh.SetMethod("addVertex", &LUNAMesh::AddVertex);
	clsMesh.SetMethod("render", &LUNAMesh::Render);
	tblGraphics.SetField("Mesh", clsMesh);

	tblGraphics.MakeReadOnly();
	tblLuna.SetField("graphics", tblGraphics);
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

void LUNAGraphics::SetBackgroundColor(float r, float g, float b)
{
	renderer->SetBackgroundColor(LUNAColor::RgbFloat(r / 255.0f, g / 255.0f, b / 255.0f));
}

LuaTable LUNAGraphics::GetTextureInfo(int assetId)
{
	auto weakTexture = LUNAEngine::SharedAssets()->GetAsset<LUNATexture>(assetId, LUNAAssetType::TEXTURE);
	if(weakTexture.expired()) return nil;

	auto texture = weakTexture.lock();
	LuaTable tblRet(LUNAEngine::SharedLua());
	tblRet.SetField("width", texture->GetWidth());
	tblRet.SetField("hegiht", texture->GetHeight());

	return tblRet;
}

int LUNAGraphics::GetAtlasTexture(const LuaTable& atlasTable)
{
	LuaTable meta = atlasTable.GetMetatable();

	if(!meta.HasField("assets")) return 0;
	return meta.GetInt("textureId");
}

LuaTable LUNAGraphics::GetTextureRegionInfo(int assetId)
{
	auto weakRegion = LUNAEngine::SharedAssets()->GetAsset<LUNATextureRegion>(assetId, LUNAAssetType::TEXTURE_REGION);
	if(weakRegion.expired()) return nil;

	auto region = weakRegion.lock();
	LuaTable tblRet(LUNAEngine::SharedLua());
	tblRet.SetField("u1", region->GetU1());
	tblRet.SetField("v1", region->GetV1());
	tblRet.SetField("u2", region->GetU2());
	tblRet.SetField("v2", region->GetV2());

	return tblRet;
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

	//LUNA_LOG("Render calls: %d", renderer->GetRenderCalls());
}
