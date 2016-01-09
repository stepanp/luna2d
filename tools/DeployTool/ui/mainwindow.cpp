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
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	FillPlatformCombo();

	connect(ui->btnInputPath, &QToolButton::clicked, this, &MainWindow::OnInputPathButton);
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

void MainWindow::OnInputPathButton()
{
	QFileDialog dialog;
	dialog.setFileMode(QFileDialog::DirectoryOnly);
	dialog.setOption(QFileDialog::ShowDirsOnly);

	if(dialog.exec())
	{
		ui->editInputPath->setText(dialog.selectedFiles().first());
	}
}


void MainWindow::OnAbout()
{
	QMessageBox::about(this, "About", "luna2d DeployTool UI\nThis is part of luna2d engine\nCopyright 2014-2016 Stepan Prokofjev");
}
