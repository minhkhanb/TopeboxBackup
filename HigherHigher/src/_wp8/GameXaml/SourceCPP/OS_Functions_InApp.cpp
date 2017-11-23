#include "pch.h"
#include "../../../../game/GameCore/CGame.h"
#include "StreamBuffer.h"
#include "ServerStream/ServerStreamManager.h"
#include "../../../../game/GameCore/Os_Functions.h"
#include "WP8_Connector.h"
#include "Utils.h"
using namespace PhoneDirect3DXamlAppComponent;

int svstream_handler_getInAppList = -1;

extern "C" void			OS_LoadIAP(){

}

extern "C" SVRESULT OS_getInAppList(){
	SVRESULT ret = SVRESULT::SV_DATA_ERROR_UNKNOW;
	if(!OS_IsNetworkAvailable() && svstream_handler_getInAppList == -1)return SVRESULT::SV_DATA_ERROR_NO_NETWORK;

	if(svstream_handler_getInAppList == -1){
		svstream_handler_getInAppList = ServerStreamManager::Request(SERVER_REQUEST_CODE_PURCHASE_GETLIST,nullptr);
		ret = SVRESULT::SV_DATA_WAITING_RESPONSE;
	}else{
		if(ServerStreamManager::isStreamComplete(svstream_handler_getInAppList)){
			ServerStream^ stream = ServerStreamManager::getStream(svstream_handler_getInAppList);
			if(!ServerStreamManager::isError(svstream_handler_getInAppList)){
				int errorcode = stream->ErrorCode;
				if(errorcode == 1){
					ret = SVRESULT::SV_DATA_ERROR_NO_RECORD;
				}else{
					StreamBuffer^ buf = ref new StreamBuffer(stream->ResponseData);
					int num_of_item = buf->readInt32();
					if(num_of_item > 0){
						ret = SVRESULT::SV_DATA_SUCCESS;
						for(int i = 0; i< num_of_item; i++){
							Platform::String^ id = buf->readSStr();
							Platform::String^ price = buf->readSStr();
							int type = buf->readInt16();
							char* idc = (char*)Utils::ConvertStringToCharArray(id);
							char* pricec = (char*)Utils::ConvertStringToCharArray(price);
							//WP8_OnUpdateInAppItem(idc, pricec , type);
							printf("Load InApp:%s %s %d\n", idc, pricec, type);
						}
					}else{
						ret = SVRESULT::SV_DATA_ERROR_NO_RECORD;
					}
				}
			}else{
				ret = SVRESULT::SV_DATA_ERROR_UNKNOW;
			}
			ServerStreamManager::freeStream(svstream_handler_getInAppList);
			svstream_handler_getInAppList = -1;
		}else{
			ret = SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
	return ret;
}
extern "C" void OS_AsynLoadIAP(PineServiceHandle* handle){
#if defined(BUILD_SUPPORT)
	handle->ResponseStatus = 0;
	handle->Status = PineServiceResult::Responsed;
	return;
#endif
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
							WP8_OnUpdateInAppItem(idc, pricec , type);
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
extern "C" void PurchaseProduct(const char *indentify){
	Platform::String^ id = Utils::ConvertCharToString(indentify);
	StreamBuffer^ buf = ref new StreamBuffer(2 + 2* id->Length());
	buf->writeSStr(id);
	ServerStreamManager::Request(SERVER_REQUEST_CODE_PURCHASE,buf->Data);
}