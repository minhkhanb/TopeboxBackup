#include "pch.h"
#include "../../game/GameCore/CGame.h"
extern "C" void SaveImageToPhotosAlbum(BYTE* img_data, int img_data_len)
{
}

extern "C" bool getRankByTypeScore_extern(int type_score, BYTE type_time,  int count_to_get, bool isAscending)
{
	bool ret = true;

	return ret;
}

extern "C" bool storeToResultFromFriendList_extern(int type_score, BYTE type_time, bool isAscending)
{
	bool ret = true;

	return ret;
}

extern "C" void OS_ShowPicLibrary(){}
extern "C" void OS_ShowCamera(){}
extern "C" void OS_DisableCamera(){}
extern "C" void OS_CaptureCamera(){}
extern "C" void OS_SwitchCamera(){}

extern "C" void	OS_LoadImageToPngFile(const char* url, const char *file_name, float width_target, float height_target){}

extern "C" void OS_Popup_Mail(const char* body, const char* title){}
extern "C" void OS_Popup_Message(const char* body, const char* title){}
extern "C" void OS_FacebookInviteFriend(const char* feed, const char* title, INT_TIME* friendlist, int numfriend){}

extern "C" void OS_PostInviteToTwitter(const char* feed){}
extern "C" void OS_PostScreenToTwitter(const char* feed){}

extern "C" void OS_InitFacebook(PineSocial *social) {}
extern "C" void OS_PostScreenToFacebook(const char* feed){}

extern "C" void OS_SystemWarning(const char* feed, const char * title){}

extern "C" bool OS_IsLogginTwitter(){ return false; }

extern "C" bool	OS_IsLogginFacebook(){ return false; }

extern "C" bool OS_IsDeviceHacked(){ return false; }
extern "C" bool OS_PingToHost(const char* url, int time_out){ return false; }
extern "C" void	OS_LoadIAP(){}
extern "C" void	OS_BrowserWebPage(const char* url){}
extern "C" void	OS_RatingWithDialog(const char* title, const char* message){}

extern "C" void UnZipFile(const char* iFile, const char *oFile){}
extern "C" int GetSizeFile(const char *file_name){ return 0; }

extern "C" void DownloadFile(const char* url, const char* file_name, int time_out){}
extern "C" void chartboostDownloadConfig(){}
extern "C" void OS_InitTwitter(){}
extern "C" void OS_Exit(bool suppen){
}
extern "C" void OS_Restart(bool suppen) {
}
extern "C" void OS_TextInputStart(char* textBuf, unsigned int bufSize, int x, int y, int w, int h){}
extern "C" void OS_TextInputClose(){}
extern "C" void OS_PostFacebook(const char* msg){}
extern "C" void OS_PostTwitter(const char* msg){}
extern "C" void OS_PostImageToFacebook(const char* feed, const char* realUrl){}
extern "C" void OS_PostImageToTwitter(const char* feed, const char* realUrlImage){}
extern "C" void	OS_AsynLoadIAP(PineServiceHandle* hsvc){}
extern "C" void	OS_AsynRestoreIAP(PineServiceHandle* hsvc, const char *sku) { hsvc->Status = PineServiceResult::Responsed; hsvc->ResponseStatus = 1; }
extern "C" void	SVS_InitFaceBookAsRequire(SVHANDLE* handle){}
extern "C" void			OS_AsynLoginFacebook(PineServiceHandle* hsvc) { hsvc->Status = PineServiceResult::Responsed; hsvc->ResponseStatus = 1; }
extern "C" UINT64		OS_GetFacebookUserId(){ return 0; }
extern "C" const char*	OS_GetFacebookUserName(){ return "Not Support"; }
extern "C" void			OS_AsynFacebookLoadFriendIdList(PineServiceHandle* hsvc, PineAccountIdList* account_list){}

extern "C" void			OS_AsynLoginTwitter(PineServiceHandle* hsvc){}
extern "C" UINT64		OS_GetTwitterUserId(){ return 0; }
extern "C" const char*	OS_GetTwitterUserName(){ return "Not Support"; }
extern "C" void			OS_AsynTwitterLoadFollowerIdList(PineServiceHandle* hsvc, PineAccountIdList* account_list){}
extern "C" const char*	OS_GetFacebookAvatarUrl(){ return ""; }
extern "C" void			OS_AsynFacebookUpdateScore(PineServiceHandle* hsvc, int score) { hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1; }
extern "C" void			OS_AsynPostFacebook(PineServiceHandle* hsvc, const char* msg) { hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1; }
extern "C" void			OS_AsynFacebookPostScreen(PineServiceHandle* hsvc, const char* msg, PineGraphicComplexOrientation rotate) { hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1; }
extern "C" std::string     OS_GetFacebookToken()
{
	return "";
}
extern "C" void			OS_AsynFacebookPostScore(PineServiceHandle* hsvc, BoardScoreItem item) {
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}

