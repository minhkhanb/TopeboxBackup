#include "pch.h"
#include "PineSocialAsync.h"
#include "../GameCore/Os_Functions.h"
void PineSocialAsync::RequestLoginAccount(PineServiceHandle* hsvc, PineAccountType account_type)
{
	if (account_type == PineAccountType::Facebook){
		//SVS_InitFaceBookAsRequire(hsvc);
	}
}

void PineSocialAsync::LoginAccount(PineServiceHandle* hsvc, PineAccountType account_type){
	if (account_type == PineAccountType::Facebook){
		OS_AsynLoginFacebook(hsvc);
		//PDEBUG("login facebook status:%d", hsvc->ResponseStatus);
	}
	else if (account_type == PineAccountType::Twitter){
		OS_AsynLoginTwitter(hsvc);
	}
	else if (account_type == PineAccountType::GooglePlay){
		OS_AsynLoginGooglePlay(hsvc, false);
	}
}

void PineSocialAsync::LoadAccountInfo(PineServiceHandle* hsvc, PineAccountType account_type, PineAccountInfo* info){
	if (account_type == PineAccountType::Facebook){
		if (OS_IsLogginFacebook()){
			sprintf(info->Name, "%s", OS_GetFacebookUserName());
			UINT64 id = OS_GetFacebookUserId();
			info->Id = 300000000000000000 + id;
			sprintf(info->AvatarUrl, "%s", OS_GetFacebookAvatarUrl());
			hsvc->ResponseStatus = 0;
		}
		else{
			hsvc->ResponseStatus = 1;
		}
		hsvc->Status = PineServiceResult::Responsed;
	}
	else if (account_type == PineAccountType::Twitter){
		if (OS_IsLogginTwitter()){
			sprintf(info->Name, "%s", OS_GetTwitterUserName());
			UINT64 id = OS_GetTwitterUserId();
			info->Id = 400000000000000000 + id;
			info->AvatarUrl[0] = '\0';
			hsvc->ResponseStatus = 0;
		}
		else{
			hsvc->ResponseStatus = 1;
		}
		hsvc->Status = PineServiceResult::Responsed;
	}
	else if(account_type == PineAccountType::GooglePlay){
		sprintf(info->Name, "%s", "Not Support");
		UINT64 id = 0;
		info->Id = 500000000000000000 + id;
		sprintf(info->AvatarUrl, "%s", "");
		hsvc->ResponseStatus = 0;
		hsvc->Status = PineServiceResult::Responsed;
	}
}

void PineSocialAsync::LoadIAP(PineServiceHandle* hsvc){
	OS_AsynLoadIAP(hsvc);
}

void PineSocialAsync::LoadFriends(PineServiceHandle* hsvc, PineAccountType account_type, PineAccountIdList* account_list){
	if (account_type == PineAccountType::Facebook){
		OS_AsynFacebookLoadFriendIdList(hsvc, account_list);
		PDEBUG("Load Friend Facebook\n");
	}
	else if (account_type == PineAccountType::Twitter){
		OS_AsynTwitterLoadFollowerIdList(hsvc, account_list);
	}
}

void PineSocialAsync::ShowLeaderBoard(PineServiceHandle* hsvc, PineAccountType account_type, const char* board_id){
	if(account_type == PineAccountType::Facebook){
		
	}
	else if(account_type == PineAccountType::Twitter){
		
	}
	else if(account_type == PineAccountType::GooglePlay){
		OS_AsynGooglePlayShowLeaderBoard(hsvc, board_id);
	}
}

void PineSocialAsync::Post(PineServiceHandle* hsvc,PineAccountType account_type, const char* msg){
	if(account_type == PineAccountType::Facebook){
		OS_AsynPostFacebook(hsvc, msg);
	}
	else if(account_type == PineAccountType::Twitter){
		OS_AsynPostTwitter(hsvc, msg);
	}
	else if(account_type == PineAccountType::GooglePlay){
		OS_AsynGooglePlayPost(hsvc, msg);
	}
}

void PineSocialAsync::Post(PineAccountType account_type, const char* msg)
{
	if(account_type == PineAccountType::Facebook){
		OS_PostFacebook(msg);
	}
	else if(account_type == PineAccountType::Twitter){
		
	}
	else if(account_type == PineAccountType::GooglePlay){
		
	}
}


void PineSocialAsync::UnlockAchievement(PineServiceHandle* hsvc,PineAccountType account_type, const char* achi_id){
	if(account_type == PineAccountType::Facebook){
		
	}
	else if(account_type == PineAccountType::Twitter){
		
	}
	else if(account_type == PineAccountType::GooglePlay){
		OS_AsynGooglePlayUnlockAchievement(hsvc, achi_id);
	}
}
//Bo add
void PineSocialAsync::PostNewScore(PineServiceHandle* hsvc, BoardScoreItem item){
	if (item.AccountType == PineAccountType::Facebook){
		OS_AsynFacebookPostScore(hsvc, item);
	}else if(item.AccountType == PineAccountType::Twitter){
		
	}else if(item.AccountType == PineAccountType::GooglePlay){
		OS_AsynGooglePlayPostScore(hsvc, item);
	}
}


#if defined(BUILD_SUPPORT)

void PineSocialAsync::AskSupportAccount(PineServiceHandle* hsvc)
{
	OS_AsyncSupportAskAccount(hsvc);
}

#endif