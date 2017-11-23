#if !defined(_H_OS_FUNCTION)
#define _H_OS_FUNCTION

////////////////////////////////////////////////////
///WORK WITH MUSIC AND SOUND
////////////////////////////////////////////////////
#include "CType.h"
#include "../Lib/PineSocial.h"
#include <string>

#if defined(MAC_OS)
extern "C" void		OS_LoadMusic(const char* filename);
extern "C" void		OS_PlayCurrentMusic(int num_loop);
extern "C" void		OS_StopCurrentMusic();
extern "C" void		OS_PauseCurrentMusic();
extern "C" bool		OS_IsCurrentMusicPlaying();
extern "C" void		OS_setMusicVolume(float vol);
extern "C" void		OS_LoadSFX(const char* filename, int sound_id);
extern "C" void		OS_PlaySFX(int sound_id);
extern "C" void		OS_StopAllSFX();
extern "C" void     OS_TryPlayMusic();

extern "C" void		OS_LoadLibMusic();
extern "C" void		OS_PlayLibMusic(int num_loop);
extern "C" void		OS_StopLibMusic();
extern "C" void		OS_PauseLibMusic();
extern "C" void		OS_UpdateLibSongList();
extern "C" int		OS_GetLibSongNum();
extern "C" char*	OS_GetLibSongTitle(int index);
extern "C" char*	OS_GetLibCurrentSongTitle();
extern "C" void		OS_PreviousMusic(int numberSecond);
extern "C" void		OS_ForwardMusic(int numberSecond);
extern "C" double	OS_GetCurrentLibMusicTime();
extern "C" double	OS_GetLibSongTime();
extern "C" double	OS_GetCurrentMusicTime();
extern "C" double	OS_GetSongTime();
extern "C" float	OS_getPeakMusicPower(int channel);
extern "C" float	OS_getAverangeMusicPower(int channel);
#endif

////////////////////////////////////////////////////
///WORK WITH DATA FILE
////////////////////////////////////////////////////
extern "C" ResourceItem OS_GetResourceItem(const char* filename);
extern "C" char*	OS_GetResourceFilePath(const char* filename);
extern "C" char*	OS_GetDocumentFilePath(const char* filename);

extern "C" bool		OS_SaveAppData(const char* filename, const char* data_to_save, int data_length, bool is_append);
//extern "C" int		OS_ReadAppData(const char* filename, char*& readed_data);
extern "C" Buffer*	OS_LoadAppData(const char* filename);
extern "C" bool		OS_DeleteAppData(const char* filename);
extern "C" void		OS_DeleteFile(const char* file_name);
extern "C" void		OS_LoadImageToPngFile(const char* url, const char *file_name, float width_target, float height_target);
extern "C" void		OS_AsynDownloadFile(PineServiceHandle* hsvc, const char* url, const char *file_name);
extern "C" void		OS_AsynLoadImageToPngFile(PineServiceHandle* hsvc, const char* url, const char *file_name, float width_target, float height_target);
extern "C" bool		OS_IsDocumentFileExist(const char* filename);
////////////////////////////////////////////////////
///INAPP PURCHASE
////////////////////////////////////////////////////
extern "C" void			OS_LoadIAP();
extern "C" void			OS_AsynLoadIAP(PineServiceHandle* hsvc);
extern "C" void			OS_AsynRestoreIAP(PineServiceHandle* hsvc, const char *sku);
extern "C" SVRESULT		OS_getInAppList();//get inapp list
extern "C" void			PurchaseProduct(const char *indentify);//purchase an inapp item
#if defined(ANDROID_OS)
extern "C" void			PurchaseProductWithType(const char *indentify, int type = 1);//purchase an inapp item with type
extern "C" unsigned long long StringToU64(const char * sz);
extern "C" void			OS_GoogleAnalyticsLogEvent(const char *title, const char* decs, const char * type = "", const char * value = "");
extern "C" void			OS_SetLocalNotification(int id, const char *message, const char * ticker, int second);
#endif

////////////////////////////////////////////////////
///UTILITY
////////////////////////////////////////////////////
/*ALL*/				extern "C" bool				OS_IsNetworkAvailable();
/*ALL*/				extern "C" unsigned long	OS_GetTime(void);
/*NOT SURE*/		extern "C" double			OS_GetAccelX();
/*NOT SURE*/		extern "C" double			OS_GetAccelY();
/*NOT SURE*/		extern "C" double			OS_GetAccelZ();

