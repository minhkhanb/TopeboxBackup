#include "pch.h"
#include <Define\Defines.h>
#include "../../../game/Lib/DataType/CType.h"
class PineSocial;
#if defined(BUILD_SUPPORT)

extern "C" bool support_sand_boxed;
extern "C" char* support_account_id;
extern "C" PineAccountType support_account_type;
extern "C" PineAccountInfo support_account;

#endif


extern "C" void OS_setMusicVolume(float vol)
{
	
}
//////////////////////////////////////
///GAMECENTER
//////////////////////////////////////
extern "C" void InitGCBoard()
{

}
extern "C" int getGCErrorCode()
{
	return -1009;
}

extern "C" void loadGCAvatarPhoto()
{
}

extern "C" bool HasGCAuthenticated()
{
#if defined(BUILD_SUPPORT)
	if (support_sand_boxed)
	{
		return support_account_type == PineAccountType::GameCenter;
	}
#endif

	return false;
}
extern "C" const char* GetGCID(){
#if defined(BUILD_SUPPORT)
	if (support_sand_boxed && support_account_type == PineAccountType::GameCenter)
	{
		return support_account.StrId;
	}
#endif
	return NULL;
}
extern "C" const char* GetGCName()
{
#if defined(BUILD_SUPPORT)
	if (support_sand_boxed)
	{
		if (support_account_type == PineAccountType::GameCenter)
		{
			return support_account.Name;
		}
	}
#endif
	return NULL;
}

extern "C" INT_TIME GetGCIDNum()
{
	return 0;
}

extern "C" bool HasCompleteLoadGCAvatar()
{
	return true;
}

extern "C" bool isGCAvailable()
{
#if defined(BUILD_SUPPORT)
	if (support_sand_boxed)
	{
		return support_account_type == PineAccountType::GameCenter;
	}
#endif
	return false;
}

extern "C" bool saveGCAvatar()
{
	return false;
}

extern "C" void GetGCFriends(){}
extern "C" void showGCLeaderBoard(){}
extern "C" void GCSubmitScore(INT_TIME score, const char* category_id){}
extern "C" void GCSubmitAchievement(const char* identifier, int percentComplete){}
extern "C" void showGCAchievementBoard(){}
extern "C" void ReturnGL(){}
extern "C" void OpenURLInSafari(char* url){}
extern "C" void OpenAppStore(int appleID){}

extern "C" void OS_ShowPicLibrary(){}
extern "C" void OS_ShowCamera(){}
extern "C" void OS_DisableCamera(){}
extern "C" void OS_CaptureCamera(){}
extern "C" void OS_SwitchCamera(){}
extern "C" void OS_TextInputStart(char* textBuf, unsigned int bufSize, int x, int y, int w, int h){

}
extern "C" void OS_TextInputClose(){

}
extern "C" void OS_PostScreenToFacebook(const char* feed){}
extern "C" bool OS_PingToHost(const char* url, int time_out){ return true; }
extern "C" bool OS_IsDeviceHacked(){ return false; }
extern "C" void OS_SystemWarning(const char* feed){}
extern "C" void OS_PostScreenToTwitter(const char* feed){}
extern "C" void OS_FacebookInviteFriend(){}
extern "C" void OS_PostInviteToTwitter(const char* feed){}
extern "C" void OS_Popup_Message(const char* body, const char* title){}
extern "C" void OS_Popup_Mail(const char* body, const char* title){}
extern "C" void OS_LoadImageToPngFile(const char* url, const char *file_name, float width_target, float height_target){}
extern "C" void OS_PostImageToFacebook(const char* feed, const char* realUrl){}
extern "C" void	OS_InitTwitter(){}
extern "C" void OS_PostImageToTwitter(const char* feed, const char* realUrlImage){}

extern "C" void FBLoadMe(){}

extern "C" void OS_DisableReturnKey(bool disable){}
extern "C" void OS_ClearTextBuffer(){}
extern "C" void chartboostDownloadConfig(){}

extern "C" void OS_SystemMessageAsk(const char* feed, int code){}

extern "C" void OS_GameCenterAuthentication(PineSocial* social){}
extern "C" void OS_GameCenterLoadScore(PineSocial* social){}
extern "C" void OS_GameCenterSubmitScore(INT_TIME value, const char* leaderboard){}
extern "C" void OS_ShowLeaderboard(const char*  leaderboard){}
extern "C" void OS_ShowLeaderAchivement(){}
extern "C" void OS_GameCenterLoadFriend(PineSocial* social){}
extern "C" void OS_GameCenterSubmitAchievement(int percent, const char* leaderboard, bool noti){}
extern "C" void OS_ResetAllAchievement(){}
extern "C" void OS_GameCenterLoadAchievement(){}



extern "C" bool OS_IsLoginGooglePlay()
{
	return false;
}
extern "C" void	OS_AsynLoginGooglePlay(PineServiceHandle* hsvc)
{
	hsvc->ResponseStatus = 1;
	hsvc->Status = PineServiceResult::Responsed;
}
extern "C" UINT64 OS_GetGooglePlayId()
{
	return 0;
}
extern "C" const char* OS_GetGooglePlayUserName()
{
	return NULL;
}
extern "C" const char* OS_GetGooglePlayAvatarUrl()
{
	return NULL;
}
extern "C" void OS_AsynGooglePlayPost(PineServiceHandle* hsvc, const char* msg)
{
	hsvc->ResponseStatus = 1;
	hsvc->Status = PineServiceResult::Responsed;
}
extern "C" void OS_AsynGooglePlayPostScore(PineServiceHandle* hsvc, BoardScoreItem item)
{
	hsvc->ResponseStatus = 1;
	hsvc->Status = PineServiceResult::Responsed;
}
extern "C" void OS_AsynGooglePlayShowLeaderBoard(PineServiceHandle* hsvc, const char* board_id)
{
	hsvc->ResponseStatus = 1;
	hsvc->Status = PineServiceResult::Responsed;
}
extern "C" void OS_AsynGooglePlayUnlockAchievement(PineServiceHandle* hsvc, const char* achi_id)
{
	hsvc->ResponseStatus = 1;
	hsvc->Status = PineServiceResult::Responsed;
}