//-----------------------------------------------------------------------------
// luna2d engine
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

#pragma once

#include "lunaqtwidget.h"
#include "lunawatcher.h"
#include <QDialog>
#include <QPair>
#include <QString>

namespace Ui {
class WatcherDialog;
}

//---------------------
// Widget with userdata
//---------------------
template<typename BaseWidget>
class UserDataWidget : public BaseWidget
{
public:
	explicit UserDataWidget(QWidget *parent = 0) : BaseWidget(parent) {}

public:
	std::string table, field;

public:
	void SetUserData(const std::string& table, const std::string& field)
	{
		this->table = table;
		this->field = field;
	}
};

class WatcherDialog : public QDialog, public luna2d::LUNAWatcherListener
{
	Q_OBJECT

public:
	explicit WatcherDialog(luna2d::LUNAQtWidget* engineWidget, QWidget* parent = 0);
	~WatcherDialog();

private:
	Ui::WatcherDialog* ui;
	luna2d::LUNAQtWidget* engineWidget;
	luna2d::LUNAWatcher* watcher;

private:
	void AttachWatcher();
	void DetachWatcher();

private slots:
	void OnStringFieldChanged(const QString& value);
	void OnNumberFieldChanged(double value);
	void OnBoolFieldChanged(bool value);

public slots:
	void OnEngineInitialized();

public:
	virtual void OnTableAdded(const std::string& name);
	virtual void OnTableRemoved(const std::string& name);
	virtual void OnFieldChanged(const std::string& tableName, const std::string& fieldName, const luna2d::LuaAny& value);
	virtual void OnWatcherStopped();
};
