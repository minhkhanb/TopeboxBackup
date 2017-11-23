#ifndef _CHTTPONLINE_H_
#define _CHTTPONLINE_H_

#include "../Lib/DataType/CType.h"
#include "../Lib/ArrayList/ArrayList.h"

#define HTTP_MAX_FRAME_BUFFER				2048
#define SESSION_LENGTH						32
#define HTTP_MAX_NODE						32

#define	CONN_STATUS_ERROR					-1
#define	CONN_STATUS_SEND					0		
#define	CONN_STATUS_RECEIVE_RESPONSE		1		
#define	CONN_STATUS_RECEIVE_DATA			2	
#define	CONN_STATUS_FINISH					3
#define CONN_STATUS_FREE					4


#define HTTP_SERVER_URL						"www.colorbox-software.com"
#define HTTP_SERVER_PORT					80

//This part should be modified for specific game and packed in resource data.
#define HTTP_MAX_REQUEST_STRING				512

#define _REQUEST_SEND_SCORE									"www.colorbox-software.com/online/ldtest/postScore.php"
#define _REQUEST_GET_FRIENDS_HIGHSCORE						"www.colorbox-software.com/online/ldtest/getScore.php"
#define _REQUEST_GET_GLOBAL_HIGHSCORE						"www.colorbox-software.com/online/ldtest/getScore.php"
#define _REQUEST_GET_RANK_OF_SCORE_AND_COMPETITION			"www.colorbox-software.com/online/ldtest/getScore.php"

#define _REQUEST_REGISTER					""
#define _REQUEST_LOGIN						""
#define _REQUEST_LOGOUT						""

#define _TOP_WEEK							"w"
#define _TOP_MONTH							"m"
#define _TOP_YEAR							"y"
#define _TOP_ALL_TIME						"a"

extern "C" void OpenURLInSafari(char* url);
extern "C" bool OS_IsNetworkAvailable();
extern "C" char* SendHttpRequest(const char* url, UINT16 port, const char* request, char* httpData);
extern "C" char* EncodeAndSendHttpRequest(const char* url, UINT16 port, StringList* req_keys, StringList* req_values, char* receive_data);
extern "C" void SendAsynchronousHttpRequest(const char* url, UINT16 port, const char* request);
extern "C" void EncodeAndSendAsynchronousHttpRequest(const char* url, UINT16 port, StringList* req_keys, StringList* req_values);
extern "C" void EncodeAndSendAsynchronousHttpRequestByPostMethod(const char* url, UINT16 port, StringList* req_keys, StringList* req_values);

class XMLData
{
	char* 	_data;
	int		_pointer;
	
public:

	XMLData(const char* data);
	XMLData(const char* data, int offset, int len);
	~XMLData();
	
	char* GetFirstNodeValue(const char* node_name);
	char* GetNextNodeValue(const char* node_name);
	
private:
	
	bool IsEqual(const char* str, char* p_begin, char* p_end);
};

class CHttpOnline
{
	char 			_session[SESSION_LENGTH + 1];
	XMLData* 		_received_data;
	
	int				_conn_status;
	
	StringList*		_keys;
	StringList*		_values;
		
public:
	CHttpOnline();
	~CHttpOnline();
	
	bool IsNetworkAlive();
	
	int GetRequestStatus();
	XMLData* GetResultSet();
	
	void CancelLastRequest();
	
	void AddURLParam(const char* key, const char* value);
	void ClearAllURLParam();
	
	//Requesting URL Synchronously
	void SendRequest(char* url, UINT16 port, char* request);
	void EncodeURLParamAndSendRequest(char* url, UINT16 port);
	
	//Requesting URL Asynchronously
	void SendAsynchronousRequest(char* url, UINT16 port, char* request);
	void EncodeURLParamAndSendAsynchronousRequest(char* url, UINT16 port);
	void EncodeURLParamAndSendAsynchronousRequestByPostMethod(char* url, UINT16 port);
	
	//Notify method
	void ConnectionReceiveResponseNotify();
	void ConnectionReceiveDataNotify(BYTE* data, int len);
	void ConnectionReceiveErrorNotify(const char* err);
	void ConnectionFinishLoadingNotify();
	
	void CleanHttpData();
	
	char* GetNodeString(char* nodeName);
	int GetNodeInt(char* nodeName);
		
/**
	Game online functions.
	Should be modified for specific game.
	Realated to online features design.
*/
	void SendScore(const char* device_id, const char* nick, int score, int time, unsigned long long fbuid = 0);
	void SendFacebookScore(unsigned long long fbuid, const char* nick, int score, int time);
	
	void GetFriendsHighScore(unsigned long long fbuid, int top, const char* duration, StringList* friend_list);
	void GetGlobalHighScore(const char* device_id, int top, const char* duration);
	
	void GetRankOfScoreAndCompetition(int score, int top);
	
	bool Register(char* username, char* password);
	bool Login(char* username, char* password);
	bool Logout();
	bool IsLogin();
};

#endif //_CHTTPONLINE_H_