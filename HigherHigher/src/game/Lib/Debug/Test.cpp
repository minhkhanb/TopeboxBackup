//duy.nguyen 2014
#include "pch.h"
#include "Test.h"
#include <stdarg.h>
#if defined(_USE_TEST)
//init static of log feature
char Test::_server_url[1024] = {'\0'};
int Test::_server_url_len = 0;
char Test::_owner[255] = { '\0' };
int Test::_owner_len = 0;
CHttpRequest Test::_request_stack[TEST_HTTP_STACK_MAX] = {};
CSocketClient* Test::_request_socket[TEST_RAW_STACK_MAX] = {NULL};
char Test::_raw_log[TEST_RAW_STACK_MAX][TEST_BUFFER_MAX] = { '\0' };
int Test::_raw_sended[TEST_RAW_STACK_MAX] = { 0 };
bool Test::_has_sv = false;

//implement function
void Test::sendLog(const char* format, ...){
	if (!_has_sv) return;
	if(_server_url_len <= 0)return;
	//find a free http pile
	int handle = -1;
	for (int h = 0; h < TEST_HTTP_STACK_MAX; h++){
		if (_request_stack[h].getConnectStatus() == CHttpRequestStatus::Free){
			handle = h;
			break;
		}
	}
	if (handle > -1 && handle < TEST_HTTP_STACK_MAX){
		char buf[TEST_BUFFER_MAX];
		va_list ptr;
		va_start(ptr, format);
		vsprintf(buf, format, ptr);
		_request_stack[handle].setMethod(HttpMethod::Post);
		_request_stack[handle].setUrl(_server_url, _server_url_len);
		_request_stack[handle].addParam("log", 3, buf, strlen(buf));
		_request_stack[handle].addParam("owner", 5, _owner, _owner_len);
		if (_request_stack[handle].getConnectStatus() == CHttpRequestStatus::Ready){
			char* receive = _request_stack[handle].send();
			if (receive != NULL) delete[] receive;
			_request_stack[handle].close();
		}
		va_end(ptr);
	}
}
void Test::sendLogInRaw(const char* format, ...){
	if (!_has_sv) return;
	PDEBUG("call sendlog in Raw\n");
	char buf[TEST_BUFFER_MAX];
	va_list ptr;
	va_start(ptr, format);
	vsprintf(buf, format, ptr);
	va_end(ptr);
	int handle = -1;
	for (int s = 0; s < TEST_BUFFER_MAX; s++){
		if (_request_socket[s] == NULL){
			handle = s;
			PDEBUG("found socket at handle = :%d\n", s);
			break;
		}
	}
	if (handle > -1){
		PDEBUG("init socket at:%d to send :%s\n", handle, buf);
		sprintf(&_raw_log[handle][0], "%s", buf);
		_raw_sended[handle] = 0;
		_request_socket[handle] = new CSocketClient(_server_url, TEST_RAW_PORT);
	}
}
void Test::update(){
	if (!_has_sv) return;
	for (int h = 0; h < TEST_HTTP_STACK_MAX; h++){
		if (_request_stack[h].getConnectStatus() == CHttpRequestStatus::Ready){
			char* receive = _request_stack[h].send();
			if (receive != NULL) delete[] receive;
			_request_stack[h].close();
		}
	}
	for (int s = 0; s < TEST_RAW_STACK_MAX; s++){
		if (_request_socket[s] != NULL){
			if (_request_socket[s]->ConnectSuccess){
				int len = strlen(_raw_log[s]);
				
				if (_raw_sended[s] < len){
					if (_raw_sended[s] == 0){
						char header[4];
						header[3] = len >> 24;
						header[2] = len >> 16;
						header[1] = len >> 8;
						header[0] = len & 0xFF;
						_request_socket[s]->sendBuffer(header, 4);
					}
					PDEBUG("socket at handle:%d remain:%d sended:", s, len - _raw_sended[s], _raw_sended[s]);
					
					int sended = _request_socket[s]->sendBuffer(&_raw_log[s][_raw_sended[s]], len - _raw_sended[s]);
					if (sended > 0){
						_raw_sended[s] += sended;
					}
					PDEBUG("%d\n", _raw_sended[s]);
				}
				else{
					PDEBUG("close socket at handle:%d\n", s);
					delete _request_socket[s];
					_request_socket[s] = NULL;
				}
			}
			else{
				PDEBUG("waiting for socket at handle : %d connect\n", s);
			}
		}
	}
}
void Test::open(const char* server_address, const char* owner){
	if (!_has_sv)
	{
		for (int i = 0; i < TEST_RAW_STACK_MAX; i++){
			_request_socket[i] = NULL;
			_raw_log[i][0] = '\0';
			_raw_sended[i] = 0;
		}
	}
	int len = strlen(server_address);
	if(len > 0){
		sprintf(_server_url,"%s", server_address);
		_server_url_len = len;
		int ownerlen = strlen(owner);
		if (ownerlen > 0){
			sprintf(_owner, "%s", owner);
			_owner_len = ownerlen;
		}
		else{
			sprintf(_owner, "%s", "undefined");
			_owner_len = strlen("undefined");
		}
		_has_sv = true;
	}
	
}
void Test::logToFile(const char* fpath, const char* bytes, int len){
	FILE* pfile = fopen(fpath,"wb");
	if(pfile != NULL){
		fwrite(bytes, sizeof(char), len, pfile);
		fclose(pfile);
	}
}

//extern "C" void testGraphicDeviceInfo();
void Test::logDeviceInfo(){
	//testGraphicDeviceInfo();
}
#endif