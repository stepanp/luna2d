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

#include "lunaqtutils.h"
#include <QLocale>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

using namespace luna2d;

LUNAQtUtils::LUNAQtUtils(QWidget* parent) :
	parent(parent)
{
}

// Get system locale in "xx_XX" format
// Where "xx" is ISO-639 language code, and "XX" is ISO-3166 country code
std::string LUNAQtUtils::GetSystemLocale()
{
	return QLocale::system().name().toStdString();
}

// Open given url in system browser
void LUNAQtUtils::OpenUrl(const std::string& url)
{
	QDesktopServices::openUrl(QUrl(QString::fromStdString(url)));
}

// Show native dialog with "Ok" button
// "onClose" calls when dialog closed
void LUNAQtUtils::MessageDialog(const std::string& title, const std::string& message,
	const std::function<void()>& onClose)
{
	QMessageBox* messageBox = new QMessageBox(parent);

	messageBox->setAttribute(Qt::WA_DeleteOnClose);
	messageBox->setStandardButtons(QMessageBox::Ok);
	messageBox->setWindowTitle(QString::fromStdString(title));
	messageBox->setText(QString::fromStdString(message));

	if(onClose)
	{
		messageBox->connect(messageBox, &QMessageBox::finished, [onClose](int result) { onClose(); });
	}

	messageBox->open();
}

// Show native dialog with "Yes" and "No" buttons
// "onClose" calls with "true" when "Yes" button pressed, and with "false" otherwise
void LUNAQtUtils::ConfirmDialog(const std::string& title, const std::string& message,
	const std::function<void(bool)>& onClose)
{
	QMessageBox* messageBox = new QMessageBox(parent);

	messageBox->setAttribute(Qt::WA_DeleteOnClose);
	messageBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	messageBox->setWindowTitle(QString::fromStdString(title));
	messageBox->setText(QString::fromStdString(message));

	if(onClose)
	{
		messageBox->connect(messageBox, &QMessageBox::finished,
			[onClose](int result)
			{
				onClose(result == QMessageBox::Yes);
			});
	}

	messageBox->open();
}
