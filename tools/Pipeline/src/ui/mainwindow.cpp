//-----------------------------------------------------------------------------
// luna2d Pipeline
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
#include "utils/mathutils.h"
#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
#include <QProgressDialog>

MainWindow::MainWindow(const QString& projectPath) :
	QMainWindow(0),
	ui(new Ui::MainWindow),
	curProject(QString::null)
{
	ui->setupUi(this);

	UpdateRecentMenu();

	ui->pages->setEnabled(false);
	ui->projectTree->setContextMenuPolicy(Qt::CustomContextMenu);

	// Fill output format combo box with formats
	ui->comboFormat->addItems({ "PNG32", "PNG24", "JPEG" });

	// Fill source resolution combo box with resolutions
	QStringList resolutionsList = pipeline.GetResolutionsNames();
	ui->comboSrcResolution->addItems(resolutionsList);

	// Fill resolutions list with check boxes
	ui->listResolutions->clear();
	for(QString res : resolutionsList)
	{
		QListWidgetItem* item = new QListWidgetItem(res, ui->listResolutions);
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		item->setCheckState(Qt::Unchecked);
	}

	// Set stretch sections in header of atlas sizes table
	ui->tableAtlasSizes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	connect(ui->actionNew, &QAction::triggered, this, &MainWindow::OnNewProject);
	connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::OnOpenProject);
	connect(ui->actionSave, &QAction::triggered, this, &MainWindow::OnSaveProject);
	connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::OnSaveProjectAs);
	connect(ui->actionClose, &QAction::triggered, this, &MainWindow::OnCloseProject);
	connect(ui->actionAddTask, &QAction::triggered, this, &MainWindow::OnAddTask);
	connect(ui->actionRemoveTask, &QAction::triggered, this, &MainWindow::OnRemoveTask);
	connect(ui->actionAddFolder, &QAction::triggered, this, &MainWindow::OnAddFolder);
	connect(ui->actionAddFile, &QAction::triggered, this, &MainWindow::OnAddFile);
	connect(ui->actionRemoveNode, &QAction::triggered, this, &MainWindow::OnRemoveNode);
	connect(ui->actionRun, &QAction::triggered, this, &MainWindow::OnRunProject);
	connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::OnAbout);
	connect(ui->projectTree, &QTreeWidget::currentItemChanged, this, &MainWindow::OnSelectedTreeItem);
	connect(ui->projectTree, &QTreeWidget::customContextMenuRequested, this, &MainWindow::OnProjectTreeMenu);

	// "Project settings" page signals
	connect(ui->checkRelative, &QCheckBox::toggled, this, &MainWindow::OnProjectChangedRelative);

	// "Task settings" tab signals
	connect(ui->editName, &QLineEdit::textChanged, this, &MainWindow::OnTabsChangedName);
	connect(ui->editOutput, &QLineEdit::textChanged, this, &MainWindow::OnTabsChangedOutput);
	connect(ui->btnOutput, &QToolButton::clicked, this, &MainWindow::OnTabsOutputButton);
	connect(ui->comboFormat, SIGNAL(currentIndexChanged(int)), this, SLOT(OnTabsCnangedFormat(int)));
	connect(ui->checkResize, &QCheckBox::toggled, this, &MainWindow::OnTabsChangedResize);
	connect(ui->checkAtlas, &QCheckBox::toggled, this, &MainWindow::OnTabsChangedAtlas);

	// "Resize settings" tab signals
	connect(ui->comboSrcResolution, &QComboBox::currentTextChanged, this, &MainWindow::OnTabsChangedSourceResolution);
	connect(ui->checkPot, &QCheckBox::toggled, this, &MainWindow::OnTabsChangedPotSize);

	// "Atlas settings" tab signals
	connect(ui->editAtlasName, &QLineEdit::textChanged, this, &MainWindow::OnTabsChangedAtlasName);
	connect(ui->editPadding,  SIGNAL(valueChanged(int)), this, SLOT(OnTabsChangedPadding(int)));
	connect(ui->checkDuplicatePadding, &QCheckBox::toggled, this, &MainWindow::OnTabsChangedDuplicatePadding);

	// Try open project specifed in command line arguments
	if(!projectPath.isEmpty()) OpenProject(projectPath);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::SelectPage()
{
	QTreeWidgetItem* item = ui->projectTree->currentItem();
	if(!item)
	{
		ui->pages->setEnabled(false);
		return;
	}

	ProjectTreeData data = qvariant_cast<ProjectTreeData>(item->data(0, Qt::UserRole));
	if(data.type == ProjectTreeDataType::PROJECT)
	{
		ui->pages->setCurrentIndex(PAGE_PROJECT);
		ui->pages->setEnabled(true);
		UpdateProjectPage();
	}
	else if(data.type == ProjectTreeDataType::TASK)
	{
		ui->pages->setCurrentIndex(PAGE_TASK);
		ui->pages->setEnabled(true);
		UpdateTaskTabs();
	}
	else
	{
		ui->pages->setEnabled(false);
	}
}

