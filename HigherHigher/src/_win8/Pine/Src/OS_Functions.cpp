#include "pch.h"
#include "../../game/GameCore/CGame.h"
#include "ServerStream\ServerStreamManager.h"
#include "DeviceInfomation.h"
#include "PlayerInfomation.h"
#include "StreamBuffer.h"
#include "ApplicationInfomation.h"
using namespace Pine;

////////////////////////////////////////////////////
///WORK WITH DATA FILE
////////////////////////////////////////////////////
extern "C" char* OS_GetResourceFilePath(const char* filename)
{

	Windows::ApplicationModel::Package^ package = Windows::ApplicationModel::Package::Current;
	Windows::Storage::StorageFolder^ installedLocation = package->InstalledLocation;
	char* installedpath = Utils::ConvertStringToCharArray(installedLocation->Path);

	char* path = new char[255];
	sprintf(path, "%s\\Assets\\%s", installedpath, filename);
	delete[] installedpath;
	return path;
}

extern "C" ResourceItem OS_GetResourceItem(const char* filename){
	ResourceItem resitem;
	char* respath = OS_GetResourceFilePath(filename);
	sprintf(resitem.Path, "%s", respath);
	delete[] respath;
	resitem.Offset = 0;
	resitem.Length = -1;
	return resitem;
}

extern "C" char*	OS_GetDocumentFilePath(const char* filename)
{

	Windows::Storage::StorageFolder^ local = Windows::Storage::ApplicationData::Current->LocalFolder;
	Platform::String^ path = local->Path;
	char* cpath = Utils::ConvertStringToCharArray(path);
	char* documentpath = new char[255];
	sprintf(documentpath, "%s\\%s", cpath, filename);
	return documentpath;
}

extern "C" bool OS_SaveAppData(const char* filename, const char* data_to_save, int data_length, bool is_append)
{
	char* path = OS_GetDocumentFilePath(filename);
	FILE* file = NULL;
	file = fopen(path, "wb");
	delete[] path;
	if (file != NULL){
		fwrite(data_to_save, 1, data_length, file);
		fclose(file);
		return true;
	}
	return false;
}
extern "C" int OS_ReadAppData(const char* filename, char* readed_data)
{
	char* path = OS_GetDocumentFilePath(filename);
	FILE* file = NULL;
	file = fopen(path, "rb");
	delete[] path;
	if (file != NULL){
		fseek(file, 0, SEEK_END);
		int fsize = ftell(file);
		fseek(file, 0, SEEK_SET);
		int readed = fread(readed_data,1,fsize, file);
		fclose(file);
		return readed;
	}
	return 0;
}
extern "C" Buffer* OS_LoadAppData(const char* filename)
{
	char* path = OS_GetDocumentFilePath(filename);
	FILE* file = NULL;
	file = fopen(path, "rb");
	delete[] path;
	if (file != NULL){
		fseek(file, 0, SEEK_END);
		int fsize = ftell(file);
		Buffer* buff = new Buffer(fsize);
		fseek(file, 0, SEEK_SET);
		int readed = fread(buff->Data(), 1, fsize, file);
		fclose(file);
		return buff;
	}
	return NULL;
}
extern "C" bool OS_DeleteAppData(const char* filename)
{
	return true;
}

extern "C" void OS_DeleteFile(const char* file_name)
{
}

////////////////////////////////////////////////////
///UTILITY
////////////////////////////////////////////////////
extern "C" unsigned long OS_GetTime(void){
	return clock();
}
extern "C" double OS_GetAccelX(){
	return 0;
}
extern "C" double OS_GetAccelY(){
	return 0;
}
extern "C" double OS_GetAccelZ(){
	return 0;
}

extern "C" bool OS_IsNetworkAvailable(){
	return DeviceInfomation::IsNetworkAvailable;
}

