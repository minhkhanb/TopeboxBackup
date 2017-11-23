//
//  PineChartBoost.h
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 5/6/14.
//
//
extern "C" void DownloadFile(const char* url, const char* file_name,int time_out);
extern "C" int GetSizeFile(const char *file_name);
extern "C" int ReadAppDataBinary(const char* filename, BYTE* readed_data);

#ifndef PineEntertainment_PineChartBoost_h
#define PineEntertainment_PineChartBoost_h
extern "C" void OS_ClearTouchView();
-(bool) chartboostAvailable
{
    return _chartboost_app_id != nil && _chartboost_app_syn != nil;
}
-(void) chartboostDownloadFile
{
    char filename[1024];
#ifdef MAC_OS
    sprintf(filename,"http://datancus.blob.core.windows.net/pineadvert/%d/chartboost.config",ADVERT_APPLE_ID);
    DownloadFile(filename,CHARTBOOST_CONFIG_FILE,30);
    printf("\nDownload chartboost file");
#endif
}

-(void) chartboostDidBecameActive:(UIApplication*)application
{
    _chartboost_app_id = nil;
    _chartboost_app_syn = nil;
    //load chartboost config.
    int length = GetSizeFile(CHARTBOOST_CONFIG_FILE);
    char chartboost_ids[512];
    char chartboost_syn[512];
    if(length > 10)
    {
        BYTE *infomation = new BYTE[length+1];
        ReadAppDataBinary(CHARTBOOST_CONFIG_FILE,infomation);
        infomation[length]='\0';
        //printf("\ninfomation = %s",infomation);
        int ch = 0;
        int step = 0;
        for(int i = 0; i < length; i++)
        {
            if(infomation[i] != '#')
            {
                if(step == 0)
                {
                    chartboost_ids[ch]=infomation[i];
                }
                else if(step == 1)
                {
                    chartboost_syn[ch]=infomation[i];
                }
                ch++;
            }
            else
            {
                if(step == 0)
                {
                    chartboost_ids[ch] = 0;
                }
                else if(step == 1)
                {
                    chartboost_syn[ch] = 0;
                }
                step++;
                ch = 0;
            }
        }
        //
        if(step >= 2)
        {
            _chartboost_app_id = [NSString stringWithFormat:@"%s",chartboost_ids];
            _chartboost_app_syn = [NSString stringWithFormat:@"%s",chartboost_syn];
        }
        delete[] infomation;
    }
    if([self chartboostAvailable])
    {
        [Chartboost startWithAppId:_chartboost_app_id appSignature:_chartboost_app_syn delegate:self];
        //Chartboost *cb = [Chartboost sharedChartboost];
        
        //cb.appId = _chartboost_app_id;
        //cb.appSignature = _chartboost_app_syn;
        
        // Required for use of delegate methods. See "Advanced Topics" section below.
        //cb.delegate = self;
        
        // Begin a user session. Must not be dependent on user actions or any prior network requests.
        // Must be called every time your app becomes active.
        //[cb startSession];
        //[cb cacheInterstitial];
        
        // Cache an interstitial at some named locations -- (Pro Tip: do this!)
        //[cb cacheInterstitial:@"Main Menu"];
        //[cb cacheInterstitial:@"Pause screen"];
        // Show an interstitial
        //[cb showInterstitial:@"Main Menu"];
        
        // Cache the more apps page so it's loaded & ready
        
        
        // Pro Tip: Use code below to Print IFA (Identifier for Advertising) in Output section. iOS 6+ devices only.
        //NSString* ifa = [[[NSClassFromString(@"ASIdentifierManager") sharedManager] advertisingIdentifier] UUIDString];
        //ifa = [[ifa stringByReplacingOccurrencesOfString:@"-" withString:@""] lowercaseString];
        //NSLog(@"IFA: %@",ifa);
        [Chartboost  cacheMoreApps:@"More App"];
        [Chartboost cacheInterstitial:@"Main Menu"];
        [Chartboost cacheRewardedVideo:@"Reward Video"];
        
        //[[Chartboost sharedChartboost] showInterstitial:CBLocationMainMenu];
        //[cb showMoreApps];
    }
}

