//duy.nguyen 2014
#if !defined(_H_TEST)
#define _H_TEST
#include "../../Lib/Network/Network.h"

#define TEST_HTTP_STACK_MAX		150
#define TEST_RAW_STACK_MAX		100
#if defined(_USE_TEST)
#	define TEST_LOG 			Test::sendLog
#	define TEST_LOG_RAW			Test::sendLogInRaw
#	define USE_TEST_SERVER		Test::open
#	define TEST_UPDATE			Test::update
#	define TEST_LOG_FILE		Test::logToFile
#	define TEST_LOG_DEVICE		Test::logDeviceInfo

#define TEST_BUFFER_MAX	2048
#define TEST_RAW_PORT	7777
class Test{
protected:

	//log feature
	static char _server_url[1024];
	static int _server_url_len;
	static char _owner[255];
	static int _owner_len;
	static CHttpRequest _request_stack[TEST_HTTP_STACK_MAX];
	static CSocketClient* _request_socket[TEST_RAW_STACK_MAX];
	static char _raw_log[TEST_RAW_STACK_MAX][TEST_BUFFER_MAX];
	static int _raw_sended[TEST_RAW_STACK_MAX];
	static bool _has_sv;
public:
	static void sendLog(const char* format, ...);
	static void sendLogInRaw(const char* format, ...);
	static void open(const char* server_address, const char* owner);
	static void update();
	static void logToFile(const char* file, const char* bytes, int len);
	static void logDeviceInfo();
};
#else
#	define TEST_LOG(...)
#	define USE_TEST_SERVER(...)
#	define TEST_UPDATE(...)
#	define TEST_LOG_RAW(...)
#	define TEST_LOG_FILE(...)
#	define TEST_LOG_DEVICE(...)
#endif


#endif//_H_TEST