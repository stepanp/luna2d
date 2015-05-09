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

#include "pipeline.h"
#include <QJsonDocument>
#include <QHash>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include "utils/mathutils.h"

static QString MakeFilename(const QString& name, const QString& resolution, const QString& extension)
{
	if(resolution == QString::null) return QString("%1.%2").arg(name).arg(extension);
	else return QString("%1@%2.%3").arg(name).arg(resolution).arg(extension);
}

static void SaveImage(const QImage& image, const QString& directory, const QString& filename)
{
	QDir outputDir(directory);
	image.save(outputDir.absoluteFilePath(filename), "PNG");
}


Pipeline::Pipeline() :
	project(nullptr)
{
}

Pipeline::~Pipeline()
{
	CloseProject();
}

// Check task params for errors
QString Pipeline::CheckTask(Task *task)
{
	if(task->resize)
	{
		if(task->outputRes.empty()) return "Output resolutions aren't selected";
	}

	if(task->atlas)
	{
		const AtlasParams& params = task->atlasParams;
		if(params.name.isEmpty()) return "Atlas name isn't specifed";
		if(!MathUtils::IsPowerOfTwo(params.maxWidth)) return "Atlas max width must be power of two";
		if(!MathUtils::IsPowerOfTwo(params.maxHeight)) return "Atlas max height must be power of two";
	}

	return QString::null;
}

void Pipeline::RunTask(Task* task)
{
	// Get list of all filenames in task
	QStringList allFilenames = task->GetAllFilenames();

	// Load all source images
	ImageList sourceImages;
	for(QString filename : allFilenames)
	{
		QFileInfo fileInfo(filename);
		QImage image(filename);

		sourceImages.push_back(QPair<QString,QImage>(fileInfo.completeBaseName(), image));
	}

	if(task->resize) ResizeStage(sourceImages, task);
	else
	{
		QHash<QString,ImageList> images;
		images[QString::null] = sourceImages;
		BuildAtlasStage(images, task);
	}
}

void Pipeline::ResizeStage(ImageList images, Task* task)
{
	// Resize images and split it by resolutions
	QHash<QString, ImageList> imagesByRes;
	for(QString res : task->outputRes)
	{
		imagesByRes[res] = resizer.Run(images, task->sourceRes, res, task->potSize);
	}

	// Pass images to build atals stage
	if(task->atlas) BuildAtlasStage(imagesByRes, task);

	// Save resized images without building atlas
	else
	{
		for(const QString& resolution : imagesByRes.keys())
		{
			for(auto entry : imagesByRes[resolution])
			{
				SaveImage(entry.second, task->outputDir, MakeFilename(entry.first, resolution, "png"));
			}
		}
	}
}

void Pipeline::BuildAtlasStage(QHash<QString,ImageList> images, Task* task)
{
	for(const QString& resolution : images.keys())
	{
		QPair<QImage,QJsonObject> atlas = atlasBuilder.Run(images[resolution], task->atlasParams);

		// Save atlas image
		SaveImage(atlas.first, task->outputDir, MakeFilename(task->atlasParams.name, resolution, "png"));

		// Save atlas json description
		QDir outputDir(task->outputDir);
		QString filename = MakeFilename(task->atlasParams.name, resolution, "atlas");
		QFile file(outputDir.absoluteFilePath(filename));
		if(!file.open(QIODevice::WriteOnly)) return;

		QJsonDocument doc(atlas.second);
		file.write(doc.toJson());
	}
}

QStringList Pipeline::GetResolutionsNames()
{
	return resizer.GetResolutionsNames();
}

Project* Pipeline::GetProject()
{
	return project;
}

bool Pipeline::IsProjectOpened()
{
	return project != nullptr;
}

bool Pipeline::NewProject()
{
	if(IsProjectOpened()) return false;

	project = new Project();
	return true;
}

bool Pipeline::OpenProject(const QString& path)
{
	if(IsProjectOpened()) return false;

	QFile file(path);
	if(!file.open(QIODevice::ReadOnly)) return false;

	QByteArray data = file.readAll();
	QJsonDocument doc(QJsonDocument::fromJson(data));
	QString projectName = QFileInfo(path).baseName();

	project = new Project(projectName, QFileInfo(path).path(), doc.object());
	return true;
}

bool Pipeline::SaveProject(const QString& path)
{
	if(!IsProjectOpened()) return false;

	QFile file(path);
	QFileInfo fileInfo(path);
	if(!file.open(QIODevice::WriteOnly)) return false;

	project->SetName(fileInfo.baseName());
	project->SetProjectPath(fileInfo.path());

	QJsonDocument doc(project->ToJson());
	file.write(doc.toJson());

	return true;
}

void Pipeline::CloseProject()
{
	delete project;
	project = nullptr;
}

QString Pipeline::RunProject()
{
	if(!IsProjectOpened()) return QString::null;

	// Check task params for errors
	for(Task* task : project->GetTasks())
	{
		QString error = CheckTask(task);
		if(error != QString::null)
		{
			return QString("Errors in task \"%1\":\n%2").arg(task->name).arg(error);
		}
	}

	for(Task* task : project->GetTasks()) RunTask(task);

	return QString::null;
}
