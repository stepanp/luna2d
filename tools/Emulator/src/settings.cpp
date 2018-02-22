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

#include "settings.h"
#include <QSettings>
#include <QDir>

// Default values of settings
QStringList Settings::recentGames = QStringList();
bool Settings::openLastGame = true;
bool Settings::gameWasOpened = false;
bool Settings::scaleHdpiResolution = true;
QVector<Resolution> Settings::resolutions = QVector<Resolution>();
int Settings::curResolution = 0;
int Settings::bullitinCount = 0;
bool Settings::openLogWhenError = true;
bool Settings::clearLogOnStart = true;
bool Settings::showFps = true;
bool Settings::showAnalyticsEvents = false;
QHash<QString,QString> Settings::pipelineProjects = QHash<QString,QString>();
QHash<QString,QString> Settings::preferredLanguages = QHash<QString,QString>();
QHash<QString,QVector<WatcherValue>> Settings::watcherValues = QHash<QString,QVector<WatcherValue>>();
QRect Settings::logRect;
QRect Settings::watcherRect;

void Settings::Load()
{
	QSettings settings(ORGANIZATION_NAME, APP_NAME);

	openLastGame = settings.value("openLastGame", openLastGame).toBool();
	gameWasOpened = settings.value("gameWasOpened", gameWasOpened).toBool();
	scaleHdpiResolution = settings.value("scaleHdpiResolution", scaleHdpiResolution).toBool();
	curResolution = settings.value("curResolution", curResolution).toInt();
	openLogWhenError = settings.value("openLogWhenError", openLogWhenError).toBool();
	clearLogOnStart = settings.value("clearLogOnStart", clearLogOnStart).toBool();
	showFps = settings.value("showFps", showFps).toBool();
	showAnalyticsEvents = settings.value("showAnalyticsEvents", showAnalyticsEvents).toBool();
	logRect = settings.value("logRect").toRect();
	watcherRect = settings.value("watcherRect").toRect();

	// Load recent games
	int recentGamesCount = settings.beginReadArray("recentGames");
	for(int i = 0; i < recentGamesCount; i++)
	{
		settings.setArrayIndex(i);
		recentGames.push_back(settings.value("path").toString());
	}
	settings.endArray();

	// Add bullit-in resolutions
	resolutions.push_back(Resolution(480, 320, "HVGA", true));
	resolutions.push_back(Resolution(800, 480, "WVGA", true));
	resolutions.push_back(Resolution(960, 540, "qHD", true));
	resolutions.push_back(Resolution(960, 640, "iPhone", true));
	resolutions.push_back(Resolution(1136, 640, "iPhone 5", true));
	resolutions.push_back(Resolution(1334, 750, "iPhone 6", true));
	resolutions.push_back(Resolution(2208, 1242, "iPhone 6 Plus", true));
	resolutions.push_back(Resolution(1280, 720, "HD", true));
	resolutions.push_back(Resolution(1024, 768, "iPad", true));
	resolutions.push_back(Resolution(1920, 1080, "FullHD", true));
	resolutions.push_back(Resolution(2048, 1536, "Retina iPad", true));
	resolutions.push_back(Resolution(2732, 2048, "iPad Pro", true));
	resolutions.push_back(Resolution(2436, 1125, "iPhone X",
		ScreenMargins(132, 102, "margin_top_iphonex", "margin_bottom_iphonex"), true));
	bullitinCount = resolutions.size();

	// Load user-defined resolutions
	int resolutionsCount = settings.beginReadArray("resolutions");
	for(int i = 0; i < resolutionsCount; i++)
	{
		settings.setArrayIndex(i);

		int width = settings.value("width").toInt();
		int height = settings.value("height").toInt();
		QString name = settings.value("name").toString();

		resolutions.push_back(Resolution(width, height, name));
	}
	settings.endArray();

	// Load pipeline projects paths
	int pipelineProjectsCount = settings.beginReadArray("pipelineProjects");
	for(int i = 0; i < pipelineProjectsCount; i++)
	{
		settings.setArrayIndex(i);
		Settings::pipelineProjects[settings.value("game").toString()] = settings.value("project").toString();
	}
	settings.endArray();

	// Load preferred languages
	int preferredLanguagesCount = settings.beginReadArray("preferredLanguages");
	for(int i = 0; i < preferredLanguagesCount; i++)
	{
		settings.setArrayIndex(i);
		Settings::preferredLanguages[settings.value("game").toString()] = settings.value("language").toString();
	}
	settings.endArray();

	// Load watcher values
	int watcherValuesCount = settings.beginReadArray("watcherValuesByGame");
	for(int i = 0; i < watcherValuesCount; i++)
	{
		settings.setArrayIndex(i);
		auto gameName = settings.value("game").toString();

		int valuesCount = settings.beginReadArray("watcherValues");
		for(int j = 0; j < valuesCount; j++)
		{
			settings.setArrayIndex(j);

			auto tableName = settings.value("tableName").toString();
			auto valueName = settings.value("valueName").toString();
			auto value = settings.value("value");

			Settings::watcherValues[gameName].push_back({ tableName, valueName, value });
		}

		settings.endArray();
	}
	settings.endArray();
}