-(void) cacheVideoReward:(NSString*) location
{
    if([self chartboostAvailable])
    {
        [Chartboost cacheRewardedVideo:location];
    }
}
-(bool) hasAdsInterstitial:(NSString*) location
{
    if([self chartboostAvailable])
    {
        return [Chartboost hasInterstitial:location];
    }
    return false;
}
-(bool) hasVideoRewards:(NSString*) location
{
    if([self chartboostAvailable])
    {
         return [Chartboost hasRewardedVideo:location];
        
    }
    return false;
}
extern "C" bool _music_is_playing;
-(void) showVideoRewards:(NSString*) location
{
    if([self chartboostAvailable])
    {
        if([self hasVideoRewards:location])
        {
            if(CGame::_this->IsCurrentMusicPlaying())
            {
                _music_is_playing = true;
                CGame::_this->StopCurrentMusic();
            }
            [Chartboost showRewardedVideo:location];
            [Chartboost cacheRewardedVideo:location];
        }
        else
        {
            [self cacheVideoReward:location];
        }
    }
}
/*!
 @abstract
 */
- (void)didCloseRewardedVideo:(CBLocation)location
{
    if(CGame::_this->IsCurrentMusicPlaying())
    {
        _music_is_playing = true;
        CGame::_this->StopCurrentMusic();
    }
    PineAdBox::showAdState = SHOW_AD_COMPLETED;
    OS_ClearTouchView();
}
- (void)didClickRewardedVideo:(CBLocation)location
{
    if(CGame::_this->IsCurrentMusicPlaying())
    {
        _music_is_playing = true;
        CGame::_this->StopCurrentMusic();
    }
    PineAdBox::showAdState = SHOW_AD_COMPLETED;
      OS_ClearTouchView();
}
- (void)didCompleteRewardedVideo:(CBLocation)location withReward:(int)reward
{
    if(CGame::_this->IsCurrentMusicPlaying())
    {
        _music_is_playing = true;
        CGame::_this->StopCurrentMusic();
    }
    PineAdBox::showAdState = SHOW_AD_COMPLETED;
      OS_ClearTouchView();
}
-(bool) chartboostHasMoreGame
{
    return [self chartboostAvailable] && [Chartboost hasMoreApps:@"More App"];
}
-(void) chartboostMoreGame
{
    if([self chartboostAvailable])
    {
        //Chartboost *cb = [Chartboost sharedChartboost];
        // To quickly display the more apps page, you may cache it similar to interstitials above
        //[cb cacheMoreApps];
        
        // Then show the more apps page in the same way
         if([Chartboost hasMoreApps:@"More App"])
         {
             [Chartboost showMoreApps:nil location:@"More App"];
         }
        
       
    }
}
-(void) chartboostShowAtMenu:(NSString*)title
{
    if([self chartboostAvailable])
    {
        //Chartboost *cb = [Chartboost sharedChartboost];
        if(title == NULL)
        {
            //[cb cacheInterstitial];
            [Chartboost showInterstitial:CBLocationMainMenu];
        }
        else
        {
            //[cb cacheInterstitial:title];
            [Chartboost showInterstitial:title];
        }
    }
    
}

/*
 * Chartboost Delegate Methods
 *
 */


/*
 * shouldDisplayInterstitial
 *
 * This is used to control when an interstitial should or should not be displayed
 * The default is YES, and that will let an interstitial display as normal
 * If it's not okay to display an interstitial, return NO
 *
 * For example: during gameplay, return NO.
 *
 * Is fired on:
 * -Interstitial is loaded & ready to display
 */

- (BOOL)shouldDisplayInterstitial:(NSString *)location {
    NSLog(@"about to display interstitial at location %@", location);
    
    // For example:
    // if the user has left the main menu and is currently playing your game, return NO;
    
    // Otherwise return YES to display the interstitial
#if USE_ADBOX
    return PineAdBox::shouldAppPopup;
#else
    return YES;
#endif
}


/*
 * didFailToLoadInterstitial
 *
 * This is called when an interstitial has failed to load. The error enum specifies
 * the reason of the failure
 */

