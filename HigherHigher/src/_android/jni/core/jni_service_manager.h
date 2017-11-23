#if !defined(_H_JNI_SERVICE_STREAM_MANAGER)
#define _H_JNI_SERVICE_STREAM_MANAGER
#include "../../../game/Lib/DataType/ServiceStream.h"
class ServiceStreamManager{
private:
	ServiceStreamManager();
	static ServiceStreamManager* _this;
protected:
	ServiceStream _svc_stream[50];
	int _count;
	int _stream_num;
public:
	static ServiceStreamManager* getInstance();

	static int request(int code, Buffer data, bool auto_release = false);
	static int request(int code, bool auto_release = false);
	static void freeStream(int handle);
	static bool isStreamResponse(int handle);
	static const ServiceStream* getStream(int handle);
	static void update();
	static void response(int id, Buffer response_data, int error_code);
};
#endif//_H_JNI_SERVICE_STREAM_MANAGER