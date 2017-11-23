
#import "PineViewController.h"
#import "CGame.h"
#import "PineGameDelegate.h"
#import <iAd/UIViewControlleriAdAdditions.h>
#import "Os_Functions.h"
#import <Foundation/Foundation.h>



extern "C" BOOL isGameCenterAPIAvailable();
@implementation PineViewController;
extern "C" void OS_ClearTouchView();
extern "C" int _is_openning_gc_view = 0;
bool _music_is_playing = false;
#if ADD_VUNGLEADS
NSString* vungleappid = nil;
-(void) VungleAdsLoad:(NSString*)id
{
    //dispatch_async(dispatch_get_main_queue(), ^(){
    NSString* appID = id;
    VungleSDK *sdk = [VungleSDK sharedSDK];
    
    // start vungle publisher library
    NSLog(@"vungleappidvungleappidvungleappid  = %@", appID);
    [sdk startWithAppId:appID];
    //Set VungleSDK Delegate
    [[VungleSDK sharedSDK] setDelegate:self];
    vungleappid = id;
    //});

}
-(bool) VungleAdsHasToShow
{
    
    [[VungleSDK sharedSDK] setDelegate:self];

    bool  PlayAble = [[VungleSDK sharedSDK] isAdPlayable];
    /*
    if(!PlayAble)
    {
         NSDictionary* debugInfo = [[VungleSDK sharedSDK] debugInfo];
        for(NSString *key in [debugInfo allKeys]) {
            NSLog(@"%@",[debugInfo objectForKey:key]);
        }
        
    }
     */
     //NSLog(@"vungleappidvungleappidvungleappid %@ PlayAble = %d", vungleappid,PlayAble);
    return  vungleappid != nil && PlayAble;
}
-(void) VungleAdsShow
{
    @try
    {
        [[VungleSDK sharedSDK] setDelegate:self];
        if([[VungleSDK sharedSDK] isAdPlayable])
        {
            // Play a Vungle ad (with default options)
            
            dispatch_sync(dispatch_get_main_queue(), ^(){
                [[[PineGameDelegate sharedInstance] window] addSubview:[self view]];
                [[[PineGameDelegate sharedInstance] window]bringSubviewToFront:[self view]];
                VungleSDK* sdk = [VungleSDK sharedSDK];
                NSError *error;
            bool success = [sdk playAd:self error:&error];
            if (error) {
                NSLog(@"Error encountered playing ad: %@", error);
            }
            else
            {
                PineAdBox::showAdState = SHOW_AD_PROCESSING;
                if(CGame::_this->IsCurrentMusicPlaying())
                {
                    _music_is_playing = true;
                    CGame::_this->StopCurrentMusic();
                    
                }
                
            }
            });
        }
        
        
    }
    @catch(NSException* eeee)
    {
        
    }
    
}

extern "C" bool iOS_VungleHasAdsToShow()
{
    return  [[PineGameDelegate sharedInstance]->_pine_view_vungle VungleAdsHasToShow];
    
}
extern "C" void iOS_VungleShowAds()
{
    
    [[PineGameDelegate sharedInstance]->_pine_view_vungle VungleAdsShow];
    
}
#pragma mark - VungleSDK Delegate

- (void)vungleSDKAdPlayableChanged:(BOOL)isAdPlayable {
    if (isAdPlayable) {
        NSLog(@"An ad is available for playback");
        
    } else {
        NSLog(@"No ads currently available for playback");
       
    }
}

- (void)vungleSDKwillShowAd {
    NSLog(@"An ad is about to be played!");
    //Use this delegate method to pause animations, sound, etc.
}

