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

#include "lunaiossharing.h"
#include "lunansstring.h"

using namespace luna2d;

void LUNAIosSharing::DoShare(NSArray* dataToShare)
{
	UIViewController* rootViewController = [[[[UIApplication sharedApplication] delegate] window] rootViewController];
	
	auto bounds = [[UIScreen mainScreen] bounds];
	auto rect = CGRectMake(bounds.size.width / 2, bounds.size.height / 2, 0, 0);
	
	UIActivityViewController* shareController = [[UIActivityViewController alloc] initWithActivityItems:dataToShare applicationActivities:nil];
	shareController.popoverPresentationController.sourceView = rootViewController.view;
	shareController.popoverPresentationController.sourceRect = rect;
	shareController.popoverPresentationController.permittedArrowDirections = 0;
	
	[rootViewController presentViewController:shareController animated:YES completion:nil];
}

// Share given text using system sharing dialog
void LUNAIosSharing::Text(const std::string& text)
{
	NSArray* dataToShare = @[ToNsString(text)];
	DoShare(dataToShare);
}

// Share given image witg given text using system sharing dialog
// Image should be located in "LUNAFileLocation::APP_FOLDER"
void LUNAIosSharing::Image(const std::string& filename, const std::string& text)
{
	NSArray* dataToShare = @[ToNsString(text)];
	DoShare(dataToShare);
}
