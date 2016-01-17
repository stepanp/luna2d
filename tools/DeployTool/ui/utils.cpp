//-----------------------------------------------------------------------------
// luna2d DeployTool UI
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

#include "utils.h"
#include <QDir>
#include <QCoreApplication>
#include <QProcess>
#include <QDebug>

QString GetLuna2dPath()
{
	QDir dir(QCoreApplication::applicationDirPath() + "../../../");
	return dir.absolutePath();
}

QString GetScriptsPath()
{
	return QCoreApplication::applicationDirPath() + "/python/";
}

QStringList RunScript(const QString& name, const QStringList& args)
{
	QProcess process;
	QStringList output;

	process.connect(&process, &QProcess::readyRead,
		[&]()
		{
			QString outputPart = process.readAll();
			QStringList outputPartList = outputPart.split(QRegExp("\n|\r\n|\r"), QString::SkipEmptyParts);

			outputPartList.append(output);
			output = outputPartList;
		});

	process.setProcessChannelMode(QProcess::MergedChannels);
	process.start("python " + GetScriptsPath() + name + " " + args.join(" "));
	process.waitForFinished();

	return output;
}
