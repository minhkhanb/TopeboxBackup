#include "pch.h"
#include "Network.h"
#include "../String/StringUtils.h"

#if defined(USE_STREAM_SOCKET)
#include <ppltasks.h>
using namespace Microsoft::WRL;
using namespace Windows::Storage::Streams;
using namespace Windows::System::Threading;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Concurrency;
#endif

///
///write by Duy Nguyen - 2012
///

extern "C" unsigned long OS_GetTime(void);

///////////////////////////////////////////////
///CSOCKET 
///////////////////////////////////////////////
int CSocket::_num_of_socket = 0;
void CSocket::start(){
	if(_num_of_socket == 0){
#if defined(_WINDOWS)
		WSADATA info;
		if (WSAStartup(MAKEWORD(2,0), &info)){
			throw "Could not start WSA";
		}
#endif
	}
	_num_of_socket++;
}
void CSocket::end(){
	_num_of_socket--;
#if defined(_WINDOWS)
	if(_num_of_socket == 0){
		WSACleanup();
	}
#endif
}

#if defined(USE_SOCKET) || defined (USE_WIN_SOCK)
CSocket::CSocket():_s(0),_connect_status(SocketConnectStatus::None){
	start();
	_s = socket(AF_INET,SOCK_STREAM,0);
	if (_s == INVALID_SOCKET) {
		//throw "INVALID_SOCKET";
		printf("INVALID_SOCKET");
	}
	_buf_len = 0;
	_read_remain = 0;
	_pack_num = 0;
	_packs = new CSocketData();
	_packs_end = _packs;
	_packs->_data = new char[SOCKET_BUFFER_LENGHT];
}
CSocket::CSocket(SOCKET s):_s(s)
{
	start();
}
#else
CSocket::CSocket(): _connect_status(SocketConnectStatus::None){
	_buf_len = 0;
	_read_remain = 0;
	_pack_num = 0;
	_packs = new CSocketData();
	_packs_end = _packs;
	_packs->_data = new char[SOCKET_BUFFER_LENGHT];

}
#endif