/*REMOVING*/		extern "C" void				CheckLockRotate();
/*REMOVING*/		extern "C" void				ReturnGL();
/*REMOVING*/		extern "C" void				OpenURLInSafari(char* url);
/*REMOVING*/		extern "C" void				OpenAppStore(INT_TIME appleID); //replace 

/*IOS	WP8	WIN8*/	extern "C" void				OS_BrowserWebPage(const char* url);//open web page in browser which depend on OS platform
/*NOT SURE*/		extern "C" void				OS_Rating();//open rating page
/*NOT SURE*/		extern "C" void				OS_RatingWithDialog(const char* title, const char* message);//show dialog and open rating page if user choice yes.
/*NOT SURE*/		extern "C" const char*		OS_GetRegionShort();//get Region format of phone. example: VN
/*NOT SURE*/		extern "C" void				OS_Exit(bool sudden);
/*NOT SURE*/		extern "C" void				OS_Restart(bool sudden);
/*NOT SURE*/		extern "C" void				ShowRateDialog();
/*REMOVING*/		extern "C" void				GoToReviewInItune();

/*NOT SURE*/		extern "C" void				OS_SendSMS(const char* title, const char* content);
/*NOT SURE*/		extern "C" void				OS_SendMail(const char* title, const char* content);

#if defined(MAC_OS) || defined(_WINDOWS)
extern "C" int				GetMotionAnchor();
extern "C" void				SetMotionAnchor(int anchor);
extern "C" void				StartServerTheard();

extern "C" void				ServerADVERTStart();

extern "C" void				DevicePushRemoteNotification(char* device_token, char* message);

#endif
/*WP8*/				extern "C" const char*		OS_GetAppId();
#if defined (_WINDOWS_PHONE8)
/*WP8*/		extern "C" void				OS_AsyncShareLink(PineServiceHandle* hsvc, const char * title, const char * url, const char * message);
/*WP8*/		extern "C" void				OS_AsyncShareMedia(PineServiceHandle* hsvc, const char * fileName);
#elif defined (ANDROID_OS)
/*ANDROID*/	extern "C" int				OS_GetDeviceOsVersion();
/*ANDROID*/ extern "C" void				OS_ShowMessageBox(const char * title, const char * message, int type = 0);
/*ANDROID*/ extern "C" void				OS_AsyncShowMessageBox(PineServiceHandle* hsvc, const char * title, const char * message, int type = 1);
/*ANDROID*/ extern "C" void				OS_AsyncShowSendGiftIntent(PineServiceHandle* hsvc, int type, const char * message, int * numGiftReward);
#endif

////////////////////////////////////////////////////
///WOKING WITH FACEBOOK
////////////////////////////////////////////////////
extern "C" void			FBLoadMe();
extern "C" void			FBLoadListFriend(bool includeNotPlayGame);
extern "C" bool			HasLoginFB();
extern "C" void			InitFaceBook();
extern "C" const char*	OS_GetFBUserId();
extern "C" const char*	OS_GetFBUserName();
extern "C" void			loadFBAvatarPhoto();
extern "C" int			getFBErrorCode();
extern "C" bool			FBSessionValid();
extern "C" void			FBFeedMe(const char * msg);
extern "C" SVRESULT		OS_FBFeedMe(const char* msg);

/*REMOVING*/		extern "C" void			SVS_InitFaceBookAsRequire(SVHANDLE* handle);//require login to playgame
/*REMOVING*/		extern "C" void			SVS_InitFaceBook(SVHANDLE* handle);//login fb
/*REMOVING*/		extern "C" void			SVS_FBLoadListFriend(SVHANDLE* handle);//load friend of facebook
/*REMOVING*/		extern "C" void			SVS_FBFeed(SVHANDLE* handle, const char* msg);
/*NOTSURE*/			extern "C" void			LogOutFaceBook();
extern "C" void			FaceBookPost(const char * msg);
extern "C" void			FBRequestToId(INT_TIME fbId);
extern "C" void			FB_apprequests(const char * msg, const INT_TIME * arrayID, int count);

