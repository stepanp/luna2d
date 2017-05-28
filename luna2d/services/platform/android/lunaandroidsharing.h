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

#pragma once

#include "lunasharing.h"
#include "lunaandroidjni.h"

namespace luna2d{

class LUNAAndroidSharingService : public LUNASharingService
{
public:
	LUNAAndroidSharingService(const std::string& javaClasspath);
	
private:
	jclass javaClass;
	jobject javaObject = nullptr;
	jmethodID javaGetName;
	jmethodID javaText;
	jmethodID javaImage;
	bool isLoaded = false;
	
public:
	// Check for java part of sharing service was loaded successufully
	bool IsLoaded();

	// Get name of sharing service. Should be in lower case
	virtual std::string GetName();
	
	// Share given text
	virtual void Text(const std::string& text);
	
	// Share given image with given text
	// Image should be located in "LUNAFileLocation::APP_FOLDER"
	virtual void Image(const std::string& filename, const std::string& text);
};


class LUNAAndroidSharing : public LUNASharing
{
public:	
	// Load service instance by name
	virtual std::shared_ptr<LUNASharingService> LoadService(const std::string& name);
};

}
