#include "pch.h"
#include <time.h>
#include <stdio.h>
#include <Assert.h>
#include <xaudio2.h>

#include "PineRenderer.h"
#include "Environment.h"
#include "DeviceInfomation.h"
#include "PlayerInfomation.h"
#include "ApplicationInfomation.h"
#include "WP8_Connector.h"
#include "Utils.h"
#include "ServerStream/ServerStreamManager.h"
#include "StreamBuffer.h"
#include <Lib\DataType\CByteBuffer.h>
using namespace PhoneDirect3DXamlAppComponent;

#if defined(BUILD_SUPPORT)
//support sand box
extern "C" bool support_sand_boxed = false;

extern "C" char* support_account_id = NULL;

extern "C" PineAccountType support_account_type = PineAccountType::Unknown;
extern "C" PineAccountInfo support_account = {};

extern "C" PineAccountType GEX_ImplementSupportSanboxAccount(const char* id, PineAccountInfo* info);

extern "C" void OS_AsyncSupportAskAccount(PineServiceHandle* svc)
{
	if (svc->isFree())
	{
		svc->Handle = ServerStreamManager::Request((int)PineServiceCode::SupportGetAccountId);
		svc->Status = PineServiceResult::WaitingResponse;
	}
	else
	{
		if (ServerStreamManager::isStreamComplete(svc->Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(svc->Handle);
			if (stream->ErrorCode == 0){

				StreamBuffer^ buff = ref new StreamBuffer(stream->ResponseData);
				Platform::String^ acc_id = buff->readSStr();

				if (support_account_id != NULL) delete[] support_account_id;

				support_account_id = (char*)Utils::ConvertStringToCharArray(acc_id);

				support_account_type = GEX_ImplementSupportSanboxAccount(support_account_id, &support_account);

				support_sand_boxed = true;

				svc->ResponseStatus = 0;
				svc->Status = PineServiceResult::Responsed;
			}
			else{
				support_sand_boxed = false;

				svc->ResponseStatus = 1;
				svc->Status = PineServiceResult::Responsed;
			}
		}
	}
}

extern "C" PineAccountType OS_SupportAccountType()
{
	return support_account_type;
}
#endif


////////////////////////////////////////////////////
///WORK WITH DATA FILE
////////////////////////////////////////////////////
extern "C" int OS_ReadAppData(const char* filename, char*& readed_data){
	FILE* stream = NULL;
	filename = Utils::getAbsoluteURL(filename, Utils::DATA_PATH);
	stream = fopen(filename, "rb");
	if (stream != NULL)	{
		fseek(stream, 0, SEEK_END);
		int size = ftell(stream);
		fseek(stream, 0, SEEK_SET);
		//readed_data = new char[size];
		fread(readed_data, sizeof(char), size, stream);		
		fclose(stream);
		return size;
	}
	return 0;
}
extern "C" Buffer* OS_LoadAppData(const char* filename){
	FILE* stream = NULL;
	filename = Utils::getAbsoluteURL(filename, Utils::DATA_PATH);
	stream = fopen(filename, "rb");
	if (stream != NULL)	{
		fseek(stream, 0, SEEK_END);
		int size = ftell(stream);
		fseek(stream, 0, SEEK_SET);
		Buffer* buff = new Buffer(size);
		fread(buff->Data(), sizeof(char), size, stream);
		fclose(stream);
		return buff;
	}
	return NULL;
}
extern "C" bool OS_SaveAppData(const char* filename, const char* data_to_save, int data_length, bool is_append)
{
	filename = Utils::getAbsoluteURL(filename, Utils::DATA_PATH);
	FILE* stream = NULL;
	if (is_append){
		stream = fopen(filename, "ab");
	}
	else{
		stream = fopen(filename, "wb");
	}
	if (stream != NULL){
		fwrite(data_to_save, sizeof(BYTE), data_length, stream);
		fclose(stream);
	}
	else{
		return false;
	}
	return true;
}
extern "C" bool OS_DeleteAppData(const char* filename)
{
	return 0;
}
extern "C" char* OS_GetResourceFilePath(const char* filename){
	return Utils::getAbsoluteURL(filename, Utils::RESOURCE_PATH);
}

extern "C" char* OS_GetDocumentFilePath(const char* filename)
{
	return Utils::getAbsoluteURL(filename, Utils::DATA_PATH);
}

extern "C" void OS_DeleteFile(const char* filepath){
	Utils::DeleteFile(filepath);
}

extern "C" unsigned long OS_GetTime(void){
	return clock();
}
extern "C" void CheckLockRotate(){
}

/////////PRIVATE VARIABLE////////////////////
//DON't USE IT OUT SIDE
/////////////////////////////////////////////
char* app_id = NULL;
char* user_id = NULL;
UINT64 id_waiting[1000];
int id_waiting_count = 0;

/////////WP8_UTIL_FUNCTION////////////////////
//THESE FUNCTION DECLARE IN WP8_UTILS.H
//Please include WP8_utils.h to use them outside
/////////////////////////////////////////////


//data_stream_status: to hold status of data stream between c# c++
//0: ready for transaction
//1: server not found data of user
//2: server loaded data
//3: client(c++) uploaded data to stream

//OS_GetUserId: get user id of current player
//player only have user id if he loged in to facebook
extern "C" const char* OS_GetUserId(){
	if(user_id != NULL){
		delete[] user_id;
		user_id = NULL;
	}
	if(PhoneDirect3DXamlAppComponent::PlayerInfomation::UserId != nullptr){
		user_id = Utils::ConvertStringToCharArray(PhoneDirect3DXamlAppComponent::PlayerInfomation::UserId);
		return (const char*) user_id;
	}
	return NULL;
}


extern "C" void OS_FacebookConnect(){
	
}
extern "C" void OS_FacebookLoadFriend(){
	
}
//OS_FacebookFeed: post to user's facebook wall.
//if user not loged in system will change to login page
extern "C" void OS_FacebookFeed(){
}
extern "C" void OS_FacebookLogout(){
}
//OS_FacebookGetAppUser: send a request to get friend
//after load friends whose play the game. system request server to get the users who have saved data
//and auto syn with offline data
extern "C" void OS_FacebookGetAppUser(){
}
extern "C" void OS_TwitterConnect(){
}
extern "C" void OS_TwitterFeed(){
}
extern "C" void OS_TwitterLogout(){
}

extern "C" bool OS_IsNetworkAvailable(){
	return PhoneDirect3DXamlAppComponent::DeviceInfomation::IsNetworkAvailable;
}
extern "C" void OS_Exit(bool sudden){
	//PineRenderer::shareInstance()->CallBaseService(CONNECTOR_GAME_EXIT,NULL,0);
	//SERVER_REQUEST_CODE_GAME_EXIT
	StreamBuffer^ buf = ref new StreamBuffer(4);
	if (sudden){
		buf->writeInt32(1);
	}
	else{
		buf->writeInt32(0);
	}
	ServerStreamManager::Request(SERVER_REQUEST_CODE_GAME_EXIT, buf->Data, true);
}
#if defined (_WINDOWS_PHONE8)
extern "C" void	OS_AsyncShareLink(PineServiceHandle* hsvc, const char * title, const char * url, const char * message)
{
	if (hsvc->isFree())
	{
		Platform::String^ strTitle = Utils::ConvertCharToString(title);
		Platform::String^ strUrl = Utils::ConvertCharToString(url);
		Platform::String^ strMessage = Utils::ConvertCharToString(message);
		int size = 2 + (2 * strTitle->Length()) + 2 + (2 * strUrl->Length()) + 2 + (2 * strMessage->Length());
		StreamBuffer^ buf = ref new StreamBuffer(size);
		buf->writeSStr(strTitle);
		buf->writeSStr(strUrl);
		buf->writeSStr(strMessage);		
		hsvc->Status = PineServiceResult::WaitingResponse;
		ServerStreamManager::Request(SERVER_REQUEST_CODE_SHARE_LINK_TASK, buf->Data);
	}
	else if (hsvc->Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(hsvc->Handle)) {
			if (!ServerStreamManager::isError(hsvc->Handle)) {
				hsvc->ResponseStatus = 0;
			}
			else {
				hsvc->ResponseStatus = 1;
			}
			hsvc->Status = PineServiceResult::Responsed;
			ServerStreamManager::freeStream(hsvc->Handle);
		}
	}
}

