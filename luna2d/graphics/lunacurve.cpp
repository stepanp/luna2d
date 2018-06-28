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

void LUNACurve::SetupReadFunctions(LUNATextureMappingMode textureMappingMode)
{
	if(textureMappingMode == LUNATextureMappingMode::STRETCH)
	{
		if(params.vertical)
		{
			getLt = [](float progressBegin, float, int, const RegionData& region)
			{
				return glm::vec2(region.u2, math::Lerp(region.v1, region.v2, progressBegin));
			};

			getLb = [](float progressBegin, float, int, const RegionData& region)
			{
				return glm::vec2(region.u1, math::Lerp(region.v1, region.v2, progressBegin));
			};

			getRt = [](float, float progressEnd, int, const RegionData& region)
			{
				return glm::vec2(region.u2, math::Lerp(region.v1, region.v2, progressEnd));
			};

			getRb = [](float, float progressEnd, int, const RegionData& region)
			{
				return glm::vec2(region.u1, math::Lerp(region.v1, region.v2, progressEnd));
			};
		}
		else
		{
			getLt = [](float progressBegin, float, int, const RegionData& region)
			{
				return glm::vec2(math::Lerp(region.u1, region.u2, progressBegin), region.v1);
			};

			getLb = [](float progressBegin, float, int, const RegionData& region)
			{
				return glm::vec2(math::Lerp(region.u1, region.u2, progressBegin), region.v2);
			};

			getRt = [](float, float progressEnd, int, const RegionData& region)
			{
				return glm::vec2(math::Lerp(region.u1, region.u2, progressEnd), region.v1);
			};

			getRb = [](float, float progressEnd, int, const RegionData& region)
			{
				return glm::vec2(math::Lerp(region.u1, region.u2, progressEnd), region.v2);
			};
		}
	}

	else if(params.textureMappingMode == LUNATextureMappingMode::REPEAT)
	{
		if(params.vertical)
		{
			getLt = [](float, float, int segmentIndex, const RegionData& region)
			{
				float t = 0.0f;

				if(region.segmentsToRepeat > 1)
				{
					int index = segmentIndex % region.segmentsToRepeat;
					t = (1.0f / (float)region.segmentsToRepeat) * index;
				}

				return glm::vec2(region.u2, math::Lerp(region.v1, region.v2, t));
			};

			getLb = [](float, float, int segmentIndex, const RegionData& region)
			{
				float t = 0.0f;

				if(region.segmentsToRepeat > 1)
				{
					int index = segmentIndex % region.segmentsToRepeat;
					t = (1.0f / (float)region.segmentsToRepeat) * index;
				}

				return glm::vec2(region.u1, math::Lerp(region.v1, region.v2, t));
			};

			getRt = [](float, float, int segmentIndex, const RegionData& region)
			{
				float t = 1.0f;

				if(region.segmentsToRepeat > 1)
				{
					int index = segmentIndex % region.segmentsToRepeat;
					t = (1.0f / (float)region.segmentsToRepeat) * (index + 1);
				}

				return glm::vec2(region.u2, math::Lerp(region.v1, region.v2, t));
			};

			getRb = [](float, float, int segmentIndex, const RegionData& region)
			{
				float t = 1.0f;

				if(region.segmentsToRepeat > 1)
				{
					int index = segmentIndex % region.segmentsToRepeat;
					t = (1.0f / (float)region.segmentsToRepeat) * (index + 1);
				}

				return glm::vec2(region.u1, math::Lerp(region.v1, region.v2, t));
			};
		}
		else
		{
			getLt = [](float, float, int segmentIndex, const RegionData& region)
			{
				float t = 0.0f;

				if(region.segmentsToRepeat > 1)
				{
					int index = segmentIndex % region.segmentsToRepeat;
					t = (1.0f / (float)region.segmentsToRepeat) * index;
				}

				return glm::vec2(math::Lerp(region.u1, region.u2, t), region.v1);
			};

			getLb = [](float, float, int segmentIndex, const RegionData& region)
			{
				float t = 0.0f;

				if(region.segmentsToRepeat > 1)
				{
					int index = segmentIndex % region.segmentsToRepeat;
					t = (1.0f / (float)region.segmentsToRepeat) * index;
				}

				return glm::vec2(math::Lerp(region.u1, region.u2, t), region.v2);
			};

			getRt = [](float, float, int segmentIndex, const RegionData& region)
			{
				float t = 1.0f;

				if(region.segmentsToRepeat > 1)
				{
					int index = segmentIndex % region.segmentsToRepeat;
					t = (1.0f / (float)region.segmentsToRepeat) * (index + 1);
				}

				return glm::vec2(math::Lerp(region.u1, region.u2, t), region.v1);
			};

			getRb = [](float, float, int segmentIndex, const RegionData& region)
			{
				float t = 1.0f;

				if(region.segmentsToRepeat > 1)
				{
					int index = segmentIndex % region.segmentsToRepeat;
					t = (1.0f / (float)region.segmentsToRepeat) * (index + 1);
				}

				return glm::vec2(math::Lerp(region.u1, region.u2, t), region.v2);
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


	size_t headSegments = std::min((size_t)params.headSegments, points.size() - 1);
	size_t tailSegments = std::min((size_t)params.tailSegments, points.size() - 1 - headSegments);

	float headLen = 0;
	float tailLen = 0;
	float commonLen = 0;

	for(int i = 0; i < headSegments; i++) headLen += lenghts[i];
	for(int i = 0; i < tailSegments; i++) tailLen += lenghts[lenghts.size() - i - 1];
	commonLen = lenSumm - headLen - tailLen;

	//---------------------
	// Build mesh by spline
	//---------------------
	float commonHalfWidth = params.width / 2.0f;
	glm::vec2 prevA, prevB;

	auto headRegion = params.textureRegionHead.lock();
	auto region = params.textureRegion.lock();
	auto tailRegion = params.textureRegionTail.lock();

	RegionData regionData;
	SegmentMode segmentMode = SegmentMode::NONE;

	float lenPointer = 0.0f, commonLenPointer = 0.0f;
	float totalLen = lenSumm;

	mesh->Clear();
	for(size_t i = 1; i < points.size(); i++)
	{
		glm::vec2 a, b, c, d;

		if(i <= headSegments && headRegion && segmentMode != SegmentMode::HEAD)
		{
			regionData.u1 = headRegion->GetU1();
			regionData.v1 = headRegion->GetV1();
			regionData.u2 = headRegion->GetU2();
			regionData.v2 = headRegion->GetV2();
			regionData.segmentsToRepeat = params.headSegments;

			segmentMode = SegmentMode::HEAD;
			SetupReadFunctions(LUNATextureMappingMode::STRETCH);

			lenPointer = 0.0f;
			totalLen = headLen;
		}

		else if(i > points.size() - 1 - tailSegments && tailRegion && segmentMode != SegmentMode::TAIL)
		{
			regionData.u1 = tailRegion->GetU1();
			regionData.v1 = tailRegion->GetV1();
			regionData.u2 = tailRegion->GetU2();
			regionData.v2 = tailRegion->GetV2();
			regionData.segmentsToRepeat = params.tailSegments;

			segmentMode = SegmentMode::TAIL;
			SetupReadFunctions(LUNATextureMappingMode::STRETCH);

			lenPointer = 0.0f;
			totalLen = tailLen;
		}

		else if(i > params.headSegments && segmentMode != SegmentMode::COMMON && segmentMode != SegmentMode::TAIL)
		{
			regionData.u1 = region->GetU1();
			regionData.v1 = region->GetV1();
			regionData.u2 = region->GetU2();
			regionData.v2 = region->GetV2();
			regionData.segmentsToRepeat = params.segmentsToRepeat;

			segmentMode = SegmentMode::COMMON;
			SetupReadFunctions(params.textureMappingMode);

			lenPointer = 0.0f;
			totalLen = commonLen;
		}

		float halfWidthEnd = GetWidthForPercent(commonLenPointer + segmentLen, lenSumm) / 2.0f;

		// First segment
		if(i == 1)
		{
			float halfWidthBegin = GetWidthForPercent(commonLenPointer, lenSumm) / 2.0f;

			glm::vec2 point1 = points[i - 1];
			glm::vec2 point2 = points[i];
			glm::vec2 perp = (point2 - point1);
			perp = glm::normalize(glm::vec2(-perp.y, perp.x));

			a = point1 + (perp * halfWidthBegin);
			b = point1 - (perp * halfWidthBegin);
			c = point2 + (perp * halfWidthEnd);
			d = point2 - (perp * halfWidthEnd);
		}

		// Last segment
		else if(i == points.size() - 1)
		{
			glm::vec2 point2 = points[i - 1];
			glm::vec2 point3 = points[i];
			glm::vec2 perp = (point3 - point2);
			perp = glm::normalize(glm::vec2(-perp.y, perp.x));
			perp *= halfWidthEnd;

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
			perp *= halfWidthEnd;

			a = prevA;
			b = prevB;
			c = point2 + perp;
			d = point2 - perp;
		}

		float segmentLen = lenghts[i - 1];
		float progressBegin = lenPointer / totalLen;
		float progressEnd = (lenPointer + segmentLen) / totalLen;

		glm::vec2 lt = getLt(progressBegin, progressEnd, i - 1, regionData);
		glm::vec2 lb = getLb(progressBegin, progressEnd, i - 1, regionData);
		glm::vec2 rt = getRt(progressBegin, progressEnd, i - 1, regionData);
		glm::vec2 rb = getRb(progressBegin, progressEnd, i - 1, regionData);

		mesh->AddVertex(a.x, a.y, color.r, color.g, color.b, color.a, lt.x, lt.y);
		mesh->AddVertex(b.x, b.y, color.r, color.g, color.b, color.a, lb.x, lb.y);
		mesh->AddVertex(c.x, c.y, color.r, color.g, color.b, color.a, rt.x, rt.y);
		mesh->AddVertex(b.x, b.y, color.r, color.g, color.b, color.a, lb.x, lb.y);
		mesh->AddVertex(c.x, c.y, color.r, color.g, color.b, color.a, rt.x, rt.y);
		mesh->AddVertex(d.x, d.y, color.r, color.g, color.b, color.a, rb.x, rb.y);

		prevA = c;
		prevB = d;
		lenPointer += segmentLen;
		commonLenPointer += segmentLen;
	}

	needBuild = false;
}

float LUNACurve::GetWidthForPercent(float lenPointer, float len)
{
	if(widthIntervals.empty()) return params.width;

	float beginScale = 1.0f;
	float endScale = 1.0f;
	float percent = lenPointer / len;
	float localPercent = 0.0f;
	LUNAEasingFunc easing = luna2d::easing::Linear;

	if(widthIntervals.size() == 1)
	{
		const auto& key = widthIntervals.front();
		float keyLenPointer = len * key.percent;

		if(key.percent >= percent)
		{
			endScale = key.scale;
			localPercent = lenPointer / keyLenPointer;
		}
		else
		{
			beginScale = key.scale;
			localPercent = (lenPointer - keyLenPointer) / (len - keyLenPointer);
		}

		easing = key.easing;
	}
	else
	{
		for(size_t i = 1; i < widthIntervals.size(); i++)
		{
			const auto& prevKey = widthIntervals[i - 1];
			const auto& key = widthIntervals[i];

			if(prevKey.percent <= percent && key.percent >= percent)
			{
				float prevKeyLenPointer = len * prevKey.percent;
				float keyLenPointer = len * key.percent;

				beginScale = prevKey.scale;
				endScale = key.scale;
				easing = key.easing;
				localPercent = (lenPointer - prevKeyLenPointer) / (keyLenPointer - prevKeyLenPointer);
				break;
			}
		}
	}

	return params.width * math::EaseLerp(beginScale, endScale, localPercent, easing);
}

void LUNACurve::SetParams(const LUNACurveParams& params)
{
	auto region = params.textureRegion.lock();

	if(!region) LUNA_RETURN_ERR("Attempt to set invalid curve params");

	this->params = params;
	mesh->SetTexture(region->GetTexture());
	needBuild = true;

	if(this->params.textureRegionHead.expired()) this->params.headSegments = 0;
	if(this->params.textureRegionTail.expired()) this->params.tailSegments = 0;
}

void LUNACurve::SetShader(const std::weak_ptr<LUNAShader>& shader)
{
	mesh->SetShader(shader);
}

void LUNACurve::ClearCustomWidths()
{
	widthIntervals.clear();
}

void LUNACurve::SetCustomWidth(float percent, float scale, LUNAEasingFunc easing)
{
	widthIntervals.push_back(WidthKey{ percent, scale, easing });

	std::sort(widthIntervals.begin(), widthIntervals.end(), [](WidthKey& w1, WidthKey& w2)
	{
		return w1.percent < w2.percent;
	});
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
	if(index < 1 || index > (int)knots.size())
	{
		LUNA_LOGE("Knot index \"%d\" is out of range", index);
		return;
	}

	knots.erase(knots.begin() + index - 1);
	needBuild = true;
}

void LUNACurve::SetKnot(int index, float x, float y)
{
	if(index < 1 || index > (int)knots.size())
	{
		LUNA_LOGE("Knot index \"%d\" is out of range", index);
		return;
	}

	knots[index - 1].x = x;
	knots[index - 1].y = y;
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
	table.SetField("headSegments", params.headSegments);
	table.SetField("tailSegments", params.tailSegments);

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
	params.headSegments = table.GetInt("headSegments");
	params.tailSegments = table.GetInt("tailSegments");

	std::string strMappingMode = table.GetString("textureMappingMode");
	if(TEXTURE_MAPPING_MODE.HasKey(strMappingMode)) params.textureMappingMode = TEXTURE_MAPPING_MODE.FromString(strMappingMode);

	return params;
}
