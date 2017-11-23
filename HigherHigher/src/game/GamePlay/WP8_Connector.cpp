#include "pch.h"
//This file declare some function header that game must be implement
//These function the system will call to notify to game that event coming.
#include "../GameCore/CGame.h"
#include "../GameCore/Os_Functions.h"
//WP8_AppClosing : Called when app begin to closing.
extern "C" void WP8_OnAppClosing(){
	if(GAME()->_GameState > k_GS_LOGO){
		//GAME()->PushAllNotify();
		//OS_NotificationSetRun();
	}
}
//WP8_AppActive : Called when app resume, don't call when app open
extern "C" void WP8_OnAppActive(){
	if(GAME()->_GameState > k_GS_LOGO){
		//OS_NOTIFY().Reset_Notify();
    }
}
//WP8_AppDeActive : Called when app pause, don't call when app close
extern "C" void WP8_OnAppDeActive(){
}
//WP8_LoginFacebook: Called when user login to facebook.
extern "C" void WP8_OnLoginFacebook(){
	//if(GAME()->_GameState > k_GS_LOGO){
		//GAME()->SetGameState(k_GS_LOGO, false);
	//}
}
extern "C" void WP8_OnBuyInAppItemComplete(bool success, const char* item_id){
	//for(int i = 0;i< TOTAL_IAP_IN_GAME; i++){
	//	if(strcmp(GAME()->_IAP_INFO[i], item_id) == 0){
	//		GAME()->IAP_Complete(i,success);
	//	}
	//}
}
extern "C" void WP8_OnLoginFacebookFail(){
	//GAME()->_ask_friend_loading = false;
	//GAME()->_challange_friend_loading = false;
}
extern "C" void WP8_OnBackPress(){
	//GAME()->BackToSceen();
	GAME()->OnBackKeyPress();
}

extern "C" void WP8_OnPopupOn(){
	//GAME()->_isPause = true;
}
extern "C" void WP8_OnPupupOff(){
	//GAME()->_isPause = false;
}
extern "C" void WP8_OnUpdateInAppItem(const char* id, const char* price, int type)
{
	
}
extern "C" void WP8_PreRequestExtern(int handle, unsigned int request_code){

}
extern "C" void WP8_PostPequestExtern(int handle, unsigned int request_code){

}