void MainWindow::UpdateProjectPage()
{
	if(!pipeline.IsProjectOpened()) return;

	ui->checkRelative->setChecked(pipeline.GetProject()->IsUseRelativePath());
}

void MainWindow::UpdateTaskTabs()
{
	Task* task = GetSelectedTask();

	ui->editName->setText(task->name);
	ui->editOutput->setText(task->outputDir);
	ui->comboFormat->setCurrentIndex(static_cast<int>(task->outputFormat));
	ui->checkResize->setChecked(task->resize);
	ui->checkAtlas->setChecked(task->atlas);
	ui->tabsSettings->setTabEnabled(TAB_RESIZE, task->resize);
	ui->tabsSettings->setTabEnabled(TAB_ATLAS, task->atlas);

	UpdateResizeTab();
	UpdateAtlasTab();
}

void MainWindow::UpdateUi()
{
	if(pipeline.IsProjectOpened())
	{
		Project *project = pipeline.GetProject();

		setWindowTitle(QString(WINDOW_TITLE_PATTERN).arg(WINDOW_TITLE).arg(project->GetName()));
		ui->projectTree->topLevelItem(0)->setText(0, project->GetName());

		ui->actionSave->setEnabled(true);
		ui->actionSaveAs->setEnabled(true);
		ui->actionClose->setEnabled(true);
		ui->actionAddTask->setEnabled(true);
		ui->actionRemoveTask->setEnabled(true);
		ui->actionAddFolder->setEnabled(true);
		ui->actionAddFile->setEnabled(true);
		ui->actionRemoveNode->setEnabled(true);
		ui->actionRun->setEnabled(true);
	}
	else
	{
		setWindowTitle(WINDOW_TITLE);

		ui->projectTree->clear();
		ui->actionSave->setEnabled(false);
		ui->actionSaveAs->setEnabled(false);
		ui->actionClose->setEnabled(false);
		ui->actionAddTask->setEnabled(false);
		ui->actionRemoveTask->setEnabled(false);
		ui->actionAddFolder->setEnabled(false);
		ui->actionAddFile->setEnabled(false);
		ui->actionRemoveNode->setEnabled(false);
		ui->actionRun->setEnabled(false);
		ui->pages->setEnabled(false);
	}
}

void MainWindow::UpdateRecentMenu()
{
	ui->menuRecent->clear();

	QStringList recentProjects = Settings::GetRecentProjects();
	ui->menuRecent->setEnabled(!recentProjects.empty());

	for(QString project : recentProjects)
	{
		QAction *action = new QAction(project, this);
		ui->menuRecent->addAction(action);
		connect(action, &QAction::triggered, this, &MainWindow::OnRecentProject);
	}
}

