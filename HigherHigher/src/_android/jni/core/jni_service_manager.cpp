#include "pch.h"
#include "jni_service_manager.h"
#include "Android_Connector.h"
#include "../../../game/Lib/DataType/CType.h"
#include "../../../game/Lib/DataType/BufferStream.h"
#include "../../../game/Lib/String/StringUtils.h"

extern "C" void jni_serviceRequest(ServiceStream* svc);

ServiceStreamManager* ServiceStreamManager::_this = NULL;
ServiceStreamManager::ServiceStreamManager(){
	for(int i = 0; i< 50; i++){
		_svc_stream[i].reset();
	}
	_stream_num = 50;
}
ServiceStreamManager* ServiceStreamManager::getInstance(){
	if(_this == NULL) _this = new ServiceStreamManager();
	return _this;
}
int ServiceStreamManager::request(int code, bool auto_release )
{
	ServiceStreamManager* instance = getInstance();
	int handle = -1;
	for(int i = 0; i< instance->_stream_num; i++){
		if(instance->_svc_stream[i].Status == PineServiceResult::Free){
			instance->_svc_stream[i].reset();
			instance->_svc_stream[i].Status = PineServiceResult::WaitingResponse;
			instance->_svc_stream[i].ServiceCode = code;
			instance->_svc_stream[i].AutoRelease = auto_release;
			handle = i;
			break;
		}
	}
	if(handle > -1){
		//call service process;
		jni_serviceRequest(&instance->_svc_stream[handle]);
	}
	return handle;
}
int ServiceStreamManager::request(int code, Buffer data, bool auto_release){
	ServiceStreamManager* instance = getInstance();
	int handle = -1;
	for(int i = 0; i< instance->_stream_num; i++){
		if(instance->_svc_stream[i].Status == PineServiceResult::Free){
			instance->_svc_stream[i].Status = PineServiceResult::WaitingResponse;
			instance->_svc_stream[i].ServiceCode = code;
			instance->_svc_stream[i].RequestData = data;
			instance->_svc_stream[i].AutoRelease = auto_release;
			handle = i;
			break;
		}
	}
	if(handle > -1){
		//call service process;
		jni_serviceRequest(&instance->_svc_stream[handle]);
	}
	PDEBUG("handle:%d", handle);
	return handle;
}

void ServiceStreamManager::freeStream(int handle){
	ServiceStreamManager* instance = getInstance();
	if(handle >= 0 && handle < instance->_stream_num){
		instance->_svc_stream[handle].reset();
	}
}
bool ServiceStreamManager::isStreamResponse(int handle){
	ServiceStreamManager* instance = getInstance();
	if(handle >= 0 && handle < instance->_stream_num){
		return instance->_svc_stream[handle].Status == PineServiceResult::Responsed;
	}
	return true;
}
const ServiceStream* ServiceStreamManager::getStream(int handle){
	ServiceStreamManager* instance = getInstance();
	if(handle >= 0 && handle < instance->_stream_num){
		return &instance->_svc_stream[handle];
	}
	return NULL;
}

void ServiceStreamManager::update(){

	ServiceStreamManager* instance = getInstance();
	for(int i = 0; i< instance->_stream_num; i++){

		
		if(instance->_svc_stream[i].Status == PineServiceResult::Responsed){
			
			if(!instance->_svc_stream[i].AutoRelease){
				switch(instance->_svc_stream[i].ServiceCode){
					case (int)PineServiceCode::Purchase:
					{

						const ServiceStream* stream = &instance->_svc_stream[i];

						Buffer* pbuff = (Buffer*) &stream->ResponseData;
						BufferStream buff(pbuff->Data(), pbuff->Length());
						PineString iap_id = buff.ReadSStr();
						
						char* tmp = StringUtils::cstringFromPineString(iap_id);
						
						//PDEBUG("purchase id:%s\n", tmp);
						
						if (tmp != NULL)
						{
							Android_OnBuyInAppItemComplete(stream->ErrorCode, tmp);
							
							delete[] tmp;
							tmp = NULL;
						}
						instance->_svc_stream[i].reset();
						
					}break;
				}
			}
			else{
				instance->_svc_stream[i].reset();

			}
		}
	}
}

void ServiceStreamManager::response(int id, Buffer response_data, int error_code){
	//PDEBUG("service stream manager response stream:%d", id);
	ServiceStreamManager* instance = getInstance();
	for(int i = 0; i< instance->_stream_num; i++){
		if(instance->_svc_stream[i].ID == id){
			instance->_svc_stream[i].ResponseData = response_data;
			instance->_svc_stream[i].Status = PineServiceResult::Responsed;
			instance->_svc_stream[i].ErrorCode = error_code;
			PDEBUG("set responsed in :%d data_len:%d", id, instance->_svc_stream[i].ResponseData.Length());
			break;
		}
	}
}