void CSocket::cleanBuffer(){
	while(_packs != _packs_end){
		CSocketData* tmp = _packs;
		_packs = _packs->_next;
		delete tmp;
	}
	_read_remain = 0;
	_buf_len = 0;
	_pack_num = 0;
}
void CSocket::recivedata(){
#if defined(USE_SOCKET)
	int total = 0;
	int readed = recv(_s, _packs_end->_data, SOCKET_BUFFER_LENGHT, 0);
	while(readed > 0){
		if(_buf_len + readed == SOCKET_BUFFER_LENGHT){
			_pack_num++;
			_packs_end->_next = new CSocketData();
			_packs_end = _packs_end->_next;
			_packs_end->_data = new char[SOCKET_BUFFER_LENGHT];
			_buf_len = 0;
		}else{
			_buf_len += readed;
		}
		readed = recv(_s, _packs_end->_data + _buf_len, SOCKET_BUFFER_LENGHT - _buf_len, 0);
	}
	if(readed > 0 && readed < SOCKET_BUFFER_LENGHT){
		_buf_len = readed;
	}
#elif defined(USE_STREAM_SOCKET)
	if (_s != nullptr && false){
		Windows::Storage::Streams::DataReader^ reader = ref new Windows::Storage::Streams::DataReader(_s->InputStream);
		int readed = 0;
		while (true){
			Windows::Storage::Streams::IBuffer^ ibuffer = reader->ReadBuffer(SOCKET_BUFFER_LENGHT);
			
			Platform::Array<unsigned char>^ result = ref new Platform::Array<unsigned char>(ibuffer->Length);

#if defined(_WIN8)
			Windows::Security::Cryptography::CryptographicBuffer::CopyToByteArray(ibuffer, &result);
#elif defined(_WINDOWS_PHONE8)
			auto reader = ::Windows::Storage::Streams::DataReader::FromBuffer(ibuffer);
			std::vector<unsigned char> data(reader->UnconsumedBufferLength);
			if (!data.empty())
				reader->ReadBytes(
				::Platform::ArrayReference<unsigned char>(
				&data[0], data.size()));

			for (int i = 0; i < data.size(); i++){
				result[i] = data[i];
			}
#endif
			readed = result->Length;
			for (int i = 0; i < result->Length; i++){
				_packs_end->_data[i] = result[i];
			}

			if (_buf_len + result->Length == SOCKET_BUFFER_LENGHT){
				_pack_num++;
				_packs_end->_next = new CSocketData();
				_packs_end = _packs_end->_next;
				_packs_end->_data = new char[SOCKET_BUFFER_LENGHT];
				_buf_len = 0;
				
			}
			else{
				_buf_len += readed;
				break;
			}
		}
		if (readed > 0 && readed < SOCKET_BUFFER_LENGHT){
			_buf_len = readed;
		}
	}
#endif
}
int CSocket::sendBuffer(char* buffer, int buffer_len){
	if (_connect_status != SocketConnectStatus::Connected) return 0;
	_connect_status = SocketConnectStatus::Writting;
#if defined(USE_STREAM_SOCKET)
	if (_s == nullptr)return 0;
	OutputData = ref new Platform::Array<unsigned char>(buffer_len);
	for (int i = 0; i < buffer_len; i++){
		OutputData[i] = buffer[i];
	}
#if defined(_WIN8)
	DataWriter^ dw = ref new DataWriter(_s->OutputStream);
	

	Windows::Storage::Streams::IBuffer^ ibuffer = Windows::Security::Cryptography::CryptographicBuffer::CreateFromByteArray(OutputData);
	dw->WriteBuffer(ibuffer);

	/*concurrency::create_task(dw->StoreAsync()).then([this, dw](task<unsigned int> writeTask)
	{
		//try
		{

			// Try getting an exception.
			writeTask.get();
			dw->DetachStream();
			delete dw;
			//delete _s;
		}
		//catch (Exception^ exception)
		//{
		//	rootPage->NotifyUser("Send failed with error: " + exception->Message, NotifyType::ErrorMessage);
		//}
	});*/
	
	
	auto action = dw->StoreAsync();

	//Windows::Foundation::IAsyncOperationWithProgress<unsigned int, unsigned int>^ action = _s->OutputStream->WriteAsync(ibuffer);
	auto task = concurrency::create_task(action);
	_connect_status = SocketConnectStatus::Connected;

	//Windows::Foundation::IAsyncOperation<bool>^ action_plush = _s->OutputStream->FlushAsync();

	//auto task_plush = concurrency::create_task(action_plush);
	//task_plush.get();

	/*task.then([this,dw](int){
		dw->DetachStream();
		delete dw;
		Windows::Foundation::IAsyncOperation<bool>^ action_plush = _s->OutputStream->FlushAsync();
		
		auto task_plush = concurrency::create_task(action_plush);
		task_plush.wait();
		task_plush.get();
		
	});*/
	task.wait();
	dw->DetachStream();
	delete dw;
	dw = nullptr;
	
	//DataReader^ dr = ref new DataReader(_s->InputStream);
	//dr->InputStreamOptions = InputStreamOptions::Partial;

	//auto action_read = dr->LoadAsync(100);
	//auto task_read = concurrency::create_task(action_read);
	//int readed = task_read.wait();

	return task.get();

	return buffer_len;
#elif defined(_WINDOWS_PHONE8)
	Windows::Storage::Streams::Buffer^ ibuffer = ref new Windows::Storage::Streams::Buffer(buffer_len);
	DataWriter^ d = ref new DataWriter(_s->OutputStream);
	d->WriteBytes(OutputData);
	concurrency::task<unsigned int>  store_action(d->StoreAsync());
	store_action.wait();
	d->DetachStream();
	_connect_status = SocketConnectStatus::Connected;
	return store_action.get();
#endif
	
#elif defined(USE_SOCKET)
	int sended = send(_s, buffer, buffer_len, 0);
	_connect_status = SocketConnectStatus::Connected;
	return sended;
#endif //USE_STREAM_SOCKET
}

