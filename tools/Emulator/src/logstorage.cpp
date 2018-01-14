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

#include "logstorage.h"
#include "settings.h"

void LogStorage::OnLogInfo(const QString& message)
{
	logMessages.push_back(LogMessage(LogType::LOG_INFO, message));
}

void LogStorage::OnLogWarning(const QString& message)
{
	logMessages.push_back(LogMessage(LogType::LOG_WARNING, message));
}

void LogStorage::OnLogError(const QString& message)
{
	logMessages.push_back(LogMessage(LogType::LOG_ERROR, message));
}

void LogStorage::OnAnalyticsData(const QString& event, const QHash<QString,QString>& data)
{
	if(!Settings::showAnalyticsEvents) return;

	OnLogInfo(FormatAnalyticsData(event, data));
}

void LogStorage::Clear()
{
	logMessages.clear();
}

const QList<LogMessage>& LogStorage::GetLogMessages()
{
	return logMessages;
}

QString LogStorage::FormatAnalyticsData(const QString &event, const QHash<QString, QString> &data)
{
	QString ret = QString("Analytics event: \"%1\"").arg(event);

	for(const QString& key : data.keys())
	{
		ret.append(QString("\n  [%1] = %2").arg(key).arg(data[key]));
	}

	return ret;
}

