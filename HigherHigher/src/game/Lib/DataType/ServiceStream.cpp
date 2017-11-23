#include "pch.h"
#include "ServiceStream.h"

int ServiceStream::_count_id = 0;
int ServiceStream::genID(){
	_count_id++;
	_count_id %= 50000;
	return _count_id;
}
ServiceStream::ServiceStream(){
	reset();
}
ServiceStream::ServiceStream(int svc_request_code, bool auto_release){
	ServiceCode = svc_request_code;
	ID = genID();
	ErrorCode = -1;
	Status = PineServiceResult::WaitingResponse;
	RequestData.Release();
	ResponseData.Release();
	AutoRelease = auto_release;
}
ServiceStream::ServiceStream(int svc_request_code, Buffer request_data, bool auto_release){
	ServiceCode = svc_request_code;
	ID = genID();
	ErrorCode = -1;
	Status = PineServiceResult::WaitingResponse;
	RequestData = request_data;
	ResponseData.Release();
	AutoRelease = auto_release;
}
void ServiceStream::reset(){
	Status = PineServiceResult::Free;
	ErrorCode = -1;
	ID = genID();
	RequestData.Release();
	ResponseData.Release();
}
