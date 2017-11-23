#if !defined(_H_SOCKET)
#define _H_SOCKET
///
///written by Duy Nguyen - 2012
///modify to work with win8
///
#include "../DataType/CType.h"
#include "../../Define/Defines.h"
#include "../Math/CMath.h"

#define SOCKET_BUFFER_LENGHT 2048
#if defined(MAC_OS)
#	include <fcntl.h>
#	include <sys/ioctl.h>
#	include <sys/socket.h>
#	include <sys/types.h>
#	import <netinet/in.h>
//#	import <netinet6/in6.h>
#	import <arpa/inet.h>
#	import <ifaddrs.h>
#	include <netdb.h>
#	include <unistd.h>
#	define USE_SOCKET
#elif defined(_WINDOWS_PHONE8)
	//using namespace Windows::Networking::Sockets;

//#	define USE_STREAM_SOCKET
#elif defined(_WIN8)
	using namespace Windows::Networking::Sockets;
#	define USE_STREAM_SOCKET
#elif defined(ANDROID_OS)
#	include <fcntl.h>
#	include <sys/ioctl.h>
#	include <sys/socket.h>
#	include <sys/types.h>
#	import <netinet/in.h>
//#	import <netinet6/in6.h>
#	import <arpa/inet.h>
//#	import <ifaddrs.h>
#	include <netdb.h>
#	include <unistd.h>
#	define USE_SOCKET
#else
#	pragma comment(lib, "ws2_32.lib")
#	include <winsock2.h>
#	define USE_SOCKET
#	define USE_WIN_SOCK
#endif
#include <stdio.h>

#if defined(_WINDOWS_PHONE8)
using namespace Windows::Networking::Sockets;

#	define USE_STREAM_SOCKET
#endif
////#include "CEvent.h"
////#include "CUtils.h"
////extern "C" void OS_ThrowEvent(int evt_code, const char* data, int len);

/////////////////////////////////
///SOCKET DATA
/////////////////////////////////
enum class SocketConnectStatus{
	None = 0,
	Waiting = 1,
	Connected = 2,
	Error = 3,
	Writting = 4,
	Reading = 5,
};
class CSocketData{
public:
	CSocketData* _next;
	char* _data;
	CSocketData(){
		_next = NULL;
		_data = NULL;
	};
	~CSocketData(){
		if(_data != NULL){
			delete[] _data;
			_data = NULL;
		}
		_next = NULL;
	};
};

///////////////////////////////
///HTTP PARAM
///////////////////////////////
class CHttpParam{
public:
	char* _key;
	int _key_len;
	char* _value;
	int _value_len;
	CHttpParam* _next;
	CHttpParam(){
		_key = NULL;
		_value = NULL;
	};
	~CHttpParam(){
		if(_key != NULL)delete[] _key;
		if(_value != NULL)delete[] _value;
		_key = NULL;
		_value = NULL;
	};
};

class CSocket{
protected:
	static int _num_of_socket;
#if defined(USE_SOCKET) || defined(USE_WIN_SOCK)
	SOCKET _s;
#elif defined(USE_STREAM_SOCKET)
	StreamSocket^ _s;
	Platform::Array<unsigned char>^ OutputData;
	Platform::Array<unsigned char>^ InputData;
#endif
	int _buf_len;
	int _read_remain;
	int _pack_num;
	void recivedata();
	void cleanBuffer();
	CSocketData* _packs;
	CSocketData* _packs_end;
	SocketConnectStatus _connect_status;
	
public:
	void start();
	void end();
	CSocket();
#if defined(USE_SOCKET) || defined(USE_WIN_SOCK)
	CSocket(SOCKET s);
	SOCKET getSocket(){return _s;};
#elif defined(USE_STREAM_SOCKET)
	
#endif
	int sendBuffer(char* buffer, int buffer_len);
	char* getBytes();
	int getRemainLenght();
	char* getLine();
	SocketConnectStatus getStatus();
	static int getActiveSocket(){
		return _num_of_socket;
	};
	void stop();
	~CSocket(void);
};


//CLASS CSOCKET CLIENT
class CSocketClient:public CSocket{
public:
#if defined(USE_STREAM_SOCKET)
	bool TryConnect;
#endif
	bool ConnectSuccess;
	CSocketClient(const char* server_address, int sever_port);
	~CSocketClient(void);
};

enum class CHttpRequestStatus{
	Free = 0,
	WaitingConnect = 1,
	Ready = 2
};
class CHttpSendStatus{
private:

	char* _response_data;
public:

};
class CHttpRequest{
private:
	CSocketClient* _socket;
	static int _sid;
	CHttpParam* _begin;
	CHttpParam* _end;
	int _param_len;
	char* _host;
	char* _page;
	unsigned long _send_begin_time;
	int _data_len;
	static int genID(){
		int id = CHttpRequest::_sid;
		CHttpRequest::_sid = (CHttpRequest::_sid + 1) % 50000000;
		return id;
	};
	CHttpRequestStatus _connect_status;
public:
	HttpMethod _method;
	bool _sending;
	int _sended_count;
	static int _error_count;
	HttpStatus _status;
	int _id;
	char* _data;
	char* _session;
	char* _redirect_location;
	unsigned long _response_time;
	CHttpRequest();
	CHttpRequest(const char* url, int url_len, HttpMethod method = HttpMethod::Post);
	~CHttpRequest();
	void setUrl(const char* url, int url_len);
	void setMethod(HttpMethod method);
	void addParam(const char* key, int key_len, const char* value, int value_len);
	void setParam(const char* key, int key_len, const char* value, int value_len);
	void setData(const char* data, int data_len);
	CHttpRequestStatus getConnectStatus();
	char* send();
	void close();
	int getDataLength();
	void setSession(const char* session, int len){
		if (_session != NULL) delete[] _session;
		_session = new char[len + 1];
		MEMCOPY(session, _session, len);
		_session[len] = '\0';
	};
	const char* getHost(){ return _host; };
};
#endif //_H_SOCKET