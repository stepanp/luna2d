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

#include "lunacurverenderer.h"
#include "lunasplinesmodule.h"

using namespace luna2d;

LUNACurveRenderer::LUNACurveRenderer(const LuaTable& params)
{
	textureId = params.GetInt("texture");
	u1 = params.GetFloat("u1");
	v1 = params.GetFloat("v1");
	u2 = params.GetFloat("u2");
	v2 = params.GetFloat("v2");
	width = params.GetFloat("width");

	mesh = std::unique_ptr<LUNAMesh>(new LUNAMesh(textureId));
	splines = LUNAEngine::SharedModule<LUNASplinesModule>("splines");
}

void LUNACurveRenderer::ClearKnots()
{
	knots.clear();
}

int LUNACurveRenderer::GetKnotsCount()
{
	return knots.size();
}

void LUNACurveRenderer::AddKnot(float x, float y)
{
	knots.push_back(glm::vec2(x, y));
}

void LUNACurveRenderer::RemoveKnot(int index)
{
	if(index < 0 || index >= (int)knots.size())
	{
		LUNA_LOGE("Knot index \"%d\" is out of range", index);
		return;
	}

	knots.erase(knots.begin() + index);
}

void LUNACurveRenderer::SetKnot(int index, float x, float y)
{
	if(index < 0 || index >= (int)knots.size())
	{
		LUNA_LOGE("Knot index \"%d\" is out of range", index);
		return;
	}

	knots[index].x = x;
	knots[index].y = y;
}

void LUNACurveRenderer::SetKnots(const std::vector<glm::vec2>& knots)
{
	this->knots.clear();
	this->knots.insert(this->knots.begin(), knots.begin(), knots.end());
}

// Build curve mesh by knots
void LUNACurveRenderer::Build()
{
	if(knots.size() < 3)
	{
		LUNA_LOGE("Cannot build curve by less than 3 knots");
		return;
	}

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
		float startT = (i == 1 ? 0.0f : CURVE_SMOOTH_FACTOR);

		for(float t = startT; t <= 1.0f; t += CURVE_SMOOTH_FACTOR)
		{
			points.push_back(splines->QuadraticBSpline(knot0, knot1, knot2, t));

			// Calculate lenght for each segment and segments lenghts summ for texture mapping
			float sementLen = glm::distance2(points[points.size() - 1], points[points.size() - 2]);
			lenghts.push_back(sementLen);
			lenSumm += sementLen;
		}
	}
	points.push_back(knots[knots.size() - 1]);
	float sementLen = glm::distance2(points[points.size() - 1], points[points.size() - 2]);
	lenghts.push_back(sementLen);
	lenSumm += sementLen;

	//---------------------
	// Build mesh by spline
	//---------------------
	float ltX = u1;
	float ltY = v1;
	float lbX = u1;
	float lbY = v2;
	float rtX = ltX;
	float rtY = v1;
	float rbX = lbX;
	float rbY = v2;
	float regionLen = u2 - u1;
	float halfWidth = width / 2.0f;
	glm::vec2 prevA, prevB;

	mesh->Clear();
	for(size_t i = 1; i < points.size(); i++)
	{
		glm::vec2 a,b,c,d;

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

			float step = regionLen * (lenghts[i - 1] / lenSumm);
			rtX += step;
			rbX += step;
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

			ltX = rtX;
			lbX = rbX;
			rtX = u2;
			rbX = u2;
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

			float step = regionLen * (lenghts[i - 1] / lenSumm);
			ltX = rtX;
			lbX = rbX;
			rtX += step;
			rbX += step;
		}

		mesh->AddVertex(a.x, a.y, 1, 1, 1, 1, ltX, ltY);
		mesh->AddVertex(b.x, b.y, 1, 1, 1, 1, lbX, lbY);
		mesh->AddVertex(c.x, c.y, 1, 1, 1, 1, rtX, rtY);
		mesh->AddVertex(b.x, b.y, 1, 1, 1, 1, lbX, lbY);
		mesh->AddVertex(c.x, c.y, 1, 1, 1, 1, rtX, rtY);
		mesh->AddVertex(d.x, d.y, 1, 1, 1, 1, rbX, rbY);

		prevA = c;
		prevB = d;
	}
}

void LUNACurveRenderer::Render()
{
	mesh->Render();
}

