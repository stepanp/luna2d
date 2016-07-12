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

#include "lunaiosfiles.h"
#include "lunalog.h"
#include "lunansstring.h"
#include <cstdio>

using namespace luna2d;

LUNAIosFiles::LUNAIosFiles()
{
	fileManager = [NSFileManager defaultManager];
}

// Convert given path in a path relative to root directory of given location
NSString* LUNAIosFiles::GetPathInLocation(const std::string& path, LUNAFileLocation location)
{
	std::string ret = GetRootFolder(location);
	
	// Remove slashes at end of path, because NSBundle pathForResource cannot find path with slash at end
	if(path != "/") ret = ret + path;
	if(ret[ret.length() - 1] == '/') ret = ret.substr(0, ret.length() - 1);
	
	return ToNsString(ret);
}

// Get root folder for file location
std::string LUNAIosFiles::GetRootFolder(LUNAFileLocation location)
{
	switch(location)
	{
	case LUNAFileLocation::ASSETS:
		return "game/";
	}
	
	return "";
}

// Check for given path is file
bool LUNAIosFiles::IsFile(const std::string& path, LUNAFileLocation location)
{
	if(location == LUNAFileLocation::ASSETS)
	{
		NSString* fullPath = [[NSBundle mainBundle] pathForResource:GetPathInLocation(path, location) ofType:nil];
		BOOL isDir = false;
		
		return [fileManager fileExistsAtPath:fullPath isDirectory:&isDir] && !isDir;
	}
	
	return false;
}

// Check for given path is directory
bool LUNAIosFiles::IsDirectory(const std::string& path, LUNAFileLocation location)
{
	if(location == LUNAFileLocation::ASSETS)
	{
		NSString* fullPath = [[NSBundle mainBundle] pathForResource:GetPathInLocation(path, location) ofType:nil];
		BOOL isDir = false;
		
		return [fileManager fileExistsAtPath:fullPath isDirectory:&isDir] && isDir;
	}
	
	return false;
}

// Check for path is exists
bool LUNAIosFiles::IsExists(const std::string& path, LUNAFileLocation location)
{
	if(location == LUNAFileLocation::ASSETS)
	{
		NSString* fullPath = [[NSBundle mainBundle] pathForResource:GetPathInLocation(path, location) ofType:nil];
		return [fileManager fileExistsAtPath:fullPath];
	}
	
	return false;
}

// Get list of files and subdirectories in given directory
std::vector<std::string> LUNAIosFiles::GetFileList(const std::string& path, LUNAFileLocation location)
{
	std::vector<std::string> ret;
	
	if(!IsDirectory(path, location)) return std::move(ret); // Return empty file list
	
	if(location == LUNAFileLocation::ASSETS)
	{
		NSString* fullPath = [[NSBundle mainBundle] pathForResource:GetPathInLocation(path, location) ofType:nil];
		NSArray* fileUrls = [fileManager contentsOfDirectoryAtURL:[NSURL fileURLWithPath:fullPath]
			includingPropertiesForKeys:[NSArray arrayWithObject:NSURLNameKey]
			options:NSDirectoryEnumerationSkipsHiddenFiles
			error:nil];
		
		for(NSURL* url in fileUrls)
		{
			NSNumber* isDir;
			BOOL result = [url getResourceValue:&isDir forKey:NSURLIsDirectoryKey error:nil];
			std::string name = [[url lastPathComponent] UTF8String];
			
			if(result && [isDir boolValue]) ret.push_back(name + "/");
			else ret.push_back(name);
		}
		
		return std::move(ret);
	}

	return std::move(ret);
}

// Get size of file
ssize_t LUNAIosFiles::GetFileSize(const std::string& path, LUNAFileLocation location)
{
	if(location == LUNAFileLocation::ASSETS)
	{
		NSString* fullPath = [[NSBundle mainBundle] pathForResource:GetPathInLocation(path, location) ofType:nil];
		FILE* file = fopen([fullPath UTF8String], "r");
		if(!file) return -1;
		
		fseek(file, 0, SEEK_END);
		ssize_t size = ftell(file);
		fclose(file);
		
		return size;
	}
	
	return -1;
}

// Read all file data
std::vector<unsigned char> LUNAIosFiles::ReadFile(const std::string &path, LUNAFileLocation location)
{
	std::vector<unsigned char> ret;
	
	if(!IsFile(path, location)) return std::move(ret);
	
	NSString* fullPath = [[NSBundle mainBundle] pathForResource:GetPathInLocation(path, location) ofType:nil];
	FILE* file = fopen([fullPath UTF8String], "r");
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
std::string LUNAIosFiles::ReadFileToString(const std::string &path, LUNAFileLocation location)
{
	if(!IsFile(path, location)) return "";
	
	NSString* fullPath = [[NSBundle mainBundle] pathForResource:GetPathInLocation(path, location) ofType:nil];
	FILE* file = fopen([fullPath UTF8String], "r");
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

// Write given byte buffer to file
bool LUNAIosFiles::WriteFile(const std::string &path, const std::vector<unsigned char> &data, LUNAFileLocation location)
{
	LUNA_LOGE("Method LUNAIosFiles::WriteFile is not implemented");
	return false;
}

// Write given text data to file
bool LUNAIosFiles::WriteFileFromString(const std::string& path, const std::string& data, LUNAFileLocation location)
{
	LUNA_LOGE("Method LUNAIosFiles::WriteFileFromString is not implemented");
	return false;
}

// Read all data from file compressed using "Deflate" algorithm
std::vector<unsigned char> LUNAIosFiles::ReadCompressedFile(const std::string& path, LUNAFileLocation location)
{
	LUNA_LOGE("Method LUNAIosFiles::ReadCompressedFile is not implemented");
	return std::vector<unsigned char>();
}

// Write given byte buffer to file and compress it with "Deflate" algorithm
bool LUNAIosFiles::WriteCompressedFile(const std::string& path, const std::vector<unsigned char>& data, LUNAFileLocation location)
{
	LUNA_LOGE("Method LUNAIosFiles::WriteCompressedFile is not implemented");
	return false;
}
