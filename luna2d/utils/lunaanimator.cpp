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

#include "lunaanimator.h"
#include "lunaanimatoractions.h"

using namespace luna2d;

// Check for given params is action params
bool IsAction(const LuaAny& params)
{
	if(params.GetType() == LUA_TFUNCTION) return true;

	LuaTable tblParams = params.ToTable();
	return tblParams && tblParams.HasField("action");
}

// Check for given params is sequence of actions
bool IsSequence(const LuaTable& params)
{
	return params && IsAction(params.GetArrayField<LuaAny>(1));
}

// Check for given params is timeline
bool IsTimeline(const LuaTable& params)
{
	return params && IsSequence(params.GetArrayField<LuaTable>(1));
}

// Create LUNAAction object from lua params table
std::shared_ptr<LUNAAction> CreateAction(const LuaAny& params)
{
	if(params.GetType() == LUA_TFUNCTION) return std::make_shared<LUNAActionFunction>(params.ToFunction());

	if(!IsAction(params)) return nullptr;

	LuaTable tblParams = params.ToTable();
	std::string name = tblParams.GetString("action");

	if(name == "move") return std::make_shared<LUNAActionMove>(tblParams);
	else if(name == "fade") return std::make_shared<LUNAActionFade>(tblParams);
	else if(name == "scale") return std::make_shared<LUNAActionScale>(tblParams);
	else if(name == "rotate") return std::make_shared<LUNAActionRotate>(tblParams);
	else if(name == "color") return std::make_shared<LUNAActionColor>(tblParams);
	else if(name == "shake") return std::make_shared<LUNAActionShake>(tblParams);
	else if(name == "wait") return std::make_shared<LUNAActionWait>(tblParams);
	else if(name == "custom") return std::make_shared<LUNAActionCustom>(tblParams);

	LUNA_LOGE("Unknown animator action \"%s\"", name.c_str());

	return nullptr;
}

// Create LUNASequence object from lua params table
std::shared_ptr<LUNASequence> CreateSequence(const LuaTable& params)
{
	std::shared_ptr<LUNASequence> ret = std::make_shared<LUNASequence>();

	int count = params.GetArrayCount();
	for(int i = 1; i <= count; i++)
	{
		auto action = CreateAction(params.GetArrayField<LuaAny>(i));
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

LUNAAction::LUNAAction(float time) :
	time(0),
	totalTime(time)
{
}

float LUNAAction::GetPercent()
{
	return time / totalTime;
}

float LUNAAction::GetTime()
{
	return time;
}

bool LUNAAction::IsDone()
{
	return time >= totalTime;
}

// Reset sequence to initial position
void LUNAAction::Reset()
{
	time = 0;
}

float LUNAAction::Update(float deltaTime)
{
	float extraTime = 0;

	time += deltaTime;
	if(time >= totalTime)
	{
		extraTime = time - totalTime;
		time = totalTime;
	}
	OnUpdate();

	return extraTime;
}


//--------------------
// Sequence of actions
//--------------------
void LUNASequence::AddAction(const std::shared_ptr<LUNAAction>& action)
{
	actions.push_back(action);
}

bool LUNASequence::IsDone()
{
	return curAction >= actions.size();
}

// Reset sequence to initial position
void LUNASequence::Reset()
{
	curAction = 0;
	for(auto& action : actions) action->Reset();
}

void LUNASequence::Update(float deltaTime)
{
	float dt = deltaTime;

	while(true)
	{
		if(curAction >= actions.size()) break;

		auto& action = actions[curAction];
		dt = action->Update(dt);

		if(!action->IsDone()) break;

		curAction++;
	}
}


LUNAAnimator::LUNAAnimator(const LuaAny& params)
{
	// Params is just one action
	if(IsAction(params))
	{
		auto action = CreateAction(params);
		if(!action) LUNA_RETURN_ERR("Attempt to create animator from invalid action");

		auto seq = std::make_shared<LUNASequence>();
		seq->AddAction(action);
		sequences.push_back(seq);
		return;
	}

	LuaTable tblParams = params.ToTable();
	if(tblParams)
	{
		// Params is one sequence
		if(IsSequence(tblParams))
		{
			auto seq = CreateSequence(tblParams);
			if(!seq) LUNA_RETURN_ERR("Attempt to create animator from invalid sequence");
			sequences.push_back(seq);
			return;
		}

		// Params is full timeline
		if(IsTimeline(tblParams))
		{
			int count = tblParams.GetArrayCount();
			for(int i = 1; i <= count; i++)
			{
				auto seq = CreateSequence(tblParams.GetArrayField<LuaTable>(i));
				if(seq) sequences.push_back(seq);
			}

			return;
		}
	}

	LUNA_LOGE("Attempt to create animator from invalid params");
}

bool LUNAAnimator::IsLoop()
{
	return loop;
}

void LUNAAnimator::SetLoop(bool loop)
{
	this->loop = loop;
}

bool LUNAAnimator::IsRunning()
{
	return running;
}

void LUNAAnimator::Start()
{
	running = true;
}

void LUNAAnimator::Pause()
{
	running = false;
}

void LUNAAnimator::Stop()
{
	running = false;
	for(auto& seq : sequences) seq->Reset();
}

void LUNAAnimator::Update(float deltaTime)
{
	if(!running) return;

	// Update sequences
	bool done = true;
	for(auto& seq : sequences)
	{
		seq->Update(deltaTime);
		if(!seq->IsDone()) done = false;
	}

	// Handle timeline ending
	if(done)
	{
		Stop();
		if(loop) Start();
	}
}
