//-----------------------------------------------------------------------------
// luna2d Pipeline
// This is part of luna2d engine
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

#pragma once

#include "project/project.h"
#include "resizer.h"
#include "atlasbuilder.h"
#include <QStringList>

const QString ERROR_MASK = "In task \"%1\" : %2";
const QString ERROR_SAVE_MASK = "Cannot save file \"%1\"";

class Pipeline : public QObject
{
	Q_OBJECT

public:
	Pipeline();
	~Pipeline();

private:
	Project* project = nullptr;
	Resizer resizer;
	AtlasBuilder atlasBuilder;
	QStringList errors;

	// Info for displaying progress percentage
	// SEE: "Project::GetStagesCount"
	int curStages = 0;
	int totalStages = 0;

private:
	QString CheckTask(Task* task); // Check task params for errors
	void RunTask(Task* task);
	void ResizeStage(ImageList images, Task* task);
	void BuildAtlasStage(QHash<QString,ImageList> images, Task* task);
	void UpdateProgress();

signals:
	void progressUpdated(float percent);

public:
	QStringList GetResolutionsNames();
	Project* GetProject();
	bool IsProjectOpened();
	bool NewProject();
	bool OpenProject(const QString& path);
	bool SaveProject(const QString& path);
	void CloseProject();
	QStringList RunProject();
	QStringList RunTaskList(const QList<Task*>& taskList);
};
