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

#pragma once

#include "resolution.h"
#include <QStringList>
#include <QVector>
#include <QHash>

const QString ORGANIZATION_NAME = "luna2d";
const QString APP_NAME = "Emulator";
const int MAX_RECENT_GAMES = 10;

class Settings
{
public:
	static QStringList recentGames; // List of recent games
	static bool openLastGame; // Open last game on startup
	static bool gameWasOpened; // Is game was opened before closing
	static QVector<Resolution> resolutions;
	static int curResolution; // Index of current resolution in "resolutions" vector
	static int bullitinCount; // Count of bullit-in resolutions
	static bool openLogWhenError; // Open log window when occurs log message with error
	static bool clearLogOnStart; // Clear log when game stated
	static bool showFps; // Show FPS in window caption
	static QHash<QString,QString> pipelineProjects; // Pipeline projects attachet to game name

public:
	static void Load();
	static void Save();
	static void AddRecentGame(const QString& gamePath);
	static void SetPipelineProject(const QString& gameName, const QString& projectPath); // Set pipeline project form given game
	static QString GetPipelineProject(const QString& gameName); // Get path to pipeline project form given game
};
