

#import "ADManager.h"
#import "CBoxAppAppDelegate.h"

static ADManager* g_sharedADInstance = nil;

@implementation ADManager
- (void) InitADBanner
{
    [self setView:[[CBoxAppAppDelegate sharedInstance] glView]];
    
#ifdef USE_IAD
    // --- WARNING ---
    // If you are planning on creating banner views at runtime in order to support iOS targets that don't support the iAd framework
    // then you will need to modify this method to do runtime checks for the symbols provided by the iAd framework
    // and you will need to weaklink iAd.framework in your project's target settings.
    // See the iPad Programming Guide, Creating a Universal Application for more information.
    // http://developer.apple.com/iphone/library/documentation/general/conceptual/iPadProgrammingGuide/Introduction/Introduction.html
    // --- WARNING ---
    
    // Depending on our orientation when this method is called, we set our initial content size.
    // If you only support portrait or landscape orientations, then you can remove this check and
    // select either ADBannerContentSizeIdentifierPortrait (if portrait only) or ADBannerContentSizeIdentifierLandscape (if landscape only).
	NSString *contentSize = ADBannerContentSizeIdentifierPortrait;
	/*if (&ADBannerContentSizeIdentifierPortrait != nil)
     {
     contentSize = UIInterfaceOrientationIsPortrait(self.interfaceOrientation) ? ADBannerContentSizeIdentifierPortrait : ADBannerContentSizeIdentifierLandscape;
     }
     else
     {
     // user the older sizes 
     contentSize = UIInterfaceOrientationIsPortrait(self.interfaceOrientation) ? ADBannerContentSizeIdentifier320x50 : ADBannerContentSizeIdentifier480x32;
     }*/
	
    // Calculate the intial location for the banner.
    // We want this banner to be at the bottom of the view controller, but placed
    // offscreen to ensure that the user won't see the banner until its ready.
    // We'll be informed when we have an ad to show because -bannerViewDidLoadAd: will be called.
    CGRect frame;
    frame.size = [ADBannerView sizeFromBannerContentSizeIdentifier:contentSize];
    frame.origin = CGPointMake(0.0f, 0.0f);
    
    // Now to create and configure the banner view
    ADBannerView *bannerView = [[ADBannerView alloc] initWithFrame:frame];
    // Set the delegate to self, so that we are notified of ad responses.
    bannerView.delegate = self;
    // Set the autoresizing mask so that the banner is pinned to the bottom
    bannerView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleTopMargin;
    // Since we support all orientations in this view controller, support portrait and landscape content sizes.
    // If you only supported landscape or portrait, you could remove the other from this set.
    
	bannerView.requiredContentSizeIdentifiers = (&ADBannerContentSizeIdentifierPortrait != nil) ?
        [NSSet setWithObjects:ADBannerContentSizeIdentifierPortrait, ADBannerContentSizeIdentifierLandscape, nil] : 
        [NSSet setWithObjects:ADBannerContentSizeIdentifier320x50, ADBannerContentSizeIdentifier480x32, nil];
    
    // At this point the ad banner is now be visible and looking for an ad.
    [self.view addSubview:bannerView];
    //[self.view removeFromSuperview];
    banner = bannerView;
    
    [bannerView release];
#endif
    
    // Create a view of the standard size at the bottom of the screen.
    ADbanner = [[GADBannerView alloc]
                   initWithFrame:CGRectMake(0.0,
                                            0.0,
                                            GAD_SIZE_320x50.width,
                                            GAD_SIZE_320x50.height)];
    
    // Specify the ad's "unit identifier." This is your AdMob Publisher ID.
    ADbanner.adUnitID = @"a14d93f8c2bc5a5";
    
    // Let the runtime know which UIViewController to restore after taking
    // the user wherever the ad goes and add it to the view hierarchy.
    ADbanner.rootViewController = self;
    [self.view addSubview:ADbanner];
    
    // Initiate a generic request to load it with an ad.
    [ADbanner loadRequest:[GADRequest request]];

}

- (void) RemoveADBanner
{
    [banner removeFromSuperview];
    //[banner release];
}

+(ADManager*)getSharedInstance
{
	return g_sharedADInstance;
}

+(void) setSharedInstance:(ADManager*)instance
{
	if (g_sharedADInstance != nil && g_sharedADInstance != instance)
		[g_sharedADInstance release];
	
	g_sharedADInstance = instance;
}

#pragma mark ADBannerViewDelegate methods

/*-(void)bannerViewDidLoadAd:(ADBannerView *)banner
{
   
}*/

-(void)bannerView:(ADBannerView *)banner didFailToReceiveAdWithError:(NSError *)error
{
    
}

-(BOOL)bannerViewActionShouldBegin:(ADBannerView *)banner willLeaveApplication:(BOOL)willLeave
{
    return YES;
}

-(void)bannerViewActionDidFinish:(ADBannerView *)banner
{
}


@end

extern "C" void InitADManager()
{
	ADManager* ad = [[ADManager alloc] init];
	[ADManager setSharedInstance: ad];
	//[ad initGCBoard];
}

extern "C" void InitADBanner()
{
    [[CBoxAppAppDelegate sharedInstance] performSelectorOnMainThread:@selector(_InitADBanner:) withObject:nil waitUntilDone:NO];
    
}

extern "C" void RemoveADBanner()
{
    [[CBoxAppAppDelegate sharedInstance] performSelectorOnMainThread:@selector(_RemoveADBanner:) withObject:nil waitUntilDone:NO];
}
