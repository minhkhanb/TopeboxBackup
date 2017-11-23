
#import "MailTutorialViewController.h"
#import "CGame.h"
#import "PineGameDelegate.h"

@implementation MailTutorialViewController

- (void)viewDidLoad {

[super viewDidLoad];

}

- (IBAction)actionEmailComposer {
/*    printf("\n before if \n");
if ([MFMailComposeViewController canSendMail]) {
    printf("\n after if \n");
	MFMailComposeViewController *mailViewController = [[MFMailComposeViewController alloc] init];
	mailViewController.mailComposeDelegate = self;
	[mailViewController setSubject:@"Join me in Pocket Army!"];
	[mailViewController setMessageBody:@"Come visit my army in Pocket Army! You can build your own army and challenge my army too! If you don't already have it, go <a href='http://itunes.apple.com/app/id529961657'>download it now</a> on your iPhone, iPod Touch, or iPad!" isHTML:YES];

	[self presentModalViewController:mailViewController animated:YES];
    
	[mailViewController release];

}

else {

NSLog(@"Device is unable to send email in its current state.");

}*/

}
- (void) iOS_BasicPostSocial:(NSString*) feed withImage:(UIImage*) image
{
    NSMutableArray *items = [NSMutableArray new];
    [items addObject:feed];
    [items addObject:image];
    NSArray *activityItems = [NSArray arrayWithArray:items];
    UIActivityViewController *activityViewController = [[UIActivityViewController alloc] initWithActivityItems:activityItems applicationActivities:nil];
    [self presentViewController:activityViewController animated:YES completion:nil];
    [activityViewController setCompletionHandler:^(NSString *activityType, BOOL completed)
     {
         NSLog(@"Activity = %@",activityType);
         NSLog(@"Completed Status = %d",completed);
         
         if (completed)
         {
            
         }else
         {
         }
         //[self removeFromParentViewController];
         UIWindow* window = [[PineGameDelegate sharedInstance] window];
         [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
     }];

    //[self presentModalViewController:activityViewController animated:YES];
   
}
#ifndef MAIL_SUPPORT_STRING 
#define MAIL_SUPPORT_STRING @"support@pine-entertainment.com"
#endif
- (void)PopupEmail:(NSString*) body withTitle:(NSString*) title{
    printf("\n popupMail \n");
    if ([MFMailComposeViewController canSendMail]) {
        // [self setView:(UIView*)[[PineGameDelegate sharedInstance] glView]];
        MFMailComposeViewController *mailViewController = [[MFMailComposeViewController alloc] init];
        mailViewController.mailComposeDelegate = self;
        [mailViewController setSubject:title];
        [mailViewController setMessageBody:body isHTML:YES];
        
        // Set up recipients
        NSArray *toRecipients = [NSArray arrayWithObject:MAIL_SUPPORT_STRING];
        // NSArray *ccRecipients = [NSArray arrayWithObjects:@"nghia.do@pine-entertainment.com", @"ell.tee@pine-entertainment.com", nil];
        
        [mailViewController setToRecipients:toRecipients];
        //[mailViewController setCcRecipients:ccRecipients];
        
        
        
        //  [mailViewController setToRecipients:<#(nullable NSArray<NSString > )#>];
        
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
        [[[PineGameDelegate sharedInstance] _opgenGLESViewController] presentViewController:mailViewController animated:YES completion:^{
            
            
        }];
#else
        
        [[[PineGameDelegate sharedInstance] window] addSubview:[self view]];
        [[[PineGameDelegate sharedInstance] window]bringSubviewToFront:[self view]];
        
        [self presentViewController:gcViewController animated:YES completion:^{
            [[[PineGameDelegate sharedInstance] window]bringSubviewToFront:[self view]];
            [[self view] addSubview:mailViewController.view];
        }];
#endif
        
        
        
        //[self presentModalViewController:mailViewController animated:YES];
        
        [mailViewController release];
         GAME()->_statusBasicPostSocial = CGame::enumStatusPostSocial::POST_SOCIAL_STATE_COMPLETE;
        
    }
    
    else {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Error!" message:@"Please config your email first!"
                                                       delegate:self cancelButtonTitle:@"OK" otherButtonTitles: nil];
        //[alert show];
        GAME()->_statusBasicPostSocial = CGame::enumStatusPostSocial::POST_SOCIAL_STATE_CANCEL;
        //[self dismissModalViewControllerAnimated:YES];
        //NSLog(@"Device is unable to send Message in its current state.");
        //NSLog(@"Device is unable to send email in its current state.");
        
    }
    
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
	//NSLog(@"\n buttonIndex = %d \n", buttonIndex);
    //CGame::_this->ClearTouchesEnd();
    [self dismissModalViewControllerAnimated:NO];
	[alertView release];
}

// Dismisses the email composition interface when users tap Cancel or Send. Proceeds to update the message field with the result of the operation.
- (void)mailComposeController:(MFMailComposeViewController*)controller didFinishWithResult:(MFMailComposeResult)result error:(NSError*)error
{
    // Notifies users about errors associated with the interface
    switch (result)
    {
        case MFMailComposeResultCancelled:
            NSLog(@"\nMAIL cancel\n");
            break;
        case MFMailComposeResultSaved:
            NSLog(@"\nMAIL save\n");
            break;
        case MFMailComposeResultSent:
            NSLog(@"\nMAIL Sendt\n");
            break;
        case MFMailComposeResultFailed:
            NSLog(@"\nMAIL Faile\n");
            break;
        default:
            
            break;
    }
    
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
    [[[PineGameDelegate sharedInstance] _opgenGLESViewController] dismissViewControllerAnimated:YES completion:^{
        
        [self removeFromParentViewController];
        UIWindow* window = [[PineGameDelegate sharedInstance] window];
        [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
    }];
#else
    
   	[self dismissModalViewControllerAnimated:YES];
    [[UIApplication sharedApplication] setStatusBarStyle: UIStatusBarStyleBlackOpaque animated:YES];
    [self removeFromParentViewController];
    UIWindow* window = [[PineGameDelegate sharedInstance] window];
    [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
#endif
    
    
    //CGame::_this->ClearTouchesEnd();
    //CGame::_this->ClearAllTouch();
    NSLog(@"\nMAIL finish\n");
}

- (void)didReceiveMemoryWarning {

[super didReceiveMemoryWarning];

}

- (void)viewDidUnload {
   
}

- (void)dealloc {

[super dealloc];

}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    //NSLog(@"\nshouldAutorotateToInterfaceOrientation = %d\n",interfaceOrientation);
   // return NO;
    // Return YES for supported orientations
    /*return (interfaceOrientation == UIInterfaceOrientationLandscapeLeft ||
            interfaceOrientation == UIInterfaceOrientationLandscapeRight);*/
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
@end