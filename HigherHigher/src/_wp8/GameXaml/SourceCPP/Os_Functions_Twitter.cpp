#include "pch.h"
#include "../../../../game/GameCore/CGame.h"
#include "ServerStream/ServerStreamManager.h"
#include "StreamBuffer.h"
#include "../../../../game/GameCore/Os_Functions.h"
#include "Utils.h"
#include "DeviceInfomation.h"
#include "PlayerInfomation.h"
#include "ApplicationInfomation.h"
using namespace PhoneDirect3DXamlAppComponent;
extern "C" void PostTwitter(){}
extern "C" void ShowTWConnectDialog(){}
extern "C" void PublishTWStream(const char* msg){}
extern "C" int TWLoginIsDone(){
	return false;
}
extern "C" void InitTwitter(){}
extern "C" void TwitterLoginDialog(){}
extern "C" void TwitterPostTweet(const char * msg){
	int buff_size = 2;
	Platform::String^ message = Utils::ConvertCharToString(msg);
	buff_size += 2 * message->Length();
	StreamBuffer^ buff = ref new StreamBuffer(buff_size);
	buff->writeSStr(message);
	ServerStreamManager::Request(SERVER_REQUEST_CODE_TW_FEED, buff->Data,true);
}

int svstream_handler_initTwitter = -1;
extern "C" SVRESULT SVS_InitTwitter(){
	if(!OS_IsNetworkAvailable() && svstream_handler_initTwitter == -1){
		svstream_handler_initTwitter = -1;
		return SVRESULT::SV_DATA_ERROR_NO_NETWORK;
	}
	SVRESULT ret = SVRESULT::SV_DATA_ERROR_UNKNOW;
	if(svstream_handler_initTwitter == -1){
		svstream_handler_initTwitter = ServerStreamManager::Request(SERVER_REQUEST_CODE_TW_LOGIN,nullptr);
		ret = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}else{
		if(ServerStreamManager::isStreamComplete(svstream_handler_initTwitter)){
			if(!ServerStreamManager::isError(svstream_handler_initTwitter)){
				ret = SVRESULT::SV_DATA_SUCCESS;
			}else{
				ret = SVRESULT::SV_DATA_ERROR_NO_RECORD;
			}
			ServerStreamManager::freeStream(svstream_handler_initTwitter);
			svstream_handler_initTwitter = -1;
		}else{
			ret = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
	return ret;
}
extern "C" void OS_AsynLoginTwitter(PineServiceHandle* handle){
	if (!OS_IsNetworkAvailable() && handle->Handle == -1){
		handle->ResponseStatus = 1;
		handle->Status = PineServiceResult::Responsed;
	}
	if (handle->isFree()){
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_TW_LOGIN, nullptr);
		handle->Status = PineServiceResult::WaitingResponse;
	}
	else{
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

int svstream_handler_twfeed = -1;
extern "C" SVRESULT SVS_TWFeed(const char* msg){
	SVRESULT ret = SVRESULT::SV_DATA_ERROR_UNKNOW;
	if(!OS_IsNetworkAvailable() && svstream_handler_twfeed == -1){
		return SVRESULT::SV_DATA_ERROR_NO_NETWORK;
		svstream_handler_twfeed = -1;
	}
	if(svstream_handler_twfeed == -1){
		int buff_size = 2;
		Platform::String^ message = Utils::ConvertCharToString(msg);
		buff_size += 2 * message->Length();
		StreamBuffer^ buff = ref new StreamBuffer(buff_size);
		buff->writeSStr(message);
		svstream_handler_twfeed = ServerStreamManager::Request(SERVER_REQUEST_CODE_TW_FEED, buff->Data);
		ret = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}else{
		if(ServerStreamManager::isStreamComplete(svstream_handler_twfeed)){
			if(!ServerStreamManager::isError(svstream_handler_twfeed)){
				ret = SVRESULT::SV_DATA_SUCCESS;
			}else{
				ret = SVRESULT::SV_DATA_ERROR_UNKNOW;
			}
			svstream_handler_twfeed = -1;
		}else{
			ret = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
	return ret;
}

char* tw_username = NULL;
char* tw_id = NULL;

extern "C" UINT64		OS_GetTwitterUserId(){
	if (tw_id != NULL){
		delete[] tw_id;
		tw_id = NULL;
	}
	if (PhoneDirect3DXamlAppComponent::PlayerInfomation::TwitterId != nullptr){
		//ConvertStringToCharArray return a non const char* 
		//we must delete after use.
		tw_id = (char*)Utils::ConvertStringToCharArray(PhoneDirect3DXamlAppComponent::PlayerInfomation::TwitterId);
		UINT64 id = 0;
		int len = strlen(tw_id);
		for (int i = 0; i < len; i++) id = id * 10 + (tw_id[i] - '0');
		return id;
	}
	return 0;
}
extern "C" const char*	OS_GetTwitterUserName(){
	if (tw_username != NULL){
		delete[] tw_username;
		tw_username = NULL;
	}
	if (PhoneDirect3DXamlAppComponent::PlayerInfomation::TwitterUserName != nullptr){
		//ConvertStringToCharArray return a non const char* 
		//we must delete after use.
		tw_username = (char*)Utils::ConvertStringToCharArray(PhoneDirect3DXamlAppComponent::PlayerInfomation::TwitterUserName);
		return (const char*)tw_username;
	}
	return NULL;
}
extern "C" bool OS_IsLogginTwitter(){
	return PhoneDirect3DXamlAppComponent::PlayerInfomation::LoginedTwitter;
}
extern "C" void	OS_AsynTwitterLoadFollowerIdList(PineServiceHandle* handle, PineAccountIdList* account_list){
	if (handle->isFree()){
		handle->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_TW_GET_FRIEND, nullptr);
		handle->RequestCode = SERVER_REQUEST_CODE_FB_GET_FRIEND;
		handle->Status = PineServiceResult::WaitingResponse;
	}
	else if(handle->Handle > -1){
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
extern "C" void	OS_PostTwitter(const char* msg){
	int buff_size = 2;
	Platform::String^ message = Utils::ConvertCharToString(msg);
	buff_size += 2 * message->Length();
	StreamBuffer^ buff = ref new StreamBuffer(buff_size);
	buff->writeSStr(message);
	svstream_handler_twfeed = ServerStreamManager::Request(SERVER_REQUEST_CODE_TW_FEED, buff->Data,true);
}

extern "C" void	OS_AsynPostTwitter(PineServiceHandle* hsvc, const char* msg)
{
	if (hsvc->isFree()){
		int buff_size = 2;
		Platform::String^ message = Utils::ConvertCharToString(msg);
		buff_size += 2 * message->Length();
		StreamBuffer^ buff = ref new StreamBuffer(buff_size);
		buff->writeSStr(message);
		hsvc->Handle = ServerStreamManager::Request(SERVER_REQUEST_CODE_TW_FEED, buff->Data, false);
	}
	else if (hsvc->Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(hsvc->Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(hsvc->Handle);
			hsvc->ResponseStatus = stream->ErrorCode;
			hsvc->Status = PineServiceResult::Responsed;
			ServerStreamManager::freeStream(hsvc->Handle);
			hsvc->Handle = -1;
		}
	}
}