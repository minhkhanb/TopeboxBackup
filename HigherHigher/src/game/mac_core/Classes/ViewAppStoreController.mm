#import "ViewAppStoreController.h"
#import "CGame.h"
#import "PineGameDelegate.h"


@interface ViewAppStoreController ()<SKStoreProductViewControllerDelegate>
{
    BOOL canRotateToAllOrientations;
}
@end

@implementation ViewAppStoreController
/*- (void) viewWillAppear:(BOOL)animated
{
	[super viewWillAppear:animated];
    
	//	We're going onto the screen, disable auto rotations
	canRotateToAllOrientations = NO;
}

- (void) viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
    
	//	We're now on the screen, reenable auto rotations
	canRotateToAllOrientations = YES;
}

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
	//	(iOS 5)
	//	If we can auto rotate then only all orientations except upside down
	if(canRotateToAllOrientations)
		return (toInterfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
	//	Otherwise only allow our forced orienation (see below)
	return (toInterfaceOrientation == self.preferredInterfaceOrientationForPresentation);
}

- (BOOL) shouldAutorotate
{
	//	(iOS 6)
	//	Only auto rotate if we're on the screen (see above)
	return canRotateToAllOrientations;
}
- (UIInterfaceOrientation)preferredInterfaceOrientationForPresentation
{
	//	(iOS 6)
	//	Prefer (force) landscape
	return UIInterfaceOrientationPortrait;
}*/

- (BOOL) shouldAutorotate
{
	//	(iOS 6)
	//	Only auto rotate if we're on the screen (see above)
	return YES;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    //NSLog(@"\nshouldAutorotateToInterfaceOrientation = %d\n",interfaceOrientation);
    //return NO;
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    //[self showMyApps];
}

-(void)showMyApps
{
    SKStoreProductViewController* spvc = [[SKStoreProductViewController alloc] init];
    NSLog(@"\nPOP UP Game ID %@", _game_id);
    [spvc loadProductWithParameters:@{SKStoreProductParameterITunesItemIdentifier : _game_id}
                    completionBlock:nil];
    spvc.delegate = self;
    [self presentViewController:spvc animated:YES completion:nil];
    
}

-(void)productViewControllerDidFinish:(SKStoreProductViewController *)viewController
{
    [self dismissViewControllerAnimated:YES completion:nil];
    //[self dismissModalViewControllerAnimated:YES];
    [self removeFromParentViewController];
    UIWindow* window = [[PineGameDelegate sharedInstance] window];
    [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
    CGame::_this->ClearTouchesEnd();
}

- (void)setGameId:(NSNumber*) gameId
{
    printf("\n -----------------------------");
    NSLog(@"\nSET Game ID %@", gameId);
    _game_id = gameId;
    NSLog(@"\nRESULT Game ID %@", _game_id);
}


@end