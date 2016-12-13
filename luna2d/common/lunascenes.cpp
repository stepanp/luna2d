//-----------------------------------------------------------------------------
// luna2d engine
// Copyright 2014-2016 Stepan Prokofjev
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

#include "lunascenes.h"
#include "lunaengine.h"

using namespace luna2d;

LUNAScenes::LUNAScenes() :
	tblCurScene(nil),
	fnUpdate(nil),
	fnRender(nil),
	fnTouchDown(nil),
	fnTouchMoved(nil),
	fnTouchUp(nil)
{
	LuaScript* lua = LUNAEngine::SharedLua();

	// Register "luna.scenes" module
	LuaTable tblLuna = lua->GetGlobalTable().GetTable("luna");
	LuaTable tblScenes(lua);

	tblScenes.SetField("getCurrentScene", LuaFunction(lua, this, &LUNAScenes::GetCurrrentScene));
	tblScenes.SetField("setScene", LuaFunction(lua, this, &LUNAScenes::SetScene));

	tblLuna.SetField("scenes", tblScenes);
}

LuaTable LUNAScenes::GetCurrrentScene()
{
	return tblCurScene;
}

void LUNAScenes::SetScene(const LuaTable& tblScene)
{
	if(tblScene == nil)
	{
		LUNA_LOGE("Scene object is nil");
		return;
	}

	if(!tblScene.HasField("onUpdate"))
	{
		LUNA_LOGE("Scene hasn'n \"onUpdate\" method");
		return;
	}

	if(!tblScene.HasField("onRender"))
	{
		LUNA_LOGE("Scene hasn'n \"onRender\" method");
		return;
	}

	if(tblCurScene)
	{
		auto fnLeave = tblCurScene.GetFunction("onLeave");
		if(fnLeave) fnLeave.CallVoid(tblCurScene);
	}

	auto onEnter = tblScene.GetFunction("onEnter");
	if(onEnter) onEnter.CallVoid(tblScene);

	tblCurScene = tblScene;

	// Cache refs to scene methods
	fnPause = tblCurScene.GetFunction("onPause");
	fnResume = tblCurScene.GetFunction("onResume");
	fnUpdate = tblCurScene.GetFunction("onUpdate");
	fnRender = tblCurScene.GetFunction("onRender");
	fnTouchDown = tblCurScene.GetFunction("onTouchDown");
	fnTouchMoved = tblCurScene.GetFunction("onTouchMoved");
	fnTouchUp = tblCurScene.GetFunction("onTouchUp");
}

void LUNAScenes::OnPause()
{
	if(fnPause != nil) fnPause.CallVoid(tblCurScene);
}

void LUNAScenes::OnResume()
{
	if(fnResume != nil) fnResume.CallVoid(tblCurScene);
}

void LUNAScenes::OnTouchDown(float x, float y, int touchIndex)
{
	if(fnTouchDown != nil) fnTouchDown.CallVoid(tblCurScene, x, y, touchIndex);
}

void LUNAScenes::OnTouchMoved(float x, float y, int touchIndex)
{
	if(fnTouchMoved != nil) fnTouchMoved.CallVoid(tblCurScene, x, y, touchIndex);
}

void LUNAScenes::OnTouchUp(float x, float y, int touchIndex)
{
	if(fnTouchUp != nil) fnTouchUp.CallVoid(tblCurScene, x, y, touchIndex);
}

void LUNAScenes::OnUpdate(float deltaTime)
{
	if(tblCurScene != nil) fnUpdate.CallVoid(tblCurScene, deltaTime);
}

void LUNAScenes::OnRender()
{
	if(tblCurScene != nil) fnRender.CallVoid(tblCurScene);
}
