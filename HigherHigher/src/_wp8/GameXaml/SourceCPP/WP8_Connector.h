//This file declare some function header that game must be implement
//These function the system will call to notify to game that event coming.

#if !defined(_H_WP8_CONNECTOR)
#define _H_WP8_CONNECTOR
//WP8_AppClosing : Called when app begin to closing.
extern "C" void WP8_OnAppClosing();
//WP8_AppActive : Called when app resume, don't call when app open
extern "C" void WP8_OnAppActive();
//WP8_AppDeActive : Called when app pause, don't call when app close
extern "C" void WP8_OnAppDeActive();
//WP8_LoginFacebook: Called when user login to facebook.
extern "C" void WP8_OnLoginFacebook();
//
extern "C" void WP8_OnUpdateFacebookFriendRecord();
extern "C" void WP8_OnBuyInAppItemComplete(bool success, const char* item_id);
extern "C" void WP8_OnLoginFacebookFail();
extern "C" void WP8_OnBackPress();
extern "C" void WP8_OnPopupOn();
extern "C" void WP8_OnPupupOff();
extern "C" void WP8_OnUpdateInAppItem(const char* id, const char* price, int type);
#if defined(_DEBUG_SUPPORT)
extern "C" void WP8_OnSupport(int add_money, int add_gem, int add_exp);
extern "C" void WP8_OnSupportNeedUploadData();
#endif
extern "C" void WP8_PreRequestExtern(int handle, unsigned int request_code);
extern "C" void WP8_PostPequestExtern(int handle, unsigned int request_code);
#endif//_H_WP8_CONNECTOR