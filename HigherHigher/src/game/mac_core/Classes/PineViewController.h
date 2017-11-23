
//#import "PineGameDelegate.h"
//#import "Defines.h"
#import "CGame.h"
#ifndef PINE_VIEW_CONTROLLER
#define PINE_VIEW_CONTROLLER 1
#import <UIKit/UIKit.h>
#import <GameKit/GKLocalPlayer.h>
#import <GameKit/GKLeaderboard.h>
#import <GameKit/GKGameCenterViewController.h>
#import <GameKit/GKScore.h>
#import <GameKit/GKAchievement.h>
#if ADD_IADVERT
#import <iAd/iAd.h>
#endif
#if ADD_ADMOB
#import <GoogleMobileAds/GoogleMobileAds.h>
#endif
#if ADD_ADMOB_NATIVE_ADS
#import <GoogleMobileAds/GADNativeAppInstallAd.h>
#endif
#if ADD_ADMOB_VIDEO_ADS
#import <GoogleMobileAds/GADRewardBasedVideoAd.h>
#if ADD_VUNGLE_TO_MEDIATION
#import "VungleAdNetworkExtras.h"
#endif
#endif
#if ADD_UNITYADS
#import <UnityAds/UnityAds.h>
#endif
#if USE_PINEGAMEPLAYREPLAY
#import <ReplayKit/RPScreenRecorder.h>
#import <ReplayKit/RPPreviewViewController.h>
#endif
#if USE_AUDIO_RECORDER
#import <AVFoundation/AVAudioRecorder.h>
#endif
#if ADD_VUNGLEADS
#import <VungleSDK/VungleSDK.h>
#endif
#import <AVFoundation/AVFoundation.h>

#if ADD_GOOGLE_ANALYTICS
#import <FirebaseAnalytics/FIRApp.h>
#import <FirebaseAnalytics/FIRAnalytics.h>
#if ADD_REMOTE_CONFIG
#import <FirebaseRemoteConfig/FIRRemoteConfig.h>
#endif
#endif
#import <StoreKit/StoreKit.h>

@interface PineViewController : UIViewController <NSObject,
UIPopoverPresentationControllerDelegate
,UIPopoverControllerDelegate
#if ADD_GAMECENTER
,GKGameCenterControllerDelegate
#endif
#if ADD_IADVERT
,ADInterstitialAdDelegate
#endif
#if ADD_ADMOB
,GADInterstitialDelegate
#endif
#if ADD_ADMOB_VIDEO_ADS
,GADRewardBasedVideoAdDelegate
#endif
#if ADD_ADMOB_NATIVE_ADS
,GADNativeAppInstallAdLoaderDelegate
,GADNativeContentAdLoaderDelegate
#endif
#if ADD_UNITYADS
,UnityAdsDelegate
#endif
#if USE_PINEGAMEPLAYREPLAY
,RPPreviewViewControllerDelegate
,RPScreenRecorderDelegate
#endif
#if USE_AUDIO_RECORDER
,AVAudioRecorderDelegate
#endif
#if ADD_VUNGLEADS
,VungleSDKDelegate
#endif
>
{
#if USE_PINEGAMEPLAYREPLAY
@public
    RPScreenRecorder* _record;
    //RPPreviewViewController* _record_controller;
    
#endif
@public
#if USE_AUDIO_RECORDER
    AVAudioRecorder* AVRecorder;
#endif
#if ADD_GAMECENTER
    bool _hasGamecenterAuthentication;
#endif
#if ADD_IADVERT
    ADInterstitialAd *interstitial;
    BOOL requestingAd;
#endif
#if ADD_ADMOB
    GADInterstitial *interstitialAdmob;
    NSString *admobID;
#endif


    
}
#if ADD_VUNGLEADS
-(void) VungleAdsLoad:(NSString*)id;
#endif
#if USE_AUDIO_RECORDER
- (AVAudioRecorder*) AVRecorder;
-(double) GetMicroAudioPass;
-(void) StopToBlow;
-(void)readyToBlow;
-(void)levelTimerCallbackBlow:(NSTimer *)timer;
#endif
#if USE_PINEGAMEPLAYREPLAY
- (RPScreenRecorder*) sharedRecorder;
- (void) GamePlayRecordStart;
- (void) GamePlayRecordStop;
-(void) shareRecordGamePlay;
- (void) GamePlayRecordForceStop;
@property (nonatomic, retain) IBOutlet RPPreviewViewController *_record_controller;
#endif

- (void) iOS_BasicPostSocial:(NSString*) feed withImage:(UIImage*) image;
-(void) iOS_VideoPostSocial:(NSString*) feed withVideoURL:(NSString*) url;
-(void) iOS_ShowWebPage:(NSString*) url isoffline:(BOOL) offline;

#if ADD_GAMECENTER
@property (retain,readwrite) NSString * currentPlayerID;
@property (retain,readwrite) NSString * currentName;
@property (retain,readwrite) NSString * currentAlias;
- (void) GameCenterLoadLeaderboardInfo;
-(void) GameCenterAuthentication:(PineSocial*)social;
-(void) GameCenterShowLeaderboard:(NSString*) leaderid;
-(void) GameCenterShowAchievement:(NSString*) leaderid;
- (void) reportScore: (int64_t) value forCategory: (NSString*) category;
-(void)updateAchievements:(NSString*) Identifier progress:(int)progress show:(bool)hasnoti;
-(void) ResetAllAchievements;
- (void) GameCenterLoadScore:(PineSocial*)social;
-(void) GameCenterRetrieveFriends:(PineSocial*)social;
-(void) closeView;
//lam.nguyen Add loadachievement
-(void) GameCenterLoadAchievements;
#endif

#if ADD_IADVERT
-(void)showFullScreen_iAd;
- (void)cycleInterstitial;
#endif

#if ADD_ADMOB
-(void) adMobLoad:(NSString*)id;
-(void) AdMobShow;
-(bool) admob_hasadstoshow;
#endif
#if ADD_ADMOB_NATIVE_ADS
/// You must keep a strong reference to the GADAdLoader during the ad loading process.
@property(nonatomic, strong) GADAdLoader *adLoader;
/// The native ad view that is being presented.
@property(nonatomic, strong) UIView *nativeAdView;
@property(nonatomic, strong) UIView *nativeAdPlaceholder;
 - (void) cycleNativeAds;
#endif
#if ADD_UNITYADS
-(void) UnityAdsLoad:(NSString*)id;
-(void) UnityAdsShow;
-(void) UnityAdsVideoRewardShow;
-(bool) UnityHasAdsToShow;
-(bool) UnityHasVideoAdsToShow;
#endif
@end
#endif
