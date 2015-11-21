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

static bool SaveImage(const QImage& image, const QString& directory, const QString& filename, OutputFormat format)
{
	QDir outputDir(directory);
	const char* strFormat = nullptr;

	switch(format)
	{
	case OutputFormat::PNG_32:
	case OutputFormat::PNG_24:
		strFormat = "PNG";
		break;
	case OutputFormat::JPEG:
		strFormat = "JPEG";
		break;
	}

	return image.save(outputDir.absoluteFilePath(filename), strFormat);
}

static QString ExtensionForFormat(OutputFormat format)
{
	switch(format)
	{
	case OutputFormat::PNG_32:
	case OutputFormat::PNG_24:
		return "png";
	case OutputFormat::JPEG:
		return "jpg";
	}

	return "";
}

Pipeline::Pipeline()
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
	}

	if(!task->atlas && !task->resize) return "Task haven't any pipeline stage";

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
		UpdateProgress();
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
				QString filename = MakeFilename(entry.first, resolution, ExtensionForFormat(task->outputFormat));
				if(!SaveImage(entry.second, task->outputDir, filename, task->outputFormat))
				{
					errors.push_back(ERROR_MASK.arg(task->name).arg(ERROR_SAVE_MASK.arg(filename)));
				}
			}
		}
	}
}

void Pipeline::BuildAtlasStage(QHash<QString,ImageList> images, Task* task)
{
	for(const QString& resolution : images.keys())
	{
		int width = task->atlasParams.sizes[resolution].width();
		int height = task->atlasParams.sizes[resolution].height();

		if(!MathUtils::IsPowerOfTwo(width) || !MathUtils::IsPowerOfTwo(height))
		{
			QString error = ERROR_MASK.arg(task->name).arg("Sizes of atlas for resolution \"%1\" must be power of two");
			errors.push_back(error.arg(resolution));
			return;
		}

		QPair<QImage,QJsonObject> atlas = atlasBuilder.Run(images[resolution], width, height,
			task->outputFormat, task->atlasParams);

		// Save atlas image
		QString imageFilename = MakeFilename(task->atlasParams.name, resolution, ExtensionForFormat(task->outputFormat));
		if(!SaveImage(atlas.first, task->outputDir, imageFilename, task->outputFormat))
		{
			errors.push_back(ERROR_MASK.arg(task->name).arg(ERROR_SAVE_MASK.arg(imageFilename)));
			return;
		}

		// Save atlas json description
		QDir outputDir(task->outputDir);
		QString filename = MakeFilename(task->atlasParams.name, resolution, "atlas");
		QFile file(outputDir.absoluteFilePath(filename));
		if(!file.open(QIODevice::WriteOnly))
		{
			errors.push_back(ERROR_MASK.arg(task->name).arg(ERROR_SAVE_MASK.arg(filename)));
			return;
		}

		QJsonDocument doc(atlas.second);
		file.write(doc.toJson());

		auto atlasErrors = atlasBuilder.GetErrors();
		if(!atlasErrors.empty())
		{
			for(auto& error : atlasErrors)
			{
				QString resError = QString("%1 for \"%2\" resolution").arg(error).arg(resolution);
				errors.push_back(ERROR_MASK.arg(task->name).arg(resError));
			}
		}

		UpdateProgress();
	}
}

void Pipeline::UpdateProgress()
{
	curStages++;
	emit progressUpdated(curStages / (float)totalStages);
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
	totalStages = project->GetStagesCount();

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
	errors.clear();
	delete project;
	project = nullptr;
}

QStringList Pipeline::RunProject()
{
	errors.clear();

	if(!IsProjectOpened()) return errors;

	emit progressUpdated(0);

	for(Task* task : project->GetTasks())
	{
		QString error = CheckTask(task);

		if(error.isNull()) RunTask(task);
		else errors.push_back(ERROR_MASK.arg(task->name).arg(error));
	}

	curStages = 0;

	return errors;
}
