//
//  ViewAppStoreController.h
//  PineEntertainment_ipad
//
//  Created by Elita Thai on 6/18/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#import <StoreKit/SKStoreProductViewController.h>
#ifndef VIEW_APPSTORE_H
#define VIEW_APPSTORE_H

@interface ViewAppStoreController : UIViewController <SKStoreProductViewControllerDelegate>
{
@private
	NSNumber* _game_id;
}
- (void)viewDidLoad;
-(void)showMyApps;
- (void)setGameId:(NSNumber*) gameId;

@end
#endif