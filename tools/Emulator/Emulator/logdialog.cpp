//-----------------------------------------------------------------------------
// luna2d Emulator
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

#include "logdialog.h"
#include "ui_logdialog.h"

LogDialog::LogDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LogDialog)
{
	ui->setupUi(this);

	// Fill filter combo box
	ui->comboFilter->addItems({"Info", "Warning", "Error"});

	connect(ui->btnClear, &QPushButton::pressed, ui->listLog, &QListWidget::clear);
}

LogDialog::~LogDialog()
{
	delete ui;
}

void LogDialog::OnInfo(const QString& message)
{
	ui->listLog->addItem(message);
}

void LogDialog::OnWarning(const QString& message)
{
	QListWidgetItem* item = new QListWidgetItem(message);
	item->setForeground(Qt::yellow);
	ui->listLog->addItem(item);
}

void LogDialog::OnError(const QString& message)
{
	QListWidgetItem* item = new QListWidgetItem(message);
	item->setForeground(Qt::red);
	ui->listLog->addItem(item);
}
