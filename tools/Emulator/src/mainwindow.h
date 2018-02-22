//-----------------------------------------------------------------------------
// luna2d Emulator
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

#pragma once

#include "settings.h"
#include "watcherdialog.h"
#include "logdialog.h"
#include "logstorage.h"
#include <QMainWindow>

namespace Ui{
class MainWindow;
}

const QString WINDOW_TITLE = "luna2d Emulator";
const QString WINDOW_TITLE_NAME = "%1 - " + WINDOW_TITLE;
const QString WINDOW_TITLE_FPS = "%1 - " + WINDOW_TITLE + " [FPS: %2]";
const QString MENU_NO_PIPELINE_PROJECT = "<Project is not set>";
const QString SCREENSHOT_NAME = "Screenshot (%1).png";
const QString FORBIDDEN_FILE_SYMBOLS =  "\\/:?\"<>|";

class MainWindow : public QMainWindow
{
	Q_OBJECT

	static const int SCREEN_MARGIN = 80;

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow* ui;
	LogStorage* logStorage;
	LogDialog* logDlg;
	WatcherDialog* watcherDlg;
	QString curGamePath; // Path to current opened game
	QString curGameName; // Name of current opened game
	QPixmap screenshotsPixmap; // Pixmap to taking screnshots
	ScreenOrientation curScreenOrientation = ScreenOrientation::LANDSCAPE; // Current screen orientation
	QImage topMask, bottomMask, bottomBarMask;
	QPoint topMaskPos, bottomMaskPos, bottomBarMaskPos;

private:
	void SetupRecentGames(); // Setup recent games menu
	void SetupResolutionMenu(); // Setup resolution menu
	QString CheckGameDirectory(const QString& path); // Check given directory for it's valid game directory
	void OpenGame(const QString& gamePath); // Launch game from given path
	void CloseGame();
	void SetScreenOrientation(ScreenOrientation orientation);
	Resolution GetResolution(int resolutionIndex);
	void SetResolution(int resolutionIndex);
	void SetLanguage(QString localeCode);
	void SetScreenMask(const Resolution& resolution);
	void OpenLogDialog();
	void UpdatePipelineMenu(); // Update "Tools/Pipeline" menu
	void UpdateLanguagesMenu(); // Update "Settings/Game languages" menu
	QString MakeScreenhotsFolder();
	QString GetPipelinePath();

public slots:
	void OnGlSurfaceInitialized();
	void OnGameLoopIteration();
	void OnRenderIteration(QOpenGLPaintDevice* paintDevice);
	void OnActionOpen();
	void OnActionRestart();
	void OnActionClose();
	void OnRecentGame();
	void OnActionLog();
	void OnActionWatcher();
	void OnResolutionChanged();
	void OnLanguageChanged();
	void OnActionSettings();
	void OnAbout();
	void OnLogClosed();
	void OnWatcherClosed();
	void OnLogError();
	void OnRunPipelineProject();
	void OnRunPipelineProjectAndRestart();
	void OnOpenInPipeline();
	void OnSetPipelineProject();
	void OnTakeScreenshot();
	void OnOpenScreenshotsFolder();
	void OnClearGamePrefs();

public:
	void closeEvent(QCloseEvent*);

	// Move window to screen center
	void MoveToCenter();
};
