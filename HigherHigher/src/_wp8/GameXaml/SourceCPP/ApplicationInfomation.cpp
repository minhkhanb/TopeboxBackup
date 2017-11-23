#include "pch.h"
#include "ApplicationInfomation.h"
#include "Environment.h"
////#include "../../../game/WindowsPhoneCore/PineDevice.h"
#include "WP8_Connector.h"

using namespace PhoneDirect3DXamlAppComponent;
void ApplicationInfomation::onActive(){
	/*if(Environment::GetInstace()->_game._game != NULL){
		Environment::GetInstace()->_game._game->_g->OnChangeContext();
		Environment::GetInstace()->_game._game->Resume();
	}*/
	WP8_OnAppActive();
}

void ApplicationInfomation::onDeactive(){
	////if(Environment::GetInstace()->_game._game != NULL){
	////	Environment::GetInstace()->_game._game->Pause();
	////}
	////PineDevice::SetContext(NULL);
	////PineDevice::SetDevice(NULL);
	WP8_OnAppDeActive();
}
void ApplicationInfomation::onLoginFacebook(){
	WP8_OnLoginFacebook();
}
void ApplicationInfomation::onClosing(){
	WP8_OnAppClosing();
}
void ApplicationInfomation::onPopupOn(){
	WP8_OnPopupOn();
}
void ApplicationInfomation::onPopupOff(){
	WP8_OnPupupOff();
}
#if defined(_DEBUG_SUPPORT)
void ApplicationInfomation::onSupport(int add_money, int add_gem, int add_exp){
	WP8_OnSupport(add_money, add_gem, add_exp);
}
void ApplicationInfomation::onNeedSupportUpload(){
	WP8_OnSupportNeedUploadData();
}
#endif