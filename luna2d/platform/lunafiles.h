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

#pragma once

#include "lunaengine.h"

namespace luna2d{

// Sets root folder for file operations
enum class LUNAFileLocation
{
	ASSETS, // Files in main game folder (e.g. on Android "game" folder located in "assets" folder in .apk)
	APP_FOLDER, // Files in application data folder (e.g. on Android is /data/data/%app_name")
};

//---------------------
// File utils interface
//---------------------
class LUNAFiles
{
public:
	virtual ~LUNAFiles() {}

public:
	// Get root folder for file location
	virtual std::string GetRootFolder(LUNAFileLocation location = LUNAFileLocation::ASSETS) = 0;

	// Check for given path is file
	virtual bool IsFile(const std::string& path, LUNAFileLocation location = LUNAFileLocation::ASSETS) = 0;

	// Check for given path is directory
	virtual bool IsDirectory(const std::string& path, LUNAFileLocation location = LUNAFileLocation::ASSETS) = 0;

	// Check for path is exists
	virtual bool IsExists(const std::string& path, LUNAFileLocation location = LUNAFileLocation::ASSETS) = 0;

	// Get list of files and subdirectories in given directory
	virtual std::vector<std::string> GetFileList(const std::string& path, LUNAFileLocation location = LUNAFileLocation::ASSETS) = 0;

	// Get size of file
	virtual ssize_t GetFileSize(const std::string& path, LUNAFileLocation location = LUNAFileLocation::ASSETS) = 0;

	// Read all file data
	virtual std::vector<unsigned char> ReadFile(const std::string& path, LUNAFileLocation location = LUNAFileLocation::ASSETS) = 0;

	// Read all file data as string
	virtual std::string ReadFileToString(const std::string& path, LUNAFileLocation location = LUNAFileLocation::ASSETS) = 0;

	// Write given byte buffer to file
	virtual bool WriteFile(const std::string& path, const std::vector<unsigned char>& data, LUNAFileLocation location = LUNAFileLocation::APP_FOLDER) = 0;

	// Write given text data to file
	virtual bool WriteFileFromString(const std::string& path, const std::string& data, LUNAFileLocation location = LUNAFileLocation::APP_FOLDER) = 0;

	// Read all data from file compressed using "Deflate" algorithm
	virtual std::vector<unsigned char> ReadCompressedFile(const std::string& path, LUNAFileLocation location = LUNAFileLocation::ASSETS) = 0;

	// Write given byte buffer to file and compress it with "Deflate" algorithm
	virtual bool WriteCompressedFile(const std::string& path, const std::vector<unsigned char>& data, LUNAFileLocation location = LUNAFileLocation::APP_FOLDER) = 0;

	std::string GetExtension(const std::string& path); // Get extension of file
	std::string GetBasename(const std::string& path); // Get filename without path and extension
	std::string GetParentPath(const std::string& path); // Get parent part of path

	// Split basename to name and @Suffix
	// Return pair: name - suffix
	std::pair<std::string,std::string> SplitSuffix(const std::string& basename);

	// Append @Suffix to filename
	std::string AppendSuffix(const std::string& path, const std::string& suffix);

	// Replace extension of given filename
	std::string ReplaceExtension(const std::string path, const std::string ext);
};

}
