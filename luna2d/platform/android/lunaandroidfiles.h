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

#pragma once

#include "lunafiles.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <zip.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

namespace luna2d{

//-------------------------------------
// File utils implementaton for Android
//-------------------------------------
class LUNAAndroidFiles : public LUNAFiles
{
public:
	LUNAAndroidFiles(const std::string& apkPath, const std::string& appFolderPath);

private:
	std::string apkPath; // Saved path to this .apk file
	std::string appFolderPath; // Path to application folder in /data/data/
	std::unordered_map<std::string, std::pair<int, int>> directoryCache;
	std::unordered_map<std::string, int> filesCache;

private:
	zip* OpenApk(); // Open .apk with libzip
	void CacheZipNames(); // Cache list of files in apk file for quick access to files\directories in .apk
	std::string GetPathInLocation(const std::string& path, LUNAFileLocation location);

public:
	// Get root folder for file location
	virtual std::string GetRootFolder(LUNAFileLocation location = LUNAFileLocation::ASSETS);

	// Check for given path is file
	virtual bool IsFile(const std::string& path, LUNAFileLocation location = LUNAFileLocation::ASSETS);

	// Check for given path is directory
	virtual bool IsDirectory(const std::string& path, LUNAFileLocation location = LUNAFileLocation::ASSETS);

	// Check for path is exists
	virtual bool IsExists(const std::string& path, LUNAFileLocation location = LUNAFileLocation::ASSETS);

	// Get list of files and subdirectories in given directory
	virtual std::vector<std::string> GetFileList(const std::string& path, LUNAFileLocation location = LUNAFileLocation::ASSETS);

	// Get size of file
	virtual ssize_t GetFileSize(const std::string& path, LUNAFileLocation location = LUNAFileLocation::ASSETS);

	// Read all file data
	virtual std::vector<unsigned char> ReadFile(const std::string& path, LUNAFileLocation location = LUNAFileLocation::ASSETS);

	// Read all file data as string
	virtual std::string ReadFileToString(const std::string& path, LUNAFileLocation location = LUNAFileLocation::ASSETS);

	// Write given byte buffer to file
	virtual bool WriteFile(const std::string& path, const std::vector<unsigned char>& data, LUNAFileLocation location = LUNAFileLocation::APP_FOLDER);

	// Write given text data to file
	virtual bool WriteFileFromString(const std::string& path, const std::string& data, LUNAFileLocation location);

	// Read all data from file compressed using "Deflate" algorithm
	virtual std::vector<unsigned char> ReadCompressedFile(const std::string& path, LUNAFileLocation location);

	// Write given byte buffer to file and compress it with "Deflate" algorithm
	virtual bool WriteCompressedFile(const std::string& path, const std::vector<unsigned char>& data, LUNAFileLocation location);
};

}
