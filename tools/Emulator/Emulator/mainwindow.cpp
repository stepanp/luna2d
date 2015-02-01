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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_settings.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QCommandLineParser>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	logStorage(new LogStorage()),
	curGamePath(QString::null)
{
	ui->setupUi(this);

	SetupRecentGames();
	SetupResolutionMenu();
	SetResolution(Settings::curResolution);

	ui->actionRestart_game->setEnabled(false);
	ui->actionClose_game->setEnabled(false);

	// Set placeholder image to engine widget
	ui->centralWidget->SetPlaceholderImage(QImage(":/images/placeholder_image.png"));

	connect(ui->centralWidget, &luna2d::LUNAQtWidget::glSurfaceInitialized, this, &MainWindow::OnGlSurfaceInitialized);
	connect(ui->centralWidget, &luna2d::LUNAQtWidget::gameLoopIteration, this, &MainWindow::OnGameLoopIteration);
	connect(ui->centralWidget, &luna2d::LUNAQtWidget::logInfo, logStorage, &LogStorage::OnLogInfo);
	connect(ui->centralWidget, &luna2d::LUNAQtWidget::logWarning, logStorage, &LogStorage::OnLogWarning);
	connect(ui->centralWidget, &luna2d::LUNAQtWidget::logError, logStorage, &LogStorage::OnLogError);
	connect(ui->actionOpen_game, &QAction::triggered, this, &MainWindow::OnActionOpen);
	connect(ui->actionRestart_game, &QAction::triggered, this, &MainWindow::OnActionRestart);
	connect(ui->actionClose_game, &QAction::triggered, this, &MainWindow::OnActionClose);
	connect(ui->actionLog, &QAction::triggered, this, &MainWindow::OnActionLog);
	connect(ui->actionWatcher, &QAction::triggered, this, &MainWindow::OnActionWatcher);
	connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::OnActionSettings);
	connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::OnAbout);
}

MainWindow::~MainWindow()
{
	delete ui;
	delete logStorage;
}

// Setup recent games menu
void MainWindow::SetupRecentGames()
{
	// Remove previous items
	ui->menuRecent->clear();

	// Disable recent games menu if list of recent games is empty
	ui->menuRecent->setEnabled(!Settings::recentGames.empty());

	// Add all recent games from settings
	for(QString game : Settings::recentGames)
	{
		QAction *action = new QAction(game, this);
		ui->menuRecent->addAction(action);
		connect(action, &QAction::triggered, this, &MainWindow::OnRecentGame);
	}
}

void MainWindow::SetupResolutionMenu()
{
	ui->menuResolution->clear();

	QString format = "%1 (%2x%3)";
	QActionGroup *group = new QActionGroup(this);

	for(int i = 0; i < Settings::resolutions.size(); i++)
	{
		auto& res = Settings::resolutions.at(i);

		QAction *action = new QAction(format.arg(res.name).arg(res.width).arg(res.height), this);
		action->setCheckable(true);
		action->setActionGroup(group);
		action->setData(i);
		ui->menuResolution->addAction(action);
		connect(action, &QAction::triggered, this, &MainWindow::OnResolutionChanged);

		// Check current selected resolution
		if(i == Settings::curResolution) action->setChecked(true);

		// Add separator after bullit-in resolutions
		if(i == Settings::bullitinCount - 1) ui->menuResolution->addSeparator();
	}
}

// Check given directory for it's valid game directory
QString MainWindow::CheckGameDirectory(const QString &path)
{
	QDir dir(path);
	if(!dir.exists("scripts/")) return "directory \"scripts\" not found";

	dir.cd("scripts/");
	if(!dir.exists("main.lua")) return "\"main.lua\" not found";

	return QString::Null();
}

// Launch game from given path
void MainWindow::OpenGame(const QString &gamePath)
{
	QString error = CheckGameDirectory(gamePath);

	if(!error.isNull())
	{
		QString errorMessage = "Cannot open game: " + error;
		QMessageBox::critical(this, "Error opening game", errorMessage);
		return;
	}

	// Update recent games list
	Settings::AddRecentGame(gamePath);
	Settings::gameWasOpened = true;
	SetupRecentGames();

	curGamePath = gamePath;
	ui->actionRestart_game->setEnabled(true);
	ui->actionClose_game->setEnabled(true);

	// Launch game
	auto& resolution = Settings::resolutions.at(Settings::curResolution);
	ui->centralWidget->DeinitializeEngine();
	ui->centralWidget->InitializeEngine(gamePath, resolution.width, resolution.height);
}

