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
#include "settings.h"

LogDialog::LogDialog(LogStorage* logStorage, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LogDialog),
	logStorage(logStorage)
{
	ui->setupUi(this);

	// Fill filter combo box
	ui->comboFilter->addItems({"Info", "Warning", "Error"});

	connect(ui->btnClear, &QPushButton::pressed, ui->listLog, &QListWidget::clear);
	connect(ui->btnClear, &QPushButton::pressed, logStorage, &LogStorage::Clear);

	ui->checkClearOnStart->setChecked(Settings::clearLogOnStart);
	connect(ui->checkClearOnStart, &QCheckBox::toggled, this, &LogDialog::OnStartClearChecked);

	// Show all log messages from log storage
	for(LogMessage msg: logStorage->GetLogMessages()) ShowLogMessage(msg.first, msg.second);
}

LogDialog::~LogDialog()
{
	delete ui;
}

void LogDialog::ShowLogMessage(LogType type, const QString& message)
{
	QListWidgetItem* item = new QListWidgetItem(message);

	if(type == LogType::LOG_WARNING) item->setForeground(Qt::yellow);
	else if(type == LogType::LOG_ERROR) item->setForeground(Qt::red);

	ui->listLog->addItem(item);
}

void LogDialog::OnStartClearChecked(bool checked)
{
	Settings::clearLogOnStart = checked;
}

void LogDialog::OnLogInfo(const QString& message)
{
	ShowLogMessage(LogType::LOG_INFO, message);
}

void LogDialog::OnLogWarning(const QString& message)
{
	ShowLogMessage(LogType::LOG_WARNING, message);
}

void LogDialog::OnLogError(const QString& message)
{
	ShowLogMessage(LogType::LOG_ERROR, message);
}

void LogDialog::Clear()
{
	ui->listLog->clear();
}
