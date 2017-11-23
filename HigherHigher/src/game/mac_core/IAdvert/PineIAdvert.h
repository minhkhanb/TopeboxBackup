//
//  PineChartBoost.h
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 5/6/14.
//
//

//////////////////////////
//// README
/////////////////////////




extern "C" void DownloadFile(const char* url, const char* file_name,int time_out);
extern "C" int GetSizeFile(const char *file_name);
extern "C" int ReadAppDataBinary(const char* filename, BYTE* readed_data);
#ifndef PineEntertainment_IAdvert_h
#define PineEntertainment_IAdvert_h

-(void) AdBoxDownloadFile
{
    char filename[1024];
#ifdef MAC_OS
    /*
    sprintf(filename,"https://datancus.blob.core.windows.net/pineadvert/%d/adbox%s.config",ADVERT_APPLE_ID,ADS_VERSION);
    if(PineSocial::PingToHost(filename,3))
    {
         DownloadFile(filename,CHARTBOOST_CONFIG_FILE,5);
         printf("\nDownload chartboost file");
    }
    else
    {
        printf("\nCan't not found chartboost file");
    }
     */
   
#endif
}
-(void) ShowiAdFullScreen
{
#if ADD_IADVERT
    [_pine_view_iAD showFullScreen_iAd];
#endif
}
extern "C" int _build_version = 0;
-(void) AdBoxDidBecameActive:(UIApplication*)application
{
#if ADD_CHARTBOOST
    _chartboost_app_id = nil;
    _chartboost_app_syn = nil;
#endif
#if ADD_ADMOB
    _admob_app_id = nil;
    _admob_app_syn = nil;
#endif
#if ADD_UNITYADS
    _unity_app_id = nil;
#endif
    //load chartboost config.
    int length = GetSizeFile(CHARTBOOST_CONFIG_FILE);
    char chartboost_ids[512];
    char chartboost_syn[512];
    char unity_ads_id[512];
    char admob_app_id[512];
    char admob_app_syn[512];
    char adbox_type[128];
    char build_ver[128];
    sprintf(build_ver,"%d",0);
    sprintf(chartboost_ids,"%s",CHARTBOOST_ID1_DEFAULT);
    sprintf(chartboost_syn,"%s",CHARTBOOST_ID2_DEFAULT);
    sprintf(admob_app_id,"%s",ADMOB_ID1_DEFAULT);
    sprintf(admob_app_syn,"%s",ADMOB_ID2_DEFAULT);
#if ADD_UNITYADS
    sprintf(unity_ads_id,"%s",UNITY_ADS_ID_DEFAULT);
#endif
#if ADD_VUNGLEADS
    char vungle_ads_id[128];
    sprintf(vungle_ads_id,"%s",VUNGLE_ID_DEFAULT);
#endif
    //chartboost    iad     admob       ver     unityads    vungle nativeads    video_admob
    //0             0       1           0       1           1       1           1           00
    sprintf(adbox_type,"0010111100");
    //sprintf(adbox_type,"0000010000");

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
                else if(step == 2)
                {
                    adbox_type[ch] = infomation[i];
                }
                else if(step == 3)
                {
                    admob_app_id[ch] = infomation[i];
                }
                else if(step == 4)
                {
                    admob_app_syn[ch] = infomation[i];
                }
                else if (step == 5)
                {
                    build_ver[ch] = infomation[i];
                }
                else if(step == 6)
                {
                    unity_ads_id[ch] = infomation[i];
                }
#if ADD_VUNGLEADS
                else if(step == 7)
                {
                    vungle_ads_id[ch] = infomation[i];
                }
#endif
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
                else if(step == 2)
                {
                    adbox_type[ch] = 0;
                }
                else if(step == 3)
                {
                    admob_app_id[ch] = 0;
                }
                else if(step == 4)
                {
                    admob_app_syn[ch] = 0;
                }
                else if(step == 5)
                {
                    build_ver[ch] = 0;
                }
                else if(step == 6)
                {
                     unity_ads_id[ch] = 0;
                }
#if ADD_VUNGLEADS
                else if(step == 7)
                {
                    vungle_ads_id[ch] = 0;
                }
#endif
                step++;
                ch = 0;
            }
        }
        //detect enable ad system
        PineAdBox::AdsAvailableNum = 0;
        for(int i = 0; i < strlen(adbox_type); i++)
        {
            if(!(SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"7.0")) && (i==ADVERT_IADVERT_TYPE))
            {
                continue;
            }
            if(adbox_type[i] == '1')
            {
               
                {

                    PineAdBox::AdsTypeArray[PineAdBox::AdsAvailableNum] = i;
                    PineAdBox::AdsAvailableNum++;
                    printf("Enable Add type = %d",i);
                }
                
            }
        }
        printf("\nSum up PineAdBox::AdsAvailableNum = %d",PineAdBox::AdsAvailableNum);
