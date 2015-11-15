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

#include "task.h"
#include "project.h"
#include "common/lunaresolutions.h"
#include <QDir>
#include <QStandardPaths>

Task::Task(Project* project) :
	project(project),
	name("New task"),
	outputDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)),
	resize(true),
	atlas(true),
	sourceRes(QString::fromStdString(luna2d::DEFAULT_RESOLUTION)),
	potSize(false)
{
}

Task::Task(Project* project, const QJsonObject& jsonTask) :
	project(project),
	name(jsonTask["name"].toString()),
	outputDir(AbsolutePath(jsonTask["outputDir"].toString())),
	resize(jsonTask["resize"].toBool()),
	atlas(jsonTask["atlas"].toBool()),
	sourceRes(jsonTask["sourceRes"].toString()),
	potSize(jsonTask["potSize"].toBool())
{
	for(auto jsonRes : jsonTask["outputRes"].toArray())
	{
		outputRes.push_back(jsonRes.toString());
	}

	for(auto jsonNodeRef : jsonTask["nodes"].toArray())
	{
		QJsonObject jsonNode = jsonNodeRef.toObject();
		QString path = AbsolutePath(jsonNode["path"].toString());

		if(jsonNode["type"] == "file") nodes.push_back(new FileNode(path));
		else if(jsonNode["type"] == "folder") nodes.push_back(new FolderNode(path));
	}

	if(atlas)
	{
		QJsonObject jsonAtlasParams = jsonTask["atlasParams"].toObject();
		atlasParams.name = jsonAtlasParams["name"].toString();
		atlasParams.maxWidth = jsonAtlasParams["maxWidth"].toInt();
		atlasParams.maxHeight = jsonAtlasParams["maxHeight"].toInt();
		atlasParams.padding = jsonAtlasParams["padding"].toInt();
		atlasParams.duplicatePadding = jsonAtlasParams["duplicatePadding"].toBool();
	}
}

Task::~Task()
{
	for(TaskNode* node : nodes) delete node;
}

QString Task::RelativePath(const QString& path)
{
	if(!project->IsUseRelativePath()) return path;

	QDir dir(project->GetProjectPath());
	return dir.relativeFilePath(path);
}

QString Task::AbsolutePath(const QString& path)
{
	if(!project->IsUseRelativePath()) return path;

	QDir dir(project->GetProjectPath());
	return dir.cleanPath(dir.absoluteFilePath(path));
}

void Task::AddNode(TaskNode *node)
{
	nodes.push_back(node);
}

void Task::RemoveNode(TaskNode* node)
{
	nodes.removeOne(node);
}

// Get list of all filenames in task(include files in folders)
QStringList Task::GetAllFilenames()
{
	QStringList allFiles;

	for(TaskNode* node : nodes)
	{
		if(node->GetNodeType() == TaskNodeType::FILE) allFiles.push_back(node->GetPath());
		else if(node->GetNodeType() == TaskNodeType::FOLDER)
		{
			FolderNode* folder = static_cast<FolderNode*>(node);
			for(FileNode* file : folder->GetFiles()) allFiles.push_back(file->GetPath());
		}
	}

	return allFiles;
}


QJsonObject Task::ToJson()
{
	QJsonObject jsonTask;
	jsonTask["name"] = name;
	jsonTask["outputDir"] = RelativePath(outputDir);
	jsonTask["resize"] = resize;
	jsonTask["atlas"] = atlas;
	jsonTask["sourceRes"] = sourceRes;
	jsonTask["potSize"] = potSize;

	QJsonArray jsonResolutions;
	for(QString res : outputRes) jsonResolutions.append(res);
	jsonTask["outputRes"] = jsonResolutions;

	QJsonArray jsonNodes;
	for(TaskNode* node : nodes)
	{
		QJsonObject jsonNode;
		jsonNode["type"] = node->GetNodeType() == TaskNodeType::FILE ? "file" : "folder";
		jsonNode["path"] = RelativePath(node->GetPath());
		jsonNodes.append(jsonNode);
	}
	jsonTask["nodes"] = jsonNodes;

	if(atlas)
	{
		QJsonObject jsonAtlasParams;
		jsonAtlasParams["name"] = atlasParams.name;
		jsonAtlasParams["maxWidth"] = atlasParams.maxWidth;
		jsonAtlasParams["maxHeight"] = atlasParams.maxHeight;
		jsonAtlasParams["padding"] = atlasParams.padding;
		jsonAtlasParams["duplicatePadding"] = atlasParams.duplicatePadding;
		jsonTask["atlasParams"] = jsonAtlasParams;
	}

	return jsonTask;
}
