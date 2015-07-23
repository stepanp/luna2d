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

#include "lunascenes.h"
#include "lunaengine.h"

using namespace luna2d;

LUNAScenes::LUNAScenes()
{
	SqVm* sq = LUNAEngine::SharedSquirrel();

	// Register "luna.scenes" module
	SqTable tblLuna = sq->GetRootTable().GetTable("luna");
	SqTable tblScenes(sq);

	tblScenes.NewSlot("getCurrentScene", SqFunction(sq, this, &LUNAScenes::GetCurrrentScene));
	tblScenes.NewSlot("setScene", SqFunction(sq, this, &LUNAScenes::SetScene));

	tblLuna.NewSlot("scenes", tblScenes);
}

SqTable LUNAScenes::GetCurrrentScene()
{
	return tblCurScene;
}

void LUNAScenes::SetScene(const SqTable& tblScene)
{
	if(tblScene.IsNull())
	{
		LUNA_LOGE("Scene object is nil");
		return;
	}

	if(!tblScene.HasSlot("onUpdate"))
	{
		LUNA_LOGE("Scene hasn'n \"onUpdate\" method");
		return;
	}

	if(!tblScene.HasSlot("onRender"))
	{
		LUNA_LOGE("Scene hasn'n \"onRender\" method");
		return;
	}

	tblCurScene = tblScene;

	// Cache refs to scene methods
	fnUpdate = tblCurScene.GetFunction("onUpdate");
	fnRender = tblCurScene.GetFunction("onRender");
	fnTouchDown = tblCurScene.GetFunction("onTouchDown");
	fnTouchMoved = tblCurScene.GetFunction("onTouchMoved");
	fnTouchUp = tblCurScene.GetFunction("onTouchUp");
}

void LUNAScenes::OnTouchDown(float x, float y, int touchIndex)
{
	if(!fnTouchDown.IsNull()) fnTouchDown.CallWithEnv(tblCurScene, x, y, touchIndex);
}

void LUNAScenes::OnTouchMoved(float x, float y, int touchIndex)
{
	if(!fnTouchMoved.IsNull()) fnTouchMoved.CallWithEnv(tblCurScene, x, y, touchIndex);
}

void LUNAScenes::OnTouchUp(float x, float y, int touchIndex)
{
	if(!fnTouchUp.IsNull()) fnTouchUp.CallWithEnv(tblCurScene, x, y, touchIndex);
}

void LUNAScenes::OnUpdate(float dt)
{
	if(!tblCurScene.IsNull()) fnUpdate.CallWithEnv(tblCurScene, dt);
}

void LUNAScenes::OnRender()
{
	if(!tblCurScene.IsNull()) fnRender.CallWithEnv(tblCurScene);
}