extern "C" void			OS_FacebookInviteFriend(const char* feed, const char* title, INT_TIME* friendlist, int numfriend);
extern "C" void			OS_PostScreenToFacebook(const char* feed);
extern "C" std::string     OS_GetFacebookToken();

////////////////////////////////////////////////////
///WOKING WITH FACEBOOK
////////////////////////////////////////////////////
/*NOTSURE*/			extern "C" bool			OS_IsLogginFacebook();
/*NOTSURE*/			extern "C" void			OS_InitFacebook(PineSocial *social);
/*NOTSURE*/			extern "C" void			OS_AsynInitFacebook(PineServiceHandle* hsvc);

/*NOTSURE*/			extern "C" void			OS_LoginFacebook();
/*NOTSURE*/			extern "C" void			OS_AsynLoginFacebook(PineServiceHandle* hsvc);

/*NOTSURE*/			extern "C" void			OS_PostFacebook(const char* msg);
/*NOTSURE*/			extern "C" void			OS_AsynPostFacebook(PineServiceHandle* hsvc, const char* msg);

/*NOTSURE*/			extern "C" void			OS_LogoutFacebook();
/*NOTSURE*/			extern "C" void			OS_AsynLogoutFacebook(PineServiceHandle* hsvc);

/*NOTSURE*/			extern "C" UINT64		OS_GetFacebookUserId();
/*NOTSURE*/			extern "C" const char*	OS_GetFacebookUserName();
/*NOTSURE*/			extern "C" const char*	OS_GetFacebookAvatarUrl();

/*NOTSURE*/			extern "C" void			OS_AsynFacebookLoadFriendIdList(PineServiceHandle* hsvc, PineAccountIdList* account_list);

/*NOTSURE*/			extern "C" void			OS_FacebookPostPhoto(const char* file_name, const char* msg);
/*NOTSURE*/			extern "C" void			OS_AsynFacebookPostPhoto(PineServiceHandle* hsvc, const char* file_name, const char* msg);
extern "C" void	OS_AsynFacebookPostScreen(PineServiceHandle* hsvc, const char* msg, PineGraphicComplexOrientation rotate);
extern "C" void OS_AsynFacebookPostScore(PineServiceHandle* hsvc,BoardScoreItem item);


extern "C" void OS_AsynFacebookAppInvite(PineServiceHandle* hsvc, const char* applink, const char* preview_img_url);
extern "C" void OS_FacebookAppInvite(const char* applink, const char* preview_img_url);


#if defined(MAC_OS)|| defined(_WINDOWS)

//extern "C" void FBFeedMe();
extern "C" void FBFeedFriend();
extern "C" void StartFBThread();
extern "C" int getFBErrorSubCode();
#endif

////////////////////////////////////////////////////
///WOKING WITH TWITTER
////////////////////////////////////////////////////
extern "C" void PostTwitter();
extern "C" void ShowTWConnectDialog();
extern "C" void PublishTWStream(const char* msg);
extern "C" int TWLoginIsDone();
extern "C" void InitTwitter();
extern "C" void TwitterLoginDialog();
extern "C" void TwitterPostTweet(const char * msg);

extern "C" void OS_PostInviteToTwitter(const char* feed);
extern "C" void OS_PostScreenToTwitter(const char* feed);


extern "C" SVRESULT SVS_InitTwitter();
extern "C" SVRESULT SVS_TWFeed(const char* msg);

////////////////////////////////////////////////////
///WOKING WITH TWITTER
////////////////////////////////////////////////////
extern "C" bool			OS_IsLogginTwitter();
extern "C" void			OS_InitTwitter();
extern "C" void			OS_AsynInitTwitter(PineServiceHandle* hsvc);
extern "C" void			OS_LoginTwitter();
extern "C" void			OS_AsynLoginTwitter(PineServiceHandle* hsvc);
extern "C" void			OS_PostTwitter(const char* msg);
extern "C" void			OS_AsynPostTwitter(PineServiceHandle* hsvc, const char* msg);
extern "C" void			OS_LogoutTwitter();
extern "C" void			OS_AsynLogoutTwitter(PineServiceHandle* hsvc);
extern "C" UINT64		OS_GetTwitterUserId();
extern "C" const char*	OS_GetTwitterUserName();
extern "C" void			OS_AsynTwitterLoadFollowerIdList(PineServiceHandle* hsvc, PineAccountIdList* account_list);