void MainWindow::UpdateProjectTree()
{
	if(!pipeline.IsProjectOpened()) return;

	Project *project = pipeline.GetProject();
	QTreeWidgetItem* projItem = new QTreeWidgetItem();
	ui->projectTree->addTopLevelItem(projItem);
	projItem->setText(0, project->GetName());
	projItem->setExpanded(true);
	projItem->setData(0, Qt::UserRole, QVariant::fromValue(ProjectTreeData(ProjectTreeDataType::PROJECT)));

	for(Task* task : project->GetTasks())
	{
		QTreeWidgetItem* taskItem = MakeTreeItem(task->name, ProjectTreeData(ProjectTreeDataType::TASK, task));
		projItem->addChild(taskItem);

		for(TaskNode* node : task->nodes)
		{
			QString type = node->GetNodeType() == TaskNodeType::FILE ? "File: " : "Folder: ";
			taskItem->addChild(MakeTreeItem(type + node->GetPath(), ProjectTreeData(ProjectTreeDataType::TASK_NODE, node)));
		}
	}

	ui->projectTree->setCurrentItem(ui->projectTree->topLevelItem(0));
}

void MainWindow::UpdateResizeTab()
{
	Task* task = GetSelectedTask();

	ui->comboSrcResolution->setCurrentText(task->sourceRes);
	ui->checkPot->setChecked(task->potSize);

	disconnect(ui->listResolutions, &QListWidget::itemChanged, this, &MainWindow::OnTabsChangedResolutionsList);
	for(int i = 0; i < ui->listResolutions->count(); i++)
	{
		QListWidgetItem* item = ui->listResolutions->item(i);
		bool checked = task->outputRes.indexOf(item->text()) != -1;
		item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
	}
	connect(ui->listResolutions, &QListWidget::itemChanged, this, &MainWindow::OnTabsChangedResolutionsList);
}

void MainWindow::UpdateAtlasTab()
{
	Task* task = GetSelectedTask();

	ui->editAtlasName->setText(task->atlasParams.name);
	ui->editPadding->setValue(task->atlasParams.padding);
	ui->checkDuplicatePadding->setChecked(task->atlasParams.duplicatePadding);

	UpdateAtlasSizesTable();
}

void MainWindow::UpdateAtlasSizesTable()
{
	// Remove all rows
	while(ui->tableAtlasSizes->rowCount() > 0) ui->tableAtlasSizes->removeRow(0);

	Task* task = GetSelectedTask();
	QStringList resolutions = pipeline.GetResolutionsNames();
	int rowIndex = 0;

	for(int i = 0; i < resolutions.size(); i++)
	{
		auto res = resolutions.at(i);

		if(task->outputRes.indexOf(res) == -1) continue;

		QSize atlasSize = task->atlasParams.sizes[res];
		QComboBox* comboWidth = MakeAtlasSizeComboBox(atlasSize.width());
		QComboBox* comboHeight = MakeAtlasSizeComboBox(atlasSize.height());

		connect(comboWidth, &QComboBox::currentTextChanged,
			[this, comboWidth, res](const QString& value)
			{
				GetSelectedTask()->atlasParams.sizes[res].setWidth(value.toInt());
			});

		connect(comboHeight, &QComboBox::currentTextChanged,
			[this, comboHeight, res](const QString& value)
			{
				GetSelectedTask()->atlasParams.sizes[res].setHeight(value.toInt());
			});

		ui->tableAtlasSizes->insertRow(rowIndex);
		ui->tableAtlasSizes->setItem(rowIndex, 0, new QTableWidgetItem(resolutions.at(i)));
		ui->tableAtlasSizes->setCellWidget(rowIndex, 1, comboWidth);
		ui->tableAtlasSizes->setCellWidget(rowIndex, 2, comboHeight);

		rowIndex++;
	}
}

