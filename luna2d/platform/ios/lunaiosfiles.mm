//-----------------------------------------------------------------------------
// luna2d engine
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
std::string LUNAIosFiles::GetPathInLocation(const std::string& path, LUNAFileLocation location)
{
	return GetRootFolder(location) + path;
}

// Get root folder for file location
std::string LUNAIosFiles::GetRootFolder(LUNAFileLocation location)
{
	switch(location)
	{
	case LUNAFileLocation::ASSETS:
		return FromNsString([[NSBundle mainBundle] pathForResource:@"game" ofType:nil]) + "/";
	case LUNAFileLocation::APP_FOLDER:
	{
		NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		return FromNsString([paths objectAtIndex:0]) + "/";
	}
	default:
		return "";
	}
}

// Check for given path is file
bool LUNAIosFiles::IsFile(const std::string& path, LUNAFileLocation location)
{
	NSString* fullPath = ToNsString(GetPathInLocation(path, location));
	BOOL isDir = false;
	
	return [fileManager fileExistsAtPath:fullPath isDirectory:&isDir] && !isDir;
}

// Check for given path is directory
bool LUNAIosFiles::IsDirectory(const std::string& path, LUNAFileLocation location)
{
	NSString* fullPath = ToNsString(GetPathInLocation(path, location));
	BOOL isDir = false;
	
	return [fileManager fileExistsAtPath:fullPath isDirectory:&isDir] && isDir;
}

// Check for path is exists
bool LUNAIosFiles::IsExists(const std::string& path, LUNAFileLocation location)
{
	return [fileManager fileExistsAtPath:ToNsString(GetPathInLocation(path, location))];
}

// Get list of files and subdirectories in given directory
std::vector<std::string> LUNAIosFiles::GetFileList(const std::string& path, LUNAFileLocation location)
{
	if(!IsDirectory(path, location)) return {}; // Return empty file list
	
	std::vector<std::string> ret;
	
	NSString* fullPath = ToNsString(GetPathInLocation(path, location));
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
	
	return ret;
}

// Get size of file
ssize_t LUNAIosFiles::GetFileSize(const std::string& path, LUNAFileLocation location)
{
	FILE* file = fopen(GetPathInLocation(path, location).c_str(), "r");
	if(!file) return -1;
	
	fseek(file, 0, SEEK_END);
	ssize_t size = ftell(file);
	fclose(file);
	
	return size;
}

// Read all file data
std::vector<unsigned char> LUNAIosFiles::ReadFile(const std::string &path, LUNAFileLocation location)
{
	FILE* file = fopen(GetPathInLocation(path, location).c_str(), "r");
	if(!file) return {};
	
	// Get file size
	fseek(file, 0, SEEK_END);
	ssize_t size = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	// Read file
	std::vector<unsigned char> ret;
	ret.resize(size);
	fread(&ret[0], sizeof(unsigned char), size, file);
	fclose(file);
	
	return ret;
}

// Read all file data as string
std::string LUNAIosFiles::ReadFileToString(const std::string &path, LUNAFileLocation location)
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
	
	return ret;
}

// Write given byte buffer to file
bool LUNAIosFiles::WriteFile(const std::string &path, const std::vector<unsigned char> &data, LUNAFileLocation location)
{
	// Assets folder is readonly
	if(location == LUNAFileLocation::ASSETS) return false;
	
	FILE* file = fopen(GetPathInLocation(path, location).c_str(), "wb");
	if(!file) return false;
	
	fwrite(data.data(), sizeof(unsigned char), data.size(), file);
	fclose(file);
	
	return true;
}

// Write given text data to file
bool LUNAIosFiles::WriteFileFromString(const std::string& path, const std::string& data, LUNAFileLocation location)
{
	// Assets folder is readonly
	if(location == LUNAFileLocation::ASSETS) return false;
	
	FILE* file = fopen(GetPathInLocation(path, location).c_str(), "w");
	if(!file) return false;
	
	fwrite(data.data(), sizeof(char), data.size(), file);
	fclose(file);
	
	return true;
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
