//-----------------------------------------------------------------------------
// luna2d DeployTool UI
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include <QMessageBox>
#include <QFileDialog>
#include <json11.hpp>

using namespace json11;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	FillPlatformCombo();

	ui->btnNext->setDisabled(true);

	connect(ui->btnInputPath, &QToolButton::clicked, this, &MainWindow::OnInputPathButton);
	connect(ui->btnNext, &QPushButton::clicked, this, &MainWindow::OnNext);
	connect(ui->btnBackParams, &QPushButton::clicked, this, &MainWindow::OnParamsBack);
	connect(ui->btnNextParams, &QPushButton::clicked, this, &MainWindow::OnParamsNext);
	connect(ui->pages, &QStackedWidget::currentChanged, this, &MainWindow::OnPageOpened);
	connect(ui->btnAbout, &QPushButton::clicked, this, &MainWindow::OnAbout);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::FillPlatformCombo()
{
	ui->comboTargetOs->addItem("iOS");
	ui->comboTargetOs->addItem("Android");
	ui->comboTargetOs->addItem("Windows Phone");

	// Disable iOS target on non-OSX platforms
#ifndef Q_OS_MAC
	ui->comboTargetOs->setItemData(0, QVariant(0), Qt::UserRole - 1);
	ui->comboTargetOs->setCurrentIndex(1);
#endif

	// Disable Windows Phone target on non-Windows platforms
#ifndef Q_OS_WIN32
	ui->comboTargetOs->setItemData(2, QVariant(0), Qt::UserRole - 1);
#endif
}

QString MainWindow::CheckGameProjectPath(const QString& path)
{
	QFile file(path + "/config.luna2d");

	if(!file.exists()) return "Game project directory must contains \"config.luna2d\" file";
	else return "";
}

void MainWindow::OnInputPathButton()
{
	QFileDialog dialog;
	dialog.setFileMode(QFileDialog::DirectoryOnly);
	dialog.setOption(QFileDialog::ShowDirsOnly);

	if(dialog.exec())
	{
		QString path = dialog.selectedFiles().first();
		QString error = CheckGameProjectPath(path);

		if(error.isEmpty())
		{
			ui->editInputPath->setText(path);
			ui->btnNext->setDisabled(false);
		}
		else
		{
			QMessageBox::critical(this, "Error opening game project", error);
			ui->btnNext->setDisabled(!CheckGameProjectPath(ui->editInputPath->text()).isEmpty());
		}
	}
}

void MainWindow::OnNext()
{
	QString inputPath = ui->editInputPath->text();
	QString error = CheckGameProjectPath(inputPath);

	if(!error.isEmpty())
	{
		QMessageBox::critical(this, "Error opening game project", error);
		ui->btnNext->setDisabled(true);
		return;
	}

	ui->pages->setCurrentWidget(ui->pageParams);

	QString outputPath = inputPath.left(inputPath.lastIndexOf("/")) + "/";
	outputPath += "project-wp";

	ui->editOutputPath->setText(outputPath);

	QFile configFile(inputPath + "/config.luna2d");
	if(!configFile.open(QIODevice::ReadOnly)) return;

	std::string jsonErr;
	Json jsonConfig = Json::parse(configFile.readAll().toStdString(), jsonErr, JsonParse::COMMENTS);

	if(jsonConfig == nullptr) return;

	if(jsonConfig["name"].is_string())
	{
		ui->editName->setText(QString::fromStdString(jsonConfig["name"].string_value()));
	}
}

void MainWindow::OnParamsBack()
{
	ui->pages->setCurrentWidget(ui->pageMain);
}

void MainWindow::OnParamsNext()
{
	QString projectPath = ui->editOutputPath->text();
	QDir dir(projectPath);

	if(dir.exists())
	{
		QMessageBox::critical(this, "Directory already exists",
			"Cannot create project in \"" + projectPath + "\".\nDirectory already exists.");
		return;
	}

	ui->pages->setCurrentWidget(ui->pageOutput);
}

void MainWindow::OnOutputPathButton()
{
	QFileDialog dialog;
	dialog.setFileMode(QFileDialog::DirectoryOnly);
	dialog.setOption(QFileDialog::ShowDirsOnly);

	if(dialog.exec())
	{
		ui->editOutputPath->setText(dialog.selectedFiles().first());
	}
}

void MainWindow::OnPageOpened(int pageIndex)
{
	if(pageIndex != ui->pages->indexOf(ui->pageOutput)) return;

	QStringList args =
	{
		"--luna2d_path", GetLuna2dPath(),
		"--game_path", ui->editInputPath->text(),
		"--project_path", ui->editOutputPath->text(),
		"--template", "project-wp",
		"--name", ui->editName->text(),
		"--platform", "wp",
	};

	auto output = RunScript("generateproject.py", args);
	for(auto& str : output) ui->txtOutput->append(str);
}

void MainWindow::OnAbout()
{
	QMessageBox::about(this, "About", "luna2d DeployTool UI\nThis is part of luna2d engine\nCopyright 2014-2016 Stepan Prokofjev");
}