extern "C" void CheckLockRotate(){}
extern "C" void ReturnGL(){}
extern "C" void PurchaseProduct(const char *indentify){
	Platform::String^ id = Utils::ConvertCharToString(indentify);
	StreamBuffer^ buf = ref new StreamBuffer(2 + 2 * id->Length());
	buf->writeSStr(id);
	ServerStreamManager::Request(SERVER_REQUEST_CODE_PURCHASE, buf->Data);
}
extern "C" void OpenURLInSafari(char* url){}
extern "C" void OpenAppStore(INT_TIME appleID){}
extern "C" void OS_BrowserWebPage(const char* url){
	Platform::String^ urlstr = Utils::ConvertCharToString(url);
	int size = 2 + (2 * urlstr->Length());
	StreamBuffer^ buf = ref new StreamBuffer(size);
	buf->writeSStr(urlstr);
	ServerStreamManager::Request(SERVER_REQUEST_CODE_OPEN_BROWSER, buf->Data);
}//open web page in browser which depend on OS platform
extern "C" void OS_Rating(){}//open rating page
extern "C" void OS_RatingWithDialog(const char* title, const char* message){}//show dialog and open rating page if user choice yes.
extern "C" const char* OS_GetRegionShort(){ return "VN"; }//get Region format of phone. example: VN
extern "C" void OS_Exit(bool sudden){}
extern "C" void ShowRateDialog(){}
extern "C" void GoToReviewInItune(){}

extern "C" bool OS_EnableScaleView()
{
	return false;
}
extern "C" bool OS_ScaleView(float scalex, float scaley)
{
	return false;
}

////////////////////////////////////////////////////
///FACEBOOK
////////////////////////////////////////////////////
/////////PRIVATE VARIABLE////////////////////
//DON't USE IT OUT SIDE
/////////////////////////////////////////////
char* app_id = NULL;
char* user_id = NULL;
char* fb_username = NULL;
char* fb_id = NULL;
char* fb_avatar = NULL;