- (void) vungleSDKwillCloseAdWithViewInfo:(NSDictionary *)viewInfo willPresentProductSheet:(BOOL)willPresentProductSheet {
    
    if(_music_is_playing)
    {
        _music_is_playing = false;
        CGame::_this->PlayCurrentMusic(-1);
    }
    if (willPresentProductSheet) {
        //In this case we don't want to resume animations and sound, the user hasn't returned to the app yet
        NSLog(@"The ad presented was tapped and the user is now being shown the App Product Sheet");
        NSLog(@"ViewInfo Dictionary:");
        for(NSString * key in [viewInfo allKeys]) {
            NSLog(@"%@ : %@", key, [[viewInfo objectForKey:key] description]);
        }
    } else {
        //In this case the user has declined to download the advertised application and is now returning fully to the main app
        //Animations / Sound / Gameplay can be resumed now
        NSLog(@"The ad presented was not tapped - the user has returned to the app");
        NSLog(@"ViewInfo Dictionary:");
        for(NSString * key in [viewInfo allKeys]) {
            NSLog(@"%@ : %@", key, [[viewInfo objectForKey:key] description]);
        }
    }
    @try
    {
        if(vungleappid != nil && self != nil)
        {
            VungleSDK *sdk = [VungleSDK sharedSDK];
            if(sdk != nil)
            {
                [[VungleSDK sharedSDK] setDelegate:self];
                [sdk startWithAppId:vungleappid];
                
            }
           
        }
        PineAdBox::showAdState = SHOW_AD_COMPLETED;
        [self removeFromParentViewController];
        UIWindow* window = [[PineGameDelegate sharedInstance] window];
        [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
        OS_ClearTouchView();
    }
    @catch(NSException* eeee)
    {
        PineAdBox::showAdState = SHOW_AD_COMPLETED;
        [self removeFromParentViewController];
        UIWindow* window = [[PineGameDelegate sharedInstance] window];
        [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
        OS_ClearTouchView();
        
    }
   
   
}

- (void)vungleSDKwillCloseProductSheet:(id)productSheet {
    NSLog(@"The user has downloaded an advertised application and is now returning to the main app");
    if(_music_is_playing)
    {
        _music_is_playing = false;
        CGame::_this->PlayCurrentMusic(-1);
    }
    PineAdBox::showAdState = SHOW_AD_COMPLETED;
    [self removeFromParentViewController];
    UIWindow* window = [[PineGameDelegate sharedInstance] window];
    [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
    OS_ClearTouchView();
    //This method can be used to resume animations, sound, etc. if a user was presented a product sheet earlier
}

#endif
#if ADD_UNITYADS
-(void) UnityAdsLoad:(NSString*)id
{
    //[[UnityAds sharedInstance] setTestMode:YES];
    //[[UnityAds sharedInstance] setDebugMode:YES];
    [UnityAds initialize:id delegate:self];
    //+ (void)initialize:(NSString *)gameId
//delegate:(nullable id<UnityAdsDelegate>)delegate;
    //[[UnityAds sharedInstance] startWithGameId:id andViewController:self];
    //[[UnityAds sharedInstance] setDelegate:self];
}
-(void) UnityAdsShow
{
    // Set the zone before checking readiness or attempting to show.
    //[UnityAds setZone:@"mainMenu"];
    
    // Use the canShow method to check for zone readiness,
    //  then use the canShowAds method to check for ad readiness.
    if ([UnityAds isReady:@"mainMenu"])
    {
        if(CGame::_this->IsCurrentMusicPlaying())
        {
            _music_is_playing = true;
            CGame::_this->StopCurrentMusic();
        }
        // If both are ready, show the ad.
        PineAdBox::showAdState = SHOW_AD_PROCESSING;
        //[[UnityAds sharedInstance] setDelegate:self];
        [UnityAds show:self placementId:@"mainMenu"];
        [[[PineGameDelegate sharedInstance] window] addSubview:[self view]];
        [[[PineGameDelegate sharedInstance] window]bringSubviewToFront:[self view]];
        
    }
}


/**
 *  Called when `UnityAds` is ready to show an ad. After this callback you can call the `UnityAds` `show:` method for this placement.
 *  Note that sometimes placement might no longer be ready due to exceptional reasons. These situations will give no new callbacks.
 *
 *  @warning To avoid error situations, it is always best to check `isReady` method status before calling show.
 *  @param placementId The ID of the placement that is ready to show, as defined in Unity Ads admin tools.
 */
- (void)unityAdsReady:(NSString *)placementId
{
    
}
/**
 *  Called when `UnityAds` encounters an error. All errors will be logged but this method can be used as an additional debugging aid. This callback can also be used for collecting statistics from different error scenarios.
 *
 *  @param error   A `UnityAdsError` error enum value indicating the type of error encountered.
 *  @param message A human readable string indicating the type of error encountered.
 */
- (void)unityAdsDidError:(UnityAdsError)error withMessage:(NSString *)message
{
    NSLog(@"unityAdsFetchFailed");
    if(PineAdBox::showAdState == SHOW_AD_PROCESSING)
    {
        if(_music_is_playing)
        {
            _music_is_playing = false;
            CGame::_this->PlayCurrentMusic(-1);
        }
        PineAdBox::showAdState = SHOW_AD_COMPLETED;
        //[[UnityAds sharedInstance] hide];
        [self removeFromParentViewController];
        UIWindow* window = [[PineGameDelegate sharedInstance] window];
        [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
        OS_ClearTouchView();
    }
}
/**
 *  Called on a successful start of advertisement after calling the `UnityAds` `show:` method.
 *
 * @warning If there are errors in starting the advertisement, this method may never be called. Unity Ads will directly call `unityAdsDidFinish:withFinishState:` with error status.
 *
 *  @param placementId The ID of the placement that has started, as defined in Unity Ads admin tools.
 */
- (void)unityAdsDidStart:(NSString *)placementId;
{
    
}
/**
 *  Called after the ad has closed.
 *
 *  @param placementId The ID of the placement that has finished, as defined in Unity Ads admin tools.
 *  @param state       An enum value indicating the finish state of the ad. Possible values are `Completed`, `Skipped`, and `Error`.
 */
- (void)unityAdsDidFinish:(NSString *)placementId
          withFinishState:(UnityAdsFinishState)state
{
    NSLog(@"unityAdsDidHide");
    if(PineAdBox::showAdState == SHOW_AD_PROCESSING)
    {
        if(_music_is_playing)
        {
            _music_is_playing = false;
            CGame::_this->PlayCurrentMusic(-1);
        }
        PineAdBox::showAdState = SHOW_AD_COMPLETED;
        //[[UnityAds sharedInstance] hide];
        [self removeFromParentViewController];
        UIWindow* window = [[PineGameDelegate sharedInstance] window];
        [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
        OS_ClearTouchView();
    }
}
-(void) UnityAdsVideoRewardShow
{
    if ([UnityAds isReady:@"videoReward"])
    {
        if(CGame::_this->IsCurrentMusicPlaying())
        {
            _music_is_playing = true;
            CGame::_this->StopCurrentMusic();
        }
        // If both are ready, show the ad.
        PineAdBox::showAdState = SHOW_AD_PROCESSING;
        //[[UnityAds sharedInstance] setDelegate:self];
        [UnityAds show:self placementId:@"videoReward"];
        [[[PineGameDelegate sharedInstance] window] addSubview:[self view]];
        [[[PineGameDelegate sharedInstance] window]bringSubviewToFront:[self view]];
        
    }
  
}
-(bool) UnityHasAdsToShow
{
    // Set the zone before checking readiness or attempting to show.
    //[[UnityAds sharedInstance] setZone:@"mainMenu"];
    return [UnityAds isReady:@"mainMenu"];//[[UnityAds sharedInstance] canShow];
}
-(bool) UnityHasVideoAdsToShow
{
    // Set the zone before checking readiness or attempting to show.
    //[[UnityAds sharedInstance] setZone:@"videoReward"];
    return [UnityAds isReady:@"videoReward"];//[[UnityAds sharedInstance] canShow];
   // return [[UnityAds sharedInstance] canShow];
}






#endif
#if ADD_ADMOB
-(void) adMobLoad:(NSString*)id
{
    interstitialAdmob = [[GADInterstitial alloc] initWithAdUnitID:id];
    //interstitialAdmob.adUnitID = id;
    admobID = id;
    interstitialAdmob.delegate = self;
    GADRequest *request = [GADRequest request];
#if ADD_VUNGLE_TO_MEDIATION
    VungleAdNetworkExtras *extras = [[VungleAdNetworkExtras alloc] init];
    extras.allPlacements = @[VUNGLE_INTERTIAL_PLAYING_PLACEMENTS];
    extras.playingPlacement = VUNGLE_INTERTIAL_PLAYING_PLACEMENTS; // Optional if you provide through AdMob dashboard
    [request registerAdNetworkExtras:extras];
#endif
    // Requests test ads on simulators.
#ifdef GAD_SIMULATOR_ID
    request.testDevices = @[GAD_SIMULATOR_ID];
#endif
    [interstitialAdmob loadRequest:request];
}
-(bool) admob_hasadstoshow
{
    return [interstitialAdmob isReady];
}
-(void) AdMobShow
{
    if ([interstitialAdmob isReady]) {

        //[[[PineGameDelegate sharedInstance] _opgenGLESViewController]view]]
        
       
        [[[PineGameDelegate sharedInstance] window] addSubview:[self view]];
        [[[PineGameDelegate sharedInstance] window]bringSubviewToFront:[self view]];
       [interstitialAdmob presentFromRootViewController:self];
        //[interstitialAdmob presentFromRootViewController:self];
    }
}
- (void)interstitial:(GADInterstitial *)ad didFailToReceiveAdWithError:(GADRequestError *)error
{
    if(error != nil)
    {
        NSLog(@"error admob show error %@",error);
        
    }
    
}
- (void)interstitialWillDismissScreen:(GADInterstitial *)ad;
{
    PineAdBox::showAdState = SHOW_AD_COMPLETED;
    interstitialAdmob.delegate = nil;
    [interstitialAdmob release];
    
    // Prepare next interstitial
    interstitialAdmob = [[GADInterstitial alloc] initWithAdUnitID:admobID];
    //interstitialAdmob = [[GADInterstitial alloc] init];
    //interstitialAdmob.adUnitID = admobID;
    interstitialAdmob.delegate = self;
    GADRequest *request = [GADRequest request];
    // Requests test ads on simulators.
    //request.testDevices = @[ GAD_SIMULATOR_ID ];
    [interstitialAdmob loadRequest:request];
    
    [self removeFromParentViewController];
    UIWindow* window = [[PineGameDelegate sharedInstance] window];
    [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
    OS_ClearTouchView();
    
}
- (void)interstitialDidDismissScreen:(GADInterstitial *)ad
{
    
    PineAdBox::showAdState = SHOW_AD_COMPLETED;
    OS_ClearTouchView();
   
}
- (void)interstitialWillLeaveApplication:(GADInterstitial *)ad;
{
    PineAdBox::showAdState = SHOW_AD_COMPLETED;
    [self removeFromParentViewController];
    UIWindow* window = [[PineGameDelegate sharedInstance] window];
    [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
    printf("\nClick appp");
    OS_ClearTouchView();
}
#endif
#if ADD_ADMOB_VIDEO_ADS
-(void) cycleAdmobVideoAds
{
     //dispatch_async(dispatch_get_main_queue(), ^(){
    GADRequest *request = [GADRequest request];
#if ADD_VUNGLE_TO_MEDIATION
    VungleAdNetworkExtras *extras = [[VungleAdNetworkExtras alloc] init];
    extras.allPlacements = @[VUNGLE_VIDEO_PLAYING_PLACEMENTS];
    extras.playingPlacement = VUNGLE_VIDEO_PLAYING_PLACEMENTS; // Optional if you provide through AdMob dashboard
    [request registerAdNetworkExtras:extras];
#endif
    //request.testDevices = @[ @"f1f0fcf79cab52a3cc290b5b4c40322b" ];
    [GADRewardBasedVideoAd sharedInstance].delegate = self;
    [[GADRewardBasedVideoAd sharedInstance] loadRequest:request
                                           withAdUnitID:@ADMOB_IOS_VIDEO_ADS_ID];
     //});

}
-(void) showAdmobVideoAds
{
    if ([[GADRewardBasedVideoAd sharedInstance] isReady]) {
        PineAdBox::showAdState = SHOW_AD_PROCESSING;
        [[[PineGameDelegate sharedInstance] window] addSubview:[self view]];
        [[[PineGameDelegate sharedInstance] window]bringSubviewToFront:[self view]];
        [[GADRewardBasedVideoAd sharedInstance] presentFromRootViewController:self];
        if(CGame::_this->IsCurrentMusicPlaying())
        {
            _music_is_playing = true;
            CGame::_this->StopCurrentMusic();
        }
        
        //PineAdBox::showAdState = SHOW_AD_WAIT_REWARD;
    }
}
- (void)rewardBasedVideoAd:(GADRewardBasedVideoAd *)rewardBasedVideoAd
   didRewardUserWithReward:(GADAdReward *)reward {
    
   
    PineAdBox::showAdState = SHOW_AD_WAIT_REWARD;
    
}

- (void)rewardBasedVideoAdDidReceiveAd:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    NSLog(@"Reward based video ad is received.");
    
}

- (void)rewardBasedVideoAdDidOpen:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    NSLog(@"Opened reward based video ad.");
    //PineAdBox::showAdState = SHOW_AD_PROCESSING;
  
}

- (void)rewardBasedVideoAdDidStartPlaying:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    NSLog(@"Reward based video ad started playing.");
    //PineAdBox::showAdState = SHOW_AD_SHOWING;
}

- (void)rewardBasedVideoAdDidClose:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    NSLog(@"Reward based video ad is closed.");
    [self removeFromParentViewController];
    UIWindow* window = [[PineGameDelegate sharedInstance] window];
    [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
    OS_ClearTouchView();
    if(_music_is_playing)
    {
        _music_is_playing = false;
        CGame::_this->PlayCurrentMusic(-1);
    }
    [self cycleAdmobVideoAds];
    if(PineAdBox::showAdState  == SHOW_AD_WAIT_REWARD)
    {
        PineAdBox::showAdState = SHOW_AD_COMPLETED;
    }
    else
    {
         PineAdBox::showAdState = SHOW_AD_CANCEL;
    }
    
}

- (void)rewardBasedVideoAdWillLeaveApplication:(GADRewardBasedVideoAd *)rewardBasedVideoAd {
    NSLog(@"Reward based video ad will leave application.");
    
    [self removeFromParentViewController];
    UIWindow* window = [[PineGameDelegate sharedInstance] window];
    [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
    OS_ClearTouchView();
    if(_music_is_playing)
    {
        _music_is_playing = false;
        CGame::_this->PlayCurrentMusic(-1);
    }
    
    PineAdBox::showAdState = SHOW_AD_COMPLETED;
    
}

- (void)rewardBasedVideoAd:(GADRewardBasedVideoAd *)rewardBasedVideoAd
    didFailToLoadWithError:(NSError *)error {
    NSLog(@"Reward based video ad failed to load.");
}
extern "C" bool OS_HasAdmobVideoAds()
{
    return [[GADRewardBasedVideoAd sharedInstance] isReady];
}
extern "C" void OS_ShowAdmobVideoAds()
{
    if(OS_HasAdmobVideoAds())
    {
        dispatch_async(dispatch_get_main_queue(), ^(){
            [[PineGameDelegate sharedInstance]->_pine_view_video_admob showAdmobVideoAds];
        });
    }
}
#endif
#if ADD_ADMOB_NATIVE_ADS
- (void) cycleNativeAds
{
    // Loads an ad for any of app install, content, or custom native ads.
    PineAdBox::_native_ads_available = false;
    if(PineAdBox::_native_ads_processing)
    {
        PineAdBox::showAdState = SHOW_AD_COMPLETED;
        [self removeFromParentViewController];
        UIWindow* window = [[PineGameDelegate sharedInstance] window];
        [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
        NSLog(@"May Be Click Install Ads");
    }
    PineAdBox::_native_ads_processing = false;
    NSMutableArray *adTypes = [[NSMutableArray alloc] init];
    [adTypes addObject:kGADAdLoaderAdTypeNativeAppInstall];
    //[adTypes addObject:kGADAdLoaderAdTypeNativeContent];
   
    
    if (adTypes.count == 0) {
        NSLog(@"At least one ad format must be selected to refresh the ad.");
    } else {
        //self.refreshButton.enabled = NO;
        self.adLoader = [[GADAdLoader alloc] initWithAdUnitID:@ADMOB_IOS_NATIVE_ADS_ID
                                           rootViewController:self
                                                      adTypes:adTypes
                                                      options:nil];
        self.adLoader.delegate = self;
        [self.adLoader loadRequest:[GADRequest request]];
    }

}
-(void) AdMobNativeShow
{
    if(PineAdBox::_native_ads_available)
    {
        [[[PineGameDelegate sharedInstance] window] addSubview:[self view]];
        [[[PineGameDelegate sharedInstance] window]bringSubviewToFront:[self view]];
        PineAdBox::showAdState = SHOW_AD_PROCESSING;
        PineAdBox::_native_ads_processing = true;
    }
}
extern "C" void OS_AdmobNativeShow()
{
    if(PineAdBox::_native_ads_available)
    {
        dispatch_async(dispatch_get_main_queue(), ^(){
             [[PineGameDelegate sharedInstance]->_pine_view_admob_native AdMobNativeShow];
        });
       
    }
}
- (void)setAdView:(UIView *)view {
    // Remove previous ad view.
    //[self.nativeAdView removeFromSuperview];
    //self.nativeAdView = view;
    
    // Add new ad view and set constraints to fill its container.
    [[self view] addSubview:view];
    
    //if(Device::IsIpadDevice())
    {
        UIView* glview = (UIView*)[PineGameDelegate sharedInstance]->_opgenGLESViewController;
        
        CGRect rect = [[UIScreen mainScreen] bounds];
        //CGRect rect = [[UIScreen mainScreen] applicationFrame];
        float width  = rect.size.width;
        float height = rect.size.height;
        float w = width - view.frame.size.width;
        float h = height - view.frame.size.height;
        float scaleX = width/view.frame.size.width;
        float scaleY = height/view.frame.size.height;
        printf("\nw = %f, h = %f, ww = %f, hh = %f",width,height, view.frame.size.width, view.frame.size.height);
        CGAffineTransform  translate = CGAffineTransformMakeTranslation(w*scaleX/2,h*scaleY/2);
    
        CGAffineTransform  scale = CGAffineTransformMakeScale(width/view.frame.size.width, height/view.frame.size.height);
        [self view].transform  = CGAffineTransformConcat(scale,translate);
        //[self view].center = CGPointMake(rect.size.width/2,rect.size.height/2);
    }
  
}
#pragma mark GADAdLoaderDelegate implementation

- (void)adLoader:(GADAdLoader *)adLoader didFailToReceiveAdWithError:(GADRequestError *)error {
    NSLog(@"%@ failed with error: %@", adLoader, [error localizedDescription]);
    PineAdBox::_native_ads_available = false;
    //self.refreshButton.enabled = YES;
}
#pragma mark GADNativeAppInstallAdLoaderDelegate implementation

- (void)adLoader:(GADAdLoader *)adLoader
didReceiveNativeAppInstallAd:(GADNativeAppInstallAd *)nativeAppInstallAd {
    NSLog(@"Received native app install ad: %@", nativeAppInstallAd);
    //self.refreshButton.enabled = YES;
    
    // Create and place ad in view hierarchy.

    GADNativeAppInstallAdView *appInstallAdView;
    if(Device::IsIpadDevice())
    {
        appInstallAdView = [[[NSBundle mainBundle] loadNibNamed:@"NativeAppInstallAdView"
                                   owner:nil
                                 options:nil] firstObject];
    }
    else
    {
        appInstallAdView = [[[NSBundle mainBundle] loadNibNamed:@"NativeAppInstallAdViewIphone6"
                                                          owner:nil
                                                        options:nil] firstObject];
    }
    //NSLog(@"appInstallAdView: %@", appInstallAdView);
    [self setAdView:appInstallAdView];
    int i = 0;
    UIButton* exitNativeAds = nil;
    for(UIView *subview in appInstallAdView.subviews)
    {
        if ( [subview isKindOfClass:[UIButton class]] )
        {
            exitNativeAds  = (UIButton*)subview;
            //NSLog(@"BUTTTON: [%d]: title %@", i,exitNativeAds.titleLabel);
            if([exitNativeAds.titleLabel.text  isEqual: @"Exit"])
            {
                 // = (UIButton*)subview;
                [exitNativeAds addTarget:self action:@selector(exitNativeAdsAction) forControlEvents:UIControlEventTouchUpInside];
                break;
                //
            }
        }
        i++;
    }
    //appInstallAdView.delegate = self;
    // Associate the app install ad view with the app install ad object. This is required to make the
    // ad clickable.
    appInstallAdView.nativeAppInstallAd = nativeAppInstallAd;
    
    // Populate the app install ad view with the app install ad assets.
    // Some assets are guaranteed to be present in every app install ad.
    ((UILabel *)appInstallAdView.headlineView).text = nativeAppInstallAd.headline;
    ((UIImageView *)appInstallAdView.iconView).image = nativeAppInstallAd.icon.image;
    ((UILabel *)appInstallAdView.bodyView).text = nativeAppInstallAd.body;
    ((UIImageView *)appInstallAdView.imageView).image =
    ((GADNativeAdImage *)[nativeAppInstallAd.images firstObject]).image;
    [((UIButton *)appInstallAdView.callToActionView)setTitle:nativeAppInstallAd.callToAction
                                                    forState:UIControlStateNormal];
    
    // Other assets are not, however, and should be checked first.
    if (nativeAppInstallAd.starRating) {
        ((UIImageView *)appInstallAdView.starRatingView).image =
        [self imageForStars:nativeAppInstallAd.starRating];
        appInstallAdView.starRatingView.hidden = NO;
    } else {
        appInstallAdView.starRatingView.hidden = YES;
    }
    
    if (nativeAppInstallAd.store) {
        ((UILabel *)appInstallAdView.storeView).text = nativeAppInstallAd.store;
        appInstallAdView.storeView.hidden = NO;
    } else {
        appInstallAdView.storeView.hidden = YES;
    }
    
    if (nativeAppInstallAd.price) {
        ((UILabel *)appInstallAdView.priceView).text = nativeAppInstallAd.price;
        appInstallAdView.priceView.hidden = NO;
    } else {
        appInstallAdView.priceView.hidden = YES;
    }
    
    // In order for the SDK to process touch events properly, user interaction should be disabled.
    appInstallAdView.callToActionView.userInteractionEnabled = NO;
    PineAdBox::_native_ads_available = true;
    //[self AdMobNativeShow];
}
-(void)exitNativeAdsAction
{
    PineAdBox::showAdState = SHOW_AD_CANCEL;
    PineAdBox::_native_ads_processing = false;
    [self removeFromParentViewController];
    UIWindow* window = [[PineGameDelegate sharedInstance] window];
    [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
    NSLog(@"Click Exits Ads");
    [self cycleNativeAds];
    //your implementation
}
/// Gets an image representing the number of stars. Returns nil if rating is less than 3.5 stars.
- (UIImage *)imageForStars:(NSDecimalNumber *)numberOfStars {
    double starRating = [numberOfStars doubleValue];
    if (starRating >= 5) {
        return [UIImage imageNamed:@"stars_5"];
    } else if (starRating >= 4.5) {
        return [UIImage imageNamed:@"stars_4_5"];
    } else if (starRating >= 4) {
        return [UIImage imageNamed:@"stars_4"];
    } else if (starRating >= 3.5) {
        return [UIImage imageNamed:@"stars_3_5"];
    } else {
        return nil;
    }
}

#pragma mark GADNativeContentAdLoaderDelegate implementation

- (void)adLoader:(GADAdLoader *)adLoader
didReceiveNativeContentAd:(GADNativeContentAd *)nativeContentAd {
    NSLog(@"Received native content ad: %@", nativeContentAd);
    //self.refreshButton.enabled = YES;
    
    // Create and place ad in view hierarchy.
    GADNativeContentAdView *contentAdView =
    [[[NSBundle mainBundle] loadNibNamed:@"NativeContentAdView"
                                   owner:nil
                                 options:nil] firstObject];
    [self setAdView:contentAdView];
    
    // Associate the content ad view with the content ad object. This is required to make the ad
    // clickable.
    contentAdView.nativeContentAd = nativeContentAd;
    
    // Populate the content ad view with the content ad assets.
    // Some assets are guaranteed to be present in every content ad.
    ((UILabel *)contentAdView.headlineView).text = nativeContentAd.headline;
    ((UILabel *)contentAdView.bodyView).text = nativeContentAd.body;
    ((UIImageView *)contentAdView.imageView).image =
    ((GADNativeAdImage *)[nativeContentAd.images firstObject]).image;
    ((UILabel *)contentAdView.advertiserView).text = nativeContentAd.advertiser;
    [((UIButton *)contentAdView.callToActionView)setTitle:nativeContentAd.callToAction
                                                 forState:UIControlStateNormal];
    
    // Other assets are not, however, and should be checked first.
    if (nativeContentAd.logo && nativeContentAd.logo.image) {
        ((UIImageView *)contentAdView.logoView).image = nativeContentAd.logo.image;
        contentAdView.logoView.hidden = NO;
    } else {
        contentAdView.logoView.hidden = YES;
    }
    
    // In order for the SDK to process touch events properly, user interaction should be disabled.
    contentAdView.callToActionView.userInteractionEnabled = NO;
}

#endif

#if ADD_IADVERT

- (void)cycleInterstitial
{
    // Release the old interstial and create a new one.
    [interstitial release];
    [ADInterstitialAd release];
    interstitial = [[ADInterstitialAd alloc] init];
    interstitial.delegate = self;
}
-(void)showFullScreen_iAd
{
    //Check if already requesting ad
    if (requestingAd == NO)
    {
        if(interstitial == nil)
        {
            [self cycleInterstitial];
        }
        self.interstitialPresentationPolicy = ADInterstitialPresentationPolicyManual;
        [self requestInterstitialAdPresentation];
        NSLog(@"interstitialAdREQUEST");
        requestingAd = YES;
    }//end if
}

-(void)interstitialAd:(ADInterstitialAd *)interstitialAd didFailWithError:(NSError *)error
{
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
#else
    [self closeView];
#endif
    interstitial = nil;
    [interstitialAd release];
    [ADInterstitialAd release];
    requestingAd = NO;
    NSLog(@"interstitialAd didFailWithERROR");
    NSLog(@"%@", error);
    
}

-(void)interstitialAdDidLoad:(ADInterstitialAd *)interstitialAd
{
    NSLog(@"interstitialAdDidLOAD");
    if (interstitialAd != nil && interstitial != nil && requestingAd == YES)
    {
        if(interstitial.loaded && PineAdBox::shouldAppPopup)
        {
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
            [interstitial presentInView:[[[PineGameDelegate sharedInstance] _opgenGLESViewController]view]];
#else
            [interstitial presentFromViewController:self];
            [[[PineGameDelegate sharedInstance] window] addSubview:[self view]];
            [[[PineGameDelegate sharedInstance] window]bringSubviewToFront:[self view]];
#endif
            NSLog(@"interstitialAdDidPRESENT");
        }
        else
        {
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
#else
            [self closeView];
#endif
            interstitial = nil;
            [interstitialAd release];
            [ADInterstitialAd release];
            requestingAd = NO;
            
        }
    }
}

-(void)interstitialAdDidUnload:(ADInterstitialAd *)interstitialAd
{
    //interstitial = nil
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
#else
    [self closeView];
#endif
    [self cycleInterstitial];
    [interstitialAd release];
    //[ADInterstitialAd release];
    requestingAd = NO;
    
    NSLog(@"interstitialAdDidUNLOAD");
}

-(void)interstitialAdActionDidFinish:(ADInterstitialAd *)interstitialAd {
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
#else
    [self closeView];
#endif
    interstitial = nil;
    [self cycleInterstitial];
    [interstitialAd release];
    requestingAd = NO;
    
    NSLog(@"interstitialAdDidFINISH");
}
#endif

#pragma mark - Game Center Methods

#if ADD_GAMECENTER
- (void) disableGameCenter
{
    // Write something to disable gamecenter.
    // gameCenterAvailable = FALSE;
}

-(void)showAuthenticationDialogWhenReasonable:(UIViewController *)viewController
{
    // Pause Tasks Here
    //[[[(AppDelegate *)[[UIApplication sharedApplication] delegate] window] rootViewController] presentViewController:viewController animated:YES completion:nil];
}

-(bool)isStringFormat:(NSString*)stringId
{
    bool ret = false;
    
    char tempStr[128];
    int strLength = sprintf(tempStr, "%s", [stringId UTF8String]);
    
    for (int i = 0; i < strLength; i++)
    {
        bool isNumber = false;
        for (int j = '0'; j <= '9'; j++)
        {
            if (tempStr[i] == j)
            {
                isNumber = true;
                break;
            }
        }
        if (!isNumber)
        {
            ret = true;
            break;
        }
    }
    
    return ret;
}
int PineGameCenterController::_iGameCenterAuthenStatus = 0;
-(void) GameCenterAuthentication:(PineSocial*)social
{
    NSLog(@"Authencation game center");
    if (!isGameCenterAPIAvailable()) {
        // Game Center is not available.
        _hasGamecenterAuthentication = false;
        social->SetFlag(SOCIAL_FLAG_ACCESS_DENINE);
        social->SetState(PINESOCIAL_STATE_COMPLETED);
        PineGameCenterController::_iGameCenterAuthenStatus = 2;
    } else {
        
        GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
        localPlayer.authenticateHandler = ^(UIViewController *viewController, NSError *error){
            if (viewController != nil)
            {
                
                CGame::_is_portrail_exception = true;
                social->SetState(PINESOCIAL_STATE_GC_LOGIN);
                PineSocial::HasloggingSystem = true;
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
                [[[PineGameDelegate sharedInstance] _opgenGLESViewController] presentViewController:viewController animated:YES completion:nil];
#else
                [[self view] addSubview:viewController.view];
                [[[PineGameDelegate sharedInstance] window] addSubview:[self view]];
                [[[PineGameDelegate sharedInstance] window]bringSubviewToFront:[self view]];
                [self presentViewController:viewController animated:YES completion:nil];
               
#endif
            }
            else
                if (localPlayer.isAuthenticated)
                {
                     PineSocial::HasloggingSystem = false;
                     PineGameCenterController::_iGameCenterAuthenStatus = 1;
                    _hasGamecenterAuthentication = true;
#if USE_PINESOCIAL_AT_ID_STRING
                    self.currentPlayerID = [[NSString alloc] initWithString:[localPlayer.playerID stringByReplacingOccurrencesOfString:@":" withString:@"_"]];
#else
                    self.currentPlayerID = [[NSString alloc] initWithFormat:@"%@",[localPlayer.playerID componentsSeparatedByString:@":"][1]] ;
#endif
                    self.currentName = [[NSString alloc] initWithFormat:@"%@",localPlayer.displayName];
                    self.currentAlias = [[NSString alloc] initWithFormat:@"%@",localPlayer.alias];
                    NSLog(@"currentPlayerID %@ currentName %@ currentAlias %@",self.currentPlayerID,self.currentName,self.currentAlias);
                    //[self GameCenterRetrieveFriends];
                    // Load game instance for new current player, if none exists create a new.
#if USE_PINESOCIAL_AT_ID_STRING
                    social->SetInfomation([self.currentPlayerID UTF8String], [self.currentAlias UTF8String]);
#else
                    social->SetInfomation([self.currentPlayerID longLongValue],[self.currentAlias UTF8String]);
#endif
                    social->_currentUnicodeUserNameLength = [self.currentAlias length];
                    if (social->_currentUnicodeUserNameLength > 32)
                    {
                        social->_currentUnicodeUserNameLength = 32;
                    }
                    
                    for (int i = 0; i < social->_currentUnicodeUserNameLength; i++)
                    {
                        social->_unicodeUserName[i] = [self.currentAlias characterAtIndex:i];
                    }
                    
                    social->SetState(PINESOCIAL_STATE_SUCCESS);
                    
                    if ([self isStringFormat:self.currentPlayerID])
                    {
                        social->SetFlag(SOCIAL_FLAG_ID_IS_STRING_FORMAT);
                    }
                    else
                    {
                        social->SetFlag(SOCIAL_FLAG_FULL_ACCESS);
                    }
                    [viewController dismissViewControllerAnimated:YES completion:nil];
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
#else
                    
                    [viewController removeFromParentViewController];
                    UIWindow* window = [[PineGameDelegate sharedInstance] window];
                    [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
#endif
                   
                }
                else
                {
                     PineSocial::HasloggingSystem = false;
                    PineGameCenterController::_iGameCenterAuthenStatus = 2;
                    _hasGamecenterAuthentication = false;
                    social->SetFlag(SOCIAL_FLAG_ACCESS_DENINE);
                    social->SetState(PINESOCIAL_STATE_COMPLETED);
                    [viewController dismissViewControllerAnimated:YES completion:nil];
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
#else
                    
                    [viewController removeFromParentViewController];
                    UIWindow* window = [[PineGameDelegate sharedInstance] window];
                    [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
#endif
                    CGame::_is_portrail_exception = false;
                    
                }
            
        };
        
    }
}
-(void) GameCenterRetrieveFriends:(PineSocial*)social
{
    GKLocalPlayer *lp = [GKLocalPlayer localPlayer];
    if (lp.authenticated)
    {
        [lp loadFriendsWithCompletionHandler:^(NSArray *friends, NSError *error)
         {
             //NSLog(@"friends %@", friends);
             social->ResetFriendList();
             if (friends != nil)
             {
                 //[self GameCenterLoadPlayerData: friends];
                 
                 for (NSString *fid in friends) {
                     if(fid != nil)
                     {
#if USE_PINESOCIAL_AT_ID_STRING
                         social->AddFriend([[fid stringByReplacingOccurrencesOfString:@":" withString:@"_"] UTF8String],"");
#else
                         fid = [[NSString alloc] initWithFormat:@"%@",[fid componentsSeparatedByString:@":"][1]];
                         social->AddFriend([fid  longLongValue],"");
#endif
                         //NSLog(@"fid %@", fid);
                     }
                 }
                 
             }
             social->SetState(PINESOCIAL_STATE_SUCCESS);
             
         }];
        
    }
    
}
-(void) GameCenterLoadPlayerData: (NSArray *) identifiers
{
    [GKPlayer loadPlayersForIdentifiers:identifiers withCompletionHandler:^(NSArray *players, NSError *error)
     {
         
         if (error != nil)
         {
             // Handle the error.
             NSLog(@"error %@", error);
         }
         if (players != nil)
         {
             // Process the array of GKPlayer objects.
             NSLog(@"players %@", players);
             
         }
     }];
    
}
- (void) GameCenterLoadLeaderboardInfo
{
    [GKLeaderboard loadLeaderboardsWithCompletionHandler:^(NSArray *leaderboards, NSError *error) {
        //self.leaderboards = leaderboards;
        NSLog(@"leaderboards %@", leaderboards);
    }];
}

- (void) reportScore: (int64_t) value forCategory: (NSString*) category
{
    printf("\nSUbmit socre");
    if(SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"7.0"))
    {
        GKScore *score = [[GKScore alloc] initWithLeaderboardIdentifier:category];
        score.value = value;
        
        [GKScore reportScores:@[score] withCompletionHandler:^(NSError *error) {
            if (error != nil) {
                NSLog(@"Submit score %@", [error localizedDescription]);
            }
            else
            {
                NSLog(@"Submit score sucessfull !");
            }
        }];
    }
    else
    {
        GKScore *scoreReporter = [[GKScore alloc] initWithCategory:category];
        scoreReporter.value = value;
        
        [GKScore reportScores:@[scoreReporter] withCompletionHandler:^(NSError *error) {
            if (error != nil) {
                NSLog(@"Submit score %@", [error localizedDescription]);
            }
            else
            {
                NSLog(@"Submit score sucessfull !");
            }
        }];
    }
    
}

-(void) GameCenterShowLeaderboard:(NSString*) leaderid
{
    NSLog(@"Show Leaderboard id=%@",leaderid);
    GKGameCenterViewController *gcViewController = [[GKGameCenterViewController alloc] init];
    gcViewController.gameCenterDelegate = self;
    gcViewController.viewState = GKGameCenterViewControllerStateLeaderboards;
   
    _is_openning_gc_view = 1;

#if USE_NEW_SYSTEM_VIEW_IOS_7_8
    gcViewController.leaderboardIdentifier = leaderid;
    [[[PineGameDelegate sharedInstance] _opgenGLESViewController] presentViewController:gcViewController animated:YES completion:^{
        
        
    }];
#else
    
    [[[PineGameDelegate sharedInstance] window] addSubview:[self view]];
    [[[PineGameDelegate sharedInstance] window]bringSubviewToFront:[self view]];
    
    [self presentViewController:gcViewController animated:YES completion:^{
        [[[PineGameDelegate sharedInstance] window]bringSubviewToFront:[self view]];
        [[self view] addSubview:gcViewController.view];
    }];
#endif
    
}



-(void) GameCenterShowAchievement:(NSString*) leaderid
{
    GKGameCenterViewController *gcViewController = [[GKGameCenterViewController alloc] init];
    gcViewController.gameCenterDelegate = self;
    gcViewController.viewState = GKGameCenterViewControllerStateAchievements;
     _is_openning_gc_view = 1;
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
    [[[PineGameDelegate sharedInstance] _opgenGLESViewController] presentViewController:gcViewController animated:YES completion:^{
       
        
    }];
#else
    
    [[[PineGameDelegate sharedInstance] window] addSubview:[self view]];
    [[[PineGameDelegate sharedInstance] window]bringSubviewToFront:[self view]];
    
    [self presentViewController:gcViewController animated:YES completion:^{
        [[[PineGameDelegate sharedInstance] window]bringSubviewToFront:[self view]];
        [[self view] addSubview:gcViewController.view];
    }];
#endif
}
-(void) ResetAllAchievements
{
    [GKAchievement resetAchievementsWithCompletionHandler: ^(NSError *error)
     {
         if (!error) {
             
         } else {
             
         }
     }];
}
-(void)updateAchievements:(NSString*) Identifier progress:(int)progress show:(bool)hasnoti{
    //NSString *achievementIdentifier;
    float progressPercentage = progress;
    
    GKAchievement *levelAchievement = nil;
    
    
    levelAchievement = [[GKAchievement alloc] initWithIdentifier:Identifier];
    levelAchievement.percentComplete = progressPercentage;
    if(progressPercentage == 100)
    {
        levelAchievement.showsCompletionBanner = hasnoti;
    }
    
    
    
    NSArray *achievements = @[levelAchievement];
    
    [GKAchievement reportAchievements:achievements withCompletionHandler:^(NSError *error) {
        if (error != nil) {
            NSLog(@"Submit achievement error: %@", [error localizedDescription]);
        }
        else
        {
            NSLog(@"Submit score sucessfull !");
            if(progressPercentage >= 100)
            {

#ifdef MACRO_OS_GCNotifyAchievementCompleted
                MACRO_OS_GCNotifyAchievementCompleted([Identifier UTF8String]);
#endif
            }
        }
    }];
}

// Lam Add Loadachivement
- (void) GameCenterLoadAchievements
{    [GKAchievement loadAchievementsWithCompletionHandler:^(NSArray *achievements, NSError *error) {
    if (error != nil)
    {
        NSLog(@"Error in loading achievements: %@", error);
    }
    if (achievements != nil)
    {
        // Process the array of achievements.
        for(int i = 0; i< achievements.count; i++)
        {
            GKAchievement *achieve = [[GKAchievement alloc	] initWithIdentifier:[achievements objectAtIndex:i]];
#ifdef MACRO_OS_GCNotifyLoadAchievement
            MACRO_OS_GCNotifyLoadAchievement([achieve.identifier UTF8String],achieve.percentComplete);
#endif
        }
        }
    }];
}
-(void) closeView
{
    [self removeFromParentViewController];
    UIWindow* window = [[PineGameDelegate sharedInstance] window];
    [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
}
-(void)gameCenterViewControllerDidFinish:(GKGameCenterViewController *)gameCenterViewController
{
    [gameCenterViewController dismissViewControllerAnimated:YES completion:^
     {
         [self closeView];
         _is_openning_gc_view = 0;
     }
     ];
}
- (void) GameCenterLoadScore:(PineSocial*)social
{
    [GKLeaderboard loadLeaderboardsWithCompletionHandler:^(NSArray *leaderboards, NSError *nsError) {
        if( nsError != nil )
        {
            NSLog(@"get leaderboard score %@",nsError) ;
            return ;
        }
        PineGameCenterController::_num_of_leaderboard = [leaderboards count];
        PineGameCenterController::_current_leaderboard_loaded = 0;
        for(int i = 0; i < PineGameCenterController::_num_of_leaderboard; i++)
        {
            PineGameCenterController::_used[i] = false;
        }
        //NSLog(@"leaderboards %@",leaderboards) ;
        //int _num_of_leaderboard;
        //int _current_leaderboard_loaded;
        //char* _leader_boardname[LEADERBOARD_MAX_NUM];
        //INT_TIME _my_score[LEADERBOARD_MAX_NUM];
        
        for( GKLeaderboard* board in leaderboards )
        {
            // fetch score for minimum amt of data, b/c must call `loadScore..` to get MY score.
            board.playerScope = GKLeaderboardPlayerScopeFriendsOnly ;
            board.timeScope = GKLeaderboardTimeScopeAllTime ;
            
            NSRange range = NSMakeRange(1,1);
            board.range = range ;
            // NSLog(@"get leaderboard board %@",board) ;
            [board loadScoresWithCompletionHandler:^(NSArray *scores, NSError *error) {
                int index = PineGameCenterController::_current_leaderboard_loaded;
                PineGameCenterController::_current_leaderboard_loaded++;
                if( nsError != nil )
                {
                    NSLog(@"get leaderboard score %@",error) ;
                    //return ;
                }
                else
                {
                    //category
                    if(SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"7.0"))
                    {
                        sprintf(PineGameCenterController::_leader_boardid[index],"%s",[board.identifier UTF8String]);
                    }
                    else
                    {
                        sprintf(PineGameCenterController::_leader_boardid[index],"%s",[board.category UTF8String]);
                        
                    }
                    PineGameCenterController::_my_score[index] = board.localPlayerScore.value;
                    PineGameCenterController::_used[index] = true;
                }
                
                if(PineGameCenterController::_current_leaderboard_loaded >= PineGameCenterController::_num_of_leaderboard)
                {
                    social->SetState( PINESOCIAL_STATE_COMPLETED);
                }
                printf( "\nYOUR SCORE ON BOARD id:%s %s WAS %lld\n", PineGameCenterController::_leader_boardid[index],[board.title UTF8String], board.localPlayerScore.value ) ;
                
            }
             ] ;
        }
    }] ;
}

#endif

#pragma mark - Activity View Control Methods
-(void) iOS_VideoPostSocial:(NSString*) feed withVideoURL:(NSString*) url
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *URL = [documentsDirectory stringByAppendingPathComponent:url];
    
    NSString* someText = feed;
    NSURL *urlToShare = [NSURL fileURLWithPath:URL isDirectory:NO];
    NSArray* dataToShare = @[someText, urlToShare];
    
    UIActivityViewController* activityViewController =
    [[UIActivityViewController alloc] initWithActivityItems:dataToShare
                                      applicationActivities:nil];
    activityViewController.excludedActivityTypes = @[UIActivityTypePrint,UIActivityTypeCopyToPasteboard,UIActivityTypeAssignToContact];
    
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
         CGame::_is_portrail_exception = false;
         UIWindow* window = [[PineGameDelegate sharedInstance] window];
         [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
     }];
    CGame::_is_portrail_exception = true;
    if (NSClassFromString(@"UIPopoverPresentationController"))
    {
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
        activityViewController.popoverPresentationController.sourceView = [[PineGameDelegate sharedInstance] _opgenGLESViewController].view;
#else
        activityViewController.popoverPresentationController.sourceView = [self view];
#endif
    }
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
    [[[PineGameDelegate sharedInstance] _opgenGLESViewController] presentViewController:activityViewController animated:YES completion:
     ^{
         
     }];
#else
    
    [[[PineGameDelegate sharedInstance] window] addSubview:[self view]];
    
    [self presentViewController:activityViewController animated:YES completion:^{
        //[[[PineGameDelegate sharedInstance] window]bringSubviewToFront:[self view]];
    }];
    
#endif
    
}
- (void) iOS_BasicPostSocial:(NSString*) feed withImage:(UIImage*) image
{
    NSMutableArray *items = [NSMutableArray new];
   /* NSArray *excludedActivities = @[UIActivityTypePostToTwitter,
                                    UIActivityTypePostToWeibo,
                                    UIActivityTypePostToFlickr,
                                    UIActivityTypePostToVimeo,
                                    UIActivityTypePostToTencentWeibo];*/
    
    [items addObject:feed];
    if (image != nil)
    {
        [items addObject:image];
    }
    
    NSArray *activityItems = [NSArray arrayWithArray:items];
    UIActivityViewController *activityViewController = [[UIActivityViewController alloc] initWithActivityItems:activityItems applicationActivities:nil];
    //activityViewController.excludedActivityTypes = excludedActivities;
    [activityViewController setCompletionHandler:^(NSString *activityType, BOOL completed)
     {
         NSLog(@"Activity = %@",activityType);
         NSLog(@"Completed Status = %d",completed);
         
         if (completed)
         {
#if USE_ADBOX
             PineAdBox::showAdState = SHOW_AD_COMPLETED;
             if ([activityType isEqualToString:UIActivityTypePostToFacebook])
             {
                 PineAdBox::typePostIOSBasic = PineAdBox::enumTypePostIOSBasic::TYPE_POST_BASIC_FACEBOOK;
             }
             else if ([activityType isEqualToString:UIActivityTypePostToTwitter])
             {
                 PineAdBox::typePostIOSBasic = PineAdBox::enumTypePostIOSBasic::TYPE_POST_BASIC_TWITER;
             }
             else
             {
                 PineAdBox::showAdState = SHOW_AD_CANCEL;
             }
             OS_ClearTouchView();

            
#endif
             GAME()->_statusBasicPostSocial = CGame::enumStatusPostSocial::POST_SOCIAL_STATE_COMPLETE;

         }else
         {
              GAME()->_statusBasicPostSocial = CGame::enumStatusPostSocial::POST_SOCIAL_STATE_CANCEL;
#if USE_ADBOX
             PineAdBox::showAdState = SHOW_AD_CANCEL;
#endif
         }
         
         //[self removeFromParentViewController];
         CGame::_is_portrail_exception = false;
         UIWindow* window = [[PineGameDelegate sharedInstance] window];
         [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
     }];
    CGame::_is_portrail_exception = true;
    if (NSClassFromString(@"UIPopoverPresentationController"))
    {
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
        activityViewController.popoverPresentationController.sourceView = [[PineGameDelegate sharedInstance] _opgenGLESViewController].view;
#else
        activityViewController.popoverPresentationController.sourceView = [self view];
#endif
    }
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
    [[[PineGameDelegate sharedInstance] _opgenGLESViewController] presentViewController:activityViewController animated:YES completion:
     ^{
         
     }];
#else
    
    [[[PineGameDelegate sharedInstance] window] addSubview:[self view]];
    
    [self presentViewController:activityViewController animated:YES completion:^{
        //[[[PineGameDelegate sharedInstance] window]bringSubviewToFront:[self view]];
    }];
    
#endif
    
}



- (void)viewDidLoad {
    
    [super viewDidLoad];
    
}

-(void)viewWillAppear:(BOOL)animated
{
    printf("\n viewWillAppear ");
    [super viewWillAppear:animated];
}

- (void)didReceiveMemoryWarning {
    
    [super didReceiveMemoryWarning];
    
}

- (void)viewDidUnload {
    
}

- (void)dealloc {
    
    [super dealloc];
    
}

//web utils
-(void) iOS_ShowWebPage:(NSString*) url isoffline:(BOOL) offline
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        //Add some method process in global queue - normal for data processing
        
        dispatch_async(dispatch_get_main_queue(), ^(){
            //Add method, task you want perform on mainQueue
            //Control UIView, IBOutlet all here
            UIWebView *wv = [[UIWebView alloc]initWithFrame:self.view.bounds];
            NSString *str = @"https://datasea.blob.core.windows.net/testflight/index.html";
            NSURL * url = [NSURL URLWithString:str];
            

            //NSURL *url = [NSURL fileURLWithPath:[[NSBundle mainBundle] pathForResource:@"index" ofType:@"html"] isDirectory:NO];
            [wv loadRequest:[NSURLRequest requestWithURL:url]];
            
            [[self view] addSubview:wv];
#if USE_NEW_SYSTEM_VIEW_IOS_7_8
            [[[PineGameDelegate sharedInstance] _opgenGLESViewController] presentViewController:self animated:YES completion:^{
                
                
            }];
#else
            
            [[[PineGameDelegate sharedInstance] window] addSubview:[self view]];
            [[[PineGameDelegate sharedInstance] window]bringSubviewToFront:[self view]];
            
            [self presentViewController:gcViewController animated:YES completion:^{
                [[[PineGameDelegate sharedInstance] window]bringSubviewToFront:[self view]];
                [[self view] addSubview:gcViewController.view];
            }];
#endif

            
        });
        
        //Add some method process in global queue - normal for data processing
        
    });
   }


#if USE_PINEGAMEPLAYREPLAY
@synthesize _record_controller;

- (RPScreenRecorder*) sharedRecorder
{
    _record = [RPScreenRecorder sharedRecorder];
    _record.delegate = self;
    return _record;
}
-(void) shareRecordGamePlay
{
    if(Device::ReplayGamePlayState == RECORD_GAMEPLAY_ACCESS_DENINE_STATE)
    {
        return;
    }

    if((self._record_controller = self._record_controller))
    {
               // dispatch_async(dispatch_get_main_queue(), ^(){
        Device::ReplayGamePlayState = RECORD_GAMEPLAY_SHARING;
        
        self._record_controller.modalPresentationStyle = UIModalPresentationFullScreen;
        self._record_controller.previewControllerDelegate = self;
        self._record_controller.preferredContentSize = CGSizeMake(k_SCREEN_WIDTH, k_SCREEN_HEIGHT);
        
        if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad) {
            //[[[PineGameDelegate sharedInstance] window] addSubview:[self view]];
            self._record_controller.popoverPresentationController.sourceView = [self view];
            self._record_controller.popoverPresentationController.permittedArrowDirections = UIPopoverArrowDirectionAny;
            self._record_controller.popoverPresentationController.delegate = self;
            //self._record_controller.popoverPresentationController.
            [[[PineGameDelegate sharedInstance] _opgenGLESViewController] presentViewController:_record_controller animated:YES completion:^{
                [[UIApplication sharedApplication] setStatusBarHidden:YES animated:NO];
                
                
            }];
            
        }
        else
        {
            [[[PineGameDelegate sharedInstance] _opgenGLESViewController] presentViewController:_record_controller animated:YES completion:^{
                [[UIApplication sharedApplication] setStatusBarHidden:YES animated:NO];
            
                        
            }];
        }
      
    }
    
    
}
- (void) GamePlayRecordStart
{
    if(!(SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"9.0")))
    {
        Device::ReplayGamePlayState = RECORD_GAMEPLAY_ACCESS_DENINE_STATE;
        Device::IsChangingSetting = false;
        return;
    }
    
    [[self sharedRecorder] startRecordingWithMicrophoneEnabled:Device::CurrentReplayType handler:^(NSError *error) {
        
        if(error)
        {
            Device::ReplayGamePlayState = RECORD_GAMEPLAY_ACCESS_DENINE_STATE;
            Device::IsChangingSetting = false;
             //Device::CurrentReplayType = !(Device::CurrentReplayType);
        }
        else
        {
            Device::ReplayGamePlayState = RECORD_GAMEPLAY_RECORDING_STATE;
        }
        if(Device::IsChangingSetting)
        {
            Device::CurrentReplayType = (Device::CurrentReplayType);
            //Device::FrameReplayToStop = 60;
            [self GamePlayRecordStop];
        }
        CGame::_this->StopCurrentMusic();
        CGame::_this->PlayCurrentMusic(-1);
    }];
    
}
- (void) GamePlayRecordForceStop
{
    if(Device::ReplayGamePlayState == RECORD_GAMEPLAY_ACCESS_DENINE_STATE)
    {
        Device::IsChangingSetting = false;
        return;
    }
    [[self sharedRecorder] stopRecordingWithHandler:^(RPPreviewViewController * previewViewController ,
                                                      NSError * error) {
        
        if(error)
        {
            //[self ShowRecordAlert:error.localizedDescription];
            Device::ReplayGamePlayState = RECORD_GAMEPLAY_ACCESS_DENINE_STATE;
            Device::IsChangingSetting = false;
            
        }
        else
        {
            Device::ReplayGamePlayState = RECORD_GAMEPLAY_IDLE_STATE;
        }
        
    }];
    
}
- (void) GamePlayRecordStop
{
    if(Device::ReplayGamePlayState == RECORD_GAMEPLAY_ACCESS_DENINE_STATE)
    {
        Device::IsChangingSetting = false;
        return;
    }
    [[self sharedRecorder] stopRecordingWithHandler:^(RPPreviewViewController * previewViewController ,
                                                      NSError * error) {
        
        if(error)
        {
            //[self ShowRecordAlert:error.localizedDescription];
            Device::ReplayGamePlayState = RECORD_GAMEPLAY_ACCESS_DENINE_STATE;
            Device::IsChangingSetting = false;
            
        }
        else
        if((previewViewController = previewViewController))
        {
             Device::ReplayGamePlayState = RECORD_GAMEPLAY_WAIT_SHARE;
            
            //_record_controller = previewViewController;
            //previewViewController.modalPresentationStyle = UIModalPresentationFullScreen;
            //previewViewController.previewControllerDelegate = self;
            self._record_controller = previewViewController;
            if( Device::IsChangingSetting)
            {
                Device::IsChangingSetting = false;
                Device::CurrentReplaySetting = true;
                Device::ReplayGamePlayState = RECORD_GAMEPLAY_IDLE_STATE;
                //Device::SaveReplaySetting();
            }
            
            //NSData *tempArchiveView = [NSKeyedArchiver archivedDataWithRootObject:previewViewController];
            //_record_controller = [NSKeyedUnarchiver unarchiveObjectWithData:tempArchiveView];
            //[self shareRecordGamePlay];
          
            //while(Device::ReplayGamePlayState == RECORD_GAMEPLAY_WAIT_SHARE)
            //{
                //sleep(1);
            //}
            /*
            if(Device::ReplayGamePlayState == RECORD_GAMEPLAY_SHARING)
            {
                [self shareRecordGamePlay];
            }
             */
        }
        
    }];
    
}
- (void)screenRecorder:(RPScreenRecorder *)screenRecorder
didStopRecordingWithError:(NSError *)error
 previewViewController:(RPPreviewViewController *)previewViewController
{
    //Device::ReplayGamePlayState = RECORD_GAMEPLAY_ACCESS_DENINE_STATE;
    NSLog(@"\n error %@ \n", error);
    NSLog(@"\n previewViewController %@ \n", previewViewController);
    
}

- (void)screenRecorderDidChangeAvailability:(RPScreenRecorder *)screenRecorder
{
    
}

- (void)previewController:(RPPreviewViewController *)previewController
didFinishWithActivityTypes:(NSSet<NSString *> *)activityTypes
{
    NSLog(@"\n activityTypes %@ \n", activityTypes);
    NSLog(@"Activity = %@",activityTypes);
    //NSLog(@"Completed Status = %d",completed);
    
        //NSString *activityType = [activityTypes objectAtIndex:0];
        //PineAdBox::showAdState = SHOW_AD_COMPLETED;
    
        if (activityTypes != nil && [activityTypes containsObject:UIActivityTypePostToFacebook])
        {
             Device::ReplayGamePlayState = RECORD_GAMEPLAY_SHARE_SUCESSFULL;
            PineAdBox::typePostIOSBasic = PineAdBox::enumTypePostIOSBasic::TYPE_POST_BASIC_FACEBOOK;
            dispatch_async(dispatch_get_main_queue(), ^(){
                [[UIApplication sharedApplication] setStatusBarHidden:YES animated:NO];
                UIWindow* window = [[PineGameDelegate sharedInstance] window];
                [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
                OS_ClearTouchView();
                //[previewController removeFromSuperview];
                [previewController dismissViewControllerAnimated:YES completion:nil];
                NSLog(@"\n previewController %@ \n", previewController);
            });
            
        }
        else if (activityTypes != nil && [activityTypes containsObject:UIActivityTypePostToTwitter])
        {
             Device::ReplayGamePlayState = RECORD_GAMEPLAY_SHARE_SUCESSFULL;
            PineAdBox::typePostIOSBasic = PineAdBox::enumTypePostIOSBasic::TYPE_POST_BASIC_TWITER;
            dispatch_async(dispatch_get_main_queue(), ^(){
                [[UIApplication sharedApplication] setStatusBarHidden:YES animated:NO];
                UIWindow* window = [[PineGameDelegate sharedInstance] window];
                [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
                OS_ClearTouchView();
                //[previewController removeFromSuperview];
                [previewController dismissViewControllerAnimated:YES completion:nil];
                NSLog(@"\n previewController %@ \n", previewController);
            });
        }
        else
        {
            //PineAdBox::showAdState = SHOW_AD_CANCEL;
             Device::ReplayGamePlayState = RECORD_GAMEPLAY_SHARE_DISSMISS;
        }
    
   
    //
    //[[UIApplication sharedApplication] setStatusBarHidden:YES animated:NO];
    //UIWindow* window = [[PineGameDelegate sharedInstance] window];
    //[window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
    //OS_ClearTouchView();
    //[previewController dismissViewControllerAnimated:YES completion:nil];
   

}

- (void)previewControllerDidFinish:(RPPreviewViewController *)previewController
{
    if(Device::ReplayGamePlayState != RECORD_GAMEPLAY_SHARE_SUCESSFULL)
    {
        Device::ReplayGamePlayState = RECORD_GAMEPLAY_SHARE_DISSMISS;
    }
    dispatch_async(dispatch_get_main_queue(), ^(){
        [[UIApplication sharedApplication] setStatusBarHidden:YES animated:NO];
        UIWindow* window = [[PineGameDelegate sharedInstance] window];
        [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
        OS_ClearTouchView();
        //[previewController removeFromSuperview];
        [previewController dismissViewControllerAnimated:YES completion:nil];
        NSLog(@"\n previewController %@ \n", previewController);
    });
    //[self r:previewViewController.view];
    /*
    dispatch_async(dispatch_get_main_queue(), ^(){
    [[UIApplication sharedApplication] setStatusBarHidden:YES animated:NO];
    UIWindow* window = [[PineGameDelegate sharedInstance] window];
    [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
     OS_ClearTouchView();
    //[previewController removeFromSuperview];
    [previewController dismissViewControllerAnimated:YES completion:nil];
    NSLog(@"\n previewController %@ \n", previewController);
    });
     */
}
- (UIModalPresentationStyle)adaptivePresentationStyleForPresentationController:(UIPresentationController *)controller;{
    return UIModalPresentationNone;
}
- (void)popoverPresentationControllerDidDismissPopover:(UIPopoverPresentationController *)popoverPresentationController
{
    if(Device::ReplayGamePlayState != RECORD_GAMEPLAY_SHARE_SUCESSFULL)
    {
        Device::ReplayGamePlayState = RECORD_GAMEPLAY_SHARE_DISSMISS;
    }
    dispatch_async(dispatch_get_main_queue(), ^(){
        if((self._record_controller = self._record_controller))
        {
            [[UIApplication sharedApplication] setStatusBarHidden:YES animated:NO];
            UIWindow* window = [[PineGameDelegate sharedInstance] window];
            [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
            OS_ClearTouchView();
            //[previewController removeFromSuperview];
            [self._record_controller dismissViewControllerAnimated:YES completion:nil];
        }
    });

    
}

/* Called on the delegate when the user has taken action to dismiss the popover. This is not called when -dismissPopoverAnimated: is called directly.
 */
- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController
{
    if(Device::ReplayGamePlayState != RECORD_GAMEPLAY_SHARE_SUCESSFULL)
    {
        Device::ReplayGamePlayState = RECORD_GAMEPLAY_SHARE_DISSMISS;
    }
    dispatch_async(dispatch_get_main_queue(), ^(){
        if((self._record_controller = self._record_controller))
        {
            [[UIApplication sharedApplication] setStatusBarHidden:YES animated:NO];
            UIWindow* window = [[PineGameDelegate sharedInstance] window];
            [window bringSubviewToFront:(UIView*)[[PineGameDelegate sharedInstance] glView]];
            OS_ClearTouchView();
        //[previewController removeFromSuperview];
            [self._record_controller dismissViewControllerAnimated:YES completion:nil];
            NSLog(@"\n popoverController %@ \n", popoverController);
        }
    });
    
}

#endif

#if USE_AUDIO_RECORDER
NSTimer *levelTimer;
double lowPassResults  = 0;
extern "C" const char* GetDocumentUrl();
-(void)readyToBlow {
    NSURL *url = [NSURL fileURLWithPath:@"/dev/null"];
    //NSURL *url = [NSURL fileURLWithPath:[NSString stringWithFormat:@"%s%@", GetDocumentUrl(), @"/null"]];

    
    NSDictionary *settings = [NSDictionary dictionaryWithObjectsAndKeys:
                              [NSNumber numberWithFloat: 44100.0],                 AVSampleRateKey,
                              [NSNumber numberWithInt: kAudioFormatAppleLossless], AVFormatIDKey,
                              [NSNumber numberWithInt: 1],                         AVNumberOfChannelsKey,
                              [NSNumber numberWithInt: AVAudioQualityMax],         AVEncoderAudioQualityKey,
                              nil];
    
    NSError *error;
    
    AVRecorder = [[AVAudioRecorder alloc] initWithURL:url settings:settings error:&error];
    
    if (AVRecorder) {
        [AVRecorder prepareToRecord];
        [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayAndRecord error:nil];
        [[AVAudioSession sharedInstance] setActive:YES error:nil];
        AVRecorder.meteringEnabled = YES;
        [AVRecorder record];
        levelTimer = [NSTimer scheduledTimerWithTimeInterval: 0.03 target: self selector: @selector(levelTimerCallbackBlow:) userInfo: nil repeats: YES];
        Device::AudioRecordState = AUDIO_RECORD_AUTHENTICATION_PROCESSING;
    } else
    {
        //NSLog([error description]);
        Device::AudioRecordState = AUDIO_RECORD_AUTHENTICATION_FAIL;
    }
    
}

-(void)levelTimerCallbackBlow:(NSTimer *)timer {
    [AVRecorder updateMeters];
    
    const double ALPHA = 0.05;
    double peakPowerForChannel = pow(10, (0.05 * [AVRecorder peakPowerForChannel:0]));
    lowPassResults = ALPHA * peakPowerForChannel + (1.0 - ALPHA) * lowPassResults;
    //NSLog(@"lowpassResult is %f peakPowerForChannel = %f",lowPassResults,peakPowerForChannel);
    //if (lowPassResults > 0.95){
        //lowPassResults = 0.0f;
        //NSLog(@"Mic blow detected");
        //[levelTimer invalidate];
    //}
}
-(void) SetMicroAudioPass:(double)v
{
    lowPassResults = v;
}
-(double) GetMicroAudioPass
{
    return lowPassResults;
}
-(void) StopToBlow
{
    [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryAmbient error:nil];
    
    [[AVAudioSession sharedInstance] setActive:YES error:nil];
    [AVRecorder stop];
    [levelTimer invalidate];
    [levelTimer release];
    [AVRecorder release];
}
#endif



@end
#if ADD_ADMOB_BANNER_ADS
extern "C" void OS_ShowBannerInit()
{
    dispatch_async(dispatch_get_main_queue(), ^(){
        [[[PineGameDelegate sharedInstance] glView] adViewInit];
    });

    
}
extern "C" void OS_ShowBannerAds()
{
    //[self addSubview:bannerView];
    dispatch_async(dispatch_get_main_queue(), ^(){
    [[[PineGameDelegate sharedInstance] glView] adViewShow];
    });
}
extern "C" void OS_ShowBannerDissmiss()
{
    dispatch_async(dispatch_get_main_queue(), ^(){

    [[[PineGameDelegate sharedInstance] glView] adViewDiss];
    });
}
#endif
extern "C" void OS_ShowWebPage(const char* url, bool isOffline)
{
    [[PineGameDelegate sharedInstance] iOS_ShowWebPage:@"test" isoffline:YES];
}


extern "C" BOOL isGameCenterAPIAvailable()
{
    // Check for presence of GKLocalPlayer API.
    Class gcClass = (NSClassFromString(@"GKLocalPlayer"));
    
    // The device must be running running iOS 4.1 or later.
    NSString *reqSysVer = @"4.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    BOOL osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
    
    return (gcClass && osVersionSupported);
}

extern "C" BOOL OS_GC_HasOpenning()
{
    return  _is_openning_gc_view;
}

#if ADD_GOOGLE_ANALYTICS
#if ADD_REMOTE_CONFIG
FIRRemoteConfig* remoteAppConfig;
static int RemoteConfigState = -1;
extern "C" const char* OS_RemoteConfigGetString(const char* strID, const char* default_value)
{
    if(RemoteConfigState == 1)
    {
        NSString *sTransactionID=[NSString stringWithFormat:@"%s",strID];
        FIRRemoteConfig *config = [FIRRemoteConfig remoteConfig];
        NSString *value = config[sTransactionID].stringValue;
        if([value length] <= 0)
        {
            return default_value;
        }
        return [value UTF8String];
    }
    return default_value;
}

extern "C" const long OS_RemoteConfigGetInteger(const char*  strID,long default_value)
{
    if(RemoteConfigState == 1)
    {
        NSString *sTransactionID=[NSString stringWithFormat:@"%s",strID];
        FIRRemoteConfig *config = [FIRRemoteConfig remoteConfig];
        
        NSString *value = config[sTransactionID].stringValue;
        if([value length] <= 0)
        {
            return default_value;
        }
        return atol([value UTF8String]);
    }
    return default_value;
}
extern "C" const bool OS_RemoteConfigGetBoolean(const char*  strID,bool default_value)
{
    if(RemoteConfigState == 1)
    {
        NSString *sTransactionID=[NSString stringWithFormat:@"%s",strID];
        FIRRemoteConfig *config = [FIRRemoteConfig remoteConfig];
        NSString *value = config[sTransactionID].stringValue;
        if([value length] <= 0)
        {
            return default_value;
        }
        return atol([value UTF8String]) > 0;
    }
    return default_value;
}

#endif
#ifndef APP_CONFIG_FETCHED
#define APP_CONFIG_FETCHED() {}
#endif
#if ADD_REMOTE_CONFIG
extern "C" void OS_InitRemoteConfig()
{
    remoteAppConfig = [FIRRemoteConfig remoteConfig];
    FIRRemoteConfigSettings *remoteConfigSettings = [[FIRRemoteConfigSettings alloc] initWithDeveloperModeEnabled:YES];
    remoteAppConfig.configSettings = remoteConfigSettings;
    
    long expirationDuration = 3600;
    [remoteAppConfig fetchWithExpirationDuration:expirationDuration completionHandler:^(FIRRemoteConfigFetchStatus status, NSError *error) {
        if (status == FIRRemoteConfigFetchStatusSuccess) {
            NSLog(@"Config fetched!");
            [remoteAppConfig activateFetched];
            RemoteConfigState = 1;
            APP_CONFIG_FETCHED();
            //printf("\nconfig: %s",OS_RemoteConfigGetString("century_city_moregames_url"));
        } else {
            RemoteConfigState = 0;
            NSLog(@"Config not fetched");
            NSLog(@"Error %@", error.localizedDescription);
        }
    }];
}
#endif
extern "C" void OS_InitGoogleAnalytics()
{
    [FIRApp configure];
#if ADD_REMOTE_CONFIG
    OS_InitRemoteConfig();
#endif
}
//kFIREventPurchaseRefund Log Purchase
extern "C" void OS_GoogleAnalyticsLogPurchase(const char *TransactionID,double price)
{
    /// <ul>
    ///     <li>{@link kFIRParameterCurrency} (NSString) (optional)</li>
    ///     <li>{@link kFIRParameterValue} (double as NSNumber) (optional)</li>
    ///     <li>{@link kFIRParameterTransactionID} (NSString) (optional)</li>
    /// </ul>

    NSString *sTransactionID=[NSString stringWithFormat:@"%s",TransactionID];
    [FIRAnalytics logEventWithName:kFIREventPurchaseRefund
                        parameters:@{
                                     kFIRParameterTransactionID:[NSString stringWithFormat:@"id-%@", sTransactionID],
                                     kFIRParameterValue:[NSNumber numberWithDouble:price]
                                     }];
    
}

//kFIREventSpendVirtualCurrency
extern "C" void OS_GoogleAnalyticsLogSpendVirtualCurrency(const char *CurrencyName,const char* ItemName,double price)
{
    NSString *sCurrencyName=[NSString stringWithFormat:@"%s",CurrencyName];
    NSString *sItemName=[NSString stringWithFormat:@"%s",ItemName];
    [FIRAnalytics logEventWithName:kFIREventSpendVirtualCurrency
                        parameters:@{
                                     kFIRParameterVirtualCurrencyName:[NSString stringWithFormat:@"%@", sCurrencyName],
                                     kFIRParameterItemName:sItemName,
                                     kFIRParameterValue:[NSNumber numberWithDouble:price]
                                     }];
    
}

//kFIREventUnlockAchievement
extern "C" void OS_GoogleAnalyticsLogAchievements(const char *achiID,const char* achiName)
{
    /*
     <li>{@link kFIRParameterItemID} (NSString)</li>
     ///     <li>{@link kFIRParameterItemName} (NSString)</li>
     ///     <li>{@link kFIRParameterItemCategory} (NSString)</li>
     */
    NSString *sachiID=[NSString stringWithFormat:@"%s",achiID];
    NSString *sachiName=[NSString stringWithFormat:@"%s",achiName];

    [FIRAnalytics logEventWithName:kFIREventUnlockAchievement
                        parameters:@{
                                     kFIRParameterItemID:[NSString stringWithFormat:@"id-%@", sachiID],
                                     kFIRParameterItemName:sachiName,
                                     kFIRParameterItemCategory:@""
                                     }];

}
extern "C" void OS_GoogleAnalyticsLogContents(const char *title,const char* decs)
{
    NSString *stitle=[NSString stringWithFormat:@"%s",title];
    NSString *sdecs=[NSString stringWithFormat:@"%s",decs];
[FIRAnalytics logEventWithName:kFIREventSelectContent
                    parameters:@{
                                 kFIRParameterItemID:[NSString stringWithFormat:@"id-%@", stitle],
                                 kFIRParameterItemName:stitle,
                                 @"full_text":sdecs
                                 }];
}

/// <ul>
///     <li>{@link kFIRParameterLevel} (signed 64-bit integer as NSNumber)</li>
///     <li>{@link kFIRParameterCharacter} (NSString) (optional)</li>
/// </ul>
//static NSString *const kFIREventLevelUp = @"level_up";
extern "C" void OS_GoogleAnalyticsSetUserLevelProperty(double level)
{
     [FIRAnalytics setUserPropertyString:[NSString stringWithFormat:@"%f",level] forName:@"current_level"];
}
extern "C" void OS_GoogleAnalyticsLogLevelUp(double level)
{
    //[FIRAnalytics setUserPropertyString:[NSString stringWithFormat:@"%f",level] forName:@"current_level"];
    [FIRAnalytics logEventWithName:kFIREventLevelUp
                        parameters:@{
                                     kFIRParameterLevel:[NSNumber numberWithInt:(int)level]
                                     }];
}
#endif

extern "C" void iOS_ShowRatingBasic()
{

     if((SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"10.0")))
     {
          [SKStoreReviewController requestReview];
     }
     else
     {
         GoToReviewInItune();
     }
   
}
