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

#pragma once

#include "pipeline/atlasbuilder.h"
#include "foldernode.h"
#include "filenode.h"
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>

class Project;

enum class OutputFormat
{
	PNG_32,
	PNG_24,
	JPEG
};

class Task
{
public:
	Task(Project* project);
	Task(Project* project, const QJsonObject& jsonTask);
	~Task();

public:
	Project* project;
	QString name;
	QString outputDir;
	bool resize, atlas; // Pipeline stages
	bool markAsPixmap;
	QList<TaskNode*> nodes;
	OutputFormat outputFormat;

	// Resize params
	QString sourceRes;
	QStringList outputRes;
	bool potSize;

	// Atlas params
	AtlasParams atlasParams;

private:
	QString RelativePath(const QString& path);
	QString AbsolutePath(const QString& path);

public:
	void AddNode(TaskNode* node);
	void RemoveNode(TaskNode* node);
	QStringList GetAllFilenames(); // Get list of all filenames in task(include files in folders)
	QJsonObject ToJson();
};
