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

#include "project.h"

Project::Project() :
	name("New project"),
	projectPath(""),
	useRelativePath(true)
{
}

Project::Project(const QString& name, const QString& projectPath, const QJsonObject& jsonProject) :
	name(name),
	projectPath(projectPath),
	useRelativePath(jsonProject["useRelativePath"].toBool())
{
	QJsonArray jsonTasks = jsonProject["tasks"].toArray();
	for(auto jsonTask : jsonTasks)
	{
		tasks.push_back(new Task(this, jsonTask.toObject()));
	}
}

Project::~Project()
{
	for(Task* task : tasks) delete task;
}

QString Project::GetName()
{
	return name;
}

void Project::SetName(const QString &name)
{
	this->name = name;
}

QString Project::GetProjectPath()
{
	return projectPath;
}

void Project::SetProjectPath(const QString &path)
{
	projectPath = path;
}

bool Project::IsUseRelativePath()
{
	return useRelativePath;
}

void Project::SetUseRelativePath(bool value)
{
	useRelativePath = value;
}

QList<Task*>& Project::GetTasks()
{
	return tasks;
}

void Project::AddTask(Task* task)
{
	tasks.push_back(task);
}

void Project::RemoveTask(Task *task)
{
	tasks.removeOne(task);
}

QJsonObject Project::ToJson()
{
	QJsonObject jsonProject;
	jsonProject["useRelativePath"] = useRelativePath;

	QJsonArray jsonTasks;
	for(Task* task : tasks) jsonTasks.append(task->ToJson());
	jsonProject["tasks"] = jsonTasks;

	return jsonProject;
}
