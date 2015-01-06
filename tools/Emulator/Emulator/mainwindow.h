//-----------------------------------------------------------------------------
// luna2d Emulator
// This is part of luna2d engine
// Copyright 2014 Stepan Prokofjev
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
#include <QMainWindow>

namespace Ui{
class MainWindow;
}

const QString WINDOW_TITLE = "luna2d Emulator (FPS: %1)";

class MainWindow : public QMainWindow
{
	Q_OBJECT

	static const int SCREEN_MARGIN = 80;

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow* ui;
	WatcherDialog* watcherDlg;
	QString curGamePath; // Path to current open game

private:
	void SetupRecentGames(); // Setup recent games menu
	void SetupResolutionMenu(); // Setup resolution menu
	QString CheckGameDirectory(const QString& path); // Check given directory for it's valid game directory
	void OpenGame(const QString& gamePath); // Launch game from given path
	void SetResolution(int resolutionIndex);

public slots:
	void OnGlSurfaceInitialized();
	void OnGameLoopIteration();
	void OnActionOpen();
	void OnActionRestart();
	void OnActionClose();
	void OnRecentGame();
	void OnActionWatcher();
	void OnResolutionChanged();
	void OnActionSettings();
	void OnAbout();
	void OnWatcherClosed();

public:
	void closeEvent(QCloseEvent*);
};
