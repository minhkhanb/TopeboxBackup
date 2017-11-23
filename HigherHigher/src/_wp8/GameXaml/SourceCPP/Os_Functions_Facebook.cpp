#include "pch.h"
#include "../../../../game/GameCore/CGame.h"
#include "ServerStream/ServerStreamManager.h"
#include "StreamBuffer.h"
#include "DeviceInfomation.h"
#include "PlayerInfomation.h"
#include "ApplicationInfomation.h"
#include "../../../../game/GameCore/Os_Functions.h"
using namespace PhoneDirect3DXamlAppComponent;

#if defined(BUILD_SUPPORT)

extern "C" bool support_sand_boxed;
extern "C" char* support_account_id;
extern "C" PineAccountType support_account_type;
extern "C" PineAccountInfo support_account;

#endif

extern "C" bool HasLoginFB()
{

#if defined(BUILD_SUPPORT)
	if (support_sand_boxed)
	{
		return support_account_type == PineAccountType::Facebook;
	}
#endif
	return PhoneDirect3DXamlAppComponent::PlayerInfomation::LoginedFacebook;
}

extern "C" bool OS_IsLogginFacebook()
{
#if defined(BUILD_SUPPORT)
	if (support_sand_boxed)
	{
		return support_account_type == PineAccountType::Facebook;
	}

#endif
	return PhoneDirect3DXamlAppComponent::PlayerInfomation::LoginedFacebook;
}

extern "C" void FBFeedMe(const char* msg)
{

#if defined(BUILD_SUPPORT)
	return;
#endif
	int buff_size = 2;
	Platform::String^ message = Utils::ConvertCharToString(msg);
	buff_size += 2 * message->Length();
	StreamBuffer^ buff = ref new StreamBuffer(buff_size);
	buff->writeSStr(message);
	ServerStreamManager::Request(SERVER_REQUEST_CODE_FB_FEED_ME, buff->Data, true);
}

extern "C" void FBLoadListFriend(bool play_game)
{
#if defined(BUILD_SUPPORT)
	return;
#endif
}

extern "C" void InitFaceBook()
{
}

extern "C" bool FBSessionValid()
{
	return false;
}

extern "C" void FB_apprequests(const char * msg, const INT_TIME * arrayID, int count)
{

}