QTreeWidgetItem* MainWindow::MakeTreeItem(const QString& text, const ProjectTreeData& data)
{
	QTreeWidgetItem* item = new QTreeWidgetItem();
	item->setText(0, text);
	item->setData(0, Qt::UserRole, QVariant::fromValue(data));
	return item;
}

QComboBox* MainWindow::MakeAtlasSizeComboBox(int defaultSize)
{
	QComboBox* ret = new QComboBox();

	for(int i = MIN_ATLAS_SIZE; i <= MAX_ATLAS_SIZE; i *= 2)
	{
		ret->addItem(QString::number(i), i);
	}

	ret->setCurrentText(QString::number(defaultSize));

	return ret;
}

QAction* MainWindow::MakeSeparator()
{
	QAction* separator = new QAction(this);
	separator->setSeparator(true);
	return separator;
}

Task* MainWindow::GetSelectedTask()
{
	return GetTaskFromItem(ui->projectTree->currentItem());
}

TaskNode* MainWindow::GetSelectedTaskNode()
{
	QTreeWidgetItem* item = ui->projectTree->currentItem();
	if(!item) return nullptr;

	ProjectTreeData data = qvariant_cast<ProjectTreeData>(item->data(0, Qt::UserRole));
	return data.GetData<TaskNode, ProjectTreeDataType::TASK_NODE>();
}

Task* MainWindow::GetTaskFromItem(QTreeWidgetItem *item)
{
	if(!item) return nullptr;

	ProjectTreeData data = qvariant_cast<ProjectTreeData>(item->data(0, Qt::UserRole));
	return data.GetData<Task, ProjectTreeDataType::TASK>();
}

void MainWindow::OpenProject(const QString& path)
{
	if(!pipeline.OpenProject(path))
	{
		QMessageBox::critical(this, QString::null, "Cannot open project \"" + path + "\"");
		return;
	}

	curProject = path;
	UpdateProjectTree();
	UpdateUi();

	Settings::AddRecentProject(path);
	UpdateRecentMenu();
}

void MainWindow::OnNewProject()
{
	OnCloseProject();

	pipeline.NewProject();
	UpdateProjectTree();
	UpdateUi();
}

void MainWindow::OnOpenProject()
{
	OnCloseProject();

	QFileDialog dialog;
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	dialog.setNameFilter("Pipeline project (*.pipeline)");

	if(dialog.exec()) OpenProject(dialog.selectedFiles().first());
}

void MainWindow::OnSaveProject()
{
	if(!pipeline.IsProjectOpened()) return;

	if(curProject == QString::null)
	{
		OnSaveProjectAs();
		return;
	}

	pipeline.SaveProject(curProject);
	UpdateUi();
}

void MainWindow::OnSaveProjectAs()
{
	if(!pipeline.IsProjectOpened()) return;

	QFileDialog dialog;
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setNameFilter("Pipeline project (*.pipeline)");

	if(dialog.exec())
	{
		QString path = dialog.selectedFiles().first();
		pipeline.SaveProject(path);
		curProject = path;
		UpdateUi();
		Settings::AddRecentProject(path);
		UpdateRecentMenu();
	}
}

void MainWindow::OnCloseProject()
{
	pipeline.CloseProject();
	curProject = QString::null;
	UpdateUi();
}

void MainWindow::OnRecentProject()
{
	QString path = qobject_cast<QAction*>(sender())->text();
	OnCloseProject();

	if(pipeline.OpenProject(path))
	{
		curProject = path;
		UpdateProjectTree();
		UpdateUi();

		Settings::AddRecentProject(path);
		UpdateRecentMenu();
	}
}

void MainWindow::OnAddTask()
{
	if(!pipeline.IsProjectOpened()) return;

	Project* project = pipeline.GetProject();
	Task* task = new Task(project);
	project->AddTask(task);
	QTreeWidgetItem* item = MakeTreeItem(task->name, ProjectTreeData(ProjectTreeDataType::TASK, task));

	ui->projectTree->topLevelItem(0)->addChild(item);
	ui->projectTree->setCurrentItem(item);
}

