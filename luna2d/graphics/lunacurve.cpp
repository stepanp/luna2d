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

#include "lunacurve.h"
#include "lunasplines.h"
#include "lunatextureregion.h"
#include "lunamath.h"

using namespace luna2d;

LUNACurve::LUNACurve(const LUNACurveParams& params)
{
	mesh = std::unique_ptr<LUNAMesh>(new LUNAMesh());

	SetParams(params);
}

void LUNACurve::SetupReadFunctions()
{
	auto region = params.textureRegion.lock();

	float regionU1 = region->GetU1();
	float regionV1 = region->GetV1();
	float regionU2 = region->GetU2();
	float regionV2 = region->GetV2();

	if(params.textureMappingMode == LUNATextureMappingMode::STRETCH)
	{
		if(params.vertical)
		{
			getLt = [=](float progressBegin, float, int)
			{
				return glm::vec2(regionU2, math::Lerp(regionV1, regionV2, progressBegin));
			};

			getLb = [=](float progressBegin, float, int)
			{
				return glm::vec2(regionU1, math::Lerp(regionV1, regionV2, progressBegin));
			};

			getRt = [=](float, float progressEnd, int)
			{
				return glm::vec2(regionU2, math::Lerp(regionV1, regionV2, progressEnd));
			};

			getRb = [=](float, float progressEnd, int)
			{
				return glm::vec2(regionU1, math::Lerp(regionV1, regionV2, progressEnd));
			};
		}
		else
		{
			getLt = [=](float progressBegin, float, int)
			{
				return glm::vec2(math::Lerp(regionU1, regionU2, progressBegin), regionV2);
			};

			getLb = [=](float progressBegin, float, int)
			{
				return glm::vec2(math::Lerp(regionU1, regionU2, progressBegin), regionV1);
			};

			getRt = [=](float, float progressEnd, int)
			{
				return glm::vec2(math::Lerp(regionU1, regionU2, progressEnd), regionV2);
			};

			getRb = [=](float, float progressEnd, int)
			{
				return glm::vec2(math::Lerp(regionU1, regionU2, progressEnd), regionV1);
			};
		}
	}

	else if(params.textureMappingMode == LUNATextureMappingMode::REPEAT)
	{
		if(params.vertical)
		{
			getLt = [=](float, float, int segmentIndex)
			{
				float t = 0.0f;

				if(params.segmentsToRepeat > 1)
				{
					int index = segmentIndex % params.segmentsToRepeat;
					t = (1.0f / (float)params.segmentsToRepeat) * index;
				}

				return glm::vec2(regionU2, math::Lerp(regionV1, regionV2, t));
			};

			getLb = [=](float, float, int segmentIndex)
			{
				float t = 0.0f;

				if(params.segmentsToRepeat > 1)
				{
					int index = segmentIndex % params.segmentsToRepeat;
					t = (1.0f / (float)params.segmentsToRepeat) * index;
				}

				return glm::vec2(regionU1, math::Lerp(regionV1, regionV2, t));
			};

			getRt = [=](float, float, int segmentIndex)
			{
				float t = 1.0f;

				if(params.segmentsToRepeat > 1)
				{
					int index = segmentIndex % params.segmentsToRepeat;
					t = (1.0f / (float)params.segmentsToRepeat) * (index + 1);
				}

				return glm::vec2(regionU2, math::Lerp(regionV1, regionV2, t));
			};

			getRb = [=](float, float, int segmentIndex)
			{
				float t = 1.0f;

				if(params.segmentsToRepeat > 1)
				{
					int index = segmentIndex % params.segmentsToRepeat;
					t = (1.0f / (float)params.segmentsToRepeat) * (index + 1);
				}

				return glm::vec2(regionU1, math::Lerp(regionV1, regionV2, t));
			};
		}
		else
		{
			getLt = [=](float, float, int segmentIndex)
			{
				float t = 0.0f;

				if(params.segmentsToRepeat > 1)
				{
					int index = segmentIndex % params.segmentsToRepeat;
					t = (1.0f / (float)params.segmentsToRepeat) * index;
				}

				return glm::vec2(math::Lerp(regionU1, regionU2, t), regionV2);
			};

			getLb = [=](float, float, int segmentIndex)
			{
				float t = 0.0f;

				if(params.segmentsToRepeat > 1)
				{
					int index = segmentIndex % params.segmentsToRepeat;
					t = (1.0f / (float)params.segmentsToRepeat) * index;
				}

				return glm::vec2(math::Lerp(regionU1, regionU2, t), regionV1);
			};

			getRt = [=](float, float, int segmentIndex)
			{
				float t = 1.0f;

				if(params.segmentsToRepeat > 1)
				{
					int index = segmentIndex % params.segmentsToRepeat;
					t = (1.0f / (float)params.segmentsToRepeat) * (index + 1);
				}

				return glm::vec2(math::Lerp(regionU1, regionU2, t), regionV2);
			};

			getRb = [=](float, float, int segmentIndex)
			{
				float t = 1.0f;

				if(params.segmentsToRepeat > 1)
				{
					int index = segmentIndex % params.segmentsToRepeat;
					t = (1.0f / (float)params.segmentsToRepeat) * (index + 1);
				}

				return glm::vec2(math::Lerp(regionU1, regionU2, t), regionV1);
			};
		}
	}
}

