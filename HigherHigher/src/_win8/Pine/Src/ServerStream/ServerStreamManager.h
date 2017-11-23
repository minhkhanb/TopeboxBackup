#if !defined(_H_SERVER_STREAM_MANAGER)
#define _H_SERVER_STREAM_MANAGER
#include "../Environment.h"
#include "../../Content/Renderer.h"
//#include "../PineRenderer.h"
#include "ServerStream.h"
//#include <Lib\DataType\CByteBuffer.h>
#define SERVER_STREAM_STATUS_FREE				0
#define SERVER_STREAM_STATUS_WAITING			1
#define SERVER_STREAM_STATUS_REQUESTING			2
#define SERVER_STREAM_STATUS_ERROR				3
#define SERVER_STREAM_STATUS_RESPONSED			4

#define SERVER_REQUEST_CODE_FB_LOGIN			1
#define SERVER_REQUEST_CODE_FB_GET_AVATAR		2
#define SERVER_REQUEST_CODE_OPEN_BROWSER		3
#define SERVER_REQUEST_CODE_RATING				4
#define SERVER_REQUEST_CODE_PURCHASE			5
#define SERVER_REQUEST_CODE_PURCHASE_GETLIST	6
#define SERVER_REQUEST_CODE_FB_FEED_ME			7
#define SERVER_REQUEST_CODE_TW_LOGIN			8
#define SERVER_REQUEST_CODE_TW_FEED				9
#define SERVER_REQUEST_CODE_FB_GET_FRIEND		10
#define SERVER_REQUEST_CODE_GAME_EXIT			11
#define SERVER_REQUEST_CODE_LOAD_IMAGE			12
#define SERVER_REQUEST_CODE_TW_GET_FRIEND		13
#define SERVER_REQUEST_CODE_SMS_SEND			14
#define SERVER_REQUEST_CODE_MAIL_SEND			15

