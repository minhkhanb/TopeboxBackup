#include "pch.h"
#include "ApplicationInfomation.h"
#include "Environment.h"
////#include "WP8_Connector.h"

using namespace Pine;
void ApplicationInfomation::onActive(){
	////WP8_OnAppActive();
}

void ApplicationInfomation::onDeactive(){
	////WP8_OnAppDeActive();
}
void ApplicationInfomation::onLoginFacebook(){
	////WP8_OnLoginFacebook();
}
void ApplicationInfomation::onClosing(){
	////WP8_OnAppClosing();
}
void ApplicationInfomation::onPopupOn(){
	////WP8_OnPopupOn();
}
void ApplicationInfomation::onPopupOff(){
	////WP8_OnPupupOff();
}
void ApplicationInfomation::onKeyDown(int keycode){
	if (Environment::GetInstance() != NULL){
		char key = keycode;
		Environment::GetInstance()->_game.addEvent(EVENT_KEYDOWN, &key, 1);
	}
}
void ApplicationInfomation::onKeyUp(int keycode){
	if (Environment::GetInstance() != NULL){
		char key = keycode;
		Environment::GetInstance()->_game.addEvent(EVENT_KEYUP, &key, 1);
	}
}
#if defined(_DEBUG_SUPPORT)
void ApplicationInfomation::onSupport(int add_money, int add_gem, int add_exp){
	WP8_OnSupport(add_money, add_gem, add_exp);
}
void ApplicationInfomation::onNeedSupportUpload(){
	WP8_OnSupportNeedUploadData();
}

#endif