void MainWindow::SetResolution(int resolutionIndex)
{
	const Resolution &resolution = Settings::resolutions.at(resolutionIndex);

	int wndWidth = resolution.width;
	int wndHeight = resolution.height;
	int screenWidth = QApplication::desktop()->availableGeometry().width() - SCREEN_MARGIN;
	int screenHeight = QApplication::desktop()->availableGeometry().height() - SCREEN_MARGIN;

	// Downscale window if emulator resolution large than screen resolution
	if(wndWidth > screenWidth || wndHeight + ui->menuBar->height() > screenHeight)
	{
		float scaleWidth = screenWidth / (float)wndWidth;
		float scaleHeight = screenHeight / (float)wndHeight;
		float scale = std::min(scaleWidth, scaleHeight);

		wndWidth = std::round((float)wndWidth * scale);
		wndHeight = std::round((float)wndHeight * scale);
	}

	setFixedSize(wndWidth, wndHeight + ui->menuBar->height()); // Consider menubar height

	// Place widnow to screen center
	QRect screenRect = QApplication::desktop()->availableGeometry();
	QRect wndRect = frameGeometry();
	move((screenRect.width() - wndRect.width()) / 2, (screenRect.height() - wndRect.height()) / 2);

	Settings::curResolution = resolutionIndex;
}

void MainWindow::OnGlSurfaceInitialized()
{
	// Try open game from command line
	QCommandLineParser cmdParser;
	cmdParser.process(*QApplication::instance());

	QStringList args = cmdParser.positionalArguments();
	if(!args.empty())
	{
		QString path = args.first();
		path.remove("\""); // Remove quotes from argument
		OpenGame(path);
		return;
	}

	// Launch last recent game on startup
	if(Settings::openLastGame && Settings::gameWasOpened && !Settings::recentGames.empty())
	{
		OpenGame(Settings::recentGames.first());
	}
}

void MainWindow::OnGameLoopIteration()
{
	setWindowTitle(WINDOW_TITLE_FPS.arg(ui->centralWidget->GetFps()));
}

void MainWindow::OnActionOpen()
{
	QFileDialog dialog;
	dialog.setFileMode(QFileDialog::DirectoryOnly);
	dialog.setOption(QFileDialog::ShowDirsOnly);
	dialog.setWindowTitle("Select \"assets\" directory of game");

	if(dialog.exec())
	{
		QString gamePath = dialog.selectedFiles()[0];
		OpenGame(gamePath);
	}
}

void MainWindow::OnActionRestart()
{
	if(curGamePath != QString::null) OpenGame(curGamePath);
}

void MainWindow::OnActionClose()
{
	ui->centralWidget->DeinitializeEngine();
	curGamePath = QString::null;
	ui->actionRestart_game->setEnabled(false);
	ui->actionClose_game->setEnabled(false);
	Settings::gameWasOpened = false;

	setWindowTitle(WINDOW_TITLE);
}

void MainWindow::OnRecentGame()
{
	QString gamePath = qobject_cast<QAction*>(sender())->text();
	OpenGame(gamePath);
}

void MainWindow::OnActionLog()
{
	if(ui->actionLog->isChecked())
	{
		logDlg = new LogDialog(logStorage, this);
		logDlg->show();
		connect(logDlg, &QDialog::rejected, this, &MainWindow::OnLogClosed);

		connect(ui->centralWidget, &luna2d::LUNAQtWidget::logInfo, logDlg, &LogDialog::OnLogInfo);
		connect(ui->centralWidget, &luna2d::LUNAQtWidget::logWarning, logDlg, &LogDialog::OnLogWarning);
		connect(ui->centralWidget, &luna2d::LUNAQtWidget::logError, logDlg, &LogDialog::OnLogError);
	}
	else
	{
		delete logDlg;
		logDlg = nullptr;
	}
}

void MainWindow::OnActionWatcher()
{
	if(ui->actionWatcher->isChecked())
	{
		watcherDlg = new WatcherDialog(ui->centralWidget, this);
		watcherDlg->show();
		connect(watcherDlg, &QDialog::rejected, this, &MainWindow::OnWatcherClosed);
	}
	else
	{
		delete watcherDlg;
		watcherDlg = nullptr;
	}
}

void MainWindow::OnResolutionChanged()
{
	// Set new resolution
	int resolutionIndex = qobject_cast<QAction*>(sender())->data().toInt();
	SetResolution(resolutionIndex);

	// Relaunch game with new resolution
	OnActionRestart();
}

void MainWindow::OnActionSettings()
{
	QDialog dialog(this);
	Ui::SettingsDialog uiDialog;
	uiDialog.setupUi(&dialog);

	uiDialog.openLastGame->setChecked(Settings::openLastGame);

	if(dialog.exec())
	{
		Settings::openLastGame = uiDialog.openLastGame->isChecked();
		Settings::Save();
	}
}

void MainWindow::OnAbout()
{
	QMessageBox::about(this, "About", "luna2d Emulator\nThis is part of luna2d engine\nCopyright 2014-2015 Stepan Prokofjev");
}

void MainWindow::OnLogClosed()
{
	logDlg = nullptr;
	ui->actionLog->setChecked(false);
}

void MainWindow::OnWatcherClosed()
{
	watcherDlg = nullptr;
	ui->actionWatcher->setChecked(false);
}

void MainWindow::closeEvent(QCloseEvent*)
{
	Settings::Save();
}