- (void)didFailToLoadInterstitial:(NSString *)location withError:(CBLoadError)error {
    switch(error){
        case CBLoadErrorInternetUnavailable: {
            NSLog(@"Failed to load Interstitial, no Internet connection !");
        } break;
        case CBLoadErrorInternal: {
            NSLog(@"Failed to load Interstitial, internal error !");
        } break;
        case CBLoadErrorNetworkFailure: {
            NSLog(@"Failed to load Interstitial, network error !");
        } break;
        case CBLoadErrorWrongOrientation: {
            NSLog(@"Failed to load Interstitial, wrong orientation !");
        } break;
        case CBLoadErrorTooManyConnections: {
            NSLog(@"Failed to load Interstitial, too many connections !");
        } break;
        case CBLoadErrorFirstSessionInterstitialsDisabled: {
            NSLog(@"Failed to load Interstitial, first session !");
        } break;
        case CBLoadErrorNoAdFound : {
            NSLog(@"Failed to load Interstitial, no ad found !");
        } break;
        case CBLoadErrorSessionNotStarted : {
            NSLog(@"Failed to load Interstitial, session not started !");
        } break;
        case CBLoadErrorNoLocationFound : {
            NSLog(@"Failed to load Interstitial, missing location parameter !");
        } break;
        default: {
            NSLog(@"Failed to load Interstitial, unknown error !");
        }
    }
}

/*
 * didCacheInterstitial
 *
 * Passes in the location name that has successfully been cached.
 *
 * Is fired on:
 * - All assets loaded
 * - Triggered by cacheInterstitial
 *
 * Notes:
 * - Similar to this is: (BOOL)hasCachedInterstitial:(NSString *)location;
 * Which will return true if a cached interstitial exists for that location
 */

- (void)didCacheInterstitial:(NSString *)location {
    NSLog(@"interstitial cached at location %@", location);
}

/*
 * didFailToLoadMoreApps
 *
 * This is called when the more apps page has failed to load for any reason
 *
 * Is fired on:
 * - No network connection
 * - No more apps page has been created (add a more apps page in the dashboard)
 * - No publishing campaign matches for that user (add more campaigns to your more apps page)
 *  -Find this inside the App > Edit page in the Chartboost dashboard
 */

- (void)didFailToLoadMoreApps:(CBLoadError)error {
    switch(error){
        case CBLoadErrorInternetUnavailable: {
            NSLog(@"Failed to load More Apps, no Internet connection !");
        } break;
        case CBLoadErrorInternal: {
            NSLog(@"Failed to load More Apps, internal error !");
        } break;
        case CBLoadErrorNetworkFailure: {
            NSLog(@"Failed to load More Apps, network error !");
        } break;
        case CBLoadErrorWrongOrientation: {
            NSLog(@"Failed to load More Apps, wrong orientation !");
        } break;
        case CBLoadErrorTooManyConnections: {
            NSLog(@"Failed to load More Apps, too many connections !");
        } break;
        case CBLoadErrorFirstSessionInterstitialsDisabled: {
            NSLog(@"Failed to load More Apps, first session !");
        } break;
        case CBLoadErrorNoAdFound: {
            NSLog(@"Failed to load More Apps, Apps not found !");
        } break;
        case CBLoadErrorSessionNotStarted : {
            NSLog(@"Failed to load More Apps, session not started !");
        } break;
        default: {
            NSLog(@"Failed to load More Apps, unknown error !");
        }
    }
}


/// Called when the user dismisses the interstitial
/// If you are displaying the add yourself, dismiss it now.
- (void)didDismissInterstitial:(CBLocation)location
{
    
}

/*!
 @abstract
 Called after an interstitial has been closed.
 
 @param location The location for the Chartboost impression type.
 
 @discussion Implement to be notified of when an interstitial has been closed for a given CBLocation.
 "Closed" is defined as clicking the close interface for the interstitial.
 */
- (void)didCloseInterstitial:(CBLocation)location
{
     PineAdBox::showAdState = SHOW_AD_COMPLETED;
      OS_ClearTouchView();
}

