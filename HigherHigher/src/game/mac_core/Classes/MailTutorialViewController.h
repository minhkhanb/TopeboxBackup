
//#import "PineGameDelegate.h"
//#import "Defines.h"
//#import "CGame.h"
#ifndef MAIL_POPUP_H
#define MAIL_POPUP_H 1
#import <UIKit/UIKit.h>
#import <MessageUI/MessageUI.h>
#import <MessageUI/MFMailComposeViewController.h>

@interface MailTutorialViewController : UIViewController <MFMailComposeViewControllerDelegate>{
}

- (IBAction)actionEmailComposer;
- (void) PopupEmail:(NSString*) body withTitle:(NSString*) title;

@end
#endif