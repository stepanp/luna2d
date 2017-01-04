//-----------------------------------------------------------------------------
// luna2d engine
// This is part of luna2d engine
// Copyright 2014-2017 Stepan Prokofjev
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

#include "watcherdialog.h"
#include "ui_watcherdialog.h"
#include "settings.h"
#include "lunaengine.h"
#include "lunadebug.h"
#include <cfloat>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QStringList>

using namespace luna2d;

WatcherDialog::WatcherDialog(LUNAQtWidget* engineWidget, QWidget* parent) :
	QDialog(parent),
	ui(new Ui::WatcherDialog),
	engineWidget(engineWidget)
{
	ui->setupUi(this);

	// Remove "?" button from caption
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	// Set stretch sections in header of fields table
	ui->tableFields->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->tableFields->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
	ui->tableFields->setColumnWidth(2, 20);
	ui->tableFields->setEnabled(false);

	if(engineWidget->IsEngineInitialized()) AttachWatcher();

	connect(engineWidget, &LUNAQtWidget::engineInitialized, this, &WatcherDialog::OnEngineInitialized);
}

WatcherDialog::~WatcherDialog()
{
	DetachWatcher();

	Settings::watcherRect.setTopLeft(pos());
	Settings::watcherRect.setSize(size());
	Settings::Save();

	delete ui;
}

void WatcherDialog::AttachWatcher()
{
	ui->tableFields->setEnabled(true);

	watcher = LUNAEngine::SharedDebug()->GetWatcher().get(); // TODO: replace to std::shared_ptr here
	watcher->SetListener(this);
	watcher->FetchAllTables();
}

void WatcherDialog::DetachWatcher()
{
	if(watcher)
	{
		watcher->SetListener(nullptr);
		watcher = nullptr;
	}

	// Clear table widget
	while(ui->tableFields->rowCount() > 0) ui->tableFields->removeRow(0);
	ui->tableFields->setEnabled(false);
}

void WatcherDialog::RememberValue(const std::string& tableName, const std::string& fieldName, bool remember)
{
	if(remember)
	{
		Settings::SetWatcherValue(engineWidget->GetGameName(),
			QString::fromStdString(tableName), QString::fromStdString(fieldName),
			watcher->GetValue(tableName, fieldName).To<QVariant>());
	}
	else
	{
		Settings::RemoveWatcherValue(engineWidget->GetGameName(),
			QString::fromStdString(tableName), QString::fromStdString(fieldName));
	}
}

void WatcherDialog::OnStringFieldChanged(const QString &value)
{
	UserDataWidget<QLineEdit>* edit = static_cast<UserDataWidget<QLineEdit>*>(sender());
	watcher->SetValue(edit->table, edit->field, LuaAny(LUNAEngine::SharedLua(), value));
	if(edit->checkRemember->isChecked()) RememberValue(edit->table, edit->field, true);
}

void WatcherDialog::OnNumberFieldChanged(double value)
{
	UserDataWidget<QDoubleSpinBox>* spinBox = static_cast<UserDataWidget<QDoubleSpinBox>*>(sender());
	watcher->SetValue(spinBox->table, spinBox->field, LuaAny(LUNAEngine::SharedLua(), (float)value));
	if(spinBox->checkRemember->isChecked()) RememberValue(spinBox->table, spinBox->field, true);
}

void WatcherDialog::OnBoolFieldChanged(bool value)
{
	UserDataWidget<QCheckBox>* checkBox = static_cast<UserDataWidget<QCheckBox>*>(sender());
	watcher->SetValue(checkBox->table, checkBox->field, LuaAny(LUNAEngine::SharedLua(), value));
	if(checkBox->checkRemember->isChecked()) RememberValue(checkBox->table, checkBox->field, true);
}

void WatcherDialog::OnRememberValueChanged(bool checked)
{
	UserDataWidget<QCheckBox>* checkRemember = static_cast<UserDataWidget<QCheckBox>*>(sender());
	RememberValue(checkRemember->table, checkRemember->field, checked);
}

void WatcherDialog::OnEngineInitialized()
{
	AttachWatcher();
}

void WatcherDialog::OnTableAdded(const std::string& name)
{
	//int row = ui->tableFields->rowCount();
	//ui->tableFields->insertRow(row);
	//ui->tableFields->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(name)));
}

void WatcherDialog::OnTableRemoved(const std::string& name)
{

}

void WatcherDialog::OnFieldChanged(const std::string& tableName, const std::string& fieldName, const LuaAny& value)
{
	int type = value.GetType();

	// Don't add fields with following types
	if(type == LUA_TNIL || type == LUA_TTABLE || type == LUA_TFUNCTION ||
			type == LUA_TLIGHTUSERDATA || type == LUA_TUSERDATA) return;

	// Try load saved value
	auto qValue = Settings::GetWatcherValue(engineWidget->GetGameName(),
		QString::fromStdString(tableName), QString::fromStdString(fieldName));
	bool valueSaved = !qValue.isNull();

	if(valueSaved)
	{
		// Apply saved value
		watcher->SetValue(tableName, fieldName, LuaAny(LUNAEngine::SharedLua(), qValue));
	}
	else
	{
		// Use read value
		qValue = value.To<QVariant>();
	}

	// Insert new row with field
	int row = ui->tableFields->rowCount();
	ui->tableFields->insertRow(row);

	// Set field name
	ui->tableFields->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(fieldName)));

	// Add "remember value" check box
	UserDataWidget<QCheckBox>* checkRemember = new UserDataWidget<QCheckBox>(this);
	checkRemember->SetUserData(tableName, fieldName);
	checkRemember->setChecked(valueSaved);
	connect(checkRemember, &QCheckBox::toggled, this, &WatcherDialog::OnRememberValueChanged);
	ui->tableFields->setCellWidget(row, 2, checkRemember);

	// Add widget for field value
	if(type == LUA_TSTRING)
	{
		UserDataWidget<QLineEdit>* edit = new UserDataWidget<QLineEdit>(this);
		edit->setText(qValue.toString());
		edit->SetUserData(tableName, fieldName, checkRemember);
		connect(edit, &QLineEdit::textChanged, this, &WatcherDialog::OnStringFieldChanged);

		ui->tableFields->setCellWidget(row, 1, edit);
	}

	else if(type == LUA_TNUMBER)
	{
		UserDataWidget<QDoubleSpinBox>* spinBox = new UserDataWidget<QDoubleSpinBox>(this);
		spinBox->setMinimum(-DBL_MAX);
		spinBox->setMaximum(DBL_MAX);
		spinBox->setValue(qValue.toFloat());
		spinBox->setSingleStep(0.5);
		spinBox->SetUserData(tableName, fieldName, checkRemember);
		connect(spinBox, SIGNAL(valueChanged(double)), this, SLOT(OnNumberFieldChanged(double)));

		ui->tableFields->setCellWidget(row, 1, spinBox);
	}

	else if(type == LUA_TBOOLEAN)
	{
		UserDataWidget<QCheckBox>* checkBox = new UserDataWidget<QCheckBox>(this);
		checkBox->setChecked(qValue.toBool());
		checkBox->SetUserData(tableName, fieldName, checkRemember);
		connect(checkBox, &QCheckBox::toggled, this, &WatcherDialog::OnBoolFieldChanged);

		ui->tableFields->setCellWidget(row, 1, checkBox);
	}

	// Sort items in table widget by name,
	// Because order of itens in lua hash tables is not defined
	ui->tableFields->sortByColumn(0, Qt::AscendingOrder);
}

void WatcherDialog::OnWatcherStopped()
{
	watcher = nullptr;
	DetachWatcher();
}