void Settings::Save()
{
	QSettings settings(ORGANIZATION_NAME, APP_NAME);

	settings.setValue("openLastGame", openLastGame);
	settings.setValue("gameWasOpened", gameWasOpened);
	settings.setValue("scaleHdpiResolution", scaleHdpiResolution);
	settings.setValue("curResolution", curResolution);
	settings.setValue("openLogWhenError", openLogWhenError);
	settings.setValue("clearLogOnStart", clearLogOnStart);
	settings.setValue("showFps", showFps);
	settings.setValue("showAnalyticsEvents", showAnalyticsEvents);
	settings.setValue("logRect", logRect);
	settings.setValue("watcherRect", watcherRect);

	// Save recent games
	settings.beginWriteArray("recentGames");
	for(int i = 0; i < recentGames.size(); i++)
	{
		settings.setArrayIndex(i);
		settings.setValue("path", recentGames.at(i));
	}
	settings.endArray();

	// Save resolutions
	settings.beginWriteArray("resolutions");
	for(int i = bullitinCount; i < resolutions.size(); i++)
	{
		settings.setArrayIndex(i - bullitinCount);
		settings.setValue("width", resolutions.at(i).width);
		settings.setValue("height", resolutions.at(i).height);
		settings.setValue("name", resolutions.at(i).name);
	}
	settings.endArray();

	// Save pipeline projects paths
	settings.beginWriteArray("pipelineProjects");
	int i = 0;
	for(auto it = Settings::pipelineProjects.begin(); it != Settings::pipelineProjects.end(); it++)
	{
		settings.setArrayIndex(i);
		settings.setValue("game", it.key());
		settings.setValue("project", it.value());
		i++;
	}
	settings.endArray();

	// Save preferred languages
	settings.beginWriteArray("preferredLanguages");
	i = 0;
	for(auto it = Settings::preferredLanguages.begin(); it != Settings::preferredLanguages.end(); it++)
	{
		settings.setArrayIndex(i);
		settings.setValue("game", it.key());
		settings.setValue("language", it.value());
		i++;
	}
	settings.endArray();

	// Save watcher values
	settings.beginWriteArray("watcherValuesByGame");
	i = 0;
	for(auto it = Settings::watcherValues.begin(); it != Settings::watcherValues.end(); it++)
	{
		settings.setArrayIndex(i);
		settings.setValue("game", it.key());

		settings.beginWriteArray("watcherValues");
		int j = 0;
		for(const auto& watcherValue : it.value())
		{
			settings.setArrayIndex(j);
			settings.setValue("tableName", watcherValue.tableName);
			settings.setValue("valueName", watcherValue.valueName);
			settings.setValue("value", watcherValue.value);
			j++;
		}

		settings.endArray();
		i++;
	}
	settings.endArray();
}

static QString NormalizePath(const QString& path)
{
	QDir dir(path);
	QString ret = dir.toNativeSeparators(dir.absolutePath());

#ifdef Q_OS_WIN32
	if(!ret.isEmpty()) ret[0] = ret.at(0).toUpper(); // Make disk letter upper on windows
#endif

	return ret;
}

void Settings::AddRecentGame(const QString& gamePath)
{
	QString normalizedPath = NormalizePath(gamePath);

	int index = recentGames.indexOf(normalizedPath);

	// If given game already exists, move it to top,
	// else just add
	if(index != -1) recentGames.move(index, 0);
	else
	{
		recentGames.push_front(normalizedPath);

		// Limit max games count
		if(recentGames.size() > MAX_RECENT_GAMES) recentGames.removeLast();
	}
}

// Set pipeline project form given game
void Settings::SetPipelineProject(const QString& gameName, const QString& projectPath)
{
	Settings::pipelineProjects[gameName] = projectPath;
}

// Get path to pipeline project form given game
QString Settings::GetPipelineProject(const QString& gameName)
{
	return Settings::pipelineProjects[gameName];
}

// Set prederred language for given game
void Settings::SetPreferredLanguage(const QString& gameName, const QString& language)
{
	Settings::preferredLanguages[gameName] = language;
}

// Get prederred language for given game
QString Settings::GetPreferredLanguage(const QString& gameName)
{
	if(Settings::preferredLanguages.count(gameName) == 0) return "system";
	return Settings::preferredLanguages[gameName];
}

QVariant Settings::GetWatcherValue(const QString& gameName, const QString& tableName, const QString& valueName)
{
	if(Settings::watcherValues.count(gameName) == 0) return {};

	for(const auto& watcherValue : Settings::watcherValues[gameName])
	{
		if(watcherValue.tableName == tableName && watcherValue.valueName == valueName) return watcherValue.value;
	}

	return {};
}

void Settings::SetWatcherValue(const QString& gameName, const QString& tableName, const QString& valueName, const QVariant& value)
{
	for(auto& watcherValue : Settings::watcherValues[gameName])
	{
		if(watcherValue.tableName == tableName && watcherValue.valueName == valueName)
		{
			watcherValue.value = value;
			return;
		}
	}

	Settings::watcherValues[gameName].push_back({ tableName, valueName, value });
}

void Settings::RemoveWatcherValue(const QString& gameName, const QString& tableName, const QString& valueName)
{
	if(Settings::watcherValues.count(gameName) == 0) return;

	auto& watcherValues = Settings::watcherValues[gameName];
	for(int i = 0; i < watcherValues.size(); i++)
	{
		const auto& watcherValue = watcherValues[i];

		if(watcherValue.tableName == tableName && watcherValue.valueName == valueName)
		{
			watcherValues.remove(i);
			return;
		}
	}
}
