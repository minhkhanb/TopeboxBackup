
#import "TwitterController.h"
#import "CGame.h"
#import "PineGameDelegate.h"
@implementation TwitterController

- (void)viewDidLoad {

    [super viewDidLoad];
    
    [[FHSTwitterEngine sharedEngine]permanentlySetConsumerKey:@"O1rade9oSQKYijhSxqQQoA" andSecret:@"JWJkJDpVf6nZYimNOfCCqfXmNSz15hTWxeKOGoZ4"];
    [[FHSTwitterEngine sharedEngine]setDelegate:self];
    
    GAME()->_twitterStatusCode = TWITTER_CONTROL_NORMAL_CODE;
}

- (void)PopupLoginDialog
{
    GAME()->_twitterStatusCode = TWITTER_CONTROL_PROCESSING_CODE;
    if ([[FHSTwitterEngine sharedEngine] loggedInUsername] == nil)
    {
        [[FHSTwitterEngine sharedEngine]showOAuthLoginControllerFromViewController:self withCompletion:^(BOOL success) {
            NSLog(@"\n complete show login control \n");
            
            if (success)
            {
                GAME()->_twitterStatusCode = TWITTER_CONTROL_DONE_WITH_SUCCESS;
            }
            else
            {
                GAME()->_twitterStatusCode = -1;
            }
            [[PineGameDelegate sharedInstance] ReturnGLView];
        }];
    }
    else
    {
        GAME()->_twitterStatusCode = TWITTER_CONTROL_DONE_WITH_SUCCESS;
        [[PineGameDelegate sharedInstance] ReturnGLView];
    }
}

- (void)PostTwiter:(NSString *)msg
{
    GAME()->_twitterStatusCode = TWITTER_CONTROL_PROCESSING_CODE;
    dispatch_async(GCDBackgroundThread, ^{
        @autoreleasepool {
            
            NSError *returnCode = [[FHSTwitterEngine sharedEngine]postTweet:msg];
            if (returnCode) {
                NSLog(@"\n Post Twitter error : %d -> %@ \n", returnCode.code, returnCode.debugDescription);
                GAME()->_twitterStatusCode = -1;
            } else {
                GAME()->_twitterStatusCode = TWITTER_CONTROL_DONE_WITH_SUCCESS;
            }
        }
    });
}

/*- (void)PopupMessage {
    printf("\n PopupMessage \n");
    //if (![MFMessageComposeViewController canSendText])
    {
        //[self setView:(UIView*)[[PineGameDelegate sharedInstance] glView]];
        MFMessageComposeViewController *messageViewController = [[MFMessageComposeViewController alloc] init];
        messageViewController.messageComposeDelegate  = self;
        [messageViewController setBody:@"Build your own army in Pocket Army! Download it on your iPhone now: http://bit.ly/PocketArmy"];
        //[messageViewController setMessageBody:@"Come visit my army in Pocket Army! You can build your own army and challenge my army too! If you don't already have it, go <a href=//'http://itunes.apple.com/app/id529961657'>download it now</a> on your iPhone, iPod Touch, or iPad!" isHTML:YES];
        
        [self presentModalViewController:messageViewController animated:YES];
        
        [messageViewController release];
        
    }
    
}*/

/*- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
	//NSLog(@"\n buttonIndex = %d \n", buttonIndex);
    CGame::_this->ClearTouchesEnd();
    [self dismissModalViewControllerAnimated:NO];
	[alertView release];
}*/

/*- (void)messageComposeViewController:(MFMessageComposeViewController *)controller didFinishWithResult:(MessageComposeResult)result
{
	switch (result) {
		case MessageComposeResultCancelled:
			NSLog(@"Cancelled");
			break;
		case MessageComposeResultFailed:
            NSLog(@"failed");
			break;
		case MessageComposeResultSent:
            NSLog(@"result sent");
			break;
		default:
			break;
	}
    
	[self dismissModalViewControllerAnimated:YES];
    [self removeFromParentViewController];
    UIWindow* window = [[PineGameDelegate sharedInstance] window];
    [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
    CGame::_this->ClearTouchesEnd();
}*/

- (void)didReceiveMemoryWarning {

[super didReceiveMemoryWarning];

}

- (void)storeAccessToken:(NSString *)accessToken {
    [[NSUserDefaults standardUserDefaults]setObject:accessToken forKey:@"SavedAccessHTTPBody"];
}

- (NSString *)loadAccessToken {
    return [[NSUserDefaults standardUserDefaults]objectForKey:@"SavedAccessHTTPBody"];
}

- (void)UserCancelLoginDialog
{
    GAME()->_twitterStatusCode = -2;
    [[PineGameDelegate sharedInstance] ReturnGLView];
}

- (void)viewDidUnload {
    NSLog(@"\n unload \n");
    CGame::_this->ClearTouchesEnd();
}

- (void)dealloc {
    NSLog(@"\n dealoc \n");
    [super dealloc];

}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    //NSLog(@"\nshouldAutorotateToInterfaceOrientation = %d\n",interfaceOrientation);
    //return NO;
    // Return YES for supported orientations
    //return (interfaceOrientation == UIInterfaceOrientationLandscapeLeft ||
    //        interfaceOrientation == UIInterfaceOrientationLandscapeRight);
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
@end
