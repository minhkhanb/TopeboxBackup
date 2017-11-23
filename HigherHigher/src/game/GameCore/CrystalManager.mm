
#import "CBoxAppAppDelegate.h"
#import "CrystalSession.h"

extern "C" void InitCT()
{
	[CrystalSession initWithAppID:@"1147777375" delegate:[CBoxAppAppDelegate sharedInstance] version:1.0 theme:@"Indigo_007" secretKey:@"8gnefsv7h47hlmkaigfserb63r8q8j"];
	// Enable the game centre support
	[CrystalSession activateCrystalSetting:CrystalSettingEnableGameCenterSupport value:@"YES"];
	
	// Authenticate the local player
	[CrystalSession authenticateLocalPlayerWithCompletionHandler:^ (NSError* error)
	{
		if(error)
		{     
			// Handle errors here if needed         
			NSLog(@"An error occured authenticating the local game center player");
			NSLog(@"Error: %@", [error localizedDescription]);
		}
		else
		{
			// Handle success here if needed
		}
	}];
	[CrystalSession lockToOrientation:UIDeviceOrientationLandscapeLeft];
	
}

extern "C" void ShowCTBoard()
{
	[[CBoxAppAppDelegate sharedInstance] performSelectorOnMainThread:@selector(CTDashBoard:) withObject:nil waitUntilDone:YES];
}

extern "C" void SubmitCTAchievement(const char* identifier, const char* description, bool showPopUp)
{
	NSString* iden = [[NSString alloc] initWithUTF8String:identifier];
	NSString* desc = [[NSString alloc] initWithUTF8String:description];
	[CrystalSession postAchievement:iden wasObtained:true withDescription:desc alwaysPopup: showPopUp];
}

extern "C" void ResetCTAchievement(const char* identifier)
{
	NSString* iden = [[NSString alloc] initWithUTF8String:identifier];
	[CrystalSession postAchievement:iden wasObtained:false withDescription:nil alwaysPopup: false];
}

extern "C" void SubmitCTScore(int score, const char* category_id)
{
	NSString* iden = [[NSString alloc] initWithUTF8String:category_id];
	[CrystalSession postLeaderboardResult:score forLeaderboardId:iden lowestValFirst:false];
}

extern "C" void GetTop20(const char* leaderboad_id)
{
	NSString* iden = [[NSString alloc] initWithUTF8String:leaderboad_id];
	[[CBoxAppAppDelegate sharedInstance] performSelectorOnMainThread:@selector(CTGetTop20HighScore:) withObject:iden waitUntilDone:YES];
}