extern "C" void	OS_AsyncShareMedia(PineServiceHandle* hsvc, const char * fileName)
{
	if (hsvc->isFree())
	{
		Platform::String^ strFileName = Utils::ConvertCharToString(fileName);
		int size = 2 + (2 * strFileName->Length());
		StreamBuffer^ buf = ref new StreamBuffer(size);
		buf->writeSStr(strFileName);
		hsvc->Status = PineServiceResult::WaitingResponse;
		ServerStreamManager::Request(SERVER_REQUEST_CODE_SHARE_MEDIA_TASK, buf->Data);
	}
	else if (hsvc->Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(hsvc->Handle)) {
			if (!ServerStreamManager::isError(hsvc->Handle)) {
				hsvc->ResponseStatus = 0;
			}
			else {
				hsvc->ResponseStatus = 1;
			}
			hsvc->Status = PineServiceResult::Responsed;
			ServerStreamManager::freeStream(hsvc->Handle);
		}
	}
}
#endif
extern "C" void OS_FacebookFeedMessage(const char* message){
	
}
extern "C" void OS_TwitterFeedMessage(const char* message){
	
}

extern "C" void OS_SMSSend()
{
	PineRenderer::getInstance()->CallBaseService(CONNECTOR_SMS_SEND, NULL, 0);
}

