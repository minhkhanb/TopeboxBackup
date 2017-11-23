//facebook connect
//#import <FBConnect/FBConnect.h>
//#import "FBConnect/FBConnect.h"
#import "CBoxAppAppDelegate.h"
#import "URLConnection.h"
#include "Defines.h"
#include "CGame.h"

extern CGame* g_pGame;

extern "C" void SetFacebookUserID(unsigned long long fb_uid)
{
	g_pGame->_fb_userid = fb_uid;
}

extern "C" void SetFacebookUserName(const char* name)
{
	if (name)
	{
		strcpy(g_pGame->_fb_username, name);
	}
}

extern "C" void ClearFacebookUserInfo()
{
	g_pGame->_fb_userid = 0;
}

extern "C" void InitFBSession()
{
	[[[CBoxAppAppDelegate sharedInstance] glView] initFacebookSession];
}

extern "C" void GetFriendsUseApp(unsigned long long fbuid)
{
	[[[CBoxAppAppDelegate sharedInstance] glView] queryFriendsUseApp: fbuid];
}

extern "C" void SetFacebookFriendList(NSArray* users)
{
	g_pGame->_fb_friendids->ClearAll();
	
	for (int i = 0; i < [users count]; i++)
	{
		NSDictionary* user = [users objectAtIndex:i];
		NSString* uid = [user objectForKey:@"uid"];
		
		g_pGame->_fb_friendids->AddNewString([uid UTF8String]);
	}
}


/*
void PublishingFacebookFeedStories(int new_score)
{
	NSString* newscoreString = [NSString stringWithFormat:@"%@ get new high score: %d", @"Hung", new_score];
	NSString* feedString = [NSString stringWithFormat:@"{\"name\":\"%@\",\"href\":\"%@\",\"caption\":\"%@\",\"description\":\"%@\",\"media\":[{\"type\":\"image\",\"src\":\"%@\",\"href\":\"%@\"}],\"properties\":{\"another link\":{\"text\":\"Colorbox home page\",\"href\":\"%@\"}}}", 
							@"Lines Deluxe",
							_LINK_FB_APP_LINES_DELUXE,
							newscoreString,
							@"description",
							_LINK_LINES_DELUXE_ICON,
							_LINK_FB_APP_LINES_DELUXE,
							_LINK_COLORBOX_HOMEPAGE
							];
	
	[[[CBoxAppAppDelegate sharedInstance] glView] publishFeedStories: feedString];
}
*/

extern "C" void ShowFBConnectDialog()
{
	//[[[CBoxAppAppDelegate sharedInstance] glView] showFBConnectDialog];
	[[CBoxAppAppDelegate sharedInstance] performSelectorOnMainThread:@selector(FBLoginDialog:) withObject:nil waitUntilDone:YES];
}

extern "C" void PublishFBStream(const char* msg)
{	
	NSString* message = [[NSString alloc] initWithUTF8String: msg];
	[[[CBoxAppAppDelegate sharedInstance] glView] setFBStringBuffer: message];
	[[CBoxAppAppDelegate sharedInstance] performSelectorOnMainThread:@selector(FBPublishStream:) withObject:nil waitUntilDone:YES];
}

extern "C" void LogoutFB()
{
	[[CBoxAppAppDelegate sharedInstance] performSelectorOnMainThread:@selector(FBLogout:) withObject:nil waitUntilDone:YES];
}

void ShowFBConnectButton(int x, int y)
{
	CGRect frame = CGRectMake(x, y, 50, 50);
	[[[CBoxAppAppDelegate sharedInstance] glView] showFBConnectButton: frame];
}

extern "C" void RequestFBUserInfo()
{
	[[CBoxAppAppDelegate sharedInstance] performSelectorOnMainThread:@selector(FBRequestUserInfo:) withObject:nil waitUntilDone:YES];
}

void HideFBConnectButton()
{
	[[[CBoxAppAppDelegate sharedInstance] glView] hideFBConnectButton];
}

void ShowTWConnectDialog()
{
	//[[[CBoxAppAppDelegate sharedInstance] glView] showTWConnectDialog];
	[[CBoxAppAppDelegate sharedInstance] performSelectorOnMainThread:@selector(TWLoginDialog:) withObject:nil waitUntilDone:YES];

}

