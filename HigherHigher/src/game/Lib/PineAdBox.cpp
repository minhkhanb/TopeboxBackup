#include "pch.h"
#include "../GameCore/CGame.h"
#include "../GameCore/Os_Functions.h"
#include "PineAdBox.h"
bool PineAdBox::shouldAppPopup = true;
int PineAdBox::AdsAvailableNum =  0;
int PineAdBox::AdsTypeArray[ADVERT_TYPE_NUM_MAX] = {};
int PineAdBox::AdsIndex = 0;
int PineAdBox::showAdState = 0;
PineAdBox::enumTypePostIOSBasic PineAdBox::typePostIOSBasic = enumTypePostIOSBasic::TYPE_POST_BASIC_NONE;
extern "C" void ShowIADFullScreen();
extern "C" void charboostShowAtMenu(const char* title);
extern "C" void charboostMoreGame();
extern "C" void ShowAdmobFullScreen();
extern "C" bool chartboostHasMoreGame();
#if ADD_UNITYADS
extern "C" bool HasUnityAdsToShow();
extern "C" bool HasUnityVideoAds();
extern "C" void ShowUnityAdsFullScreen();
extern "C" void ShowUnityVideoAds();
#endif
#if ADD_ADMOB_VIDEO_ADS
extern "C" bool OS_HasAdmobVideoAds();
extern "C" void OS_ShowAdmobVideoAds();
#endif
int PineAdBox::_more_game_for_video_ads = 0;
bool PineAdBox::isAdEnable(int system)
{
    for(int i = 0; i < PineAdBox::AdsAvailableNum; i++)
    {
        if(system == PineAdBox::AdsTypeArray[i])
        {
            return true;
        }
        
    }
    return false;
}
void PineAdBox::SetSkipAdvert()
{
    
    shouldAppPopup = false;
}
void PineAdBox::SetReactiveAdvert()
{
    shouldAppPopup = true;
}
void PineAdBox::ShowMoreGame()
{
    PineSocial::OpenURL("https://itunes.apple.com/us/developer/id505916072");
   
#if ADD_CHARTBOOST
    //charboostMoreGame();
#endif
}
extern "C" bool chartboost_hasAdsInterstitial(const char* loc);
extern "C" bool charboostHasShowVideoReward(const char* title);
extern "C" void charboostShowVideoReward(const char* title);
extern "C" bool HasAdmobToShow();
extern "C" void ShowAdmobFullScreen();
extern "C" bool iOS_VungleHasAdsToShow();
extern "C" void iOS_VungleShowAds();

