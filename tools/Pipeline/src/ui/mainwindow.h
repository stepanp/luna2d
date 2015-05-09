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

#pragma once

#include "pipeline/pipeline.h"
#include "project/project.h"
#include "projecttreedata.h"
#include "settings.h"
#include <QTreeWidget>
#include <QComboBox>
#include <QListWidget>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

const int PAGE_TASK = 0;
const int PAGE_PROJECT = 1;

const int TAB_TASK = 0;
const int TAB_RESIZE = 1;
const int TAB_ATLAS = 2;

const int PAGE_ATLAS_SIZE_SIMPLE = 0;
const int PAGE_ATLAS_SIZE_ADVANCED = 1;
const int ATLAS_SIZE_SIMPLE_HEIGHT = 25;
const int ATLAS_SIZE_ADVANCED_HEIGHT = 250;

const QString WINDOW_TITLE = "luna2d Pipeline";
const QString WINDOW_TITLE_PATTERN = "%2 - %1";

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	Pipeline pipeline;
	QString curProject; // Path to current open project file

private:
	void SelectPage();
	void UpdateProjectPage();
	void UpdateTaskTabs();
	void UpdateUi();
	void UpdateRecentMenu();
	void UpdateProjectTree();
	void UpdateResizeTab();
	void UpdateAtlasTab();
	void UpdateAtlasSizesTable();
	void SetAtlasSizesAdvancedMode(bool advanced);
	QTreeWidgetItem* MakeTreeItem(const QString& text, const ProjectTreeData& data);
	QComboBox* MakeAtlasSizeComboBox();
	QAction* MakeSeparator();
	Task* GetSelectedTask();
	TaskNode* GetSelectedTaskNode();
	Task* GetTaskFromItem(QTreeWidgetItem* item);

public slots:
	void OnNewProject();
	void OnOpenProject();
	void OnSaveProject();
	void OnSaveProjectAs();
	void OnCloseProject();
	void OnRecentProject();
	void OnAddTask();
	void OnRemoveTask();
	void OnAddFile();
	void OnAddFolder();
	void OnRemoveNode();
	void OnRunProject();
	void OnAbout();
    void OnSelectedTreeItem(QTreeWidgetItem*, QTreeWidgetItem*);
	void OnProjectTreeMenu(const QPoint& point);

	// "Project settings" page slots
	void OnProjectChangedRelative(bool value);

	// "Task settings" tab slots
	void OnTabsChangedName(const QString& value);
	void OnTabsChangedOutput(const QString& value);
	void OnTabsOutputButton();
	void OnTabsChangedResize(bool value);
	void OnTabsChangedAtlas(bool value);

	// "Resize settings" tab slots
	void OnTabsChangedSourceResolution(const QString& value);
	void OnTabsChangedResolutionsList(QListWidgetItem* item);
	void OnTabsChangedPotSize(bool value);

	// "Atlas settings" tab slots
	void OnTabsChangedAtlasName(const QString& value);
	void OnTabsChangedAdvancedMode(bool value);
	void OnTabsChangedMaxWidth(int value);
	void OnTabsChangedMaxHeight(int value);
	void OnTabsChangedPadding(int value);
	void OnTabsChangedDuplicatePadding(bool value);
};
