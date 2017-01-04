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

#include "task.h"
#include <QList>

class Project
{
public:
	Project();
	Project(const QString& name, const QString& projectPath, const QJsonObject& jsonProject);
	~Project();

private:
	QString name;
	QString projectPath; // For caluclating relative path
	QList<Task*> tasks;
	bool useRelativePath;

public:
	QString GetName();
	void SetName(const QString& name);
	QString GetProjectPath();
	void SetProjectPath(const QString& path);
	bool IsUseRelativePath();
	void SetUseRelativePath(bool value);

	// Get count of stages for displaying progress perentage
	// Every stage is action of resizing one texture or packing one atlas
	int GetStagesCount();

	QList<Task*>& GetTasks();
	void AddTask(Task* task);
	void RemoveTask(Task* task);

	QJsonObject ToJson();
};