extern "C" void OS_MailSend()
{
	PineRenderer::getInstance()->CallBaseService(CONNECTOR_MAIL_SEND, NULL, 0);
}

extern "C" void OS_FacebookGotoFanpage()
{
	PineRenderer::getInstance()->CallBaseService(CONNECTOR_FB_GOTO_FANPAGE, NULL, 0);
}

extern "C" void OS_TwitterGoToFanpage()
{
	PineRenderer::getInstance()->CallBaseService(CONNECTOR_TW_GOTO_FANPAGE, NULL, 0);
}

extern "C" void OS_SendSMS(const char* title, const char* content)
{
	Platform::String^ strtitle = Utils::ConvertCharToString(title);
	Platform::String^ strcontent = Utils::ConvertCharToString(content);
	int size = 2 +2 + (2 * strtitle->Length()) + 2 + (2 * strcontent->Length());
	StreamBuffer^ buf = ref new StreamBuffer(size);
	buf->writeInt16(1);
	buf->writeSStr(strtitle);
	buf->writeSStr(strcontent);
	ServerStreamManager::Request(SERVER_REQUEST_CODE_SMS_SEND, buf->Data);
}

extern "C" void OS_SendMail(const char* title, const char* content)
{
	Platform::String^ strtitle = Utils::ConvertCharToString(title);
	Platform::String^ strcontent = Utils::ConvertCharToString(content);

	int size = 2 + 2 + (2 * strtitle->Length()) + 2 + (2 * strcontent->Length());
	StreamBuffer^ buf = ref new StreamBuffer(size);
	buf->writeInt16(1);
	buf->writeSStr(strtitle);
	buf->writeSStr(strcontent);
	ServerStreamManager::Request(SERVER_REQUEST_CODE_MAIL_SEND, buf->Data);
}
extern "C" void OS_BrowserWebPage(const char* url)
{
	Platform::String^ urlstr = Utils::ConvertCharToString(url);
	int size = 2 + (2*urlstr->Length());
	StreamBuffer^ buf = ref new StreamBuffer(size);
	buf->writeSStr(urlstr);
	ServerStreamManager::Request(SERVER_REQUEST_CODE_OPEN_BROWSER,buf->Data);
}
extern "C" void OS_Rating()
{//open rating page

	int size = 4;
	StreamBuffer^ buf = ref new StreamBuffer(size);
	buf->writeInt32(0);
	ServerStreamManager::Request(SERVER_REQUEST_CODE_RATING,buf->Data);
}
extern "C" void OS_RatingWithDialog(const char* title, const char* message)
{//show dialog and open rating page if user choice yes.

	Platform::String^ titlestr = Utils::ConvertCharToString(title);
	Platform::String^ messagestr = Utils::ConvertCharToString(message);
	int size = 4 + 2 + (2*titlestr->Length()) + 2 + (2 * messagestr->Length());
	StreamBuffer^ buf = ref new StreamBuffer(size);
	buf->writeInt32(1);
	buf->writeSStr(titlestr);
	buf->writeSStr(messagestr);
	ServerStreamManager::Request(SERVER_REQUEST_CODE_RATING,buf->Data);
}

