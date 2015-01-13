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

using namespace luna2d;

// Check for given path is file
bool LUNAWpFiles::IsFile(const std::string& path, LUNAFileLocation location)
{
	if(location == LUNAFileLocation::ASSETS)
	{
		//return filesCache.count(path) == 1;
	}

	return false;
}

// Check for given path is directory
bool LUNAWpFiles::IsDirectory(const std::string& path, LUNAFileLocation location)
{
	if(location == LUNAFileLocation::ASSETS)
	{
		//return directoryCache.count(path) == 1;
	}

	return false;
}

// Check for path is exists
bool LUNAWpFiles::IsExists(const std::string& path, LUNAFileLocation location)
{
	return false;
}

// Get root folder for file location
std::string LUNAWpFiles::GetRootFolder(LUNAFileLocation location)
{
	switch(location)
	{
	case LUNAFileLocation::ASSETS:
		return "assets/";
	}
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
	if(location == LUNAFileLocation::ASSETS)
	{
		/*zip* apk = OpenApk();

		if(!IsFile(path))
		{
			zip_close(apk);
			return -1;
		}

		// Read size of file
		struct zip_stat fileInfo;
		zip_stat_init(&fileInfo);
		zip_stat_index(apk, filesCache[path], 0, &fileInfo);
		zip_close(apk);

		return fileInfo.size;*/
	}

	return -1;
}

// Read all file data
std::vector<unsigned char> LUNAWpFiles::ReadFile(const std::string& path, LUNAFileLocation location)
{
	// Read file from assets
	/*if(location == LUNAFileLocation::ASSETS)
	{
		zip* apk = OpenApk();

		if(!IsFile(path, LUNAFileLocation::ASSETS))
		{
			zip_close(apk);
			return std::move(std::vector<unsigned char>());
		}

		// Open file
		int fileIndex = filesCache[path];
		zip_file* file = zip_fopen_index(apk, fileIndex, 0);
		if(!file)
		{
			zip_close(apk);
			return std::move(std::vector<unsigned char>());
		}

		// Read size of file
		struct zip_stat fileInfo;
		zip_stat_init(&fileInfo);
		zip_stat_index(apk,fileIndex, 0, &fileInfo);

		// Read file
		std::vector<unsigned char> ret;
		ret.resize(fileInfo.size);
		int result = zip_fread(file, reinterpret_cast<char*>(&ret[0]), fileInfo.size);

		zip_fclose(file);
		zip_close(apk);

		if(result == -1) return std::move(std::vector<unsigned char>());
		else return std::move(ret);
	}*/

	return std::move(std::vector<unsigned char>());
}

// Read all file data as string
std::string LUNAWpFiles::ReadFileToString(const std::string& path, LUNAFileLocation location)
{
	// Read file from assets
	/*if(location == LUNAFileLocation::ASSETS)
	{
		zip* apk = OpenApk();

		if(!IsFile(path, LUNAFileLocation::ASSETS))
		{
			zip_close(apk);
			return "";
		}

		// Open file
		int fileIndex = filesCache[path];
		zip_file* file = zip_fopen_index(apk, fileIndex, 0);
		if(!file)
		{
			zip_close(apk);
			return "";
		}

		// Read size of file
		struct zip_stat fileInfo;
		zip_stat_init(&fileInfo);
		zip_stat_index(apk,fileIndex, 0, &fileInfo);

		// Read file
		std::string ret;
		ret.resize(fileInfo.size);
		int result = zip_fread(file, &ret[0], fileInfo.size);

		zip_fclose(file);
		zip_close(apk);

		if(result == -1) return "";
		else return std::move(ret);
	}*/

	return "";
}