SocketConnectStatus CSocket::getStatus(){
	return _connect_status;
}
int CSocket::getRemainLenght(){
	return _pack_num * SOCKET_BUFFER_LENGHT + _buf_len - _read_remain;
}
char* CSocket::getBytes(){
	char* data = NULL;
	if(_pack_num == 0 && _buf_len == 0){
		recivedata();
	}
	int len = _pack_num * SOCKET_BUFFER_LENGHT + _buf_len - _read_remain;
	///printf("GetData len:%d\n",len);
	if(len > 0){
		data = new char[len];
		int pos = 0;
		while(_packs != _packs_end){
			for(int i = _read_remain; i < SOCKET_BUFFER_LENGHT; i++){
				data[pos++] = _packs->_data[i];
			}
			_read_remain = 0;
			CSocketData* tmp = _packs;
			_packs = _packs->_next;
			delete tmp;
			tmp = NULL;
			_pack_num--;
		}
		for(int i = _read_remain; i< _buf_len; i++){
			data[pos++] = _packs_end->_data[i];
		}
		_read_remain = 0;
		_buf_len = 0;
	}
	return data;
}
char* CSocket::getLine(){
	char* line = NULL;
	//if data is empty, receive it.
	if(_pack_num == 0 && _buf_len == 0){
		recivedata();
	}
	//calculate the length of received data.
	int len = _pack_num * SOCKET_BUFFER_LENGHT + _buf_len  - _read_remain;
	///printf("===================================%d\n",len);
	int readed = 0; int total_readed = 0;
	//browser from begin of linker
	CSocketData* pointer = _packs;
	int remain = _read_remain;//the readed elements in a pack,now we read the rest.
	if(len > 0){
		char r = _packs->_data[remain];
		//if we are reading the last pack, it can be not fill of data, the data len is _buf_len.
		int max = (pointer == _packs_end)?_buf_len:SOCKET_BUFFER_LENGHT;
		while(r!='\n'){
			//read the current character
			r = pointer->_data[remain + readed];
			//move to next character
			readed++;
			total_readed++;
			//if end of the pack.
			if(remain + readed == max){
				remain = 0;
				readed = 0; // readed is the number of bytes have read in current pack.
				if(pointer != _packs_end){
					//if have more pack move to next pack.
					pointer = pointer->_next;
					//and recalculate the size of valid data.
					max = (pointer == _packs_end)?_buf_len:SOCKET_BUFFER_LENGHT;
				}else{
					//else if the current pack is the last pack, we read all. break;
					break;
				}
			}
		}
		pointer = NULL;
		///printf("LineLen:%d begin at element %d of first pack\n",total_readed,_read_remain);
		//create the new char array to hold the line's characters.
		line = new char[total_readed + 1];
		readed = 0; // readed is the number of characters that had been read in current pack.
		int pos = 0;// position of line to fill data.
		while(total_readed > 0){ // while the line not filled.
			///printf("+ the number of character remain is %d\n",total_readed);
			//calculate the number of character we will take on current pack;
			int segmen = (_packs == _packs_end)?_buf_len - _read_remain:SOCKET_BUFFER_LENGHT - _read_remain;
			///printf("\tWe can read %d character in this pack\n",segmen);
			if(segmen >= total_readed){//if the number of characters enable to read is large than the number we need.
				segmen = total_readed;//we will only take what we need.
			}
			//read all of characters we need on the pack.
			for(int i = 0; i< segmen; i++){
				line[pos++] = _packs->_data[_read_remain + i];
			}
			//after read the remain character which need to read descrease.
			total_readed -= segmen;
			//check that is end of current pack.
			if(total_readed > 0){
				///printf("\tmove to next pack\n");
				//move to next pack.
				CSocketData* tmp = _packs;
				_packs = _packs->_next;
				delete tmp;
				_pack_num--;
				//we will read at the begin of next pack
				_read_remain = 0;
			}else{//if not. we can stand in between of a pack or at end of received data.
				///printf("\tfinish reading\n");
				if(_packs == _packs_end && _read_remain == _buf_len){
					///printf("\tand stay in the end of received data, clear for next transaction");
					//is standing on the last pack at the last element of data array.
					//we have nothing to read more.
					//clean the buffer for next transaction.
					cleanBuffer();
					//and break;
					break;
				}else{
					///printf("\tmark the pos for reading next line\n");
					//save the readed postion for later reading.
					_read_remain += segmen;
				}
			}
			///printf("-we read a pack\n");
		}
		//remember to close the line.
		line[pos] = '\0';
	}
	//printf("%s\n",line);
	///printf("----------------------------------\n\n");
	return line;
}
void CSocket::stop(){
#if defined(MAC_OS)
	//if(_s != NULL){
		shutdown(_s,SHUT_RDWR);
	//}
	close(_s);
	//TLOG(TLOG_NORMAL,"release socket");
#endif
}
CSocket::~CSocket(void)
{
#if defined(_WINDOWS)
	if(_s != NULL){
		shutdown(_s,SD_BOTH);
	}
	closesocket(_s);
#endif
#if defined(MAC_OS)
	//shutdown(_s,SHUT_RDWR);
	//close(_s);
	//TLOG(TLOG_NORMAL,"release socket");
#endif
#if defined(_WIN8) || defined(_WINDOWS_PHONE8)
	if (_s != nullptr){
		delete _s;
		_s = nullptr;
	}
#endif
	cleanBuffer();
	/*if(_packs != NULL){
		while(_packs != _packs_end){
			CSocketData* tmp = _packs;
			_packs = _packs->_next;
			delete[] tmp->_data;
			delete tmp;
			_pack_num--;
		}
	}*/
	if(_packs_end != NULL){
		delete _packs_end;
	}
	end();
}



