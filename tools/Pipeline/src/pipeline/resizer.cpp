//-----------------------------------------------------------------------------
// luna2d Pipeline
// This is part of luna2d engine
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

#include "resizer.h"
#include "utils/mathutils.h"
#include <QPainter>

// Helper for sorting resolutions
static bool CompareResolutions(const QString &res1, const QString &res2)
{
	return luna2d::RESOLUTIONS_TABLE.at(res1.toStdString()) > luna2d::RESOLUTIONS_TABLE.at(res2.toStdString());
}

// Calculate scale between given resolutions
static float CalcScale(const QString &sourceRes, const QString &outputRes)
{
	int sourceHeight = luna2d::RESOLUTIONS_TABLE.at(sourceRes.toStdString());
	int outputHeight = luna2d::RESOLUTIONS_TABLE.at(outputRes.toStdString().c_str());

	return outputHeight / (float)sourceHeight;
}

ImageList Resizer::Run(ImageList images, const QString &sourceRes, const QString &outputRes, bool potSize)
{
	ImageList ret;
	float scale = CalcScale(sourceRes, outputRes);

	for(auto entry : images)
	{
		QImage& source = entry.second;

		// Min size of image is 1x1 pixels
		int newWidth = std::max(1.0f, source.width() * scale);
		int newHeight = std::max(1.0f, source.height() * scale);

		// Don't resize image if it has same resolution
		QImage resized = source;
		if(sourceRes != outputRes) resized = source.scaled(newWidth, newHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

		// Lead image to power of two sizes if required
		if(potSize)
		{
			int potWidth = MathUtils::NearestPowerOfTwo(newWidth);
			int potHeight = MathUtils::NearestPowerOfTwo(newHeight);
			QImage potImage(potWidth, potHeight, resized.format());
			potImage.fill(Qt::transparent);

			QPainter painter(&potImage);
			painter.drawImage(0, 0, resized);
			painter.end();

			ret.push_back(QPair<QString,QImage>(entry.first, potImage));
		}
		else ret.push_back(QPair<QString,QImage>(entry.first, resized));
	}

	return ret;
}

QStringList Resizer::GetResolutionsNames()
{
	QStringList ret;

	for(auto entry : luna2d::RESOLUTIONS_TABLE) ret.push_back(entry.first.c_str());
	qSort(ret.begin(), ret.end(), &CompareResolutions); // Sort resolutions descending

	return ret;
}