// Build curve mesh by knots
void LUNACurve::Build()
{
	if(knots.size() < 3)
	{
		//LUNA_LOGE("Cannot build curve by less than 3 knots");
		mesh->Clear();
		return;
	}

	if(params.textureRegion.expired()) return;

	//----------------------
	// Build spline by knots
	//----------------------
	std::vector<glm::vec2> points;
	std::vector<float> lenghts;
	float lenSumm = 0;

	points.push_back(knots[0]);
	for(size_t i = 1; i < knots.size() - 1; i++)
	{
		glm::vec2 knot0 = knots[i - 1];
		glm::vec2 knot1 = knots[i];
		glm::vec2 knot2 = knots[i + 1];
		float startT = (i == 1 ? 0.0f : smoothFactor);

		for(float t = startT; t <= 1.0f; t += smoothFactor)
		{
			points.push_back(splines::QuadraticBSpline(knot0, knot1, knot2, t));

			// Calculate lenght for each segment and segments lenghts summ for texture mapping
			float segmentLen = glm::distance2(points[points.size() - 1], points[points.size() - 2]);
			lenghts.push_back(segmentLen);
			lenSumm += segmentLen;
		}
	}
	points.push_back(knots[knots.size() - 1]);
	float segmentLen = glm::distance2(points[points.size() - 1], points[points.size() - 2]);
	lenghts.push_back(segmentLen);
	lenSumm += segmentLen;

	//---------------------
	// Build mesh by spline
	//---------------------
	float halfWidth = params.width / 2.0f;
	glm::vec2 prevA, prevB;
	float builtLen = 0;

	mesh->Clear();
	for(size_t i = 1; i < points.size(); i++)
	{
		glm::vec2 a, b, c, d;

		// First segment
		if(i == 1)
		{
			glm::vec2 point1 = points[i - 1];
			glm::vec2 point2 = points[i];
			glm::vec2 perp = (point2 - point1);
			perp = glm::normalize(glm::vec2(-perp.y, perp.x));
			perp *= halfWidth;

			a = point1 + perp;
			b = point1 - perp;
			c = point2 + perp;
			d = point2 - perp;
		}

		// Last segment
		else if(i == points.size() - 1)
		{
			glm::vec2 point2 = points[i - 1];
			glm::vec2 point3 = points[i];
			glm::vec2 perp = (point3 - point2);
			perp = glm::normalize(glm::vec2(-perp.y, perp.x));
			perp *= halfWidth;

			a = prevA;
			b = prevB;
			c = point3 + perp;
			d = point3 - perp;
		}

		// Common segments
		else
		{
			glm::vec2 point1 = points[i - 1];
			glm::vec2 point2 = points[i];
			glm::vec2 point3 = points[i + 1];
			glm::vec2 perp = (point3 - point1);
			perp = glm::normalize(glm::vec2(-perp.y, perp.x));
			perp *= halfWidth;

			a = prevA;
			b = prevB;
			c = point2 + perp;
			d = point2 - perp;
		}

		float segmentLen = lenghts[i - 1];
		float progressBegin = builtLen / lenSumm;
		float progressEnd = (builtLen + segmentLen) / lenSumm;

		glm::vec2 lt = getLt(progressBegin, progressEnd, i - 1);
		glm::vec2 lb = getLb(progressBegin, progressEnd, i - 1);
		glm::vec2 rt = getRt(progressBegin, progressEnd, i - 1);
		glm::vec2 rb = getRb(progressBegin, progressEnd, i - 1);

		mesh->AddVertex(a.x, a.y, color.r, color.g, color.b, color.a, lt.x, lt.y);
		mesh->AddVertex(b.x, b.y, color.r, color.g, color.b, color.a, lb.x, lb.y);
		mesh->AddVertex(c.x, c.y, color.r, color.g, color.b, color.a, rt.x, rt.y);
		mesh->AddVertex(b.x, b.y, color.r, color.g, color.b, color.a, lb.x, lb.y);
		mesh->AddVertex(c.x, c.y, color.r, color.g, color.b, color.a, rt.x, rt.y);
		mesh->AddVertex(d.x, d.y, color.r, color.g, color.b, color.a, rb.x, rb.y);

		prevA = c;
		prevB = d;
		builtLen += segmentLen;
	}

	needBuild = false;
}

