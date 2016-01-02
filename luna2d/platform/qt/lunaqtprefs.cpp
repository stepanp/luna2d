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

#include "lunaqtprefs.h"
#include <QSettings>

using namespace luna2d;

QString LUNAQtPrefs::GetGameName()
{
	// We can't get game name in constructor
	// because at moment of creating LUNAQtPrefs
	// engine not initialized completely yet
	// So use lazy initialization
	if(gameName.isEmpty()) gameName = QString("game%1").arg(LUNAEngine::Shared()->GetGameName().c_str());
	return gameName;
}

QVariant LUNAQtPrefs::GetValue(const std::string& name)
{
	QSettings settings(PREFS_ORGANIZATION_NAME, GetGameName());
	return settings.value(QString::fromStdString(name));
}

void LUNAQtPrefs::SetValue(const std::string& name, const QVariant& value)
{
	QSettings settings(PREFS_ORGANIZATION_NAME, GetGameName());
	settings.setValue(QString::fromStdString(name), value);
}

// Get string value from preferences
std::string LUNAQtPrefs::GetString(const std::string& name)
{
	return GetValue(name).toString().toStdString();
}

// Get int value from preferences
int LUNAQtPrefs::GetInt(const std::string& name)
{
	return GetValue(name).toInt();
}

// Get float value from preferences
float LUNAQtPrefs::GetFloat(const std::string& name)
{
	return GetValue(name).toFloat();
}

// Get bool value from preferences
bool LUNAQtPrefs::GetBool(const std::string& name)
{
	return GetValue(name).toBool();
}

// Set string value to preferences
void LUNAQtPrefs::SetString(const std::string& name, const std::string& value)
{
	SetValue(name, QString::fromStdString(value));
}

// Set int value to preferences
void LUNAQtPrefs::SetInt(const std::string& name, int value)
{
	SetValue(name, value);
}

// Set float value to preferences
void LUNAQtPrefs::SetFloat(const std::string& name, float value)
{
	SetValue(name, value);
}

// Set bool value to preferences
void LUNAQtPrefs::SetBool(const std::string& name, bool value)
{
	SetValue(name, value);
}

// Check for existing value
bool LUNAQtPrefs::HasValue(const std::string &name)
{
	QSettings settings(PREFS_ORGANIZATION_NAME, GetGameName());
	return settings.contains(QString::fromStdString(name));
}

// Remove valuee from preferences
void LUNAQtPrefs::RemoveValue(const std::string& name)
{
	QSettings settings(PREFS_ORGANIZATION_NAME, GetGameName());
	settings.remove(QString::fromStdString(name));
}

// Remove all values from preferences
void LUNAQtPrefs::Clear()
{
	QSettings settings(PREFS_ORGANIZATION_NAME, GetGameName());
	settings.clear();
}
