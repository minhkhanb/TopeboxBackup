//
//  MessageController.h
//  PineEntertainment_ipad
//
//  Created by Elita Thai on 8/2/13.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#ifndef TWITTER_CONTROLER_H
#define TWITTER_CONTROLER_H 

#import "FHSTwitterEngine.h"

@interface TwitterController : UIViewController <FHSTwitterEngineAccessTokenDelegate>{

}
- (void)PopupLoginDialog;
- (void)PostTwiter:(NSString *)msg;
@end
#endif