//-----------------------------------------------------------------------------
// luna2d engine
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

#include "lunaqtlog.h"
#include <QtDebug>

using namespace luna2d;

// Log info
void LUNAQtLog::Info(const char* message, ...)
{
	QString qmsg;
	va_list va;

	va_start(va, message);
	qmsg = QString().vsprintf(message, va);
	va_end(va);

	qDebug() << qmsg; // Print log to system console
	emit logInfo(qmsg);
}

// Log warning
void LUNAQtLog::Warning(const char* message, ...)
{
	QString qmsg;
	va_list va;

	va_start(va, message);
	qmsg = QString().vsprintf(message, va);
	va_end(va);

	qWarning() << qmsg; // Print log to system console
	emit logWarning(qmsg);
}

// Log error
void LUNAQtLog::Error(const char* message, ...)
{
	QString qmsg;
	va_list va;

	va_start(va, message);
	qmsg = QString().vsprintf(message, va);
	va_end(va);

	qCritical() << qmsg; // Print log to system console
	emit logError(qmsg);
}
