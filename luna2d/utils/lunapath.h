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

#pragma once

#include "lunavector2.h"

namespace luna2d{

class LUNAPath
{
	LUNA_USERDATA(LUNAPath)

	struct Anchor
	{
		int pointIndex = 0;
		float passedDist = 0;
		bool ignoreClosures = false;
	};

private:
	std::vector<glm::vec2> points;
	std::unordered_map<int, Anchor> anchors;
	std::unordered_map<int, int> closures;
	int uniqueAnchorId = 1;

private:
	glm::vec2 MakePos(const Anchor& anchor, const glm::vec2& dir);

	void TryApplyClosure(Anchor& anchor);

public:
	void SetClosure(int indexFrom, int indexTo);

	void RemoveClosure(int indexFrom);

	int GetPointsCount();

	const std::vector<glm::vec2>& GetPoints();

	glm::vec2 GetPoint(int index);

	void AddPoint(const glm::vec2& point);

	void InsertPoint(int index, const glm::vec2& point);

	void SetPoint(int index, const glm::vec2& point);

	void RemovePoint(int index);

	void RemovePointsRange(int from, int to);

	int AddAchnor();

	void RemoveAnchor(int anchorId);

	glm::vec2 MoveAnchorToBegin(int anchorId);

	glm::vec2 MoveAnchorToEnd(int anchorId);

	glm::vec2 MoveAnchorToPoint(int anchorId, int index);

	glm::vec2 MoveAnchor(int anchorId, float dist);

	bool IsAnchorAtBegin(int anchorId);

	bool IsAnchorAtEnd(int anchorId);

	// Get index of current point for anchor
	int GetAnchorPointIndex(int anchorId);

	// Get distance from curent point for anchor
	float GetAnchorPointDistance(int anchorId);

	bool IsAnchorIgnoreClosures(int anchorId);

	void SetAnchorIgnoreClosures(int anchorId);

	float GetLenght();

	float GetLenghtRange(int from, int to);
};

}