namespace Pine{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ServerStreamManager sealed{
	private:
		static ServerStreamManager^ _this;
		ServerStreamManager(int max_stream){
			Streams = ref new Platform::Array<ServerStream^>(max_stream);
			for(int i = 0; i< max_stream; i++){
				Streams[i] = ref new ServerStream();
				Streams[i]->Status = SERVER_STREAM_STATUS_FREE;
				Streams[i]->CurrentHandle = i;
			}
			StreamNum = max_stream;
			Count = 0;
		};
	public:
		//static const int ABC;
		property int Count;
		property int StreamNum;
		property Platform::Array<ServerStream^>^ Streams;
		static int Request(uint32 request_code){
			ServerStreamManager^ instance = getInstance();
			int handle = -1;
			for(int i = 0; i< instance->StreamNum; i++){
				if(instance->Streams[i]->Status == SERVER_STREAM_STATUS_FREE){
					instance->Streams[i]->Status = SERVER_STREAM_STATUS_WAITING;
					//instance->Streams[i]->Status = SERVER_STREAM_STATUS_ERROR;
					instance->Streams[i]->ErrorCode = -1;
					instance->Streams[i]->RequestCode = request_code;
					handle = i;
					Renderer::shareInstance()->ServiceRequest(CONNECTOR_SERVER_CHECK_AND_RUN);
					break;
				}
			}
			return handle;
		};
		static int Request(uint32 request_code, const Platform::Array<unsigned char>^ data){
			int handle = -1;
				ServerStreamManager^ instance = getInstance();
				/*if(instance->Count < instance->StreamNum){
					handle = instance->Count;
					instance->Count ++;
					instance->Streams[handle]->Status = SERVER_STREAM_STATUS_WAITING;
						instance->Streams[handle]->RequestCode = request_code;
						if(data != nullptr && data->Length > 0){
							instance->Streams[handle]->PostData = data;
						}else{
							instance->Streams[handle]->PostData = nullptr;
						}
						instance->Streams[handle]->AutoFinish = false;
						PineRenderer::shareInstance()->CallBaseService(CONNECTOR_SERVER_CHECK_AND_RUN, NULL, 0);
				}*/
				for(int i = 0; i< instance->StreamNum; i++){
					if(instance->Streams[i]->Status == SERVER_STREAM_STATUS_FREE){
						instance->Streams[i]->Status = SERVER_STREAM_STATUS_WAITING;
						//instance->Streams[i]->Status = SERVER_STREAM_STATUS_ERROR;
						instance->Streams[i]->ErrorCode = -1;
						instance->Streams[i]->RequestCode = request_code;
						if(data != nullptr && data->Length > 0){
							instance->Streams[i]->PostData = data;
						}else{
							instance->Streams[i]->PostData = nullptr;
						}
						instance->Streams[i]->AutoFinish = false;
						handle = i;
						Renderer::shareInstance()->ServiceRequest(CONNECTOR_SERVER_CHECK_AND_RUN);
						break;
					}
				}
				
			return handle;
		};
		static int Request(uint32 request_code, const Platform::Array<unsigned char>^ data, bool auto_release){
			int handle = -1;
				ServerStreamManager^ instance = getInstance();
				for(int i = 0; i< instance->StreamNum; i++){
					if(instance->Streams[i]->Status == SERVER_STREAM_STATUS_FREE){
						instance->Streams[i]->Status = SERVER_STREAM_STATUS_WAITING;
						instance->Streams[i]->ErrorCode = -1;
						instance->Streams[i]->RequestCode = request_code;
						if(data != nullptr && data->Length > 0){
							instance->Streams[i]->PostData = data;
						}else{
							instance->Streams[i]->PostData = nullptr;
						}
						instance->Streams[i]->AutoFinish = auto_release;
						handle = i;
						Renderer::shareInstance()->ServiceRequest(CONNECTOR_SERVER_CHECK_AND_RUN);
						break;
					}
				}
				
			return handle;
		};
		static int RequestFBAvatar(const Platform::Array<unsigned char>^ filename){
			int handle = -1;
			if(filename != nullptr && filename->Length > 0){
				ServerStreamManager^ instance = getInstance();
				for(int i = 0; i< instance->StreamNum; i++){
					if(instance->Streams[i]->Status == SERVER_STREAM_STATUS_FREE){
						instance->Streams[i]->Status = SERVER_STREAM_STATUS_WAITING;
						//instance->Streams[i]->Status = SERVER_STREAM_STATUS_ERROR;
						instance->Streams[i]->ErrorCode = -1;
						instance->Streams[i]->RequestCode = SERVER_REQUEST_CODE_FB_GET_AVATAR;
						instance->Streams[i]->PostData = filename;
						handle = i;
						Renderer::shareInstance()->ServiceRequest(CONNECTOR_SERVER_CHECK_AND_RUN);
						break;
					}
				}
			}
			return handle;
		};
		static ServerStreamManager^ getInstance(){
			if(_this == nullptr){
				_this = ref new ServerStreamManager(10);
			}
			return _this;
		};
		static void freeStream(int handle){
			ServerStreamManager^ instance = getInstance();
			if(handle >= 0 && handle < instance->StreamNum){
				instance->Streams[handle]->Status = SERVER_STREAM_STATUS_FREE;
				instance->Streams[handle]->ErrorCode = 0;
			}
		};
		static bool isStreamComplete(int handle){
			ServerStreamManager^ instance = getInstance();
			if(handle >= 0 && handle < instance->StreamNum){
				return (instance->Streams[handle]->Status == SERVER_STREAM_STATUS_RESPONSED || instance->Streams[handle]->Status == SERVER_STREAM_STATUS_ERROR);
			}
			return true;
		};
		static bool isError(int handle){
			ServerStreamManager^ instance = getInstance();
			if(handle >= 0 && handle < instance->StreamNum){
				return (instance->Streams[handle]->Status == SERVER_STREAM_STATUS_ERROR);
			}
			return false;
		};
		static ServerStream^ getStream(int handle){
			ServerStreamManager^ instance = getInstance();
			if(handle >= 0 && handle <instance->StreamNum){
				return instance->Streams[handle];
			}
			return nullptr;
		};
		static void Update();
		//static CByteBuffer* getResponseData(int handle){
		//	ServerStreamManager^ instance = getInstance();
		//	if(handle >= 0 && handle < instance->StreamNum){
		//		//only response when complete
		//		if(instance->Streams[handle]->Status == SERVER_STREAM_STATUS_RESPONSED || instance->Streams[handle]->Status == SERVER_STREAM_STATUS_ERROR){
		//			//only response when have data
		//			if(instance->Streams[handle]->ResponseData != nullptr && instance->Streams[handle]->ResponseData->Length > 0){
		//				int len = instance->Streams[handle]->ResponseData->Length;
		//				char* data = new char[len];
		//				for(int i = 0; i< len; i++){
		//					data[i] = instance->Streams[handle]->ResponseData[i];
		//				}
		//				CByteBuffer* buffer = CByteBuffer::swap(data, len);
		//				delete[] data;
		//				return buffer;
		//			}
		//		}
		//	}
		//	return NULL;
		//};
	};
}
#endif