////////////////////////////////////////////////////
///WOKING WITH GAMECENTER - APPLE
////////////////////////////////////////////////////
extern "C" bool isGCAvailable();
extern "C" void InitGCBoard();
extern "C" const char* GetGCName();
extern "C" bool saveGCAvatar();
extern "C" INT_TIME GetGCIDNum();
extern "C" bool HasGCAuthenticated();
extern "C" const char* GetGCID();
extern "C" int getGCErrorCode();
extern "C" void loadGCAvatarPhoto();
extern "C" bool HasCompleteLoadGCAvatar();
extern "C" void GetGCFriends();
extern "C" void showGCLeaderBoard();
extern "C" void GCSubmitScore(INT_TIME score, const char* category_id);
extern "C" void GCSubmitAchievement(const char* identifier, int percentComplete);
extern "C" void showGCAchievementBoard();

//lam Add
extern "C" void OS_GCNotifyAchievementCompleted(const char* achieID);
extern "C" void OS_GCNotifyLoadAchievement(const char* achieID,int percentcomplete);


///////////////////////////////////////////////////
///DEVICE CAPABILITY
///////////////////////////////////////////////////
extern "C" void OS_ShowPicLibrary();
extern "C" void OS_ShowCamera();
extern "C" void OS_DisableCamera();
extern "C" void OS_CaptureCamera();
extern "C" void OS_SwitchCamera();
extern "C" void OS_TextInputStart(char* textBuf, unsigned int bufSize, int x, int y, int w, int h);
extern "C" void OS_TextInputClose();


extern "C" void OS_Popup_Mail(const char* body, const char* title);
extern "C" void OS_Popup_Message(const char* body, const char* title);

extern "C" void OS_SystemWarning(const char* feed, const char* title);

extern "C" bool OS_IsDeviceHacked();
extern "C" bool OS_PingToHost(const char* url, int time_out);

extern "C" void UnZipFile(const char* iFile, const char *oFile);
extern "C" int GetSizeFile(const char *file_name);
extern "C" int ReadAppDataBinary(const char* filename, BYTE* readed_data);
extern "C" void DownloadFile(const char* url, const char* file_name, int time_out);
extern "C" void chartboostDownloadConfig();

extern "C" bool OS_GetMicroPermission();
extern "C" void OS_RequestMicroPermission();

/*NOTSURE*/			extern "C" void OS_CaptureScreen();
/*NOTSURE*/			extern "C" void OS_AsynCaptureScreen(PineServiceHandle* svc, PineGraphicComplexOrientation rotate);
/*NOTSURE*/			extern "C" void OS_AsynShareIntent(PineServiceHandle* hsvc, const char * name, const char* msg);
/*NOTSURE*/			extern "C" void OS_AsynShareScreenShotIntent(PineServiceHandle* hsvc, const char* msg);
/*NOTSURE*/			extern "C" void OS_AsynSendEmailIntent(PineServiceHandle* hsvc, const char* title, const char* body);

extern "C" bool OS_EnableScaleView();
extern "C" bool OS_ScaleView(float scalex, float scaley);

///////////////////////////////////////////////////
///ADMOB
///////////////////////////////////////////////////
extern "C" void OS_AsynAdmobInterstitialPrepare(PineServiceHandle* svc, bool auto_show, const char* ad_id);
extern "C" void OS_AsynAdmobInterstitialShow(PineServiceHandle* svc);

extern "C" void OS_AsynAdmobBannerPrepare(PineServiceHandle* svc, bool auto_show, const char* ad_id);
extern "C" void OS_AsynAdmobBannerShow(PineServiceHandle* svc);
extern "C" void OS_AsynAdmobBannerHide(PineServiceHandle* svc);

extern "C" void OS_AsynAdmobRewardedVideoPrepare(PineServiceHandle* svc);
extern "C" void OS_AsynAdmobRewardedVideoShow(PineServiceHandle* svc);