extern "C" const char* OS_GetAppId()
{
	if(app_id == NULL){
		if(PhoneDirect3DXamlAppComponent::ApplicationInfomation::AppId != nullptr){
			app_id = Utils::ConvertStringToCharArray(PhoneDirect3DXamlAppComponent::ApplicationInfomation::AppId);
		}
	}
	return (const char*)app_id;
}

extern "C" void OS_NotificationSetRun()
{
	PineRenderer::getInstance()->CallBaseService(CONNECTOR_NOTIFICATION_RUN,NULL,0);
}

extern "C" void OS_NotificationSetStop()
{
	PineRenderer::getInstance()->CallBaseService(CONNECTOR_NOTIFICATION_STOP,NULL,0);
}

char* private_region_short = NULL;
extern "C" const char* OS_GetRegionShort()
{
	if(private_region_short == NULL)
	{
		if(PhoneDirect3DXamlAppComponent::DeviceInfomation::RegionFormatShort != nullptr)
		{
			private_region_short = Utils::ConvertStringToCharArray(PhoneDirect3DXamlAppComponent::DeviceInfomation::RegionFormatShort);
		}
	}
	return private_region_short;
}

extern "C" void	OS_AsynLoadImageToPngFile(PineServiceHandle* hsvc, const char* url, const char *file_name, float width_target, float height_target)
{
	if (hsvc->isFree())
	{
		Platform::String^ str_url = Utils::ConvertCharToString(url);
		Platform::String^ str_fname = Utils::ConvertCharToString(file_name);
		int size = 2 
			+ 2 + (2 * str_url->Length())
			+ 2 + (2 * str_fname->Length())
			+ 4 + 4;
		StreamBuffer^ buff = ref new StreamBuffer(size);
		buff->writeInt16(1);
		buff->writeSStr(str_url);
		buff->writeSStr(str_fname);
		buff->writeInt32(width_target);
		buff->writeInt32(height_target);
		hsvc->Status = PineServiceResult::WaitingResponse;
		hsvc->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_LOAD_IMAGE, buff->Data);

	}
	else if (hsvc->Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(hsvc->Handle)){
			if (!ServerStreamManager::isError(hsvc->Handle)){
				hsvc->ResponseStatus = 0;
			}
			else{
				hsvc->ResponseStatus = 1;
			}
			hsvc->Status = PineServiceResult::Responsed;
			ServerStreamManager::freeStream(hsvc->Handle);
		}
	}
}