void MainWindow::OnRemoveTask()
{
	if(!GetSelectedTask() || !pipeline.IsProjectOpened()) return;

	Project* project = pipeline.GetProject();
	project->RemoveTask(GetSelectedTask());
	ui->projectTree->topLevelItem(0)->removeChild(ui->projectTree->currentItem());
}

void MainWindow::OnAddFile()
{
	if(!GetSelectedTask()) return;

	QFileDialog dialog;
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	dialog.setNameFilter("Images (*.png *.jpg *.jpeg *.psd)");

	if(dialog.exec())
	{
		QString file = dialog.selectedFiles().first();
		TaskNode* node = new FileNode(file);
		GetSelectedTask()->AddNode(node);
		ui->projectTree->currentItem()->addChild(MakeTreeItem("File: " + file, ProjectTreeData(ProjectTreeDataType::TASK_NODE, node)));
	}
}

void MainWindow::OnAddFolder()
{
	if(!GetSelectedTask()) return;

	QFileDialog dialog;
	dialog.setFileMode(QFileDialog::DirectoryOnly);
	dialog.setOption(QFileDialog::ShowDirsOnly);

	if(dialog.exec())
	{
		QString folder = dialog.selectedFiles().first();
		TaskNode* node = new FolderNode(folder);
		GetSelectedTask()->AddNode(node);
		ui->projectTree->currentItem()->addChild(MakeTreeItem("Folder: " + folder, ProjectTreeData(ProjectTreeDataType::TASK_NODE, node)));
	}
}

void MainWindow::OnRemoveNode()
{
	if(!GetSelectedTaskNode()) return;

	QTreeWidgetItem* item = ui->projectTree->currentItem();
	QTreeWidgetItem* parentItem = item->parent();

	GetTaskFromItem(parentItem)->RemoveNode(GetSelectedTaskNode());
	parentItem->removeChild(item);
}

void MainWindow::OnRunProject()
{
	if(!pipeline.IsProjectOpened()) return;

	QProgressDialog dlg("Processing project...", QString::null, 0, 100, this, TOOL_WINDOW);
	dlg.setMinimumDuration(0);
	dlg.setWindowModality(Qt::WindowModal);
	dlg.setFixedSize(400, dlg.size().height());
	dlg.setWindowTitle(WINDOW_TITLE);

	connect(&pipeline, &Pipeline::progressUpdated,
		[&dlg](float progress)
		{
			dlg.setValue(progress * 100);
			QApplication::processEvents();
		});

	QStringList errors = pipeline.RunProject();
	if(!errors.empty())
	{
		dlg.cancel();
		QMessageBox::critical(this, "Errors", errors.join("\n"));
	}
}

void MainWindow::OnAbout()
{
	QMessageBox::about(this, "About", "luna2d Pipeline\nThis is part of luna2d engine\nCopyright 2014-2016 Stepan Prokofjev");
}

void MainWindow::OnSelectedTreeItem(QTreeWidgetItem*, QTreeWidgetItem*)
{
	SelectPage();
}

void MainWindow::OnProjectTreeMenu(const QPoint &point)
{
	QMenu *menu = new QMenu(this);
	QList<QAction*> actions;

	QTreeWidgetItem* item = ui->projectTree->currentItem();
	if(!item) return;

	ProjectTreeDataType type = (qvariant_cast<ProjectTreeData>(item->data(0, Qt::UserRole))).type;
	if(type == ProjectTreeDataType::PROJECT)
	{
		actions.push_back(ui->actionAddTask);
		actions.push_back(MakeSeparator());
		actions.push_back(ui->actionRun);
	}
	else if(type == ProjectTreeDataType::TASK)
	{
		actions.push_back(ui->actionAddFile);
		actions.push_back(ui->actionAddFolder);
		actions.push_back(MakeSeparator());
		actions.push_back(ui->actionRemoveTask);
	}
	else if(type == ProjectTreeDataType::TASK_NODE)
	{
		actions.push_back(ui->actionRemoveNode);
	}

	menu->addActions(actions);
	menu->popup(ui->projectTree->mapToGlobal(point));
}


