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

#include "ui/mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QMessageBox>
#include <QProgressDialog>

enum class LaunchMode
{
	OPEN_UI, // Open project in UI
	RUN_UI, // Run project with output to UI
	RUN_CONSOLE, // Run project with output to console
	RUN_SILENT, // Run project without any output
};

void ParseCommandLine(QString& projectPath, LaunchMode& launchMode)
{
	QCommandLineParser cmdParser;
	cmdParser.addOption(QCommandLineOption("u")); // RUN_UI launch mode
	cmdParser.addOption(QCommandLineOption("c")); // RUN_CONSOLE launch mode
	cmdParser.addOption(QCommandLineOption("s")); // RUN_SILENT launch mode
	cmdParser.process(*QApplication::instance());
	QStringList args = cmdParser.positionalArguments();

	// Read path to project
	if(!args.empty())
	{
		projectPath = args.first();
		projectPath.remove("\""); // Remove quotes from path
	}

	if(cmdParser.isSet("u")) launchMode = LaunchMode::RUN_UI;
	else if(cmdParser.isSet("c")) launchMode = LaunchMode::RUN_CONSOLE;
	else if(cmdParser.isSet("s")) launchMode = LaunchMode::RUN_SILENT;
	else launchMode = LaunchMode::OPEN_UI;
}

void RunProjectUi(const QString& projectPath)
{
	Pipeline pipeline;
	if(!pipeline.OpenProject(projectPath)) return;

	QProgressDialog dlg("Processing project...", QString::null, 0, 100, nullptr, TOOL_WINDOW);
	dlg.setMinimumDuration(0);
	dlg.setFixedSize(400, dlg.size().height());

	dlg.connect(&pipeline, &Pipeline::progressUpdated,
		[&dlg](float progress)
		{
			dlg.setValue(progress * 100);
			QApplication::processEvents();
		});

	QStringList errors = pipeline.RunProject();
	if(!errors.empty())
	{
		dlg.cancel();
		QMessageBox::critical(nullptr, "Errors", errors.join("\n"));
	}
}

void RunProjectSilent(const QString& projectPath)
{
	Pipeline pipeline;
	if(pipeline.OpenProject(projectPath))
	{
		pipeline.RunProject();
	}
}

int main(int argc, char* argv[])
{	
	QApplication app(argc, argv);

	QString projectPath;
	LaunchMode launchMode = LaunchMode::OPEN_UI;
	ParseCommandLine(projectPath, launchMode);

	// Open project in UI. If project path is empty, UI will be opened without project
	if(launchMode == LaunchMode::OPEN_UI)
	{
		MainWindow wnd(projectPath);
		wnd.show();
		return app.exec();
	}

	// Run project with specifed mode
	else if(launchMode == LaunchMode::RUN_UI) RunProjectUi(projectPath);
	else if(launchMode == LaunchMode::RUN_SILENT) RunProjectSilent(projectPath);

	return 0;
}
