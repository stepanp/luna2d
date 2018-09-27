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

#include "lunapath.h"
#include "lunamacro.h"

using namespace luna2d;

glm::vec2 LUNAPath::MakePos(const LUNAPath::Anchor& anchor, const glm::vec2& dir)
{
	return points[anchor.pointIndex] + glm::normalize(dir) * anchor.passedDist;
}

bool LUNAPath::TryApplyClosure(LUNAPath::Anchor& anchor)
{
	if(anchor.ignoreClosures) return false;

	if(closures.count(anchor.pointIndex))
	{
		anchor.pointIndex = closures[anchor.pointIndex];
		return true;
	}

	return false;
}

void LUNAPath::SetClosure(int indexFrom, int indexTo)
{
	closures[indexFrom - 1] = indexTo - 1;
}

void LUNAPath::RemoveClosure(int indexFrom)
{
	closures.erase(indexFrom - 1);
}

void LUNAPath::ClearClosures()
{
	closures.clear();
}

int LUNAPath::GetPointsCount()
{
	return points.size();
}

const std::vector<glm::vec2>& LUNAPath::GetPoints()
{
	return points;
}

glm::vec2 LUNAPath::GetPoint(int index)
{
	if(index < 1 || index > points.size())
	{
		LUNA_LOGE("Point index \"%d\" out of bounds", index);
		return {};
	}

	return points[index - 1];
}

void LUNAPath::AddPoint(const glm::vec2& point)
{
	points.push_back(point);
}

void LUNAPath::InsertPoint(int index, const glm::vec2& point)
{
	if(index < 1 || index > points.size()) LUNA_RETURN_ERR("Point index \"%d\" out of bounds", index);

	points.insert(points.begin() + index - 1, point);
}

void LUNAPath::SetPoint(int index, const glm::vec2& point)
{
	if(index < 1 || index > points.size()) LUNA_RETURN_ERR("Point index \"%d\" out of bounds", index);

	points[index - 1] = point;
}

void LUNAPath::RemovePoint(int index)
{
	if(index < 1 || index > points.size()) LUNA_RETURN_ERR("Point index \"%d\" out of bounds", index);

	points.erase(points.begin() + index - 1);
}

void LUNAPath::RemovePointsRange(int from, int to)
{
	if(from > to) LUNA_RETURN_ERR("Invalid points range \"[%d,%d]\"", from, to);
	if(from < 1 || to > points.size()) LUNA_RETURN_ERR("Point range \"[%d,%d]\" out of bounds", from, to);

	points.erase(points.begin() + from - 1, points.begin() + to - 1);
}

void LUNAPath::ClearPoints()
{
	points.clear();
}

int LUNAPath::AddAchnor()
{
	int achorId = uniqueAnchorId;
	uniqueAnchorId++;

	anchors[achorId] = {};

	return achorId;
}

void LUNAPath::RemoveAnchor(int anchorId)
{
	if(anchors.count(anchorId) == 0) LUNA_RETURN_ERR("Anchor with id \"%d\" not found", anchorId);

	anchors.erase(anchorId);
}

void LUNAPath::ClearAnchors()
{
	anchors.clear();
}

glm::vec2 LUNAPath::MoveAnchorToBegin(int anchorId)
{
	if(anchors.count(anchorId) == 0)
	{
		LUNA_LOGE("Anchor with id \"%d\" not found", anchorId);
		return {};
	}

	if(points.empty())
	{
		LUNA_LOGE("Path is empty");
		return {};
	}

	if(points.size() == 1) return points[0];

	auto& anchor = anchors[anchorId];
	anchor.pointIndex = 0;
	anchor.passedDist = 0;

	TryApplyClosure(anchor);

	return points[anchor.pointIndex];
}

glm::vec2 LUNAPath::MoveAnchorToEnd(int anchorId)
{
	if(anchors.count(anchorId) == 0)
	{
		LUNA_LOGE("Anchor with id \"%d\" not found", anchorId);
		return {};
	}

	if(points.empty())
	{
		LUNA_LOGE("Path is empty");
		return {};
	}

	if(points.size() == 1) return points[0];

	auto& anchor = anchors[anchorId];
	anchor.pointIndex = points.size() - 2;
	anchor.passedDist = glm::distance(points[anchor.pointIndex], points[anchor.pointIndex + 1]);

	TryApplyClosure(anchor);

	return points[points.size() - 1];
}

glm::vec2 LUNAPath::MoveAnchorToPoint(int anchorId, int index)
{
	if(anchors.count(anchorId) == 0)
	{
		LUNA_LOGE("Anchor with id \"%d\" not found", anchorId);
		return {};
	}

	if(index < 1 || index > points.size())
	{
		LUNA_LOGE("Point index \"%d\" out of bounds", index);
		return {};
	}

	auto& anchor = anchors[anchorId];
	anchor.pointIndex = index - 1;

	return MoveAnchor(anchorId, 0);
}