/*!
 @abstract
 Called after an interstitial has been clicked.
 
 @param location The location for the Chartboost impression type.
 
 @discussion Implement to be notified of when an interstitial has been click for a given CBLocation.
 "Clicked" is defined as clicking the creative interface for the interstitial.
 */
- (void)didClickInterstitial:(CBLocation)location
{
     PineAdBox::showAdState = SHOW_AD_COMPLETED;
      OS_ClearTouchView();
}

/*!
 @abstract
 Called after an "more applications" has been dismissed.
 
 @param location The location for the Chartboost impression type.
 
 @discussion Implement to be notified of when an "more applications" has been dismissed for a given CBLocation.
 "Dismissal" is defined as any action that removed the "more applications" UI such as a click or close.
 */
- (void)didDismissMoreApps:(CBLocation)location
{
     PineAdBox::showAdState = SHOW_AD_COMPLETED;
      OS_ClearTouchView();
}

/*!
 @abstract
 Called after an "more applications" has been closed.
 
 @param location The location for the Chartboost impression type.
 
 @discussion Implement to be notified of when an "more applications" has been closed for a given CBLocation.
 "Closed" is defined as clicking the close interface for the "more applications".
 */
- (void)didCloseMoreApps:(CBLocation)location
{
     PineAdBox::showAdState = SHOW_AD_COMPLETED;
      OS_ClearTouchView();
}

/// Called when the More Apps page has been received and cached
- (void)didCacheMoreApps
{
    printf("\nMore Ads Cache!!!!!!!");
}
/*!
 @abstract
 Called after an "more applications" has been clicked.
 
 @param location The location for the Chartboost impression type.
 
 @discussion Implement to be notified of when an "more applications" has been clicked for a given CBLocation.
 "Clicked" is defined as clicking the creative interface for the "more applications".
 */
- (void)didClickMoreApps:(CBLocation)location
{
    
}

/*!
/*
 * didFailToLoadRewardedVideo
 *
 * This is called when a Rewarded Video has failed to load. The error enum specifies
 * the reason of the failure
 */

- (void)didFailToLoadRewardedVideo:(NSString *)location withError:(CBLoadError)error {
    switch(error){
        case CBLoadErrorInternetUnavailable: {
            NSLog(@"Failed to load Rewarded Video, no Internet connection !");
        } break;
        case CBLoadErrorInternal: {
            NSLog(@"Failed to load Rewarded Video, internal error !");
        } break;
        case CBLoadErrorNetworkFailure: {
            NSLog(@"Failed to load Rewarded Video, network error !");
        } break;
        case CBLoadErrorWrongOrientation: {
            NSLog(@"Failed to load Rewarded Video, wrong orientation !");
        } break;
        case CBLoadErrorTooManyConnections: {
            NSLog(@"Failed to load Rewarded Video, too many connections !");
        } break;
        case CBLoadErrorFirstSessionInterstitialsDisabled: {
            NSLog(@"Failed to load Rewarded Video, first session !");
        } break;
        case CBLoadErrorNoAdFound : {
            NSLog(@"Failed to load Rewarded Video, no ad found !");
        } break;
        case CBLoadErrorSessionNotStarted : {
            NSLog(@"Failed to load Rewarded Video, session not started !");
        } break;
        case CBLoadErrorNoLocationFound : {
            NSLog(@"Failed to load Rewarded Video, missing location parameter !");
        } break;
        default: {
            NSLog(@"Failed to load Rewarded Video, unknown error !");
        }
    }
}
/*
 * didDisplayInterstitial
 *
 * Called after an interstitial has been displayed on the screen.
 */

- (void)didDisplayInterstitial:(CBLocation)location {
    NSLog(@"Did display interstitial");
    
    // We might want to pause our in-game audio, lets double check that an ad is visible
    if ([Chartboost isAnyViewVisible]) {
        // Use this function anywhere in your logic where you need to know if an ad is visible or not.
        //NSLog(@"Pause audio");
        CGame::_this->StopCurrentMusic();
    }
}


/*!
 @abstract
 Called after an InPlay object has been loaded from the Chartboost API
 servers and cached locally.
 
 @param location The location for the Chartboost impression type.
 
 @discussion Implement to be notified of when an InPlay object has been loaded from the Chartboost API
 servers and cached locally for a given CBLocation.
 */
