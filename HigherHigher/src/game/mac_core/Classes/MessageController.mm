
#import "MessageController.h"
#import "CGame.h"
#import "PineGameDelegate.h"
@implementation MessageController

- (void)viewDidLoad {

[super viewDidLoad];

}

- (void)PopupMessage:(NSString*)body withTitle:(NSString*)title
{
    printf("\n PopupMessage \n");
    //if (![MFMessageComposeViewController canSendText])
    {
        //[self setView:(UIView*)[[PineGameDelegate sharedInstance] glView]];
        MFMessageComposeViewController *messageViewController = [[MFMessageComposeViewController alloc] init];
        messageViewController.messageComposeDelegate  = self;
        [messageViewController setBody:body];
        [self presentModalViewController:messageViewController animated:YES];
        
        [messageViewController release];
        
    }
   /* else {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error!" message:@"Please config your email first!"
                                                       delegate:self cancelButtonTitle:@"OK" otherButtonTitles: nil];
        [alert show];
        //[self dismissModalViewControllerAnimated:YES];
        //NSLog(@"Device is unable to send Message in its current state.");
        
    }*/
    
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
	//NSLog(@"\n buttonIndex = %d \n", buttonIndex);
    CGame::_this->ClearTouchesEnd();
    [self dismissModalViewControllerAnimated:NO];
	[alertView release];
}

- (void)messageComposeViewController:(MFMessageComposeViewController *)controller didFinishWithResult:(MessageComposeResult)result
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
}

- (void)didReceiveMemoryWarning {

[super didReceiveMemoryWarning];

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