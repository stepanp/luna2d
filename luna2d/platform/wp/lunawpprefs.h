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

#include "lunaprefs.h"
#include <windows.h>

namespace luna2d{

//-------------------------------------------------
// Preferences implementaton for Windows Phone / RT
//-------------------------------------------------
class LUNAWpPrefs : public LUNAPrefs
{
public:
	LUNAWpPrefs();

private:
	Windows::Storage::ApplicationDataContainer^ settings;

public:
	// Get string value from preferences
	virtual std::string GetString(const std::string& name);

	// Get int value from preferences
	virtual int GetInt(const std::string& name);

	// Get float value from preferences
	virtual float GetFloat(const std::string& name);

	// Get bool value from preferences
	virtual bool GetBool(const std::string& name);

	// Set string value to preferences
	virtual void SetString(const std::string& name, const std::string& value);

	// Set int value to preferences
	virtual void SetInt(const std::string& name, int value);

	// Set float value to preferences
	virtual void SetFloat(const std::string& name, float value);

	// Set bool value to preferences
	virtual void SetBool(const std::string& name, bool value);

	// Check for existing value
	virtual bool HasValue(const std::string& name);

	// Remove valuee from preferences
	virtual void RemoveValue(const std::string& name);

	// Remove all values from preferences
	virtual void Clear();
};

}
