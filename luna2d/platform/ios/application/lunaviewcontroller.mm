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

#import "lunaviewcontroller.h"
#import "lunaengine.h"
#import "lunaiosfiles.h"
#import "lunaioslog.h"
#import "lunaiosutils.h"

using namespace luna2d;

@implementation LUNAViewController

- (void)viewDidLoad
{
	[super viewDidLoad];
	
	self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	GLKView* view = [[GLKView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
	self.view = view;
	view.context = self.context;

	[self setupGL];
}

- (void)dealloc
{
	[self tearDownGL];
	
	if([EAGLContext currentContext] == self.context)
	{
		[EAGLContext setCurrentContext:nil];
	}
}

- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning];

	if([self isViewLoaded] && ([[self view] window] == nil))
	{
		self.view = nil;
		
		[self tearDownGL];
		
		if([EAGLContext currentContext] == self.context)
		{
			[EAGLContext setCurrentContext:nil];
		}
		self.context = nil;
	}
}

- (BOOL)prefersStatusBarHidden
{
	return YES;
}

- (void)setupGL
{
	self.preferredFramesPerSecond = 60.0f;
	[EAGLContext setCurrentContext:self.context];
	
	// Get screen size and scale
	if([[[UIDevice currentDevice] systemVersion] floatValue] <= 8.0)
	{
		screenScale = [[UIScreen mainScreen] scale];
		CGSize size = [[UIScreen mainScreen] bounds].size;
		screenWidth = size.width * screenScale;
		screenHeight = size.height * screenScale;
	}
	else
	{
		screenScale = [[UIScreen mainScreen] nativeScale];
		CGSize size = [[UIScreen mainScreen] nativeBounds].size;
		screenWidth = size.width;
		screenHeight = size.height;
	}
	
	// Swap width and height for landscape orienation
	if(UIInterfaceOrientationIsLandscape([UIApplication sharedApplication].statusBarOrientation)) std::swap(screenWidth, screenHeight);
	
	// Initialize engine
	LUNAEngine::Shared()->Assemble(new LUNAIosFiles(), new LUNAIosLog(), new LUNAIosUtils());
	LUNAEngine::Shared()->Initialize(screenWidth, screenHeight);
}

- (void)tearDownGL
{
	[EAGLContext setCurrentContext:self.context];
	
	LUNAEngine::Shared()->Deinitialize();
}

- (void)update
{
	// Not used
}

- (void)glkView:(GLKView*)view drawInRect:(CGRect)rect
{
	LUNAEngine::Shared()->MainLoop();
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
	NSEnumerator* t = [touches objectEnumerator];
	UITouch* touch = NULL;
	
	while(touch = [t nextObject])
	{
		CGPoint point = [touch locationInView:self.view];
		LUNAEngine::Shared()->OnTouchDown([self translateTouchX:point.x], [self translateTouchY:point.y]);
	}
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
	NSEnumerator* t = [touches objectEnumerator];
	UITouch* touch = NULL;
	
	while(touch = [t nextObject])
	{
		CGPoint point = [touch locationInView:self.view];
		LUNAEngine::Shared()->OnTouchMoved([self translateTouchX:point.x], [self translateTouchY:point.y]);
	}
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
	NSEnumerator* t = [touches objectEnumerator];
	UITouch* touch = NULL;
	
	while(touch = [t nextObject])
	{
		CGPoint point = [touch locationInView:self.view];
		LUNAEngine::Shared()->OnTouchUp([self translateTouchX:point.x], [self translateTouchY:point.y]);
	}
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
	NSEnumerator* t = [touches objectEnumerator];
	UITouch* touch = NULL;
	
	while(touch = [t nextObject])
	{
		CGPoint point = [touch locationInView:self.view];
		LUNAEngine::Shared()->OnTouchUp([self translateTouchX:point.x], [self translateTouchY:point.y]);
	}
}

// Translate coordinates from native screen space to engine space
- (float)translateTouchX:(float)x
{
	return x * screenScale;
}

- (float)translateTouchY:(float)y
{
	return screenHeight - (y * screenScale);
}

@end