extern "C" void OS_AsynAdmobInterstitialPrepare(PineServiceHandle* svc, bool auto_show, const char* ad_id) {
	svc->Status = PineServiceResult::Responsed;
	svc->ResponseStatus = 0;
}

extern "C" void OS_AsynAdmobInterstitialShow(PineServiceHandle* svc) {
	svc->Status = PineServiceResult::Responsed;
	svc->ResponseStatus = 0;
}

extern "C" void OS_AsynAdmobBannerPrepare(PineServiceHandle* svc, bool auto_show, const char* ad_id) {
	svc->Status = PineServiceResult::Responsed;
	svc->ResponseStatus = 0;
}
extern "C" void OS_AsynAdmobBannerShow(PineServiceHandle* svc) {
	svc->Status = PineServiceResult::Responsed;
	svc->ResponseStatus = 0;
}
extern "C" void OS_AsynAdmobBannerHide(PineServiceHandle* svc) {
	svc->Status = PineServiceResult::Responsed;
	svc->ResponseStatus = 0;
}

///////////////////////////////////////////////////
///CHARTBOOST
///////////////////////////////////////////////////
extern "C" void OS_AsynCBInterstitialPrepare(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 0;
}
extern "C" void OS_AsynCBInterstitialShow(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 0;
}

extern "C" void OS_AsynCBRewardedVideoPrepare(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 0;
}
extern "C" void OS_AsynCBRewardedVideoShow(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 0;
}

///////////////////////////////////////////////////
///UNITY ADS
///////////////////////////////////////////////////
extern "C" void OS_AsynUnityAdsInterstitialPrepare(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 0;
}
extern "C" void OS_AsynUnityAdsInterstitialShow(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 0;
}

extern "C" void OS_AsynUnityAdsRewardedVideoPrepare(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 0;
}
extern "C" void OS_AsynUnityAdsRewardedVideoShow(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 0;
}
extern "C" void OS_AsynUnityAdsSkippableVideoPrepare(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 0;
}
extern "C" void OS_AsynUnityAdsSkippableVideoShow(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 0;
}
//GOOGLE PLAY
extern "C" bool OS_IsLoginGooglePlay() {
	return false;
}
extern "C" bool OS_AsynIsLoginGooglePlay(PineServiceHandle* hsvc) {
	return false;
}
extern "C" void	OS_AsynLoginGooglePlay(PineServiceHandle* hsvc, bool needUserConfirm) {
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void	OS_AsynGetGooglePlayUserInfo(PineServiceHandle* hsvc, char * userId, char * gpId, char * userName) {
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void	OS_AsynGetGoogleFriendIdList(PineServiceHandle* hsvc, PineAccountIdList* account_list) {
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" UINT64 OS_GetGooglePlayId() {
	return 0;
}
extern "C" const char* OS_GetGooglePlayUserName() {
	return NULL;
}
extern "C" const char* OS_GetGooglePlayAvatarUrl() {
	return NULL;
}
extern "C" void OS_AsynGooglePlayPost(PineServiceHandle* hsvc, const char* msg){
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynGooglePlayPostScore(PineServiceHandle* hsvc, BoardScoreItem item) {
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynGooglePlayGetLeaderBoardRank(PineServiceHandle* hsvc, const char * board_id, int * current_rank){
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynGooglePlayShowLeaderBoard(PineServiceHandle* hsvc, const char* board_id) {
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynGooglePlayShowAchievements(PineServiceHandle* hsvc) {
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynGooglePlayUnlockAchievement(PineServiceHandle* hsvc, const char* achi_id){
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynGoogleUpdateCloudSave(PineServiceHandle* hsvc, const char* localSaveName)
{
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynShareScreenShotIntent(PineServiceHandle* hsvc, const char* msg)
{
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynSendEmailIntent(PineServiceHandle* hsvc, const char* title, const char* body)
{
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void	 OS_AsynPostTwitter(PineServiceHandle* hsvc, const char* msg)
{
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}

extern "C" Orientation GetViewOrientation()
{
	return Orientation::Portrait;
}

extern "C" void OS_CaptureScreen()
{

}
extern "C" bool chartboostHasMoreGame()
{
	return false;
}

extern "C" void OS_ShowAskNotify()
{

}

extern "C" void OS_ShowBannerInit(){}
extern "C" void OS_ShowBannerAds(){}
extern "C" void OS_ShowBannerDissmiss(){}