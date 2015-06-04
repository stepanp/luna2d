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

#include "lunawpprefs.h"
#include "lunawstring.h"

using namespace luna2d;

LUNAWpPrefs::LUNAWpPrefs()
{
	settings = Windows::Storage::ApplicationData::Current->LocalSettings;
}

// Get string value from preferences
std::string LUNAWpPrefs::GetString(const std::string& name)
{
	Platform::String^ key = ToPlatformString(name);
	if(settings->Values->HasKey(key))
	{
		Platform::String^ str = safe_cast<Platform::String^>(settings->Values->Lookup(key));
		return FromPlatfromString(str);
	}

	return "";
}

// Get int value from preferences
int LUNAWpPrefs::GetInt(const std::string& name)
{
	Platform::String^ key = ToPlatformString(name);
	if(settings->Values->HasKey(key)) return safe_cast<int>(settings->Values->Lookup(key));

	return 0;
}

// Get float value from preferences
float LUNAWpPrefs::GetFloat(const std::string& name)
{
	Platform::String^ key = ToPlatformString(name);
	if(settings->Values->HasKey(key)) return safe_cast<float>(settings->Values->Lookup(key));

	return 0;
}

// Get bool value from preferences
bool LUNAWpPrefs::GetBool(const std::string& name)
{
	Platform::String^ key = ToPlatformString(name);
	if(settings->Values->HasKey(key)) return safe_cast<bool>(settings->Values->Lookup(key));

	return false;
}

// Set string value to preferences
void LUNAWpPrefs::SetString(const std::string& name, const std::string& value)
{
	settings->Values->Insert(ToPlatformString(name), ToPlatformString(value));
}

// Set int value to preferences
void LUNAWpPrefs::SetInt(const std::string& name, int value)
{
	settings->Values->Insert(ToPlatformString(name), value);
}

// Set float value to preferences
void LUNAWpPrefs::SetFloat(const std::string& name, float value)
{
	settings->Values->Insert(ToPlatformString(name), value);
}

// Set bool value to preferences
void LUNAWpPrefs::SetBool(const std::string& name, bool value)
{
	settings->Values->Insert(ToPlatformString(name), value);
}

// Check for existing value
bool LUNAWpPrefs::HasValue(const std::string &name)
{
	return settings->Values->HasKey(ToPlatformString(name));
}

// Remove valuee from preferences
void LUNAWpPrefs::RemoveValue(const std::string& name)
{
	settings->Values->Remove(ToPlatformString(name));
}

// Remove all values from preferences
void LUNAWpPrefs::Clear()
{
	settings->Values->Clear();
}