glm::vec2 LUNAPath::MoveAnchor(int anchorId, float dist)
{
	if(anchors.count(anchorId) == 0)
	{
		LUNA_LOGE("Anchor with id \"%d\" not found", anchorId);
		return {};
	}

	if(points.empty())
	{
		LUNA_LOGE("Path is empty");
		return {};
	}

	if(points.size() == 1) return points[0];

	auto& anchor = anchors[anchorId];
	float extraDist = dist;
	bool moveForward = dist >= 0;

	while(true)
	{
		int currentPoint = anchor.pointIndex;
		int nextPoint = currentPoint + 1;

		if(currentPoint < 0)
		{
			anchor.pointIndex = 0;
			anchor.passedDist = 0;

			return points[0];
		}

		if(currentPoint > points.size() - 2)
		{
			glm::vec2 dir = points[points.size() - 1] - points[points.size() - 2];

			anchor.pointIndex = points.size() - 2;
			anchor.passedDist = glm::length(dir);

			return MakePos(anchor, dir);
		}

		glm::vec2 dir = points[nextPoint] - points[currentPoint];
		float len = glm::length(dir);

		anchor.passedDist += extraDist;

		if(moveForward && anchor.passedDist > len)
		{
			extraDist = anchor.passedDist - len;
			anchor.pointIndex++;
			anchor.passedDist = 0;

			TryApplyClosure(anchor);
		}
		else if(!moveForward && anchor.passedDist < 0)
		{
			extraDist = anchor.passedDist;
			anchor.pointIndex--;
			anchor.passedDist = glm::distance(points[anchor.pointIndex], points[anchor.pointIndex + 1]);

			if(TryApplyClosure(anchor))
			{
				anchor.pointIndex--;
				anchor.passedDist = 0;
			}
		}
		else
		{
			return MakePos(anchor, dir);
		}
	}
}

glm::vec2 LUNAPath::GetAnchorPos(int anchorId)
{
	return MoveAnchor(anchorId, 0);
}

bool LUNAPath::IsAnchorAtBegin(int anchorId)
{
	if(anchors.count(anchorId) == 0)
	{
		LUNA_LOGE("Anchor with id \"%d\" not found", anchorId);
		return false;
	}

	auto& anchor = anchors[anchorId];
	return anchor.pointIndex == 0 && anchor.passedDist <= std::numeric_limits<float>::epsilon();
}

bool LUNAPath::IsAnchorAtEnd(int anchorId)
{
	if(anchors.count(anchorId) == 0)
	{
		LUNA_LOGE("Anchor with id \"%d\" not found", anchorId);
		return false;
	}

	auto& anchor = anchors[anchorId];

	return anchor.pointIndex == points.size() - 2 &&
			anchor.passedDist >= glm::distance(points[anchor.pointIndex], points[anchor.pointIndex + 1]);
}

// Get index of current point for anchor
int LUNAPath::GetAnchorPointIndex(int anchorId)
{
	if(anchors.count(anchorId) == 0)
	{
		LUNA_LOGE("Anchor with id \"%d\" not found", anchorId);
		return 0;
	}

	return anchors[anchorId].pointIndex + 1;
}

// Get distance from curent point for anchor
float LUNAPath::GetAnchorPointDistance(int anchorId)
{
	if(anchors.count(anchorId) == 0)
	{
		LUNA_LOGE("Anchor with id \"%d\" not found", anchorId);
		return 0;
	}

	return anchors[anchorId].passedDist;
}

bool LUNAPath::IsAnchorIgnoreClosures(int anchorId)
{
	if(anchors.count(anchorId) == 0)
	{
		LUNA_LOGE("Anchor with id \"%d\" not found", anchorId);
		return false;
	}

	return anchors[anchorId].ignoreClosures;
}

void LUNAPath::SetAnchorIgnoreClosures(int anchorId)
{
	if(anchors.count(anchorId) == 0) LUNA_RETURN_ERR("Anchor with id \"%d\" not found", anchorId);

	anchors[anchorId].ignoreClosures = true;
}

float LUNAPath::GetLenght()
{
	return GetLenghtRange(1, points.size());
}

float LUNAPath::GetLenghtRange(int from, int to)
{
	if(from > to)
	{
		LUNA_LOGE("Invalid points range \"[%d,%d]\"", from, to);
		return 0;
	}

	if(from < 1 || to > points.size())
	{
		LUNA_LOGE("Point range \"[%d,%d]\" out of bounds", from, to);
		return 0;
	}

	float ret = 0;
	glm::vec2 prev = points[from - 1];

	for(int i = from; i < to - 1; i++)
	{
		ret += glm::distance(prev, points[i]);
		prev = points[i];
	}

	return ret;
}
