#if !_PINE_ADVERT_BOX_H
#define _PINE_ADVERT_BOX_H 1

#include "../GamePlay/GameObject/MenuSystem.h"
#include "../GamePlay/GameObject/CScrollBar.h"
#include "PineImage.h"
#define ADVERT_CHARTBOOST_TYPE  (0)
#define ADVERT_IADVERT_TYPE     (1)
#define ADVERT_ADMOB_TYPE      (2)
#define ADVERT_VERSION_TYPE     (3)
#define ADVERT_UNITY_TYPE       (4)
#define ADVERT_VUNGLE_TYPE      (5)
#define ADVERT_ADMOB_NATIVE_TYPE (6)
#define ADVERT_ADMOB_VIDEO_TYPE (7)
#define ADVERT_CHARTBOOST_MOREGAME_TYPE (100)

#define ADVERT_TYPE_NUM_MAX (99)

#define SHOW_AD_NONE        (0)
#define SHOW_AD_PROCESSING  (1)
#define SHOW_AD_COMPLETED   (2)
#define SHOW_AD_CANCEL      (3)
#define SHOW_AD_ERROR       (4)
#define SHOW_AD_AVAILABLE   (5)
#define SHOW_AD_WAIT_REWARD (6)
#define SHOW_AD_SHOWING     (7)
class PineAdBox
{
public:
    
    enum enumTypePostIOSBasic
    {
        TYPE_POST_BASIC_NONE,
        TYPE_POST_BASIC_FACEBOOK,
        TYPE_POST_BASIC_TWITER,
    };
    static int _more_game_for_video_ads;
    static int AdsIndex;
    static int AdsAvailableNum;
    static int AdsTypeArray[ADVERT_TYPE_NUM_MAX];
    static bool shouldAppPopup;
    static void SetSkipAdvert();
    static void SetReactiveAdvert();
    static void ShowAdvertFullScreen();
    static void ShowAdvertWithVideo();
    static bool isAdEnable(int system);
    static void ShowMoreGame();
    static int HasVideoAds(bool letSkip = true);
    static void ShowVideoAds(int type);
    static int showAdState;
    static int HasAdvertFullScreen();
    static void ShowFullScreenWithType(int type);
    static enumTypePostIOSBasic typePostIOSBasic;
#if ADD_ADMOB_NATIVE_ADS
    static bool _native_ads_processing;
    static bool HasNativeAdsAvailable();
    static bool _native_ads_available;
    static void ShowNativeAds();
#endif
#if ADD_ADMOB_BANNER_ADS
    static void InitAdmobBanner();
    static void ShowAdmobBanner();
    static void CloseAdmobBanner();
#endif
    
};
#endif