#if ADD_CHARTBOOST
        if(PineAdBox::isAdEnable(ADVERT_CHARTBOOST_TYPE))
        {
            _chartboost_app_id = [NSString stringWithFormat:@"%s",chartboost_ids];
            _chartboost_app_syn = [NSString stringWithFormat:@"%s",chartboost_syn];
        }
#endif
       
#if ADD_ADMOB
         if(PineAdBox::isAdEnable(ADVERT_ADMOB_TYPE))
         {
             _admob_app_id = [NSString stringWithFormat:@"%s",admob_app_id];;
             _admob_app_syn = [NSString stringWithFormat:@"%s",admob_app_syn];
         }
        if([self AdmobAvailable])
        {
            if(_pine_view_admob == nil)
            {
                _pine_view_admob = [PineViewController alloc];
            }
            NSString* admobID = [NSString stringWithFormat:@"ca-app-pub-%s/%s",admob_app_id,admob_app_syn];
            if(![_pine_view_admob admob_hasadstoshow])
            {
                [_pine_view_admob adMobLoad:admobID];
            }
             NSLog(@"admobID: %@",admobID);
        }
#endif
#if ADD_ADMOB_VIDEO_ADS
        if(PineAdBox::isAdEnable(ADVERT_ADMOB_VIDEO_TYPE))
        {
            if(_pine_view_video_admob == nil)
            {
                _pine_view_video_admob = [PineViewController alloc];
                
            }
            [_pine_view_video_admob cycleAdmobVideoAds];
        }
#endif
#if ADD_ADMOB_NATIVE_ADS
        if(PineAdBox::isAdEnable(ADVERT_ADMOB_NATIVE_TYPE))
        {
            if(_pine_view_admob_native == nil)
            {
                _pine_view_admob_native = [PineViewController alloc];
                
            }
            [_pine_view_admob_native cycleNativeAds];
        }
#endif
        _build_version = atoi(build_ver);
        delete[] infomation;
    }
    else
    {
        //using default ad system
        PineAdBox::AdsAvailableNum = 0;
        for(int i = 0; i < strlen(adbox_type); i++)
        {
            if(!(SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"7.0")) && (i==ADVERT_IADVERT_TYPE))
            {
                continue;
            }
            if(adbox_type[i] == '1')
            {
                PineAdBox::AdsTypeArray[PineAdBox::AdsAvailableNum] = i;
                PineAdBox::AdsAvailableNum++;
                printf("Enable Add type = %d",i);
                
            }
        }
        printf("\nSum up PineAdBox::AdsAvailableNum = %d",PineAdBox::AdsAvailableNum);
#if ADD_CHARTBOOST
        if(PineAdBox::isAdEnable(ADVERT_CHARTBOOST_TYPE))
        {
            _chartboost_app_id = [NSString stringWithFormat:@"%s",chartboost_ids];
            _chartboost_app_syn = [NSString stringWithFormat:@"%s",chartboost_syn];
        }
#endif
        