///////////////////////////////////////////////////
///CHARTBOOST
///////////////////////////////////////////////////
extern "C" void OS_AsynCBInterstitialPrepare(PineServiceHandle* hsvc);
extern "C" void OS_AsynCBInterstitialShow(PineServiceHandle* hsvc);

extern "C" void OS_AsynCBRewardedVideoPrepare(PineServiceHandle* hsvc);
extern "C" void OS_AsynCBRewardedVideoShow(PineServiceHandle* hsvc);

///////////////////////////////////////////////////
///UNITY ADS
///////////////////////////////////////////////////
extern "C" void OS_AsynUnityAdsInterstitialPrepare(PineServiceHandle* hsvc);
extern "C" void OS_AsynUnityAdsInterstitialShow(PineServiceHandle* hsvc);

extern "C" void OS_AsynUnityAdsRewardedVideoPrepare(PineServiceHandle* hsvc);
extern "C" void OS_AsynUnityAdsRewardedVideoShow(PineServiceHandle* hsvc);

extern "C" void OS_AsynUnityAdsSkippableVideoPrepare(PineServiceHandle* hsvc);
extern "C" void OS_AsynUnityAdsSkippableVideoShow(PineServiceHandle* hsvc);

///////////////////////////////////////////////////
///VUNGLE ADS
///////////////////////////////////////////////////
#if defined (_WINDOWS_PHONE8) || defined (ANDROID_OS)
extern "C" void OS_AsynVungleAdsPrepare(PineServiceHandle* svc);
extern "C" void OS_AsynVungleAdsShow(PineServiceHandle* svc);
#endif

///////////////////////////////////////////////////
///GOOGLE PLAY
///////////////////////////////////////////////////
extern "C" bool OS_IsLoginGooglePlay();
extern "C" bool OS_AsynIsLoginGooglePlay(PineServiceHandle* hsvc);
extern "C" void	OS_AsynLoginGooglePlay(PineServiceHandle* hsvc, bool needUserConfirm);
extern "C" void	OS_AsynGetGooglePlayUserInfo(PineServiceHandle* hsvc, char * userId, char * gpId, char * userName);
extern "C" void	OS_AsynGetGoogleFriendIdList(PineServiceHandle* hsvc, PineAccountIdList* account_list);
extern "C" UINT64 OS_GetGooglePlayId();
extern "C" const char* OS_GetGooglePlayUserName();
extern "C" const char* OS_GetGooglePlayAvatarUrl();
extern "C" void OS_AsynGooglePlayPost(PineServiceHandle* hsvc, const char* msg);
extern "C" void OS_AsynGooglePlayPostScreen(PineServiceHandle* hsvc, const char* msg);
extern "C" void OS_AsynGooglePlayPostScore(PineServiceHandle* hsvc, BoardScoreItem item);
extern "C" void OS_AsynGooglePlayGetLeaderBoardRank(PineServiceHandle* hsvc, const char * board_id, int * current_rank);
extern "C" void OS_AsynGooglePlayShowLeaderBoard(PineServiceHandle* hsvc, const char* board_id);
extern "C" void OS_AsynGooglePlayShowAchievements(PineServiceHandle* hsvc);
extern "C" void OS_AsynGooglePlayUnlockAchievement(PineServiceHandle* hsvc, const char* achi_id);
extern "C" void OS_AsynGoogleUpdateCloudSave(PineServiceHandle* hsvc, const char* localSaveName);

///////////////////////////////////////////////////
///FACEBOOK
///////////////////////////////////////////////////
extern "C" void OS_AsynGetAvatarFacebook(PineServiceHandle* hsvc, const char* userURL, const char* fileName, int width, int height);

///////////////////////////////////////////////////
///GAMESPARKS BACK-END
///////////////////////////////////////////////////

#if defined (ANDROID_OS)
extern "C" void OS_AsyncLoginGameSpark(PineServiceHandle* hsvc);
extern "C" void OS_AsyncGameSparkUploadSave(PineServiceHandle* hsvc);
#endif

///////////////////////////////////////////////////
///SUPPORT
///////////////////////////////////////////////////
#if defined(BUILD_SUPPORT)

extern "C" PineAccountType OS_SupportAccountType();
extern "C" void OS_AsyncSupportAskAccount(PineServiceHandle* hsvc);

#endif


#endif//_H_OS_FUNCTION