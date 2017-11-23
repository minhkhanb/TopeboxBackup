//
//  PineMacOSFunc.h
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 5/19/14.
//
//
#import <SystemConfiguration/SCNetworkReachability.h>
#ifndef __PineEntertainment__PineMacOSFuncDeclare__
#define __PineEntertainment__PineMacOSFuncDeclare__

-(int) GetNetworkSupport;
extern "C" bool ExitsFile(const char *file_name);
extern "C" bool OS_IsNetworkAvailable();
-(void)showMessageWarning:(NSArray*) stringArray;
- (void)showRating:(NSString*)title body:(NSString*)body;
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;
-(void) iOS_BasicPostSocial:(NSString*) feed;
-(void) iOS_VideoPostSocial:(NSString*) feed;
-(void) iOS_BasicPostSocialNonImage:(NSString*) feed;
-(void)getDeviceUserAgent;
//In app Purchase
-(void) InitInAppPurchase:(NSObject*)params;
-(void) SetPurchaseTimeOut:(int)time;
-(void) UpdatePurchaseTimeOut:(int)time;
-(void) PurchaseProduct:(NSString *)identify;
- (void) setBarPortrait:(NSObject*)params;
- (void) setBarLandscapeLeft:(NSObject*)params;
- (void) setBarLandscapeRight:(NSObject*)params;
-(bool) UnityAdsAvailable;
-(bool) UnityAds_HasAdToShow;
-(bool) UnityAds_HasVideoRewardToShow;
-(void) ShowUnityAds;
-(void) ShowUnityVideoAds;
#if USE_AUDIO_RECORDER
-(void) iOS_BlowSetCurrentPass:(double) v;
-(void) iOS_BlowDevice:(NSObject*)params;
-(double) iOS_GetMicroAudioPass;
-(void) iOS_StopBlowDevice;
#endif
-(PineViewController*) GetGameplayRecordView;
-(void) shareRecordGamePlay;
#endif