extern "C" void InitFaceBook()
{

}
extern "C" bool FBSessionValid()
{
	return true;
}
extern "C" void InitFaceBookAsRequire(){
}
extern "C" void SVS_InitFaceBookAsRequire(SVHANDLE* handle){
	if (handle->Handle == -1 && handle->Status == SVRESULT::SV_DATA_FREE){
		StreamBuffer^ buff = ref new StreamBuffer(2);
		buff->writeInt16(1);
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_FB_LOGIN, buff->Data);
		handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		handle->RequestCode = SERVER_REQUEST_CODE_FB_LOGIN;
	}
	else if (handle->Handle > -1){
		if (ServerStreamManager::isStreamComplete(handle->Handle)){
			if (!ServerStreamManager::isError(handle->Handle)){
				handle->Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else{
				handle->Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			ServerStreamManager::freeStream(handle->Handle);
			handle->Handle = -1;
		}
		else{
			handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}
extern "C" void SVS_InitFaceBook(SVHANDLE* handle){
	if (handle->Handle == -1 && handle->Status == SVRESULT::SV_DATA_FREE){
		//StreamBuffer^ buff = ref new StreamBuffer(2);
		//buff->writeInt16(0);
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_FB_LOGIN, nullptr);
		handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		handle->RequestCode = SERVER_REQUEST_CODE_FB_LOGIN;
	}
	else if (handle->Handle > -1){
		if (ServerStreamManager::isStreamComplete(handle->Handle)){
			if (!ServerStreamManager::isError(handle->Handle)){
				handle->Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else{
				handle->Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			ServerStreamManager::freeStream(handle->Handle);
			handle->Handle = -1;
		}
		else{
			handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}

extern "C" bool HasLoginFB(){
	return PlayerInfomation::LoginedFacebook;
}
extern "C" bool OS_IsLogginFacebook(){
	return PlayerInfomation::LoginedFacebook;
}
extern "C" int	getFBErrorCode(){
	return -1;
}
extern "C" void  loadFBAvatarPhoto(){
}
extern "C" const char* OS_GetFBUserId(){
	if (user_id != NULL){
		delete[] user_id;
		user_id = NULL;
	}
	if (PlayerInfomation::UserId != nullptr){
		user_id = Utils::ConvertStringToCharArray(PlayerInfomation::FacebookId);
		return (const char*)user_id;
	}
	return NULL;
}
extern "C" const char* OS_GetFBUserName(){
	if (fb_username != NULL){
		delete[] fb_username;
		fb_username = NULL;
	}
	if (PlayerInfomation::FacebookUserName != nullptr){
		//ConvertStringToCharArray return a non const char* 
		//we must delete after use.
		fb_username = (char*)Utils::ConvertStringToCharArray(PlayerInfomation::FacebookUserName);
		return (const char*)fb_username;
	}
	return NULL;
}

extern "C" void SVS_FBFeed(SVHANDLE* handle, const char* msg){
	if (handle->Handle == -1 && handle->Status == SVRESULT::SV_DATA_FREE){
		int buff_size = 2;
		Platform::String^ message = Utils::ConvertCharToString(msg);
		buff_size += 2 * message->Length();
		StreamBuffer^ buff = ref new StreamBuffer(buff_size);
		buff->writeSStr(message);
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_FB_FEED_ME, buff->Data);
		handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else if (handle->Handle > -1){
		if (ServerStreamManager::isStreamComplete(handle->Handle)){
			if (!ServerStreamManager::isError(handle->Handle)){
				handle->Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else{
				handle->Status = SVRESULT::SV_DATA_ERROR_UNKNOW;
			}
			ServerStreamManager::freeStream(handle->Handle);
			handle->Handle = -1;
		}
		else{
			handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}

extern "C" void OS_AsynPostFacebook(PineServiceHandle* hsvc, const char* msg){

	if (hsvc->isFree()){

		int buff_size = 2;
		Platform::String^ message = Utils::ConvertCharToString(msg);
		buff_size += 2 * message->Length();

		StreamBuffer^ buff = ref new StreamBuffer(buff_size);
		buff->writeSStr(message);

		hsvc->Handle = ServerStreamManager::Request((UINT32)PineServiceCode::FacebookPost, buff->Data);
		hsvc->Status = PineServiceResult::WaitingResponse;
	}

	else if (hsvc->Handle > -1){

		if (ServerStreamManager::isStreamComplete(hsvc->Handle)){

			if (!ServerStreamManager::isError(hsvc->Handle)){
				hsvc->ResponseStatus = 0;
				hsvc->Status = PineServiceResult::Responsed;
			}

			else{
				hsvc->ResponseStatus = 1;
				hsvc->Status = PineServiceResult::Responsed;
			}
			ServerStreamManager::freeStream(hsvc->Handle);
		}
	}
}
extern "C" void	OS_AsynFacebookPostScreen(PineServiceHandle* hsvc, const char* msg, PineGraphicComplexOrientation rotate) {

}
extern "C" void OS_AsynLoginFacebook(PineServiceHandle* handle){
	if (!OS_IsNetworkAvailable() && handle->Handle == -1){
		handle->ResponseStatus = 1;
		handle->Status == PineServiceResult::Responsed;
	}
	if (handle->isFree()){
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_FB_LOGIN, nullptr);
		handle->Status = PineServiceResult::WaitingResponse;
	}
	else if (handle->Handle > -1){
		if (ServerStreamManager::isStreamComplete(handle->Handle)){
			if (!ServerStreamManager::isError(handle->Handle)){
				handle->ResponseStatus = 0;
				handle->Status = PineServiceResult::Responsed;
			}
			else{
				handle->ResponseStatus = 1;
				handle->Status = PineServiceResult::Responsed;
			}
			ServerStreamManager::freeStream(handle->Handle);
		}
	}
}

extern "C" UINT64 OS_GetFacebookUserId(){
	if (fb_id != NULL){
		delete[] fb_id;
		fb_id = NULL;
	}
	if (PlayerInfomation::FacebookId != nullptr){
		//ConvertStringToCharArray return a non const char* 
		//we must delete after use.
		fb_id = (char*)Utils::ConvertStringToCharArray(PlayerInfomation::FacebookId);
		UINT64 id = 0;
		int len = strlen(fb_id);
		for (int i = 0; i < len; i++) id = id * 10 + (fb_id[i] - '0');
		return id;
	}
	return 0;
}

extern "C" const char* OS_GetFacebookUserName(){
	if (fb_username != NULL){
		delete[] fb_username;
		fb_username = NULL;
	}
	if (PlayerInfomation::FacebookUserName != nullptr){
		//ConvertStringToCharArray return a non const char* 
		//we must delete after use.
		fb_username = (char*)Utils::ConvertStringToCharArray(PlayerInfomation::FacebookUserName);
		return (const char*)fb_username;
	}
	return NULL;
}
extern "C" const char*	OS_GetFacebookAvatarUrl(){
	if (fb_avatar != NULL){
		delete[] fb_avatar;
		fb_avatar = NULL;
	}
	if (PlayerInfomation::FacebookUserName != nullptr){
		//ConvertStringToCharArray return a non const char* 
		//we must delete after use.
		fb_avatar = (char*)Utils::ConvertStringToCharArray(PlayerInfomation::FacebookAvatarUrl);
		return (const char*)fb_avatar;
	}
	return NULL;
}
extern "C" void	OS_AsynFacebookLoadFriendIdList(PineServiceHandle* handle, PineAccountIdList* account_list){
	if (handle->isFree()){
		handle->RequestCode = SERVER_REQUEST_CODE_FB_GET_FRIEND;
		handle->Status = PineServiceResult::WaitingResponse;
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_FB_GET_FRIEND, nullptr);
	}
	else{
		if (ServerStreamManager::isStreamComplete(handle->Handle)){
			if (!ServerStreamManager::isError(handle->Handle)){
				ServerStream^ stream = ServerStreamManager::getStream(handle->Handle);
				StreamBuffer^ buff = ref new StreamBuffer(stream->ResponseData);
				int account_num = buff->readInt32();
				if (account_num > 0){
					account_list->init(account_num);
					for (int a = 0; a < account_num; a++){
						account_list->setAccountAt(a, (UINT64)buff->readInt64());
					}
				}
				handle->ResponseStatus = 0;
				handle->Status = PineServiceResult::Responsed;
			}
			else{
				handle->ResponseStatus = 1;
				handle->Status = PineServiceResult::Responsed;
			}
			ServerStreamManager::freeStream(handle->Handle);
		}
	}
}

extern "C" void	OS_AsynFacebookPostScore(PineServiceHandle* handle, int score)
{
	if (!OS_IsNetworkAvailable() && handle->Handle == -1){
		handle->ResponseStatus = 1;
		handle->Status = PineServiceResult::Responsed;
	}
	if (handle->isFree()){
		int buff_size = 4;
		StreamBuffer^ buff = ref new StreamBuffer(buff_size);
		buff->writeInt32(score);
		handle->Handle = ServerStreamManager::Request((UINT32)PineServiceCode::FacebookPostScore, buff->Data);
		handle->Status = PineServiceResult::WaitingResponse;
	}
	else if (handle->Handle > -1){
		if (ServerStreamManager::isStreamComplete(handle->Handle)){
			if (!ServerStreamManager::isError(handle->Handle)){
				handle->Status = PineServiceResult::Responsed;
				handle->ResponseStatus = 0;
			}
			else{
				handle->Status = PineServiceResult::Responsed;
				handle->ResponseStatus = 1;
			}
			ServerStreamManager::freeStream(handle->Handle);
		}
	}
}

extern "C" SVRESULT OS_getInAppList(){
	return SVRESULT::SV_DATA_ERROR_NO_NETWORK;
}

extern "C" SVRESULT OS_InitFaceBookAsRequire(){
	return SVRESULT::SV_DATA_ERROR_NO_NETWORK;
}//require login to playgame
extern "C" void FaceBookPost(const char * msg)
{

}
extern "C" void FBLoadMe()
{
}
extern "C" void FBLoadListFriend()
{
}
extern "C" bool Check_FB_ID_list()
{
	return true;
}
extern "C" const char* GetFBName()
{
	return "Facebook User";
}

/*extern "C" void FBRequestToId(INT_TIME fbId)
{

}*/

extern "C" void FB_apprequests(const char * msg, const INT_TIME * arrayID, int count)
{

}
//twitter
extern "C" void InitTwitter()
{

}
extern "C" bool OS_IsLogginTwitter(){
	return false;
}
extern "C" void TwitterLoginDialog()
{

}
extern "C" UINT64		OS_GetTwitterUserId(){
	return 0;
}
extern "C" const char*	OS_GetTwitterUserName(){
	return NULL;
}
extern "C" void OS_AsynLoginTwitter(PineServiceHandle* hsvc){
	hsvc->ResponseStatus = 1;
	hsvc->Status == PineServiceResult::Responsed;
}
extern "C" void OS_AsynPostTwitter(PineServiceHandle* hsvc, const char* msg){
	hsvc->ResponseStatus = 1;
	hsvc->Status == PineServiceResult::Responsed;
}
extern "C" void TwitterPostTweet(const char * msg){
	int buff_size = 2;
	Platform::String^ message = Utils::ConvertCharToString(msg);
	buff_size += 2 * message->Length();
	StreamBuffer^ buff = ref new StreamBuffer(buff_size);
	buff->writeSStr(message);
	ServerStreamManager::Request(SERVER_REQUEST_CODE_TW_FEED, buff->Data, true);
}

int svstream_handler_initTwitter = -1;
extern "C" SVRESULT SVS_InitTwitter(){
	if (!OS_IsNetworkAvailable() && svstream_handler_initTwitter == -1){
		svstream_handler_initTwitter = -1;
		return SVRESULT::SV_DATA_ERROR_NO_NETWORK;
	}
	SVRESULT ret = SVRESULT::SV_DATA_ERROR_UNKNOW;
	if (svstream_handler_initTwitter == -1){
		svstream_handler_initTwitter = ServerStreamManager::Request(SERVER_REQUEST_CODE_TW_LOGIN, nullptr);
		ret = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else{
		if (ServerStreamManager::isStreamComplete(svstream_handler_initTwitter)){
			if (!ServerStreamManager::isError(svstream_handler_initTwitter)){
				ret = SVRESULT::SV_DATA_SUCCESS;
			}
			else{
				ret = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			ServerStreamManager::freeStream(svstream_handler_initTwitter);
			svstream_handler_initTwitter = -1;
		}
		else{
			ret = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
	return ret;
}
int svstream_handler_twfeed = -1;
extern "C" SVRESULT SVS_TWFeed(const char* msg){
	SVRESULT ret = SVRESULT::SV_DATA_ERROR_UNKNOW;
	if (!OS_IsNetworkAvailable() && svstream_handler_twfeed == -1){
		return SVRESULT::SV_DATA_ERROR_NO_NETWORK;
		svstream_handler_twfeed = -1;
	}
	if (svstream_handler_twfeed == -1){
		int buff_size = 2;
		Platform::String^ message = Utils::ConvertCharToString(msg);
		buff_size += 2 * message->Length();
		StreamBuffer^ buff = ref new StreamBuffer(buff_size);
		buff->writeSStr(message);
		svstream_handler_twfeed = ServerStreamManager::Request(SERVER_REQUEST_CODE_TW_FEED, buff->Data);
		ret = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else{
		if (ServerStreamManager::isStreamComplete(svstream_handler_twfeed)){
			if (!ServerStreamManager::isError(svstream_handler_twfeed)){
				ret = SVRESULT::SV_DATA_SUCCESS;
			}
			else{
				ret = SVRESULT::SV_DATA_ERROR_UNKNOW;
			}
			svstream_handler_twfeed = -1;
		}
		else{
			ret = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
	return ret;
}
extern "C" void	OS_AsynTwitterLoadFollowerIdList(PineServiceHandle* handle, PineAccountIdList* account_list){
	if (handle->isFree()){
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_TW_GET_FRIEND, nullptr);
		handle->RequestCode = SERVER_REQUEST_CODE_FB_GET_FRIEND;
		handle->Status = PineServiceResult::WaitingResponse;
	}
	else if (handle->Handle > -1){
		if (ServerStreamManager::isStreamComplete(handle->Handle)){
			if (!ServerStreamManager::isError(handle->Handle)){
				ServerStream^ stream = ServerStreamManager::getStream(handle->Handle);
				StreamBuffer^ buff = ref new StreamBuffer(stream->ResponseData);
				int account_num = buff->readInt32();
				if (account_num > 0){
					account_list->init(account_num);
					for (int a = 0; a < account_num; a++){
						account_list->setAccountAt(a, (UINT64)buff->readUInt64());
					}
				}
				handle->ResponseStatus = 0;
				handle->Status = PineServiceResult::Responsed;
			}
			else{
				handle->ResponseStatus = 1;
				handle->Status = PineServiceResult::Responsed;
			}
			ServerStreamManager::freeStream(handle->Handle);
		}
	}
}
//game center
extern "C" void GetGCFriends()
{
}

extern "C" bool Check_GC_ID_list()
{
	return true;
}

extern "C" bool isGCAvailable()
{
	return true;
}

extern "C" void InitGCBoard()
{

}

extern "C" int getGCErrorCode()
{
	return -2;
}

extern "C" const char* GetGCName()
{
	return "GC User";
}

extern "C" bool saveGCAvatar()
{
	return true;
}

extern "C" INT_TIME GetGCIDNum()
{
	return 2000000089000000000;
}

extern "C" bool HasGCAuthenticated()
{
	return true;
}

extern "C" void loadGCAvatarPhoto()
{

}

extern "C" bool HasCompleteLoadGCAvatar()
{
	return true;
}
extern "C" const char* OS_GetAppId(){
	if (app_id == NULL){
		if (Pine::ApplicationInfomation::AppId != nullptr){
			app_id = Utils::ConvertStringToCharArray(Pine::ApplicationInfomation::AppId);
		}
	}
	return (const char*)app_id;
}
extern "C" void OS_SMSSend(){
	//PineRenderer::shareInstance()->CallBaseService(CONNECTOR_SMS_SEND, NULL, 0);
}
extern "C" void OS_MailSend(){
	//PineRenderer::shareInstance()->CallBaseService(CONNECTOR_MAIL_SEND, NULL, 0);
}
extern "C" void OS_PostFacebook(const char* msg){
	int buff_size = 2;
	Platform::String^ message = Utils::ConvertCharToString(msg);
	buff_size += 2 * message->Length();
	StreamBuffer^ buff = ref new StreamBuffer(buff_size);
	buff->writeSStr(message);
	ServerStreamManager::Request(SERVER_REQUEST_CODE_FB_FEED_ME, buff->Data, true);
}
extern "C" void	OS_PostTwitter(const char* msg){
	int buff_size = 2;
	Platform::String^ message = Utils::ConvertCharToString(msg);
	buff_size += 2 * message->Length();
	StreamBuffer^ buff = ref new StreamBuffer(buff_size);
	buff->writeSStr(message);
	svstream_handler_twfeed = ServerStreamManager::Request(SERVER_REQUEST_CODE_TW_FEED, buff->Data, true);
}

extern "C" void OS_ShowPicLibrary(){}
extern "C" void OS_ShowCamera(){}
extern "C" void OS_DisableCamera(){}
extern "C" void OS_CaptureCamera(){}
extern "C" void OS_SwitchCamera(){}
extern "C" void OS_TextInputStart(char* textBuf, unsigned int bufSize, int x, int y, int w, int h){}
extern "C" void OS_TextInputClose(){}

extern "C" void OS_SendSMS(const char* title, const char* content){
	Platform::String^ strtitle = Utils::ConvertCharToString(title);
	Platform::String^ strcontent = Utils::ConvertCharToString(content);
	int size = 2 + 2 + (2 * strtitle->Length()) + 2 + (2 * strcontent->Length());
	StreamBuffer^ buf = ref new StreamBuffer(size);
	buf->writeInt16(1);
	buf->writeSStr(strtitle);
	buf->writeSStr(strcontent);
	ServerStreamManager::Request(SERVER_REQUEST_CODE_SMS_SEND, buf->Data);
}
extern "C" void OS_SendMail(const char* title, const char* content){
	Platform::String^ strtitle = Utils::ConvertCharToString(title);
	Platform::String^ strcontent = Utils::ConvertCharToString(content);
	int size = 2 + 2 + (2 * strtitle->Length()) + 2 + (2 * strcontent->Length());
	StreamBuffer^ buf = ref new StreamBuffer(size);
	buf->writeInt16(1);
	buf->writeSStr(strtitle);
	buf->writeSStr(strcontent);
	ServerStreamManager::Request(SERVER_REQUEST_CODE_MAIL_SEND, buf->Data);
}
extern "C" void	OS_AsynLoadImageToPngFile(SVHANDLE* handle, const char* url, const char *file_name, float width_target, float height_target){
	if (handle->Handle == -1 && handle->Status == SVRESULT::SV_DATA_FREE){
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
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_LOAD_IMAGE, buff->Data);
		handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else if (handle->Handle > -1){
		if (ServerStreamManager::isStreamComplete(handle->Handle)){
			if (!ServerStreamManager::isError(handle->Handle)){
				handle->Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else{
				handle->Status = SVRESULT::SV_DATA_ERROR_UNKNOW;
			}
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

extern "C" Orientation GetPhoneOrientation(){
	return Orientation::Portrait;
}
extern "C" Orientation GetViewOrientation(){
	return Orientation::Portrait;
}
extern "C" void OS_DisableReturnKey(bool disable){

}
extern "C" void OS_ClearTextBuffer(){

}

extern "C" void OS_AsynAdmobInterstitialPrepare(PineServiceHandle* svc, bool auto_show, const char* ad_id){
	if (svc->isFree()){
		Platform::String^ str_ad_id = Utils::ConvertCharToString(ad_id);
		int buff_size = 2 + 2 + (2 * str_ad_id->Length());
		StreamBuffer^ buff = ref new StreamBuffer(buff_size);
		buff->writeInt16(auto_show ? 1 : 0);
		buff->writeSStr(str_ad_id);
		svc->Handle = ServerStreamManager::Request((int)PineServiceCode::AdmobInterstitialPrepare, buff->Data, auto_show);
		if (auto_show){
			svc->Status = PineServiceResult::Responsed;
		}
		else{
			svc->Status = PineServiceResult::WaitingResponse;
		}
	}
	else{
		if (ServerStreamManager::isStreamComplete(svc->Handle)){
			ServerStream^ stream = ServerStreamManager::getStream(svc->Handle);
			if (stream->ErrorCode == 0){
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
	if (svc->isFree()){
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

extern "C" void OS_AsynLoadIAP(PineServiceHandle* handle){
	if (handle->isFree()){
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_PURCHASE_GETLIST, nullptr);
		handle->Status = PineServiceResult::WaitingResponse;
	}
	else if (handle->Handle > -1){
		if (ServerStreamManager::isStreamComplete(handle->Handle)){
			ServerStream^ stream = ServerStreamManager::getStream(handle->Handle);
			if (!ServerStreamManager::isError(handle->Handle)){
				int errorcode = stream->ErrorCode;
				if (errorcode == 1){
					handle->ResponseStatus = 1;
					handle->Status = PineServiceResult::Responsed;
				}
				else{
					StreamBuffer^ buf = ref new StreamBuffer(stream->ResponseData);
					int num_of_item = buf->readInt32();
					if (num_of_item > 0){
						for (int i = 0; i< num_of_item; i++){
							Platform::String^ id = buf->readSStr();
							Platform::String^ price = buf->readSStr();
							int type = buf->readInt16();
							char* idc = (char*)Utils::ConvertStringToCharArray(id);
							char* pricec = (char*)Utils::ConvertStringToCharArray(price);
							//WP8_OnUpdateInAppItem(idc, pricec, type);
							printf("Load InApp:%s %s %d\n", idc, pricec, type);
							handle->ResponseStatus = 0;
							handle->Status = PineServiceResult::Responsed;
						}
						handle->ResponseStatus = 0;
						handle->Status = PineServiceResult::Responsed;
					}
					else{
						handle->ResponseStatus = 1;
						handle->Status = PineServiceResult::Responsed;
					}
				}
			}
			else{
				handle->ResponseStatus = 2;
				handle->Status = PineServiceResult::Responsed;
			}
			ServerStreamManager::freeStream(handle->Handle);
		}
	}
}

//GOOGLE PLAY
extern "C" bool OS_IsLoginGooglePlay() {
	return false;
}
extern "C" void	OS_AsynLoginGooglePlay(PineServiceHandle* hsvc) {
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
	hsvc->Status = PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynGooglePlayPostScore(PineServiceHandle* hsvc, BoardScoreItem item) {
	hsvc->Status = PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynGooglePlayShowLeaderBoard(PineServiceHandle* hsvc, const char* board_id) {
	hsvc->Status = PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}
extern "C" void OS_AsynGooglePlayUnlockAchievement(PineServiceHandle* hsvc, const char* achi_id){
	hsvc->Status == PineServiceResult::Responsed; hsvc->ResponseStatus = 1;
}