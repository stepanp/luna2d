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

#include "lunaqtfiles.h"
#include "lunalog.h"
#include <QFile>
#include <QDir>
#include <QStandardPaths>

using namespace luna2d;

LUNAQtFiles::LUNAQtFiles(const QString& gamePath) :
	gamePath(gamePath.toStdString())
{
	// Append slash to end of path if it not exists in given path
	if(!gamePath.endsWith('/')) this->gamePath.append("/");
}

// Convert given path in a path relative to root directory of given location
QString LUNAQtFiles::GetPathInLocation(const std::string& path, LUNAFileLocation location)
{
	return QString::fromStdString(GetRootFolder(location) + path);
}

// Get root folder for file location
std::string LUNAQtFiles::GetRootFolder(LUNAFileLocation location)
{
	switch(location)
	{
	case LUNAFileLocation::ASSETS:
		return gamePath;

	case LUNAFileLocation::APP_FOLDER:
		std::string ret = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString();
		ret += "/luna2d/Emulator/" + LUNAEngine::Shared()->GetGameName() + "/";
		return ret;
	}

	return "";
}

// Check for given path is file
bool LUNAQtFiles::IsFile(const std::string& path, LUNAFileLocation location)
{
	QFileInfo fileInfo(GetPathInLocation(path, location));

	return fileInfo.isFile();
}

// Check for given path is directory
bool LUNAQtFiles::IsDirectory(const std::string& path, LUNAFileLocation location)
{
	QFileInfo fileInfo(GetPathInLocation(path, location));

	return fileInfo.isDir();
}

// Check for path is exists
bool LUNAQtFiles::IsExists(const std::string& path, LUNAFileLocation location)
{
	QFileInfo fileInfo(GetPathInLocation(path, location));

	return fileInfo.exists();
}

// Get list of files and subdirectories in given directory
std::vector<std::string> LUNAQtFiles::GetFileList(const std::string& path, LUNAFileLocation location)
{
	std::vector<std::string> ret;

	QDir dir(GetPathInLocation(path, location));
	if(!dir.exists()) return std::move(ret); // Return empty file list

	QFileInfoList list = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
	for(QFileInfo entry : list)
	{
		if(entry.isDir()) ret.push_back(entry.fileName().toStdString() + "/");
		else ret.push_back(entry.fileName().toStdString());
	}

	return std::move(ret);
}

// Get size of file
ssize_t LUNAQtFiles::GetFileSize(const std::string& path, LUNAFileLocation location)
{
	QFile file(GetPathInLocation(path, location));

	if(!file.exists()) return -1;
	else return	file.size();
}

// Read all file data
std::vector<unsigned char> LUNAQtFiles::ReadFile(const std::string &path, LUNAFileLocation location)
{
	QFile file(GetPathInLocation(path, location));
	std::vector<unsigned char> ret;

	if(!file.exists()) return std::move(ret);
	if(!file.open(QIODevice::ReadWrite)) return std::move(ret);

	size_t size = file.size();
	ret.resize(size);
	file.read(reinterpret_cast<char*>(&ret[0]), size);
	file.close();

	return std::move(ret);
}

// Read all file data as string
std::string LUNAQtFiles::ReadFileToString(const std::string &path, LUNAFileLocation location)
{
	QFile file(GetPathInLocation(path, location));
	std::string ret;

	if(!file.exists()) return std::move(ret);
	if(!file.open(QIODevice::ReadWrite)) return std::move(ret);

	size_t size = file.size();
	ret.resize(size);
	file.read(&ret[0], size);
	file.close();

	return std::move(ret);
}

// Write given byte buffer to file
bool LUNAQtFiles::WriteFile(const std::string &path, const std::vector<unsigned char> &data, LUNAFileLocation location)
{
	QFile file(GetPathInLocation(path, location));

	// Create dir for file if not exists
	QFileInfo info(GetPathInLocation(path, location));
	QDir dir = info.dir();
	if(!dir.exists()) dir.mkpath(".");

	if(!file.open(QIODevice::WriteOnly)) return false;

	file.write(reinterpret_cast<const char*>(data.data()), data.size());
	file.close();

	return true;
}

// Write given text data to file
bool LUNAQtFiles::WriteFileFromString(const std::string& path, const std::string& data, LUNAFileLocation location)
{
	QFile file(GetPathInLocation(path, location));

	// Create dir for file if not exists
	QFileInfo info(GetPathInLocation(path, location));
	QDir dir = info.dir();
	if(!dir.exists()) dir.mkpath(".");

	if(!file.open(QIODevice::WriteOnly)) return false;

	file.write(data.data(), data.size());
	file.close();

	return true;
}

// Read all data from file compressed using "Deflate" algorithm
std::vector<unsigned char> LUNAQtFiles::ReadCompressedFile(const std::string& path, LUNAFileLocation location)
{
	LUNA_LOGE("Method LUNAQtFiles::ReadCompressedFile is not implemented");
	return std::vector<unsigned char>();
}

// Write given byte buffer to file and compress it with "Deflate" algorithm
bool LUNAQtFiles::WriteCompressedFile(const std::string& path, const std::vector<unsigned char>& data, LUNAFileLocation location)
{
	LUNA_LOGE("Method LUNAQtFiles::WriteCompressedFile is not implemented");
	return false;
}
