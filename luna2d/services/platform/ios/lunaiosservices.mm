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

#include "lunaiosservices.h"
#include "lunaiosads.h"
#include "lunaiossharing.h"
#include "lunaiosstore.h"
#include "lunansstring.h"
#include "lunalog.h"

using namespace luna2d;

LUNAIosServices::LUNAIosServices()
{
	ads = std::make_shared<LUNAIosAds>();
	sharing = std::make_shared<LUNAIosSharing>();
	store = std::make_shared<LUNAIosStore>();
}

// Dynamically create instance of serivice by given class name
id LUNAIosServices::LoadService(const std::string& name)
{
	id obj = [[NSClassFromString(ToNsString(name)) alloc] init];
	
	if(!obj) LUNA_LOGE("Error with loading service. Class with name \"%s\" not found", name.c_str());
	
	return obj;
}

void LUNAIosServices::LoadServices()
{
	std::shared_ptr<LUNAIosAds> ads = std::static_pointer_cast<LUNAIosAds>(GetAds());
	ads->LoadServices();
}
