//-----------------------------------------------------------------------------
// luna2d Pipeline
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
#include <cmath>

QStringList Settings::GetRecentProjects()
{
	QSettings settings(ORGANIZATION_NAME, APP_NAME);
	QStringList ret;

	int recentCount = settings.beginReadArray("recentProjects");
	for(int i = 0; i < recentCount; i++)
	{
		settings.setArrayIndex(i);
		ret.push_back(settings.value("path").toString());
	}
	settings.endArray();

	return ret;
}

void Settings::AddRecentProject(const QString& project)
{
	QSettings settings(ORGANIZATION_NAME, APP_NAME);
	QStringList recentProjects = Settings::GetRecentProjects();

	int index = recentProjects.indexOf(project);

	// If given project already exists, move it to top,
	// else just add
	if(index != -1) recentProjects.move(index, 0);
	else recentProjects.push_front(project);

	int recentCount = std::min(recentProjects.size(), MAX_RECENT_PROJECTS);
	settings.beginWriteArray("recentProjects");
	for(int i = 0; i < recentCount; i++)
	{
		settings.setArrayIndex(i);
		settings.setValue("path", recentProjects.at(i));
	}
	settings.endArray();
}
