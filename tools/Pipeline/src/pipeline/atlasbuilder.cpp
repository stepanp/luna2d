//-----------------------------------------------------------------------------
// luna2d Pipeline
// This is part of luna2d engine
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

#include "atlasbuilder.h"
#include "utils/mathutils.h"
#include <MaxRectsBinPack.h>
#include <QPainter>
#include <QDebug>

using namespace rbp;

QPair<QImage,QJsonObject> AtlasBuilder::Run(ImageList images, int width, int height, const AtlasParams &params)
{
	errors.clear();

	QImage atlas(width, height, QImage::Format_ARGB32);
	atlas.fill(Qt::transparent);

	QJsonObject jsonAtlas;

	MaxRectsBinPack packer(width, height, false);
	auto heuristic = MaxRectsBinPack::RectBestAreaFit;
	int maxRight = 0;
	int maxBottom = 0;

	QPainter painter(&atlas);
	for(auto entry : images)
	{
		const QImage& image = entry.second;
		Rect rect = packer.Insert(image.width() + params.padding * 2, image.height() + params.padding * 2, heuristic);

		maxRight = std::max(rect.x + rect.width, maxRight);
		maxBottom = std::max(rect.y + rect.height, maxBottom);

		if(rect.height > 0) // Image successfully packed
		{
			int x = rect.x + params.padding;
			int y = rect.y + params.padding;

			// Draw image into atals image
			painter.drawImage(x, y, image);

			// Draw dublicate padding
			if(params.duplicatePadding)
			{
				// Edges
				for(int i = 0; i < params.padding; i++)
				{
					painter.drawImage(rect.x + i, y, image, 0, 0, 1); // Left
					painter.drawImage(x + image.width() + i, y, image, image.width() - 1, 0, 1); // Right
					painter.drawImage(x, rect.y + i, image, 0, 0, -1, 1); // Top
					painter.drawImage(x, y + image.height() + i, image, 0, image.height() - 1, -1, 1); // Bottom
				}

				// Corners
				painter.fillRect(rect.x, rect.y, params.padding, params.padding,
								 QColor::fromRgba(image.pixel(0, 0))); // Left-top
				painter.fillRect(x + image.width(), rect.y, params.padding, params.padding,
								 QColor::fromRgba(image.pixel(image.width() - 1, 0))); // Right-top
				painter.fillRect(rect.x, y + image.height(), params.padding, params.padding,
								 QColor::fromRgba(image.pixel(0, image.height() - 1))); // Left-bottom
				painter.fillRect(x + image.width(), y + image.height(), params.padding, params.padding,
								 QColor::fromRgba(image.pixel(image.width() - 1, image.height() - 1))); // Right-bottom
			}

			// Add region to json
			QJsonObject jsonRegion;
			jsonRegion["x"] = x;
			jsonRegion["y"] = y;
			jsonRegion["width"] = image.width();
			jsonRegion["height"] = image.height();
			jsonAtlas[entry.first] = jsonRegion;
		}
		else errors.push_back(QString("Image \"%1\" isn't packed").arg(entry.first));
	}
	painter.end();

	// Crop atlas to minimal possible size
	QImage croppedAtals = atlas.copy(0, 0, MathUtils::NearestPowerOfTwo(maxRight), MathUtils::NearestPowerOfTwo(maxBottom));

	return QPair<QImage,QJsonObject>(croppedAtals, jsonAtlas);
}

QStringList AtlasBuilder::GetErrors()
{
	return errors;
}
