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

	FillTemplates();
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

void MainWindow::FillTemplates()
{
	Template tmplIos("iOS", "project-ios", "ios");
	Template tmplAndroid("Android", "project-android", "android");
	Template tmplWp("Windows Phone", "project-wp", "wp");

	// Disable iOS target on non-OSX platforms
#ifndef Q_OS_MAC
	tmplIos.enabled = false;
#endif

	// Disable Windows Phone target on non-Windows platforms
#ifndef Q_OS_WIN32
	tmplWp.enabled = false;
#endif

	templates.push_back(tmplIos);
	templates.push_back(tmplAndroid);
	templates.push_back(tmplWp);
}

void MainWindow::FillPlatformCombo()
{
	int selected = 0;

	for(int i = 0; i < templates.size(); i++)
	{
		auto& tmpl = templates[i];

		ui->comboTargetOs->addItem(tmpl.displayName);
		if(!tmpl.enabled)
		{
			ui->comboTargetOs->setItemData(i, QVariant(0), Qt::UserRole - 1);
			if(selected == i) selected++;
		}
	}

	ui->comboTargetOs->setCurrentIndex(selected);
}

QString MainWindow::CheckGameProjectPath(const QString& path)
{
	QFile file(path + "/config.luna2d");

	if(!file.exists()) return "Game project directory must contains \"config.luna2d\" file";
	else return "";
}

const Template&MainWindow::GetSelectedTemplate()
{
	int index = ui->comboTargetOs->currentIndex();
	return templates[index];
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
		return;
	}

	// Load config for get game name
	QFile configFile(inputPath + "/config.luna2d");
	if(!configFile.open(QIODevice::ReadOnly))
	{
		QMessageBox::critical(this, "Error opening game project", "Error loading \"config.luna2d\" file");
		return;
	}

	std::string jsonErr;
	Json jsonConfig = Json::parse(configFile.readAll().toStdString(), jsonErr, JsonParse::COMMENTS);

	if(jsonConfig == nullptr)
	{
		QMessageBox::critical(this, "Error opening game project", "Error parsing \"config.luna2d\" file");
		return;
	}

	if(!jsonConfig["name"].is_string() || jsonConfig["name"].string_value().empty())
	{
		QMessageBox::critical(this, "Error opening game project",
			"\"config.luna2d\" file hasn't required \"name\" field");
		return;
	}

	ui->pages->setCurrentWidget(ui->pageParams);

	auto selectedTemplate = this->GetSelectedTemplate();
	QString gameName = QString::fromStdString(jsonConfig["name"].string_value());

	// Set default path using template name
	QString outputPath = inputPath.left(inputPath.lastIndexOf("/")) + "/" + selectedTemplate.name;
	ui->editOutputPath->setText(outputPath);

	// Set project name using game name from config
	ui->editName->setText(gameName);
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

	auto selectedTemplate = this->GetSelectedTemplate();

	QStringList args =
	{
		"--luna2d_path", GetLuna2dPath(),
		"--game_path", ui->editInputPath->text(),
		"--project_path", ui->editOutputPath->text(),
		"--template", selectedTemplate.name,
		"--name", ui->editName->text(),
		"--platform", selectedTemplate.platform,
	};

	auto output = RunScript("generateproject.py", args);
	for(auto& str : output) ui->txtOutput->append(str);
}

void MainWindow::OnAbout()
{
	QMessageBox::about(this, "About", "luna2d DeployTool UI\nThis is part of luna2d engine\nCopyright 2014-2016 Stepan Prokofjev");
}
