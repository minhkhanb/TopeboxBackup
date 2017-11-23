
#include <string.h>
#import "PineGameDelegate.h"
#import "MediaPicker.h"



static MediaPicker* g_sharedTextInputInstance = nil;

@implementation MediaPicker



// Responds to the user tapping Done after choosing music.
- (void) mediaPicker: (MPMediaPickerController *) mediaPicker didPickMediaItems: (MPMediaItemCollection *) mediaItemCollection {
	
	[self dismissModalViewControllerAnimated: YES];
	//[self updatePlayerQueueWithMediaCollection: mediaItemCollection];
	//[self.mediaItemCollectionTable reloadData];
	
	[[UIApplication sharedApplication] setStatusBarStyle: UIStatusBarStyleBlackOpaque animated:YES];
}


// Responds to the user tapping done having chosen no music.
- (void) mediaPickerDidCancel: (MPMediaPickerController *) mediaPicker {
	
	[self dismissModalViewControllerAnimated: YES];
	
	[[UIApplication sharedApplication] setStatusBarStyle: UIStatusBarStyleBlackOpaque animated:YES];
}



- (void) updatePlayerQueueWithMediaCollection: (MPMediaItemCollection *) mediaItemCollection {
	
	
}

- (void) showMediaPicker
{
	
	/*MPMediaPickerController *picker = [[MPMediaPickerController alloc] initWithMediaTypes: MPMediaTypeAnyAudio];                   // 1
	[picker setDelegate:  CBoxAppAppDelegate sharedInstance];                                         // 2
	[picker setAllowsPickingMultipleItems: YES];                        // 3
	
	picker.prompt =   NSLocalizedString (@"Add songs to play", "Prompt in media item picker");	
	[self presentModalViewController: picker animated: YES];    // 4
	//[[[CBoxAppAppDelegate sharedInstance] glView] addSubview:textField];
	
	[picker release];
	*/
	
	//musicController = [[UIViewController alloc] init];
	//[self setView:[[CBoxAppAppDelegate sharedInstance] glView]]; 
	[self setModalTransitionStyle: UIModalTransitionStyleCoverVertical];
	
	MPMediaPickerController *picker =
	[[MPMediaPickerController alloc] initWithMediaTypes: MPMediaTypeMusic];
	
	picker.delegate                                       = self;
	picker.allowsPickingMultipleItems = YES;
	picker.prompt                                         = @"Select songs to play";
	
	// The media item picker uses the default UI style, so it needs a default-style
	//    status bar to match it visually
	//[[UIApplication sharedApplication] setStatusBarHidden:NO animated: YES];
	
	[self presentModalViewController: picker animated: YES];
	[picker release];
	
}


+(MediaPicker*)getSharedInstance
{
	return g_sharedTextInputInstance;
}

+(void)setSharedInstance:(MediaPicker*)instance
{
	if (g_sharedTextInputInstance != nil && g_sharedTextInputInstance != instance)
		[g_sharedTextInputInstance release];
	
	g_sharedTextInputInstance = instance;
}


- (void)dealloc
{
	[super dealloc];
}

@end

//@implementation MediaPickerParams

extern "C" void ShowPicker()
{
	printf("ShowPicker");
	//MediaPickerParams* p = [[MediaPickerParams alloc] init];
	
		
	[[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(MediaPickerInit:) withObject:nil waitUntilDone:YES];
	//[[CBoxAppAppDelegate sharedInstance] performSelectorOnMainThread:@selector(TextInputInit:) withObject:p waitUntilDone:YES];
	//[p release];
}

//@end