extern "C" void OS_AsynDownloadFile(PineServiceHandle* hsvc, const char* url, const char *file_name)
{
	if (hsvc->isFree())
	{
		Platform::String^ str_url = Utils::ConvertCharToString(url);
		Platform::String^ str_fname = Utils::ConvertCharToString(file_name);

		int size = 2
			+ 2 + (2 * str_url->Length())
			+ 2 + (2 * str_fname->Length());

		StreamBuffer^ buff = ref new StreamBuffer(size);
		buff->writeInt16(1);
		buff->writeSStr(str_url);
		buff->writeSStr(str_fname);

		hsvc->Status = PineServiceResult::WaitingResponse;
		hsvc->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_LOAD_FILE, buff->Data);
	}
	else if (hsvc->Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(hsvc->Handle)){
			if (!ServerStreamManager::isError(hsvc->Handle)){
				hsvc->ResponseStatus = 0;
			}
			else{
				hsvc->ResponseStatus = 1;
			}
			hsvc->Status = PineServiceResult::Responsed;
			ServerStreamManager::freeStream(hsvc->Handle);
		}
	}
}
extern "C" bool	OS_IsDocumentFileExist(const char* filename){
	char* filepath = OS_GetDocumentFilePath(filename);
	FILE* file = fopen(filepath, "rb");
	delete[] filepath;
	bool exist = false;
	if (file != NULL){
		exist = true;
		fclose(file);
	}
	return exist;
}
//call when begin processing Request
extern "C" void ServerStreamPreRequest(int handle, uint32 request_code){
	switch (request_code)
	{
	case SERVER_REQUEST_CODE_FB_LOGIN:
	case SERVER_REQUEST_CODE_TW_LOGIN:
		CGame::_this->addEvent(EVENT_FRAME_BREAK, NULL, 0);
		break;
	case SERVER_REQUEST_CODE_FB_FEED_ME:
		break;
	default:
		WP8_PreRequestExtern(handle, request_code);
		break;
	}
}
//call when Request have been processed
extern "C" void ServerStreamPostRequest(int handle, uint32 request_code){
	switch (request_code)
	{
	case SERVER_REQUEST_CODE_FB_LOGIN:
	case SERVER_REQUEST_CODE_TW_LOGIN:
		CGame::_this->addEvent(EVENT_FRAME_CONTINUE, NULL, 0);
		break;
	default:
		WP8_PostPequestExtern(handle, request_code);
		break;
	}
}
extern "C" void OS_AsynAdmobInterstitialPrepare(PineServiceHandle* svc, bool auto_show, const char* ad_id){
	if(svc->isFree()){
		Platform::String^ str_ad_id = Utils::ConvertCharToString(ad_id);
		int buff_size = 2 + 2 + (2 * str_ad_id->Length());
		StreamBuffer^ buff = ref new StreamBuffer(buff_size);
		buff->writeInt16(auto_show? 1: 0);
		buff->writeSStr(str_ad_id);
		svc->Handle = ServerStreamManager::Request((int)PineServiceCode::AdmobInterstitialPrepare, buff->Data, auto_show);
		if (auto_show){
			svc->Status = PineServiceResult::Responsed;
		}
		else{
			svc->Status = PineServiceResult::WaitingResponse;
		}
	}else{
		if (ServerStreamManager::isStreamComplete(svc->Handle)){
			ServerStream^ stream = ServerStreamManager::getStream(svc->Handle);
			if (stream != nullptr && stream->ErrorCode == 0){
				svc->ResponseStatus = 0;
				svc->Status = PineServiceResult::Responsed;
			}
			else{
				svc->ResponseStatus = 1;
				svc->Status = PineServiceResult::Responsed;
			}
		}
	}
}
extern "C" void OS_AsynAdmobInterstitialShow(PineServiceHandle* svc){
	if(svc->isFree()){
		svc->Handle = ServerStreamManager::Request((int)PineServiceCode::AdmobInterstitialShow, nullptr, true);
		svc->Status = PineServiceResult::Responsed;
	}
}

///////////////////////////////////////////////////
///CHARTBOOST
///////////////////////////////////////////////////
extern "C" void OS_AsynCBInterstitialPrepare(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus =1;
}
extern "C" void OS_AsynCBInterstitialShow(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 1;
}

extern "C" void OS_AsynCBRewardedVideoPrepare(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynCBRewardedVideoShow(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 1;
}

///////////////////////////////////////////////////
///UNITY ADS
///////////////////////////////////////////////////
extern "C" void OS_AsynUnityAdsInterstitialPrepare(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynUnityAdsInterstitialShow(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 1;
}

extern "C" void OS_AsynUnityAdsRewardedVideoPrepare(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynUnityAdsRewardedVideoShow(PineServiceHandle* hsvc)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 1;
}

extern "C" void OS_CaptureScreen(){
	if (Environment::GetInstace() != NULL){
		Environment::GetInstace()->CaptureScreen();
	}
}

extern "C" void OS_AsynCaptureScreen(PineServiceHandle* svc, PineGraphicComplexOrientation rotate){
	if (Environment::GetInstace() != NULL){
		Environment::GetInstace()->AsynCaptureScreen(svc);
	}
	else{
		svc->ResponseStatus = 0;
		svc->Status = PineServiceResult::Responsed;
	}
}

extern "C" bool OS_EnableScaleView()
{
	return false;
}
extern "C" bool OS_ScaleView(float scalex, float scaley)
{
	return false;
}

extern "C" double			OS_GetAccelX() { return 0; }
extern "C" double			OS_GetAccelY() { return 0; }
extern "C" double			OS_GetAccelZ() { return 0; }

extern "C" void OS_FacebookAppInvite(const char* applink, const char* preview_img_url)
{

}

extern "C" void OS_AsynGetAvatarFacebook(PineServiceHandle* hsvc, const char* userURL, const char* fileName, int width, int height)
{
	hsvc->Status = PineServiceResult::Responsed;
	hsvc->ResponseStatus = 1;
}

