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

#include "lunaanimator.h"
#include "lunaanimatoractions.h"

using namespace luna2d;

// Check for given table is action params
bool IsAction(const LuaTable& params)
{
	return params && params.HasField("action");
}

// Check for given table is sequence of actions
bool IsSequence(const LuaTable& params)
{
	return params && IsAction(params.GetArrayField<LuaTable>(1));
}

// Create LUNAAction object from lua params table
std::shared_ptr<LUNAAction> CreateAction(const LuaTable& params)
{
	if(!IsAction(params)) return nullptr;

	std::string name = params.GetString("action");
	if(name == "move") return std::make_shared<LUNAActionMove>(params);
	else if(name == "fade") return std::make_shared<LUNAActionFade>(params);
	else if(name == "scale") return std::make_shared<LUNAActionScale>(params);
	else if(name == "rotate") return std::make_shared<LUNAActionRotate>(params);
	else if(name == "color") return std::make_shared<LUNAActionColor>(params);
	else if(name == "wait") return std::make_shared<LUNAActionWait>(params);
	else if(name == "custom") return std::make_shared<LUNAActionCustom>(params);

	LUNA_LOGE("Unknown animator action \"%s\"", name.c_str());

	return nullptr;
}

// Create LUNASequence object from lua params table
std::shared_ptr<LUNASequence> CreateSequence(const LuaTable& params)
{
	std::shared_ptr<LUNASequence> ret = std::make_shared<LUNASequence>();

	int count = params.GetArrayCount();
	for(int i = 1; i < count; i++)
	{
		auto action = CreateAction(params.GetArrayField<LuaTable>(i));
		if(action) ret->AddAction(action);
		else LUNA_LOGE("Invalid action in animator sequence");
	}

	return ret;
}

//--------------------------------
// Base class for animator actions
//--------------------------------
LUNAAction::LUNAAction(const LuaTable &params) :
	time(0),
	totalTime(params.GetFloat("time"))
{
}

float LUNAAction::GetPercent()
{
	return time / totalTime;
}

bool LUNAAction::IsDone()
{
	return time >= totalTime;
}

void LUNAAction::Update(float deltaTime)
{
	time += deltaTime;
	if(time >= totalTime) time = totalTime;
	OnUpdate();
}


//--------------------
// Sequence of actions
//--------------------
void LUNASequence::AddAction(const std::shared_ptr<LUNAAction>& action)
{
	actions.push_back(action);
}

void LUNASequence::Update(float deltaTime)
{
	if(curAction >= actions.size()) return;

	auto& action = actions[curAction];
	action->Update(deltaTime);
	if(action->IsDone()) curAction++;
}


LUNAAnimator::LUNAAnimator(const LuaTable& params)
{
	if(!params) LUNA_RETURN_ERR("Attempt to create animator from invalid parameters table");

	// Params is just only action
	if(IsAction(params))
	{
		auto action = CreateAction(params);
		if(!action) LUNA_RETURN_ERR("Attempt to create animator with invalid action");

		auto seq = std::make_shared<LUNASequence>();
		seq->AddAction(action);
		sequences.push_back(seq);
		return;
	}

	/*// Params is only sequence
	if(IsSequence(params))
	{
		sequences.push_back(CreateSequence(params));
		return;
	}

	// Params full timeline of sequences
	int count = params.GetArrayCount();
	for(int i = 1; i < count; i++)
	{
		auto seq = CreateSequence(params.GetArrayField<LuaTable>(i));
		if(seq) sequences.push_back(seq);
	}*/
}

void LUNAAnimator::Update(float deltaTime)
{
	for(auto& sequence : sequences) sequence->Update(deltaTime);
}