//////////////////////////////////////////////////////////////////////////////
///implement for CSocketClient class//////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
CSocketClient::CSocketClient(const char* server_address, int server_port):CSocket()
{
	ConnectSuccess = false;
#if defined(USE_SOCKET)
	hostent *he = NULL;
	he = gethostbyname(server_address);
	if (he == NULL) {
		int error_code = -1;
#if defined(USE_WIN_SOCK)
		error_code = WSAGetLastError();
#endif
		PDEBUG("get host by name false");
		return;
	}
	struct sockaddr_in addr;
	MEMSET((void *)&addr, '\0', sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;	
	addr.sin_port = htons(server_port);
	addr.sin_addr = *((in_addr *)he->h_addr);
	//addr.sin_addr.s_addr = inet_addr(server_address);
	struct timeval tv;
	tv.tv_sec = 2;
	tv.tv_usec = 20;

#if defined(USE_WIN_SOCK)
	int timeout = 2000;
	 if (setsockopt(_s, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(int)) == -1) {
		//error
		PDEBUG("set sock timeout fail");
		return;
	}
	if (setsockopt(_s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(int)) == -1) {
		//error
		PDEBUG("set sock timeout fail");
		return;
	}
#else
	if (setsockopt(_s, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) == -1) {
		//error
		PDEBUG("set sock timeout fail");
		return;
	}
	if (setsockopt(_s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1) {
		//error
		PDEBUG("set sock timeout fail");
		return;
	}
#endif

//set to non-blocking
#if defined(USE_WIN_SOCK)
	unsigned long iMode = 1;
	int iResult = ioctlsocket(_s, FIONBIO, &iMode);
	if (iResult != NO_ERROR)
	{
		printf("ioctlsocket failed with error: %ld\n", iResult);
	}
#else
	int x;
	x=fcntl(_s,F_GETFL,0);
	fcntl(_s,F_SETFL,x | O_NONBLOCK);
#endif
//try to connect
	if (connect(_s, (sockaddr *) &addr, sizeof(sockaddr)) == -1) {
#if defined(USE_WIN_SOCK)
		int error_code = WSAGetLastError();
#endif

	}
//reset to socket mode
#if defined(USE_WIN_SOCK)
	iMode = 0;
	iResult = ioctlsocket(_s, FIONBIO, &iMode);
	if (iResult != NO_ERROR)
	{
		printf("ioctlsocket failed with error: %ld\n", iResult);
	}
#else
	x = fcntl(_s, F_GETFL, 0);
	x = (x ^ O_NONBLOCK) & x;
	fcntl(_s, F_SETFL, x);
#endif

	//fd_set fdset;
	//FD_ZERO(fdset);
	//FD_SET(_s,fdset);
	//int retval = select(_s, &fdset, NULL, NULL, &tv);
	
//check on time have connect.
	fd_set Write, Err;
	FD_ZERO(&Write);
	FD_ZERO(&Err);
	FD_SET(_s, &Write);
	FD_SET(_s, &Err);
	
	// check if the socket is ready
	select(0,NULL,&Write,&Err,&tv);
	if(!FD_ISSET(_s, &Write))
	{
		//TLOG(TLOG_ERROR,"not connect in 2 sec");
		return;
	}else{
		ConnectSuccess = true;
		_connect_status = SocketConnectStatus::Connected;
		PDEBUG("[CSocketClient] connected\n");
	}
#elif defined(USE_STREAM_SOCKET)
	TryConnect = false;
	concurrency::cancellation_token_source token;
	_connect_status = SocketConnectStatus::Waiting;
	try{
		char port[255];
		sprintf(port, "%d", server_port);
		Platform::String^ svaddress = Utils::ConvertCharToString(server_address);
		Platform::String^ svport = Utils::ConvertCharToString(port);
		_s = ref new StreamSocket();
		Windows::Networking::HostName^ hostname = ref new Windows::Networking::HostName(svaddress);
		Windows::Foundation::IAsyncAction^ action = _s->ConnectAsync(hostname, svport, SocketProtectionLevel::PlainSocket);

		if (action != nullptr){
			auto task = concurrency::create_task(action);
			task.then([this]()
			{
				ConnectSuccess = true;
				
				TryConnect = true;
				_connect_status = SocketConnectStatus::Connected;
			}, token.get_token(), concurrency::task_continuation_context::use_default())
				.then([](concurrency::task<void> t)
				{
					try
					{
						t.get();
					}
					catch (Platform::COMException^ e)
					{
					}
				});
		}
		else{
			_connect_status = SocketConnectStatus::Error;
			TryConnect = true;
		}
	}
	catch (...){
		_connect_status = SocketConnectStatus::Error;
		token.cancel();
	}
#endif
}
CSocketClient::~CSocketClient()
{
	
}


//////////////////////////////////////////////////////////////////////////////
///implement for CSocketServer class//////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/*CSocketServer::CSocketServer(const char* server_name, int listen_port, int connect_num):CSocket(){
	struct sockaddr_in addr;
	sockaddr_in sai;
	sai.sin_family = AF_INET;             
	sai.sin_port = htons(listen_port);
	sai.sin_addr.s_addr = inet_addr(server_name);
	memset(&(sai.sin_zero), '\0', 8);
	if(bind(_s, (sockaddr *)&sai, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		//closesocket(s);
		//throw "INVALID_SOCKET";
		printf("INVALID_SOCKET");
	}
#if !defined(MAC_OS)
	int lastrc = WSAGetLastError();
#endif
	int ls = listen(_s, connect_num);  
	if(ls == -1){
#if !defined(MAC_OS)
		int rc = WSAGetLastError();
		printf("error:%d %d\n",lastrc,rc);
#endif
	}
	//printf("begin listen:%d\n",ls);
}
CSocket* CSocketServer::Accept(){
	sockaddr_in cli;
	int cli_len = sizeof(cli);
	SOCKET new_sock = accept(_s, (struct sockaddr*)&cli, &cli_len);
	if(new_sock == INVALID_SOCKET)return NULL;
		char ipaddress[20];
		//if(getpeername (new_sock->getCSocket(), (struct sockaddr *)&cli, &lengthptr )){
		int port = ntohs(cli.sin_port);
		sprintf(ipaddress, "%s %d", inet_ntoa(cli.sin_addr),port);
		//printf("Accept : %s\n", ipaddress);
	return new CSocket(new_sock);
}
CSocketServer::~CSocketServer(void)
{
}
*/


//////////////////////////////////////////////////////////////////////////////
///implement CHttpRequest class///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int CHttpRequest::_sid = 0;
int CHttpRequest::_error_count = 0;
CHttpRequest::CHttpRequest(){
	_host = NULL;
	_page = NULL;
	_sended_count = 0;
	_sending = false;
	_id = CHttpRequest::genID();
	_param_len = 0;
	_begin = new CHttpParam();
	_end = _begin;
	_data_len = 0;
	_data = NULL;
	_session = NULL;
	_socket = NULL;
	_status = HttpStatus::None;
	_connect_status = CHttpRequestStatus::Free;
	_redirect_location = NULL;
}
CHttpRequest::CHttpRequest(const char* url, int url_len, HttpMethod method){
	_method = method;
	_sended_count = 0;
	_sending = false;
	_id = CHttpRequest::genID();
	char* tmp_url = (char*)url;
	//remove protocol
	int protocol_pos = StringUtils::getIndexOfString(tmp_url, url_len,"http://",7);
	if(protocol_pos != -1){
		char* tmp = StringUtils::subString(tmp_url,url_len,protocol_pos + 7);
		tmp_url = tmp;
		url_len = StringUtils::getStringLen(tmp_url);
	}else{
		protocol_pos = StringUtils::getIndexOfString(tmp_url, url_len, "https://", 8);
		if(protocol_pos != -1){
			char* tmp = StringUtils::subString(tmp_url, url_len, protocol_pos + 8);
			tmp_url = tmp;
			url_len = StringUtils::getStringLen(tmp_url);
		}
	}
	//found host
	int host_pos = StringUtils::getIndexOfString(tmp_url, url_len, "/", 1);
	if(host_pos > 0){
		char* tmp_host = StringUtils::subString(tmp_url, url_len, 0, host_pos);
		char*tmp = StringUtils::subString(tmp_url, url_len, host_pos);
		url_len = StringUtils::getStringLen(tmp);
		if (tmp_url != url){
			delete[] tmp_url;
		}
		tmp_url = NULL;
		_host = tmp_host;
		_page = tmp;
	}else{
		_host = tmp_url;
		_page = new char[2];
		_page[0] = '/';
	}
	//TLOG(TLOG_NORMAL,"host:  %s",_host);
	//TLOG(TLOG_NORMAL,"page:  %s",_page);
	//printf("host:%s\n",_host);
	//printf("page:%s\n",_page);
	_param_len = 0;
	_begin = new CHttpParam();
	_end = _begin;
	_data_len = 0;
	_data = NULL;
	_session = NULL;
	_socket = NULL;
	_status = HttpStatus::None;
	_connect_status = CHttpRequestStatus::Free;
	_redirect_location = NULL;
}
void CHttpRequest::setUrl(const char* url, int url_len){
	if(_host != NULL){delete[] _host; _host = NULL;};
	if(_page != NULL){delete[] _page; _page = NULL;};
	char* tmp_url = (char*)url;
	//remove protocol
	int protocol_pos = StringUtils::getIndexOfString(tmp_url, url_len, "http://", 7);
	if(protocol_pos != -1){
		char* tmp = StringUtils::subString(tmp_url, url_len, protocol_pos + 7);
		//delete[] url;
		tmp_url = tmp;
		url_len = StringUtils::getStringLen(tmp_url);
	}else{
		protocol_pos = StringUtils::getIndexOfString(tmp_url, url_len, "https://", 8);
		if(protocol_pos != -1){
			char* tmp = StringUtils::subString(tmp_url, url_len, protocol_pos + 8);
			//delete[] url;
			tmp_url = tmp;
			url_len = StringUtils::getStringLen(tmp_url);
		}
	}
	//found host
	int host_pos = StringUtils::getIndexOfString(tmp_url, url_len, "/", 1);
	if(host_pos > 0){
		char* tmp_host = StringUtils::subString(tmp_url, url_len, 0, host_pos);
		char*tmp = StringUtils::subString(tmp_url, url_len, host_pos);
		url_len = StringUtils::getStringLen(tmp);
		delete[] tmp_url;
		tmp_url = NULL;
		_host = tmp_host;
		_page = tmp;
	}else{
		_host = tmp_url;
		_page = new char[2];
		_page[0] = '/';
	}
	//connect socket to server
	if (_socket != NULL){
		_socket->stop();
		delete _socket;
		_socket = NULL;
	}
	_socket = new CSocketClient(_host, 80);
#if defined(_WINDOWS)
	int nTimeout = 2000; // 2 seconds
	setsockopt(_socket->getSocket(), SOL_SOCKET, SO_RCVTIMEO, (const char*)&nTimeout, sizeof(int));
#endif
}
void CHttpRequest::setMethod(HttpMethod method){
	_method = method;
}
void CHttpRequest::addParam(const char* key,int key_len,const char* value,int value_len){
	//printf("ADDPARAM[%s=%s]",key,value);
 
	_end->_key = new char[key_len+1];
	MEMCOPY(key,_end->_key,key_len);
	_end->_key[key_len] = '\0';
	//_end->_key = key;
	_end->_key_len = key_len;
	_end->_value = new char[value_len+1];
	MEMCOPY(value,_end->_value, value_len);
	_end->_value[value_len] = '\0';
	//_end->_value = value;
	_end->_value_len = value_len;
	_end->_next = new CHttpParam();
	_end = _end->_next;
	_param_len += key_len + value_len + 2; //2 for '=' and '&' character
	//printf("ADDED\n");
}
void CHttpRequest::setParam(const char* key,int key_len,const char* value,int value_len){
	CHttpParam* tmp = _begin;
	while(tmp != _end){
		if (tmp->_key_len == key_len && StringUtils::getIndexOfString(key, key_len, tmp->_key, tmp->_key_len) == 0){
			if(tmp->_value != NULL)delete tmp->_value;
			tmp->_value_len = value_len;
			tmp->_value = new char[value_len + 1];
			MEMCOPY(tmp->_value, value, value_len);
			tmp->_value[value_len] = '\0';
			return;
		}
		tmp = tmp->_next;
	}
	addParam(key, key_len, value, value_len);
}
CHttpRequestStatus CHttpRequest::getConnectStatus(){
	if (_socket == NULL || _socket->getStatus() == SocketConnectStatus::None){
		_connect_status = CHttpRequestStatus::Free;
	}
	else if (_socket->getStatus() == SocketConnectStatus::Waiting){
		_connect_status = CHttpRequestStatus::WaitingConnect;
	}
	else if ((_connect_status == CHttpRequestStatus::Free || _connect_status == CHttpRequestStatus::WaitingConnect) && _socket->getStatus() == SocketConnectStatus::Connected){
		_connect_status = CHttpRequestStatus::Ready;
	}
	return _connect_status;
}
char* CHttpRequest::send(){
	//TLOG(TLOG_NORMAL,"_");
	//TLOG(TLOG_TITLE,"===SEND");
	_sended_count++;
	_sending = true;
	char data[1024];
	/*if(_socket != NULL){
		_socket->stop();
		delete _socket;
		_socket = NULL;
	}*/
	if(_redirect_location != NULL){
		delete[] _redirect_location;
		_redirect_location = NULL;
	}
	//TLOG(TLOG_ERROR,"here");
	//_socket = new CSocketClient(_host,80);
	//if ((_socket != NULL && _socket->getStatus() != SocketConnectStatus::Connected) || _page == NULL || _host == NULL){
		 //_socket->stop();
		//delete _socket;
		//_socket = NULL;
	//	_sending = false;
	//	return NULL;
	//}
	if (_connect_status != CHttpRequestStatus::Ready){
		_sending = false;
		return NULL;
	}

	_send_begin_time = OS_GetTime();
	//prepare param
	CHttpParam* sufter = _begin;
	char* param = NULL;
	if(_param_len > 0){
		param = new char[_param_len];
		int pos = 0;
		while(sufter != _end){
			//sprintf(param+pos,"%s=%s&",sufter->_key,sufter->_value);
			MEMCOPY(sufter->_key,param + pos,sufter->_key_len);
			pos += sufter->_key_len;
			param[pos++] = '=';
			MEMCOPY(sufter->_value,param + pos,sufter->_value_len);
			pos += sufter->_value_len;
			param[pos++] = '&';
			//pos+= sufter->_key_len + sufter->_value_len + 2;
			sufter = sufter->_next;
		}
		param[_param_len - 1] = '\0';
	}


if(_method == HttpMethod::Post){
	sprintf(data,"POST %s HTTP/1.0\r\n",_page);
}else if(_method == HttpMethod::Get){
	if(param != NULL){
		sprintf(data,"GET %s?%s HTTP/1.0\r\n",_page,param);
		//printf("GET %s?%s \n",_page,param);
	}else{
		sprintf(data,"GET %s HTTP/1.0\r\n",_page);
	}
}else if(_method == HttpMethod::Del){
}
_socket->sendBuffer(data, StringUtils::getStringLen(data));
	sprintf(data,"Host: %s\r\n",_host);
	_socket->sendBuffer(data, StringUtils::getStringLen(data));
	_socket->sendBuffer("User-Agent: PineGame/1.0\r\n", StringUtils::getStringLen("User-Agent: PineGame/1.0\r\n"));
	if(_method == HttpMethod::Post){
		sprintf(data,"Content-Length: %d\r\n",(_param_len>0)?(_param_len - 1):0);
		_socket->sendBuffer(data, StringUtils::getStringLen(data));
	}
	/*if(_session != NULL){
		sprintf(data,"Cookie: PHPSESSID=%s;",_session);
		_socket->sendBuffer(data,getStringLen(data));
		printf("%s\n",data);
	}*/
	_socket->sendBuffer("Content-Type: application/x-www-form-urlencoded\r\n", StringUtils::getStringLen("Content-Type: application/x-www-form-urlencoded\r\n"));
	_socket->sendBuffer("Accept-Charset: utf-8\r\n\r\n", StringUtils::getStringLen("Accept-Charset: utf-8\r\n\r\n"));
	//if post send param
	//param[_param_len] = '\0';
	
	if(param != NULL){
		if(_method == HttpMethod::Post){
			//printf("POST %s\n",param);
			_socket->sendBuffer(param,_param_len-1);
		}
		//printf("PARAMDATA:[%s]\n",param);
		delete[] param;
		param = NULL;
	}
	_socket->sendBuffer("\r\n",2);
	char* line = _socket->getLine();
	bool found_session = false;
	bool found_status = false;
	while (line != NULL && StringUtils::getStringLen(line)>2){
		//printf("%s\n",line);
		//find the session info
		if(!found_session){
			int sessionpos = StringUtils::getIndexOfString(line, StringUtils::getStringLen(line), "PHPSESSID", 9);
			if(sessionpos>=0){
				sessionpos += 10;
				int end = StringUtils::getIndexOfString(line, StringUtils::getStringLen(line), ";", 1);
				if(_session!=NULL)delete[] _session;
				_session = StringUtils::subString(line, StringUtils::getStringLen(line), sessionpos, end - sessionpos);
				//printf("SESSION:%s\n",_session);
				found_session = true;
			}
		}
		if(!found_status){
			int line_len = StringUtils::getStringLen(line);
			int httpversionpos = StringUtils::getIndexOfString(line, line_len, "HTTP/", 5);
			if(httpversionpos >= 0){
				_status = HttpStatus::None;
				char* version = StringUtils::subString(line, line_len, httpversionpos + 5);
				int version_len = StringUtils::getStringLen(version);
				//found version;
				int spacepos = StringUtils::getIndexOfString(version, version_len, " ", 1);
				if(spacepos >= 0){
					char* status = StringUtils::subString(version, version_len, spacepos + 1);
					int status_len = StringUtils::getStringLen(status);
					spacepos = StringUtils::getIndexOfString(status, status_len, " ", 1);
					if(spacepos >= 0){
						char* res_status = StringUtils::subString(status, status_len, 0, spacepos);
						int res_status_len = StringUtils::getStringLen(res_status);
						if(res_status_len > 0){
							int status = (int)_status;
							//#STR_TO_UINT(res_status,res_status_len, (int)status);
							if (_status != HttpStatus::Ok && _status != HttpStatus::None){
								_error_count++;
							}
							//TLOG_HIGHLIGHT(0xFF00FFFF,TLOG_ERROR,"line:%s",line);
							//TLOG_HIGHLIGHT(0xFF00FFFF,TLOG_ERROR,"status:version:%s status:%s res_status:%s %d",version, status, res_status,_status);
						}
						delete[] res_status;
					}
					delete[] status;
				}
				delete[] version;
				found_status = true;
			}
		}else if(_status == HttpStatus::Found){
			//redirect find location
			int line_len = StringUtils::getStringLen(line);
			int location_pos = StringUtils::getIndexOfString(line, line_len, "Location:", 9);
			if(location_pos >= 0){
				if(_redirect_location != NULL) delete[] _redirect_location;
				int rtpos = StringUtils::getIndexOfString(line, line_len, "\r\n", 2);
				if(rtpos < 0) rtpos = line_len -1;
				_redirect_location = StringUtils::subString(line, line_len, location_pos + 10, rtpos - (location_pos + 10));
				printf("_redirec_location:%s\n", _redirect_location);
			}
		}
		delete[] line;
		line = _socket->getLine();
	}
	if(line != NULL)delete[] line;
	_data_len = _socket->getRemainLenght();
	_response_time = OS_GetTime() - _send_begin_time;
	if(_data != NULL){
		delete[] _data;
	}
	_data = _socket->getBytes();
	 _socket->stop();
	delete _socket;
	_socket = NULL;
	_sending = false;
	printf("sended\n");
	return _data;
}
void CHttpRequest::close(){
	if(_socket != NULL){
		 _socket->stop();
		delete _socket;
		_socket = NULL;
	}else{
		//TLOG(TLOG_NORMAL,"===socket not null");
	}
	if(_data != NULL){
		delete[] _data;
		_data = NULL;
	}
	if(_redirect_location != NULL){
		delete[] _redirect_location;
		_redirect_location = NULL;
	}
	_status = HttpStatus::None;
	_sending = false;
	_connect_status = CHttpRequestStatus::Free;
}
int CHttpRequest::getDataLength(){
	return _data_len;
}
CHttpRequest::~CHttpRequest(){
	//TLOG(TLOG_NORMAL,"delete http request");
	close();
	if(_redirect_location != NULL){
		delete[] _redirect_location;
		_redirect_location = NULL;
	}
	if(_data != NULL){
		delete[] _data;
		_data = NULL;
	}
	if(_host != NULL){
		delete[] _host;
		_host = NULL;
	}
	if(_page != NULL){
		delete[] _page;
		_page = NULL;
	}
	if(_session != NULL){
		delete[] _session;
		_session = NULL;
	}
	CHttpParam* tmp = NULL;
	while(_begin != _end){
		tmp = _begin;
		_begin = _begin->_next;
		delete tmp;
	}
	delete _end;
	//TLOG(TLOG_NORMAL,"delete http request finish");
}