- (void)didCacheInPlay:(CBLocation)location {
    NSLog(@"Successfully cached inPlay");
    //ViewController *vc = (ViewController*)self.window.rootViewController;
    //[vc renderInPlay:[Chartboost getInPlay:location]];
}

/*!
 @abstract
 Called after a InPlay has attempted to load from the Chartboost API
 servers but failed.
 
 @param location The location for the Chartboost impression type.
 
 @param error The reason for the error defined via a CBLoadError.
 
 @discussion Implement to be notified of when an InPlay has attempted to load from the Chartboost API
 servers but failed for a given CBLocation.
 */
- (void)didFailToLoadInPlay:(CBLocation)location
withError:(CBLoadError)error {
    
    NSString *errorString = @"";
    switch(error){
        case CBLoadErrorInternetUnavailable: {
            errorString = @"Failed to load In Play, no Internet connection !";
        } break;
        case CBLoadErrorInternal: {
            errorString = @"Failed to load In Play, internal error !";
        } break;
        case CBLoadErrorNetworkFailure: {
            errorString = @"Failed to load In Play, network errorString !";
        } break;
        case CBLoadErrorWrongOrientation: {
            errorString = @"Failed to load In Play, wrong orientation !";
        } break;
        case CBLoadErrorTooManyConnections: {
            errorString = @"Failed to load In Play, too many connections !";
        } break;
        case CBLoadErrorFirstSessionInterstitialsDisabled: {
            errorString = @"Failed to load In Play, first session !";
        } break;
        case CBLoadErrorNoAdFound : {
            errorString = @"Failed to load In Play, no ad found !";
        } break;
        case CBLoadErrorSessionNotStarted : {
            errorString = @"Failed to load In Play, session not started !";
        } break;
        case CBLoadErrorNoLocationFound : {
            errorString = @"Failed to load In Play, missing location parameter !";
        } break;
        default: {
            errorString = @"Failed to load In Play, unknown error !";
        }
    }
    
   // NSLog(errorString);
    
    ///ViewController *vc = (ViewController*)self.window.rootViewController;
    //[vc renderInPlayError:errorString];
}


- (BOOL)shouldRequestInterstitialsInFirstSession {
    return YES;
}

extern "C" bool chartboostHasMoreGame()
{
    return [[PineGameDelegate sharedInstance] chartboostHasMoreGame];
    
}
extern "C" void charboostMoreGame()
{
    //[[PineGameDelegate sharedInstance] chartboostMoreGame];
    [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(chartboostMoreGame) withObject:nil waitUntilDone:YES];
}
extern "C" bool chartboost_hasAdsInterstitial(const char* loc)
{
    NSString* stitle = [NSString stringWithFormat:@"%s",loc];
    return [[PineGameDelegate sharedInstance] hasAdsInterstitial:stitle];
}

extern "C" bool charboostHasShowVideoReward(const char* title)
{
    NSString* stitle = [NSString stringWithFormat:@"%s",title];
    return [[PineGameDelegate sharedInstance] hasVideoRewards:stitle];
}

extern "C" void charboostShowVideoReward(const char* title)
{
    if(title != NULL)
    {
        NSString* stitle = [NSString stringWithFormat:@"%s",title];
        [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(showVideoRewards:) withObject:stitle waitUntilDone:YES];
    }
    else
    {
        [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(showVideoRewards:) withObject:@"Default" waitUntilDone:YES];
    }
}
extern "C" void charboostShowAtMenu(const char* title)
{
    //[[PineGameDelegate sharedInstance] chartboostMoreGame];
    if(title != NULL)
    {
        NSString* stitle = [NSString stringWithFormat:@"%s",title];
        [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(chartboostShowAtMenu:) withObject:stitle waitUntilDone:YES];
    }
    else
    {
        [[PineGameDelegate sharedInstance] performSelectorOnMainThread:@selector(chartboostShowAtMenu:) withObject:NULL waitUntilDone:YES];
    }
}
extern "C" void chartboostDownloadConfig()
{
    [[PineGameDelegate sharedInstance] chartboostDownloadFile];
}



#endif
