/*
//written by Duy Nguyen 2014
*/

#if !defined(_H_SERVICE_STREAM)
#define _H_SERVICE_STREAM
#include "CType.h"
class ServiceStream{
protected:
	static int _count_id;
	int genID();
public:
	INT32 ID;
	int ServiceCode;
	PineServiceResult Status;
	int ErrorCode;
	bool AutoRelease;
	Buffer RequestData;
	Buffer ResponseData;
	ServiceStream();
	ServiceStream(int svc_request_code, bool auto_release = false);
	ServiceStream(int svc_request_code, Buffer request_data, bool auto_release = false);
	void reset();
};
#endif