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

#include "lunawpfiles.h"
#include "platform/lunalog.h"
#include <Windows.h>

using namespace luna2d;

std::string LUNAWpFiles::GetPathInLocation(const std::string& path, LUNAFileLocation location)
{
	// Convert std::string to std::wstring
	std::string winPath = GetRootFolder(location) + path;
	
	// Replace unix slashes to windows slashes
	size_t startPos = 0;
	while((startPos = winPath.find("/", startPos)) != std::string::npos)
	{
		winPath.replace(startPos, 1, "\\");
		startPos += 1;
	}

	return winPath;
}

// Check for given path is file
bool LUNAWpFiles::IsFile(const std::string& path, LUNAFileLocation location)
{
	if(location == LUNAFileLocation::ASSETS)
	{
		return true;// IsExists(path, location);
		//stat statBuf;
		/*struct stat statbuf;
		stat(GetPathInLocation(path, location).c_str());


		FILE* file = fopen(GetPathInLocation(path, location).c_str(), "r");
		if(file)
		{
			fclose(file);
			return true;
		}
		struct stat statbuf;
		stat()*/
	}

	return false;
}

// Check for given path is directory
bool LUNAWpFiles::IsDirectory(const std::string& path, LUNAFileLocation location)
{
	if(location == LUNAFileLocation::ASSETS)
	{
		return true;
	}

	return false;
}

// Check for path is exists
bool LUNAWpFiles::IsExists(const std::string& path, LUNAFileLocation location)
{
	FILE* file = fopen(GetPathInLocation(path, location).c_str(), "r");
	if(file)
	{
		fclose(file);
		return true;
	}

	return false;
}

// Get root folder for file location
std::string LUNAWpFiles::GetRootFolder(LUNAFileLocation location)
{
	switch(location)
	{
	case LUNAFileLocation::ASSETS:
		// Get path to assets directory
		std::wstring wPath = Windows::ApplicationModel::Package::Current->InstalledLocation->Path->Data();
		wPath += L"\\Assets\\assets\\";

		// Convert path from std::wstring to std::string
		std::string ret(wPath.length(), '\n');
		WideCharToMultiByte(CP_ACP, 0, wPath.c_str(), wPath.length(), &ret[0], ret.length(), "", FALSE);

		return std::move(ret);
	}

	return "";
}

// Get list of files and subdirectories in given directory
std::vector<std::string> LUNAWpFiles::GetFileList(const std::string& path, LUNAFileLocation location)
{
	std::vector<std::string> ret;

	if(location == LUNAFileLocation::ASSETS)
	{
		/*zip* apk = OpenApk();

		if(IsDirectory(path))
		{
			int startPos = GetRootFolder().length() + (path == "/" ? 0 : path.length());
			auto dirIndexes = directoryCache[path];

			for(int i = dirIndexes.first;i <= dirIndexes.second;i++)
			{
				std::string filename = zip_get_name(apk, i, 0);

				// Is file
				int slashPos = filename.find('/', startPos);
				if(slashPos == std::string::npos)
				{
					ret.push_back(filename.substr(startPos));
				}

				// Is directory
				else
				{
					std::string dirname = filename.substr(startPos, slashPos - startPos + 1);
					if(std::find(ret.begin(), ret.end(), dirname) == ret.end()) ret.push_back(std::move(dirname));
				}
			}
		}

		zip_close(apk);*/

		return std::move(ret);
	}

	return std::move(ret);
}

// Get size of file
ssize_t LUNAWpFiles::GetFileSize(const std::string& path, LUNAFileLocation location)
{
	FILE* file = fopen(GetPathInLocation(path, location).c_str(), "r");
	if(!file) return -1;

	fseek(file, 0, SEEK_END);
	ssize_t size = ftell(file);
	fclose(file);

	return size;
}

// Read all file data
std::vector<unsigned char> LUNAWpFiles::ReadFile(const std::string& path, LUNAFileLocation location)
{
	std::vector<unsigned char> ret;

	FILE* file = fopen(GetPathInLocation(path, location).c_str(), "r");
	if(!file) return std::move(ret);

	// Get file size
	fseek(file, 0, SEEK_END);
	ssize_t size = ftell(file);
	fseek(file, 0, SEEK_SET);

	// Read file
	ret.resize(size);
	fread(&ret[0], sizeof(unsigned char), size, file);
	fclose(file);

	return std::move(ret);
}

// Read all file data as string
std::string LUNAWpFiles::ReadFileToString(const std::string& path, LUNAFileLocation location)
{
	FILE* file = fopen(GetPathInLocation(path, location).c_str(), "r");
	if(!file) return "";

	// Get file size
	fseek(file, 0, SEEK_END);
	ssize_t size = ftell(file);
	fseek(file, 0, SEEK_SET);

	// Read file as string
	std::string ret;
	ret.resize(size);
	fread(&ret[0], sizeof(char), size, file);
	fclose(file);

	return std::move(ret);
}