int svstream_handle_fb_feed_me = -1;
extern "C" SVRESULT OS_FBFeedMe(const char* msg)
{
#if defined(BUILD_SUPPORT)
	return SVRESULT::SV_DATA_ERROR_UNKNOW;
#endif

	SVRESULT ret = SVRESULT::SV_DATA_ERROR_UNKNOW;
	if(!OS_IsNetworkAvailable() && svstream_handle_fb_feed_me == -1){
		svstream_handle_fb_feed_me = -1;
		return SVRESULT::SV_DATA_ERROR_NO_NETWORK;
	}
	if(svstream_handle_fb_feed_me == -1){
		int buff_size = 2;
		Platform::String^ message = Utils::ConvertCharToString(msg);
		buff_size += 2 * message->Length();
		StreamBuffer^ buff = ref new StreamBuffer(buff_size);
		buff->writeSStr(message);
		svstream_handle_fb_feed_me = ServerStreamManager::Request(SERVER_REQUEST_CODE_FB_FEED_ME, buff->Data);
		ret = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}else{
		if(ServerStreamManager::isStreamComplete(svstream_handle_fb_feed_me)){
			if(!ServerStreamManager::isError(svstream_handle_fb_feed_me)){
				ret = SVRESULT::SV_DATA_SUCCESS;
			}else{
				ret = SVRESULT::SV_DATA_ERROR_UNKNOW;
			}
			ServerStreamManager::freeStream(svstream_handle_fb_feed_me);
			svstream_handle_fb_feed_me = -1;
		}else{
			ret = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
	return ret;
}

char* fb_username = NULL;
char* fb_id = NULL;
char* fb_avatar = NULL;

int svstream_handle_initFacebook = -1;

extern "C" SVRESULT OS_InitFaceBook()
{
	if(!OS_IsNetworkAvailable() && svstream_handle_initFacebook == -1){
		svstream_handle_initFacebook = -1;
		return SVRESULT::SV_DATA_ERROR_NO_NETWORK;
	}
	SVRESULT ret = SVRESULT::SV_DATA_ERROR_UNKNOW;
	if(svstream_handle_initFacebook == -1){
		svstream_handle_initFacebook = ServerStreamManager::Request(SERVER_REQUEST_CODE_FB_LOGIN,nullptr);
		ret = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}else{
		if(ServerStreamManager::isStreamComplete(svstream_handle_initFacebook)){
			if(!ServerStreamManager::isError(svstream_handle_initFacebook)){
				ret = SVRESULT::SV_DATA_SUCCESS;
			}else{
				ret = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			ServerStreamManager::freeStream(svstream_handle_initFacebook);
			svstream_handle_initFacebook = -1;
		}else{
			ret = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
	return ret;
}

extern "C" void OS_AsynLoginFacebook(PineServiceHandle* handle){
#if defined(BUILD_SUPPORT)
	if (support_sand_boxed)
	{
		if (support_account_type == PineAccountType::Facebook)
		{
			handle->ResponseStatus = 0;
			handle->Status = PineServiceResult::Responsed;
		}
		else
		{
			handle->ResponseStatus = 1;
			handle->Status = PineServiceResult::Responsed;
		}
		return;
	}
#endif

	if (!OS_IsNetworkAvailable() && handle->Handle == -1)
	{
		handle->ResponseStatus = 1;
		handle->Status = PineServiceResult::Responsed;
	}

	if (handle->isFree())
	{
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_FB_LOGIN, nullptr);
		handle->Status = PineServiceResult::WaitingResponse;
	}

	else if(handle->Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(handle->Handle))
		{
			if (!ServerStreamManager::isError(handle->Handle))
			{
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

int fb_svstream_handle = -1;
//int fb_avarstream_handle = -1;
extern "C" SVRESULT OS_InitFaceBookAsRequire()
{
#if defined(BUILD_SUPPORT)
	if (support_sand_boxed){
		if (support_account_type == PineAccountType::Facebook)
		{
			return SVRESULT::SV_DATA_SUCCESS;
		}
		return SVRESULT::SV_DATA_ERROR_NO_RECORD;
	}
#endif
	if(!OS_IsNetworkAvailable() && fb_svstream_handle == -1)
	{
		fb_svstream_handle = -1;
		return SVRESULT::SV_DATA_ERROR_NO_NETWORK;
	}
	SVRESULT ret = SVRESULT::SV_DATA_ERROR_UNKNOW;
	if(fb_svstream_handle == -1)
	{
		StreamBuffer^ buff = ref new StreamBuffer(2);
		buff->writeInt16(1);
		fb_svstream_handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_FB_LOGIN,buff->Data);
		ret = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else
	{
		if(ServerStreamManager::isStreamComplete(fb_svstream_handle))
		{
			if(!ServerStreamManager::isError(fb_svstream_handle))
			{
				ret = SVRESULT::SV_DATA_SUCCESS;
			}
			else
			{
				ret = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			ServerStreamManager::freeStream(fb_svstream_handle);
			fb_svstream_handle = -1;
		}
		else
		{
			ret = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
	return ret;
}
//OS_GetFBUserId: get facebook id of current user
//return facebook id if user had been loged in
//return NULL if not
extern "C" const char* OS_GetFBUserId(){
#if defined(BUILD_SUPPORT)
	if (support_sand_boxed)
	{
		if (support_account_type == PineAccountType::Facebook)
		{
			return support_account.StrId;
		}
	}
#endif

	if(fb_id != NULL){
		delete[] fb_id;
		fb_id = NULL;
	}
	if(PhoneDirect3DXamlAppComponent::PlayerInfomation::FacebookId != nullptr){
		//ConvertStringToCharArray return a non const char* 
		//we must delete after use.
		fb_id = (char*)Utils::ConvertStringToCharArray(PhoneDirect3DXamlAppComponent::PlayerInfomation::FacebookId);
		return (const char*)fb_id;
	}
	return NULL;
}
extern "C" UINT64 OS_GetFacebookUserId()
{
#if defined(BUILD_SUPPORT)
	if (support_sand_boxed)
	{
		if (support_account_type == PineAccountType::Facebook)
		{
			UINT64 id = 0;
			char* pstr = (char*)support_account.StrId;

			while (*pstr != '\0')
			{
				id = id * 10 + ((*pstr++) - '0');
			}

			if (id > 3000000000000000000)
			{
				id -= 3000000000000000000;
			}
			else if (id > 300000000000000000)
			{
				id -= 300000000000000000;
			}
			return id;
		}
	}
#endif

	if (fb_id != NULL)
	{
		delete[] fb_id;
		fb_id = NULL;
	}
	if (PhoneDirect3DXamlAppComponent::PlayerInfomation::FacebookId != nullptr){
		//ConvertStringToCharArray return a non const char* 
		//we must delete after use.
		fb_id = (char*)Utils::ConvertStringToCharArray(PhoneDirect3DXamlAppComponent::PlayerInfomation::FacebookId);
		UINT64 id = 0;
		int len = strlen(fb_id);
		for (int i = 0; i < len; i++) id = id * 10 + (fb_id[i] - '0');
		return id;
	}
	return 0;
}

extern "C" const char* OS_GetFBUserName()
{
#if defined(BUILD_SUPPORT)
	if (support_sand_boxed){
		if (support_account_type == PineAccountType::Facebook)
		{
			return support_account.Name;
		}
		return NULL;
	}
#endif
	if (fb_username != NULL){
		delete[] fb_username;
		fb_username = NULL;
	}
	if (PhoneDirect3DXamlAppComponent::PlayerInfomation::FacebookUserName != nullptr){
		//ConvertStringToCharArray return a non const char* 
		//we must delete after use.
		fb_username = (char*)Utils::ConvertStringToCharArray(PhoneDirect3DXamlAppComponent::PlayerInfomation::FacebookUserName);
		return (const char*)fb_username;
	}

	return NULL;
}

extern "C" const char* OS_GetFacebookUserName()
{

#if defined(BUILD_SUPPORT)
	if (support_sand_boxed){
		if (support_account_type == PineAccountType::Facebook)
		{
			return support_account.Name;
		}
		return NULL;
	}
#endif

	if (fb_username != NULL){
		delete[] fb_username;
		fb_username = NULL;
	}
	if (PhoneDirect3DXamlAppComponent::PlayerInfomation::FacebookUserName != nullptr){
		//ConvertStringToCharArray return a non const char* 
		//we must delete after use.
		fb_username = (char*)Utils::ConvertStringToCharArray(PhoneDirect3DXamlAppComponent::PlayerInfomation::FacebookUserName);
		return (const char*)fb_username;
	}
	return NULL;
}

extern "C" const char*	OS_GetFacebookAvatarUrl()
{

#if defined(BUILD_SUPPORT)

	return NULL;
#endif

	if (fb_avatar != NULL)
	{
		delete[] fb_avatar;
		fb_avatar = NULL;
	}
	if (PhoneDirect3DXamlAppComponent::PlayerInfomation::FacebookUserName != nullptr)
	{
		//ConvertStringToCharArray return a non const char* 
		//we must delete after use.
		fb_avatar = (char*)Utils::ConvertStringToCharArray(PhoneDirect3DXamlAppComponent::PlayerInfomation::FacebookAvatarUrl);
		return (const char*)fb_avatar;
	}
	return NULL;
}

extern "C" void  loadFBAvatarPhoto()
{
#if defined(BUILD_SUPPORT)

	return;
#endif

	const char* fbid = OS_GetFBUserId();
	if(fbid != NULL)
	{
		if(fb_svstream_handle == -1)
		{
			char filename[255];
			sprintf(filename,"A%s.png",fbid);
			Platform::Array<unsigned char>^ fnamebuff = ref new Platform::Array<unsigned char>(strlen(filename));
			for(int i = 0; i< strlen(filename); i++)
			{
				fnamebuff[i] = filename[i];
			}
			fb_svstream_handle = ServerStreamManager::RequestFBAvatar(fnamebuff);
		}
	}
}

extern "C" int	getFBErrorCode()
{
#if defined(BUILD_SUPPORT)

	return -1009;
#endif

	if(fb_svstream_handle != -1)
	{
		if(ServerStreamManager::isStreamComplete(fb_svstream_handle))
		{
			if(!ServerStreamManager::isError(fb_svstream_handle))
			{
				fb_svstream_handle = -1;
				return -2;
			}
			else
			{
				fb_svstream_handle = -1;
				return -1009;
			}
			ServerStreamManager::freeStream(fb_svstream_handle);
			fb_svstream_handle = -1;
		}
	}
	return -1;
}




extern "C" void SVS_InitFaceBookAsRequire(SVHANDLE* handle)
{
#if defined(BUILD_SUPPORT)

	handle->Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
	return;
#endif

	if(handle->Handle == -1 && handle->Status == SVRESULT::SV_DATA_FREE)
	{
		StreamBuffer^ buff = ref new StreamBuffer(2);
		buff->writeInt16(1);
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_FB_LOGIN,buff->Data);
		handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		handle->RequestCode = SERVER_REQUEST_CODE_FB_LOGIN;
	}
	else if(handle->Handle > -1)
	{
		if(ServerStreamManager::isStreamComplete(handle->Handle))
		{
			if(!ServerStreamManager::isError(handle->Handle))
			{
				handle->Status  = SVRESULT::SV_DATA_SUCCESS;
			}
			else
			{
				handle->Status  = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			ServerStreamManager::freeStream(handle->Handle);
			handle->Handle = -1; 
		}
		else
		{
			handle->Status  = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}
extern "C" void SVS_InitFaceBook(SVHANDLE* handle)
{
#if defined(BUILD_SUPPORT)
	if (support_sand_boxed)
	{
		handle->Status = (support_account_type != PineAccountType::Facebook) ? SVRESULT::SV_DATA_SUCCESS : SVRESULT::SV_DATA_ERROR_NO_RECORD;

		handle->Handle = -1;
		return;
	}
	
#endif

	if(handle->Handle == -1 && handle->Status == SVRESULT::SV_DATA_FREE)
	{
		//StreamBuffer^ buff = ref new StreamBuffer(2);
		//buff->writeInt16(0);
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_FB_LOGIN,nullptr);
		handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		handle->RequestCode = SERVER_REQUEST_CODE_FB_LOGIN;
	}
	else if(handle->Handle > -1)
	{
		if(ServerStreamManager::isStreamComplete(handle->Handle))
		{
			if(!ServerStreamManager::isError(handle->Handle))
			{
				handle->Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else
			{
				handle->Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			ServerStreamManager::freeStream(handle->Handle);
			handle->Handle = -1; 
		}
		else
		{
			handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}

extern "C" void SVS_FBLoadListFriend(SVHANDLE* handle)
{
#if defined(BUILD_SUPPORT)

	handle->Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
	handle->Handle = -1;
	return;

#endif

	if(handle->Handle == -1 && handle->Status == SVRESULT::SV_DATA_FREE)
	{
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_FB_GET_FRIEND,nullptr);
		handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		handle->RequestCode = SERVER_REQUEST_CODE_FB_GET_FRIEND;
	}
	else
	{
		if(ServerStreamManager::isStreamComplete(handle->Handle))
		{
			if(!ServerStreamManager::isError(handle->Handle))
			{
				handle->Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else
			{
				handle->Status = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			ServerStreamManager::freeStream(handle->Handle);
			handle->Handle = -1; 
		}
		else
		{
			handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}

extern "C" ResourceItem OS_GetResourceItem(const char* file)
{
	PDEBUG("get resource item:%s\n", file);
	ResourceItem resitem;
	sprintf(resitem.Path, OS_GetResourceFilePath(file));
	resitem.Offset = 0;
	resitem.Length = -1;
	return resitem;
}

extern "C" void SVS_FBFeed(SVHANDLE* handle, const char* msg)
{
#if defined(BUILD_SUPPORT)

	handle->Status = SVRESULT::SV_DATA_ERROR_UNKNOW;
	handle->Handle = -1;
	return;

#endif

	if (handle->Handle == -1 && handle->Status == SVRESULT::SV_DATA_FREE)
	{
		int buff_size = 2;
		Platform::String^ message = Utils::ConvertCharToString(msg);
		buff_size += 2 * message->Length();
		StreamBuffer^ buff = ref new StreamBuffer(buff_size);
		buff->writeSStr(message);
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_FB_FEED_ME, buff->Data);
		handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else if (handle->Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(handle->Handle))
		{
			if (!ServerStreamManager::isError(handle->Handle))
			{
				handle->Status = SVRESULT::SV_DATA_SUCCESS;
			}
			else
			{
				handle->Status = SVRESULT::SV_DATA_ERROR_UNKNOW;
			}
			ServerStreamManager::freeStream(handle->Handle);
			handle->Handle = -1;
		}
		else
		{
			handle->Status = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}

extern "C" void OS_PostFacebook(const char* msg)
{
#if defined(BUILD_SUPPORT)
	return;
#endif

	int buff_size = 2;
	Platform::String^ message = Utils::ConvertCharToString(msg);
	buff_size += 2 * message->Length();
	StreamBuffer^ buff = ref new StreamBuffer(buff_size);
	buff->writeSStr(message);
	ServerStreamManager::Request(SERVER_REQUEST_CODE_FB_FEED_ME, buff->Data,true);
}
extern "C" void OS_AsynPostFacebook(PineServiceHandle* hsvc, const char* msg) {
#if defined(BUILD_SUPPORT)

	hsvc->ResponseStatus = 1;
	hsvc->Status = PineServiceResult::Responsed;

	return;

#endif
}
extern "C" void OS_FacebookPostPhoto(const char* file_name, const char* msg)
{
#if defined(BUILD_SUPPORT)
	return;
#endif
		int buff_size = 2;
		Platform::String^ fname = Utils::ConvertCharToString(file_name);
		buff_size += 2 * fname->Length();

		buff_size += 2;
		Platform::String^ message = Utils::ConvertCharToString(msg);
		buff_size += 2 * message->Length();
		StreamBuffer^ buff = ref new StreamBuffer(buff_size);
		buff->writeSStr(fname);
		buff->writeSStr(message);
		ServerStreamManager::Request(SERVER_REQUEST_CODE_FB_POST_PHOTO, buff->Data, true);
}
extern "C" void	OS_AsynFacebookPostScreen(PineServiceHandle* hsvc, const char* msg, PineGraphicComplexOrientation rotate) {
#if defined(BUILD_SUPPORT)

	hsvc->ResponseStatus = 1;
	hsvc->Status = PineServiceResult::Responsed;

	return;

#endif
}

extern "C" void	OS_AsynFacebookLoadFriendIdList(PineServiceHandle* handle, PineAccountIdList* account_list)
{
#if defined(BUILD_SUPPORT)

	handle->ResponseStatus = 1;
	handle->Status = PineServiceResult::Responsed;

	return;
#endif

	if (handle->isFree())
	{
		handle->RequestCode = SERVER_REQUEST_CODE_FB_GET_FRIEND;
		handle->Status = PineServiceResult::WaitingResponse;
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_FB_GET_FRIEND, nullptr);
	}
	else
	{
		if (ServerStreamManager::isStreamComplete(handle->Handle))
		{
			if (!ServerStreamManager::isError(handle->Handle))
			{
				ServerStream^ stream = ServerStreamManager::getStream(handle->Handle);
				StreamBuffer^ buff = ref new StreamBuffer(stream->ResponseData);
				int account_num = buff->readInt32();
				if (account_num > 0)
				{
					account_list->init(account_num);
					for (int a = 0; a < account_num; a++)
					{
						account_list->setAccountAt(a, (UINT64)buff->readInt64());
					}
				}
				handle->ResponseStatus = 0;
				handle->Status = PineServiceResult::Responsed;
			}
			else
			{
				handle->ResponseStatus = 1;
				handle->Status = PineServiceResult::Responsed;
			}
			ServerStreamManager::freeStream(handle->Handle);
		}
	}
}
//Bo adds
extern "C" void	OS_AsynFacebookPostScore(PineServiceHandle* handle, BoardScoreItem item)
{
#if defined(BUILD_SUPPORT)

	handle->ResponseStatus = 1;
	handle->Status = PineServiceResult::Responsed;
	return;
#endif

	if (!OS_IsNetworkAvailable() && handle->Handle == -1){
		handle->ResponseStatus = 1;
		handle->Status = PineServiceResult::Responsed;
	}
	if (handle->isFree()){
		int buff_size = 4;
		StreamBuffer^ buff = ref new StreamBuffer(buff_size);
		buff->writeInt32(item.Score);
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
