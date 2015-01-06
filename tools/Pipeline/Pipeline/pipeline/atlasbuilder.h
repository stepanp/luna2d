//-----------------------------------------------------------------------------
// luna2d Pipeline
// This is part of luna2d engine
// Copyright 2014 Stepan Prokofjev
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

#include <QString>
#include <QImage>
#include <QPair>
#include <QJsonObject>

const int MIN_ATLAS_SIZE = 16;
const int MAX_ATLAS_SIZE = 4096;

struct AtlasParams
{
	QString name = "New atlas";
	int maxWidth = MIN_ATLAS_SIZE;
	int maxHeight = MIN_ATLAS_SIZE;
	int padding = 1;
	bool duplicatePadding = true;
};

typedef QList<QPair<QString,QImage>> ImageList;
typedef QList<QPair<QImage,QJsonObject>> AtlasList;

class AtlasBuilder
{
public:
	QPair<QImage,QJsonObject> Run(ImageList images, const AtlasParams& params);
};
