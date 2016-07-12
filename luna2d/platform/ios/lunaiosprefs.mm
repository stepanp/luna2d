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

#include "lunaiosprefs.h"
#include "lunansstring.h"

using namespace luna2d;

LUNAIosPrefs::LUNAIosPrefs() :
	prefs([NSUserDefaults standardUserDefaults])
{
}

// Get string value from preferences
std::string LUNAIosPrefs::GetString(const std::string& name)
{
	return FromNsString([prefs stringForKey:ToNsString(name)]);
}

// Get int value from preferences
int LUNAIosPrefs::GetInt(const std::string& name)
{
	return static_cast<int>([prefs integerForKey:ToNsString(name)]);
}

// Get float value from preferences
float LUNAIosPrefs::GetFloat(const std::string& name)
{
	return [prefs floatForKey:ToNsString(name)];
}

// Get bool value from preferences
bool LUNAIosPrefs::GetBool(const std::string& name)
{
	return [prefs boolForKey:ToNsString(name)];
}

// Set string value to preferences
void LUNAIosPrefs::SetString(const std::string& name, const std::string& value)
{
	[prefs setObject:ToNsString(value) forKey:ToNsString(name)];
	[prefs synchronize];
}

// Set int value to preferences
void LUNAIosPrefs::SetInt(const std::string& name, int value)
{
	[prefs setInteger:value forKey:ToNsString(name)];
	[prefs synchronize];
}

// Set float value to preferences
void LUNAIosPrefs::SetFloat(const std::string& name, float value)
{
	[prefs setFloat:value forKey:ToNsString(name)];
	[prefs synchronize];
}

// Set bool value to preferences
void LUNAIosPrefs::SetBool(const std::string& name, bool value)
{
	[prefs setBool:value forKey:ToNsString(name)];
	[prefs synchronize];
}

// Check for existing value
bool LUNAIosPrefs::HasValue(const std::string& name)
{
	return [prefs objectForKey:ToNsString(name)] != nil;
}

// Remove valuee from preferences
void LUNAIosPrefs::RemoveValue(const std::string& name)
{
	[prefs removeObjectForKey:ToNsString(name)];
	[prefs synchronize];
}

// Remove all values from preferences
void LUNAIosPrefs::Clear()
{
	[[NSUserDefaults standardUserDefaults] setPersistentDomain:[NSDictionary dictionary] forName:[[NSBundle mainBundle] bundleIdentifier]];
}
