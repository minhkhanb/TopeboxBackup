#include "pch.h"
#include "ServerStreamManager.h"
#include "../StreamBuffer.h"
#include "../Utils.h"

extern "C" void WP8_OnBuyInAppItemComplete(bool success, const char* item_id);

using namespace Pine;
ServerStreamManager^ ServerStreamManager::_this = nullptr;
void ServerStreamManager::Update(){
	ServerStreamManager^ instance = getInstance();
	for(int i = 0; i< instance->StreamNum;i++){
		if(instance->Streams[i]->Status != SERVER_STREAM_STATUS_FREE){
			if (!instance->Streams[i]->AutoFinish){
				switch (instance->Streams[i]->RequestCode)
				{
				case SERVER_REQUEST_CODE_PURCHASE:
					if (instance->Streams[i]->Status == SERVER_STREAM_STATUS_ERROR){
						StreamBuffer^ buf = ref new StreamBuffer(instance->Streams[i]->PostData);
						Platform::String^ id = buf->readSStr();
						char* cid = (char*)Utils::ConvertStringToCharArray(id);
						WP8_OnBuyInAppItemComplete(false, (const char*)cid);
						delete[] cid;
						instance->Streams[i]->Status = SERVER_STREAM_STATUS_FREE;
					}
					else if (instance->Streams[i]->Status == SERVER_STREAM_STATUS_RESPONSED){
						StreamBuffer^ buf = ref new StreamBuffer(instance->Streams[i]->PostData);
						Platform::String^ id = buf->readSStr();
						char* cid = (char*)Utils::ConvertStringToCharArray(id);
						WP8_OnBuyInAppItemComplete(instance->Streams[i]->ErrorCode == 0, (const char*)cid);
						delete[] cid;
						instance->Streams[i]->Status = SERVER_STREAM_STATUS_FREE;
					}
					break;
				default:
					break;
				}
			}
			else{
				if (instance->Streams[i]->Status != SERVER_STREAM_STATUS_WAITING){
					instance->Streams[i]->Status = SERVER_STREAM_STATUS_FREE;
					instance->Streams[i]->ErrorCode = 0;
				}
			}
		}
	}
}