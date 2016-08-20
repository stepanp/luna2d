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

#include "lunaiosleaderboards.h"
#include "lunaconfig.h"
#include "lunansstring.h"
#include "lunalog.h"
#include "lunamacro.h"
#import <UIKit/UIKit.h>
#import <GameKit/GameKit.h>

using namespace luna2d;

@interface GameCenterDelegate : NSObject <GKGameCenterControllerDelegate>

- (void) gameCenterViewControllerDidFinish:(GKGameCenterViewController*) gameCenter;

@end

@implementation GameCenterDelegate

- (void) gameCenterViewControllerDidFinish:(GKGameCenterViewController*) gameCenter
{
	UIViewController* rootViewController = [[[[UIApplication sharedApplication] delegate] window] rootViewController];
	[rootViewController dismissViewControllerAnimated:YES completion:nil];
}

@end


LUNAIosLeaderboards::LUNAIosLeaderboards()
{
	GKLocalPlayer* player = [GKLocalPlayer localPlayer];
	player.authenticateHandler = ^(UIViewController* viewController, NSError* error) {};
}

// Submit score to leadearboard
void LUNAIosLeaderboards::SubmitScore(int score)
{
	auto config = LUNAEngine::Shared()->GetConfig();
	std::string defaultLeaderboardId = config->GetCustomValues()["gameCenterLeaderboardId"].string_value();
	
	if(defaultLeaderboardId.empty()) LUNA_RETURN_ERR("\"gameCenterLeaderboardId\" must be set in config to submit score");
	
	GKScore* gkScore = [[GKScore alloc] initWithLeaderboardIdentifier:ToNsString(defaultLeaderboardId)];
	gkScore.value = score;
	
	NSArray* scores = @[gkScore];
	[GKScore reportScores:scores withCompletionHandler:nil];
}

// Open leadearboards popup
void LUNAIosLeaderboards::Open()
{
	GKGameCenterViewController* gameCenter = [[GKGameCenterViewController alloc] init];
	UIViewController* rootViewController = [[[[UIApplication sharedApplication] delegate] window] rootViewController];

	if(!gameCenter) return;
	
	gameCenter.gameCenterDelegate = [[GameCenterDelegate alloc] init];
	gameCenter.viewState = GKGameCenterViewControllerStateLeaderboards;
	gameCenter.leaderboardTimeScope = GKLeaderboardTimeScopeToday;
	gameCenter.leaderboardIdentifier = nil;
	
	[rootViewController presentViewController: gameCenter animated: YES completion:nil];
}