//------------------------------
// "Project settings" page slots
//------------------------------

void MainWindow::OnProjectChangedRelative(bool value)
{
	if(!pipeline.IsProjectOpened()) return;

	pipeline.GetProject()->SetUseRelativePath(value);
}


//--------------------------
// "Task settings" tab slots
//--------------------------

void MainWindow::OnTabsChangedName(const QString &value)
{
	if(!GetSelectedTask()) return;

	GetSelectedTask()->name = value;
	ui->projectTree->currentItem()->setText(0, value);
}

void MainWindow::OnTabsChangedOutput(const QString &value)
{
	if(!GetSelectedTask()) return;

	GetSelectedTask()->outputDir = value;
}

void MainWindow::OnTabsOutputButton()
{
	if(!GetSelectedTask()) return;

	QFileDialog dialog;
	dialog.setFileMode(QFileDialog::DirectoryOnly);
	dialog.setOption(QFileDialog::ShowDirsOnly);

	if(dialog.exec())
	{
		GetSelectedTask()->outputDir = dialog.selectedFiles().first();
		ui->editOutput->setText(GetSelectedTask()->outputDir);
	}
}

void MainWindow::OnTabsCnangedFormat(int index)
{
	if(!GetSelectedTask()) return;

	GetSelectedTask()->outputFormat = static_cast<OutputFormat>(index);
}

void MainWindow::OnTabsChangedResize(bool value)
{
	if(!GetSelectedTask()) return;

	GetSelectedTask()->resize = value;
	ui->tabsSettings->setTabEnabled(TAB_RESIZE, value);
}

void MainWindow::OnTabsChangedAtlas(bool value)
{
	if(!GetSelectedTask()) return;

	GetSelectedTask()->atlas = value;
	ui->tabsSettings->setTabEnabled(TAB_ATLAS, value);
}


//----------------------------
// "Resize settings" tab slots
//----------------------------

void MainWindow::OnTabsChangedSourceResolution(const QString& value)
{
	if(!GetSelectedTask()) return;

	GetSelectedTask()->sourceRes = value;
}

void MainWindow::OnTabsChangedResolutionsList(QListWidgetItem* item)
{
	if(!item || !GetSelectedTask()) return;

	QString res = item->text();

	if(item->checkState() == Qt::Checked)
	{
		auto task = GetSelectedTask();
		task->outputRes.push_back(res);
		task->atlasParams.sizes.insert(res, QSize(DEFAULT_ATLAS_SIZE, DEFAULT_ATLAS_SIZE));
	}

	else if(item->checkState() == Qt::Unchecked)
	{
		auto task = GetSelectedTask();
		task->outputRes.removeOne(res);
		task->atlasParams.sizes.remove(res);
	}

	UpdateAtlasSizesTable();
}

void MainWindow::OnTabsChangedPotSize(bool value)
{
	if(!GetSelectedTask()) return;

	GetSelectedTask()->potSize = value;
}


//---------------------------
// "Atlas settings" tab slots
//---------------------------

void MainWindow::OnTabsChangedAtlasName(const QString& value)
{
	if(!GetSelectedTask()) return;

	GetSelectedTask()->atlasParams.name = value;
}

void MainWindow::OnTabsChangedPadding(int value)
{
	if(!GetSelectedTask()) return;

	GetSelectedTask()->atlasParams.padding = value;
}

void MainWindow::OnTabsChangedDuplicatePadding(bool value)
{
	if(!GetSelectedTask()) return;

	GetSelectedTask()->atlasParams.duplicatePadding = value;
}