#if ADD_ADMOB
        if(PineAdBox::isAdEnable(ADVERT_ADMOB_TYPE))
        {
            _admob_app_id = [NSString stringWithFormat:@"%s",admob_app_id];;
            _admob_app_syn = [NSString stringWithFormat:@"%s",admob_app_syn];
        }
        if([self AdmobAvailable])
        {
            if(_pine_view_admob == nil)
            {
                _pine_view_admob = [PineViewController alloc];
            }
            NSString* admobID = [NSString stringWithFormat:@"ca-app-pub-%s/%s",admob_app_id,admob_app_syn];
            if(![_pine_view_admob admob_hasadstoshow])
            {
                [_pine_view_admob adMobLoad:admobID];
            }
            NSLog(@"admobID: %@",admobID);
        }
#endif
#if ADD_ADMOB_VIDEO_ADS
        if(PineAdBox::isAdEnable(ADVERT_ADMOB_VIDEO_TYPE))
        {
            if(_pine_view_video_admob == nil)
            {
                _pine_view_video_admob = [PineViewController alloc];
                
            }
            [_pine_view_video_admob cycleAdmobVideoAds];
        }
#endif
#if ADD_ADMOB_NATIVE_ADS
        if(PineAdBox::isAdEnable(ADVERT_ADMOB_NATIVE_TYPE))
        {
            if(_pine_view_admob_native == nil)
            {
                _pine_view_admob_native = [PineViewController alloc];
               
            }
            [_pine_view_admob_native cycleNativeAds];
        }
#endif
    }
#if ADD_VUNGLEADS
    if(PineAdBox::isAdEnable(ADVERT_VUNGLE_TYPE))
    {
        if(_pine_view_vungle == nil)
        {
            _pine_view_vungle = [PineViewController alloc];
        }
        if(![_pine_view_vungle VungleAdsHasToShow])
        {
            _vungle_app_id = [NSString stringWithFormat:@"%s",vungle_ads_id];
            //dispatch_async(dispatch_get_main_queue(), ^(){
            [_pine_view_vungle VungleAdsLoad:_vungle_app_id];
            //});
        }

    }
#endif
#if ADD_UNITYADS
    if(PineAdBox::isAdEnable(ADVERT_UNITY_TYPE))
    {
        // NSLog(@"unity_ads_id: %@",unity_ads_id);
        if(_pine_view_unityads == nil)
        {
            _pine_view_unityads = [PineViewController alloc];
        }
        _unity_app_id = [NSString stringWithFormat:@"%s",unity_ads_id];
        [_pine_view_unityads UnityAdsLoad:_unity_app_id];

    }
#endif
#if ADD_CHARTBOOST
    if([self chartboostAvailable])
    {
      
        
        // Pro Tip: Use code below to Print IFA (Identifier for Advertising) in Output section. iOS 6+ devices only.
        //NSString* ifa = [[[NSClassFromString(@"ASIdentifierManager") sharedManager] advertisingIdentifier] UUIDString];
        //ifa = [[ifa stringByReplacingOccurrencesOfString:@"-" withString:@""] lowercaseString];
        //NSLog(@"IFA: %@",ifa);

        @try
        {
            [Chartboost startWithAppId:_chartboost_app_id appSignature:_chartboost_app_syn delegate:self];
            [Chartboost   cacheMoreApps:@"More App"];
            [Chartboost cacheInterstitial:@"Main Menu"];
#if ADD_VIDEO_REWARD
            [Chartboost cacheRewardedVideo:@"Reward Video"];
#endif
        }@catch(NSException* eeee)
        {
            
        }

        //[[Chartboost sharedChartboost] showInterstitial:CBLocationMainMenu];
        //[cb showMoreApps];
    }
#endif
}


extern "C" void ShowIADFullScreen()
{
    [[PineGameDelegate sharedInstance] ShowiAdFullScreen];
}
extern "C" void OS_AdBoxDownloadFile()
{
    [[PineGameDelegate sharedInstance] AdBoxDownloadFile];
}
extern "C" int OS_GetBuildVersionOnline()
{
    return _build_version;
}


#endif
