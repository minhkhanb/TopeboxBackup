//
//  MessageController.h
//  PineEntertainment_ipad
//
//  Created by Elita Thai on 6/18/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#ifndef MESSAGE_POPUP_H
#define MESSAGE_POPUP_H 
#import <UIKit/UIKit.h>
#import <MessageUI/MessageUI.h>
#import <MessageUI/MFMessageComposeViewController.h>
@interface MessageController : UIViewController <MFMessageComposeViewControllerDelegate>{
}
- (void)PopupMessage:(NSString*)body withTitle:(NSString*)title;

@end
#endif