void LUNACurve::SetParams(const LUNACurveParams& params)
{
	auto region = params.textureRegion.lock();

	if(!region) LUNA_RETURN_ERR("Attempt to set invalid curve params");

	this->params = params;
	mesh->SetTexture(region->GetTexture());
	SetupReadFunctions();
	needBuild = true;
}

void LUNACurve::ClearKnots()
{
	knots.clear();
	needBuild = true;
}

int LUNACurve::GetKnotsCount()
{
	return knots.size();
}

const std::vector<glm::vec2>& LUNACurve::GetKnots()
{
	return knots;
}

void LUNACurve::SetKnots(const std::vector<glm::vec2>& knots)
{
	this->knots.clear();
	this->knots.insert(this->knots.begin(), knots.begin(), knots.end());
	needBuild = true;
}

void LUNACurve::AddKnot(float x, float y)
{
	knots.push_back(glm::vec2(x, y));
	needBuild = true;
}

void LUNACurve::RemoveKnot(int index)
{
	if(index < 0 || index >= (int)knots.size())
	{
		LUNA_LOGE("Knot index \"%d\" is out of range", index);
		return;
	}

	knots.erase(knots.begin() + index);
	needBuild = true;
}

void LUNACurve::SetKnot(int index, float x, float y)
{
	if(index < 0 || index >= (int)knots.size())
	{
		LUNA_LOGE("Knot index \"%d\" is out of range", index);
		return;
	}

	knots[index].x = x;
	knots[index].y = y;
	needBuild = true;
}

LUNAColor LUNACurve::GetColor()
{
	return color;
}

void LUNACurve::SetColor(float r, float g, float b)
{
	color.r = r / 255.0f;
	color.g = g / 255.0f;
	color.b = b / 255.0f;
	needBuild = true;
}

float LUNACurve::GetAlpha()
{
	return color.a;
}

void LUNACurve::SetAlpha(float alpha)
{
	color.a = alpha;
	needBuild = true;
}

float LUNACurve::GetSmoothFactor()
{
	return smoothFactor;
}

void LUNACurve::SetSmoothFactor(float factor)
{
	smoothFactor = factor;
}

void LUNACurve::Render()
{
	if(needBuild) Build();
	mesh->Render();
}


void LuaStack<LUNACurveParams>::Push(lua_State* luaVm, const LUNACurveParams& params)
{
	LuaTable table(luaVm);

	table.SetField("textureRegion", params.textureRegion);
	table.SetField("textureRegionHead", params.textureRegionHead);
	table.SetField("textureRegionTail", params.textureRegionTail);
	table.SetField("textureMappingMode", TEXTURE_MAPPING_MODE.FromEnum(params.textureMappingMode));
	table.SetField("vertical", params.vertical);
	table.SetField("width", params.width);
	table.SetField("segmentsToRepeat", params.segmentsToRepeat);

	LuaStack<LuaTable>::Push(luaVm, table);
}

LUNACurveParams LuaStack<LUNACurveParams>::Pop(lua_State* luaVm, int index)
{
	LuaTable table = LuaStack<LuaTable>::Pop(luaVm, index);
	if(table == nil) return {};

	LUNACurveParams params;

	params.textureRegion = table.GetField<decltype(LUNACurveParams::textureRegion)>("textureRegion");
	params.textureRegionHead = table.GetField<decltype(LUNACurveParams::textureRegionHead)>("textureRegionHead");
	params.textureRegionTail = table.GetField<decltype(LUNACurveParams::textureRegionTail)>("textureRegionTail");
	params.vertical = table.GetBool("vertical");
	params.width = table.GetFloat("width");
	params.segmentsToRepeat = table.GetInt("segmentsToRepeat");

	std::string strMappingMode = table.GetString("textureMappingMode");
	if(TEXTURE_MAPPING_MODE.HasKey(strMappingMode)) params.textureMappingMode = TEXTURE_MAPPING_MODE.FromString(strMappingMode);

	return params;
}
