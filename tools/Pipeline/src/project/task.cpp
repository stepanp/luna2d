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
	outputFormat(OutputFormat::PNG_32),
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
	outputFormat(OutputFormat::PNG_32),
	sourceRes(jsonTask["sourceRes"].toString()),
	potSize(jsonTask["potSize"].toBool())
{
	QString jsonOutputFormat = jsonTask["outputFormat"].toString();
	if(jsonOutputFormat == "PNG_32") outputFormat = OutputFormat::PNG_32;
	else if(jsonOutputFormat == "PNG_24") outputFormat = OutputFormat::PNG_24;
	else if(jsonOutputFormat == "JPEG") outputFormat = OutputFormat::JPEG;

	for(auto jsonRes : jsonTask["outputRes"].toArray())
	{
		outputRes.push_back(jsonRes.toString());
	}

	for(auto jsonNodeRes : jsonTask["nodes"].toArray())
	{
		QJsonObject jsonNode = jsonNodeRes.toObject();
		QString path = AbsolutePath(jsonNode["path"].toString());

		if(jsonNode["type"] == "file") nodes.push_back(new FileNode(path));
		else if(jsonNode["type"] == "folder") nodes.push_back(new FolderNode(path));
	}

	if(atlas)
	{
		QJsonObject jsonAtlasParams = jsonTask["atlasParams"].toObject();
		atlasParams.name = jsonAtlasParams["name"].toString();
		atlasParams.padding = jsonAtlasParams["padding"].toInt();
		atlasParams.duplicatePadding = jsonAtlasParams["duplicatePadding"].toBool();

		QJsonObject jsonSizes = jsonAtlasParams["sizes"].toObject();
		for(auto& res : jsonSizes.keys())
		{
			QJsonObject size = jsonSizes[res].toObject();
			atlasParams.sizes.insert(res, QSize(size["width"].toInt(), size["height"].toInt()));
		}
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

	switch(outputFormat)
	{
	case OutputFormat::PNG_32:
		jsonTask["outputFormat"] = "PNG_32";
		break;
	case OutputFormat::PNG_24:
		jsonTask["outputFormat"] = "PNG_24";
		break;
	case OutputFormat::JPEG:
		jsonTask["outputFormat"] = "JPEG";
		break;
	}

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
		jsonAtlasParams["padding"] = atlasParams.padding;
		jsonAtlasParams["duplicatePadding"] = atlasParams.duplicatePadding;

		QJsonObject jsonSizes;
		for(auto res : atlasParams.sizes.keys())
		{
			QSize size = atlasParams.sizes[res];

			QJsonObject jsonSize;
			jsonSize["width"] = size.width();
			jsonSize["height"] = size.height();

			jsonSizes[res] = jsonSize;
		}

		jsonAtlasParams["sizes"] = jsonSizes;

		jsonTask["atlasParams"] = jsonAtlasParams;
	}

	return jsonTask;
}