void PineAdBox::ShowAdvertWithVideo()
{
#ifdef MAC_OS
#if ADD_UNITYADS
    
    if(PineAdBox::isAdEnable(ADVERT_UNITY_TYPE))
    {
        if(HasUnityAdsToShow())
        {
            ShowUnityAdsFullScreen();
            showAdState = SHOW_AD_PROCESSING;
            //ShowAdmobFullScreen();
            return;
        }
    }
    
#endif

    if(PineAdBox::isAdEnable(ADVERT_ADMOB_TYPE))
    {
        if(HasAdmobToShow())
        {
            
            showAdState = SHOW_AD_PROCESSING;
            ShowAdmobFullScreen();
            return;
        }
    }

   
#endif
}
void PineAdBox::ShowAdvertFullScreen()
{
    /*
    if(PineAdBox::isAdEnable(ADVERT_VUNGLE_TYPE))
    {
        if(iOS_VungleHasAdsToShow())
        {
            iOS_VungleShowAds();
        }
    }*/
#ifdef MAC_OS
    if(PineAdBox::isAdEnable(ADVERT_ADMOB_TYPE))
    {
        if(HasAdmobToShow())
        {
            
            showAdState = SHOW_AD_PROCESSING;
            ShowAdmobFullScreen();
            return;
        }
    }
#if ADD_UNITYADS

	if (PineAdBox::isAdEnable(ADVERT_UNITY_TYPE))
	{
		if (HasUnityAdsToShow())
		{
			ShowUnityAdsFullScreen();
			showAdState = SHOW_AD_PROCESSING;
			//ShowAdmobFullScreen();
			return;
		}
	}

#endif
#endif

}
void PineAdBox::ShowFullScreenWithType(int type)
{
#ifdef MAC_OS
    switch(type)
    {
#if ADD_VUNGLEADS
        case ADVERT_VUNGLE_TYPE:
            if(iOS_VungleHasAdsToShow())
            {
                iOS_VungleShowAds();
            }
            break;
#endif
#if ADD_UNITYADS
        case ADVERT_UNITY_TYPE:
            if(HasUnityAdsToShow())
            {
                ShowUnityAdsFullScreen();
            }
            break;
#endif
        case ADVERT_CHARTBOOST_TYPE:
#if ADD_CHARTBOOST
            if(chartboost_hasAdsInterstitial("Main Menu"))
            {
               
                showAdState = SHOW_AD_PROCESSING;
                charboostShowAtMenu("Main Menu");
            }
#endif
            break;
        case ADVERT_ADMOB_TYPE:
#if ADD_ADMOB
            if(HasAdmobToShow())
            {
               
                showAdState = SHOW_AD_PROCESSING;
                ShowAdmobFullScreen();
            }
#endif
            break;
        case ADVERT_CHARTBOOST_MOREGAME_TYPE:
#if ADD_CHARTBOOST
            if(chartboostHasMoreGame())
            {
              
                showAdState = SHOW_AD_PROCESSING;
                charboostMoreGame();
            }
#endif
            break;
        default:
            break;
    }
#endif
}
int PineAdBox::HasAdvertFullScreen()
{
#ifdef MAC_OS
    return HasAdmobToShow();
#endif 
	return 0;
}

 int PineAdBox::HasVideoAds(bool letSkip)
{
#ifdef MAC_OS
#if ADD_ADMOB_VIDEO_ADS
    if(letSkip && OS_HasAdmobVideoAds())
    {
        return ADVERT_ADMOB_VIDEO_TYPE;
    }
#endif
#if ADD_VUNGLEADS
    //printf("\niOS_VungleHasAdsToShow = %d",iOS_VungleHasAdsToShow());
    if(iOS_VungleHasAdsToShow())
    {
        return ADVERT_VUNGLE_TYPE;
    }
#endif
#if ADD_UNITYADS
    if(HasUnityVideoAds())
    {
        return ADVERT_UNITY_TYPE;
    }
    
#endif
#if ADD_CHARTBOOST
    if(charboostHasShowVideoReward("Reward Video"))
    {
        return ADVERT_CHARTBOOST_TYPE;
    }
#endif
#if ADD_UNITYADS
    if(HasUnityAdsToShow())
    {
        return ADVERT_UNITY_TYPE;
    }
#endif
#endif
    return -1;
}
 void PineAdBox::ShowVideoAds(int type)
{
#ifdef MAC_OS
#if ADD_ADMOB_VIDEO_ADS
    if(type == ADVERT_ADMOB_VIDEO_TYPE)
    {
        OS_ShowAdmobVideoAds();
    }
    else
#endif
#if ADD_VUNGLEADS
    if(ADVERT_VUNGLE_TYPE == type)
    {
        if(iOS_VungleHasAdsToShow())
        {
            iOS_VungleShowAds();
        }
    }
    else
#endif
#if ADD_UNITYADS
    if(type == ADVERT_UNITY_TYPE)
    {

        if(HasUnityVideoAds())
        {
            _more_game_for_video_ads = 0;
            ShowUnityVideoAds();
            
        }
        else
        if(HasUnityAdsToShow())
        {
            _more_game_for_video_ads = 0;
            ShowUnityAdsFullScreen();
        }
        
        
        
    }
    else
#endif
#if ADD_CHARTBOOST
    if(type == ADVERT_CHARTBOOST_TYPE)
    {
       
         if(charboostHasShowVideoReward("Reward Video"))
         {
             _more_game_for_video_ads = 0;
              showAdState = SHOW_AD_PROCESSING;
              charboostShowVideoReward("Reward Video");
         }
#ifndef DISABLE_MOREGAME_LIKE_ADS
        else
        if(chartboostHasMoreGame())
        {
            _more_game_for_video_ads++;
             showAdState = SHOW_AD_PROCESSING;
            charboostMoreGame();
        }
#endif
    }
    else
#endif
#if ADD_ADMOB
    if(ADVERT_ADMOB_TYPE == type)
    {
        _more_game_for_video_ads = 0;
        showAdState = SHOW_AD_PROCESSING;
        ShowAdmobFullScreen();
    }
    else
#endif
    {
        showAdState = SHOW_AD_CANCEL;
    }


#endif
}


#if ADD_ADMOB_NATIVE_ADS
extern "C" void OS_AdmobNativeShow();
bool PineAdBox::HasNativeAdsAvailable()
{
    return _native_ads_available;
}
bool PineAdBox::_native_ads_available = false;
bool PineAdBox::_native_ads_processing = false;
void PineAdBox::ShowNativeAds()
{
    if(HasNativeAdsAvailable())
    {
        OS_AdmobNativeShow();
    }
}
#endif


#if ADD_ADMOB_BANNER_ADS
extern "C" void OS_ShowBannerInit();
extern "C" void OS_ShowBannerAds();
extern "C" void OS_ShowBannerDissmiss();

void PineAdBox::InitAdmobBanner()
{
    OS_ShowBannerInit();
}
void PineAdBox::ShowAdmobBanner()
{
    OS_ShowBannerAds();
}
void PineAdBox::CloseAdmobBanner()
{
    OS_ShowBannerDissmiss();
}
#endif
