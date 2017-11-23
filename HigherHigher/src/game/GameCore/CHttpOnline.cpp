
//#ifndef MAC_OS
#include "pch.h"
//#endif

#include "CHttpOnline.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef MAC_OS
	#include "iPhoneGameUtils.h"
#elif ANDROID_OS
#elif _WINDOWS_PHONE8
#else
	#include <WinHTTP.h>
	#include <Sensapi.h>
#endif


/**
	Start of XMLData class
*/

XMLData::XMLData(const char* rData)
{
	int len = strlen(rData);
	_data = new char[len + 1];
	strcpy(_data, rData);
	_pointer = 0;
}

XMLData::XMLData(const char* rData, int offset, int length)
{
	_data = new char[length + 1];
	strncpy(_data, rData + offset, length);
	_data[length] = '\0';
	_pointer = 0;
}

XMLData::~XMLData()
{
	SAFE_DEL_ARRAY(_data);
}

char* XMLData::GetFirstNodeValue(const char* node_name)
{
	_pointer = 0;
	
	return GetNextNodeValue(node_name);
}

char* XMLData::GetNextNodeValue(const char* node_name)
{
	char* p = _data + _pointer;
	char* p_begin, *p_end, *p_value_begin, *p_value_end;
	bool is_node_end;
	
	p_value_begin = NULL;
	p_value_end = NULL;
	while (*p != '\0')
	{
		//begin tag
		if (*p == '<')
		{
			is_node_end = false;
			
			if (*(p+1) == '/')
			{
				is_node_end = true;
				p++;
			}
			
			p_begin = p + 1;
			
			//find end tag
			while (*p != '\0' && *p != '>')
			{
				p++;
			}
			
			p_end = p - 1;
			
			//compare with input node name
			//node begin
			if (!is_node_end && IsEqual(node_name, p_begin, p_end))
			{
				p_value_begin = p_end + 2;
			}
			//node end
			else if (is_node_end && IsEqual(node_name, p_begin, p_end))
			{
				p_value_end = p_begin - 3;
				
				break;
			}
		}
		else
		{
			p++;
		}
	}
	
	if (p_value_begin != NULL && p_value_end != NULL)
	{
		int len = p_value_end + 1 - p_value_begin;
		char* result = new char[len + 1];
		
		for (int i = 0; i < len; i++)
		{
			result[i] = p_value_begin[i];
		}
		
		result[len] = '\0';
		
		_pointer = p_end - _data;
		return result;
	}
	
	_pointer = 0;
	return NULL;
}

bool XMLData::IsEqual(const char* str, char* p_begin, char* p_end)
{
	int len = strlen(str);
	
	if (len != (p_end + 1 - p_begin))
	{
		return false;
	}
	
	int off = 0;
	while (off < len)
	{
		if (str[off] != p_begin[off])
		{
			return false;
		}
		
		off++;
	}
	
	return true;
}

/**
	End of XMLData class
*/


/**
	Start of CHttpOnline class
*/
CHttpOnline::CHttpOnline()
{
	memset(_session, 0, SESSION_LENGTH + 1);
	
	_received_data = NULL;
	
	_keys = new StringList();
	_values = new StringList();
}

CHttpOnline::~CHttpOnline()
{
	CleanHttpData();
	ClearAllURLParam();
	
	SAFE_DEL(_keys);
	SAFE_DEL(_values);
}

bool CHttpOnline::IsNetworkAlive()
{
	return OS_IsNetworkAvailable();
}

int CHttpOnline::GetRequestStatus()
{
	return _conn_status;
}

XMLData* CHttpOnline::GetResultSet()
{
	_conn_status = CONN_STATUS_FREE;
	return _received_data;
}

void CHttpOnline::CancelLastRequest()
{
	CleanHttpData();
}

void CHttpOnline::AddURLParam(const char* key, const char* value)
{
	_keys->AddNewString(key);
	_values->AddNewString(value);
}

void CHttpOnline::ClearAllURLParam()
{
	_keys->ClearAll();
	_values->ClearAll();
}

void CHttpOnline::SendRequest(char* url, UINT16 port, char* request)
{
	CleanHttpData();
	char httpData[HTTP_MAX_FRAME_BUFFER + 1] = {0};
	
	SendHttpRequest(url, port, request, httpData);
	
	_received_data = new XMLData(httpData);
}

void CHttpOnline::EncodeURLParamAndSendRequest(char* url, UINT16 port)
{
	CleanHttpData();
	char httpData[HTTP_MAX_FRAME_BUFFER + 1] = {0};
	
	EncodeAndSendHttpRequest(url, port, _keys, _values, httpData);
	
	_received_data = new XMLData(httpData);
}

void CHttpOnline::SendAsynchronousRequest(char* url, UINT16 port, char* request)
{
	_conn_status = CONN_STATUS_SEND;
	
	SendAsynchronousHttpRequest(url, port, request);
}

void CHttpOnline::EncodeURLParamAndSendAsynchronousRequest(char* url, UINT16 port)
{
	_conn_status = CONN_STATUS_SEND;
	
	EncodeAndSendAsynchronousHttpRequest(url, port, _keys, _values);
}

void CHttpOnline::EncodeURLParamAndSendAsynchronousRequestByPostMethod(char* url, UINT16 port)
{
	_conn_status = CONN_STATUS_SEND;
	
	EncodeAndSendAsynchronousHttpRequestByPostMethod(url, port, _keys, _values);
}

//Notify method
void CHttpOnline::ConnectionReceiveResponseNotify()
{
	_conn_status = CONN_STATUS_RECEIVE_RESPONSE;
}

void CHttpOnline::ConnectionReceiveDataNotify(BYTE* data, int len)
{
	//do something
	CleanHttpData();
	_received_data = new XMLData((char*)data, 0, len);

	SAFE_DEL_ARRAY(data);
	
	_conn_status = CONN_STATUS_RECEIVE_DATA;
}

void CHttpOnline::ConnectionReceiveErrorNotify(const char* err)
{
	_conn_status = CONN_STATUS_ERROR;
}

void CHttpOnline::ConnectionFinishLoadingNotify()
{
	_conn_status = CONN_STATUS_FINISH;
}

void CHttpOnline::CleanHttpData()
{
	SAFE_DEL(_received_data);
	_received_data = NULL;
}

char* CHttpOnline::GetNodeString(char* nodeName)
{
	return _received_data->GetFirstNodeValue(nodeName);
}

int CHttpOnline::GetNodeInt(char* nodeName)
{
	char* str = _received_data->GetFirstNodeValue(nodeName);
	
	int ret = -1;
	if (str != NULL)
	{
		ret = atoi(str);
		SAFE_DEL_ARRAY(str);
	}
	
	return ret;
}

/**
Game online functions.
Should be modified for specific game.
Realated to online features design.
*/

void CHttpOnline::SendScore(const char* device_id, const char* nick, int score, int time, unsigned long long fbuid)
{
	char param[50];
	ClearAllURLParam();
	AddURLParam("cmd", "sendscore");
	
	AddURLParam("device", device_id);
	
	if (fbuid > 0)
	{
		sprintf(param, "%lld", fbuid);
		AddURLParam("facebookid", param);
	}
	
	AddURLParam("name", nick);
	AddURLParam("type", "1");
	
	sprintf(param, "%d", score);
	AddURLParam("score", param);
	
	sprintf(param, "%d", time);
	AddURLParam("time", param);
	
	EncodeURLParamAndSendAsynchronousRequest(_REQUEST_SEND_SCORE, 80);
}

void CHttpOnline::SendFacebookScore(unsigned long long fbuid, const char* nick, int score, int time)
{
	char param[50];
	ClearAllURLParam();
	AddURLParam("cmd", "sendscore");
	
	sprintf(param, "%lld", fbuid);
	AddURLParam("facebookid", param);
	
	AddURLParam("name", nick);
	AddURLParam("type", "1");
	
	sprintf(param, "%d", score);
	AddURLParam("score", param);
	
	sprintf(param, "%d", time);
	AddURLParam("time", param);
	
	EncodeURLParamAndSendAsynchronousRequest(_REQUEST_SEND_SCORE, 80);
}

void CHttpOnline::GetGlobalHighScore(const char* device_id, int top, const char* duration)
{
	char param[50];
	ClearAllURLParam();
	
	AddURLParam("cmd", "gettopranks");
	AddURLParam("device", device_id);
	AddURLParam("type", "1");
	
	sprintf(param, "%d", top);
	AddURLParam("top", param);
	
	AddURLParam("duration", duration);
	
	EncodeURLParamAndSendAsynchronousRequest(_REQUEST_GET_GLOBAL_HIGHSCORE, 80);
}

void CHttpOnline::GetFriendsHighScore(unsigned long long fbuid, int top, const char* duration, StringList* friend_list)
{
	if (friend_list == NULL || friend_list->_num_items <= 0)
	{
		return;
	}
	
	char param[50];
	ClearAllURLParam();
	
	AddURLParam("cmd", "gettopfaceranks");
	
	sprintf(param, "%lld", fbuid);
	AddURLParam("facebookid", param);
	
	AddURLParam("type", "1");
	
	sprintf(param, "%d", top);
	AddURLParam("top", param);
	
	AddURLParam("duration", duration);
	
	//parse friends list
	char* friends_param = new char[friend_list->_num_items*32];
	friends_param[0] = '\0';
	
	friend_list->ResetSearchPointer();
	StringListItem* friendid = friend_list->GetCurrentItem();
	
	while (friendid != NULL)
	{
		strcat(friends_param, friendid->_val);
		
		friend_list->NextItem();
		friendid = friend_list->GetCurrentItem();
		
		if (friendid != NULL)
		{
			strcat(friends_param, ",");
		}
	}
	
	AddURLParam("friendids", friends_param);
	SAFE_DEL_ARRAY(friends_param);
	
	EncodeURLParamAndSendAsynchronousRequestByPostMethod(_REQUEST_GET_FRIENDS_HIGHSCORE, 80);
}

void CHttpOnline::GetRankOfScoreAndCompetition(int score, int top)
{
	char param[50];
	ClearAllURLParam();
	
	AddURLParam("cmd", "getrankscore");
	
	sprintf(param, "%d", score);
	AddURLParam("score", param);
	
	sprintf(param, "%d", top);
	AddURLParam("top", param);
	
	AddURLParam("type", "1");
	
	EncodeURLParamAndSendAsynchronousRequest(_REQUEST_GET_RANK_OF_SCORE_AND_COMPETITION, 80);
}


bool CHttpOnline::Register(char* username, char* password)
{
	char reqString[HTTP_MAX_REQUEST_STRING];
	sprintf(reqString, _REQUEST_REGISTER, username, password);
	SendRequest(HTTP_SERVER_URL, HTTP_SERVER_PORT, reqString);
	return !strcmp("ok", GetNodeString("result"));
}

bool CHttpOnline::Login(char* username, char* password)
{
	char reqString[HTTP_MAX_REQUEST_STRING];
	sprintf(reqString, _REQUEST_LOGIN, username, password);
	SendRequest(HTTP_SERVER_URL, HTTP_SERVER_PORT, reqString);
	if(!strcmp("ok", GetNodeString("result")))
	{
		strcpy(_session, GetNodeString("session"));
		return true;
	}
	return false;
}

bool CHttpOnline::Logout()
{
	char reqString[HTTP_MAX_REQUEST_STRING];
	sprintf(reqString, _REQUEST_LOGOUT, _session);
	SendRequest(HTTP_SERVER_URL, HTTP_SERVER_PORT, reqString);
	*_session = 0;
	return !strcmp("ok", GetNodeString("result"));
}

bool CHttpOnline::IsLogin()
{
	return (strlen(_session) > 0);
}
/**
	End of CHttpOnline class
*/


#ifndef MAC_OS
/**
	Simulate HTTP request in windows platform
*/
void OpenURLInSafari(char* url)
{
}
/*bool OS_IsNetworkAvailable()
{
	LPDWORD flags;
	//if (IsNetworkAlive(flags))
	{
	//	return TRUE;
	}
	
	return TRUE;
}*/

char* SendHttpRequest(const char* url, UINT16 port, const char* strReq, char* httpData)
{
	HINTERNET  hSession = NULL,
				hConnect = NULL,
				hRequest = NULL;

	// Use WinHttpOpen to obtain a session handle.
	if(hSession = WinHttpOpen( L"WinHTTP Example/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0 ))
	{
		wchar_t* wUrl = NULL;
		if(url)
		{
			int urlLen = strlen(url);
			wUrl = new wchar_t[urlLen + 1];
			mbstowcs(wUrl, url, urlLen + 1);
		}
		wchar_t* wRequest = NULL;
		if(strReq)
		{
			int requestLen = strlen(strReq);
			wRequest = new wchar_t[requestLen + 1];
			mbstowcs(wRequest, strReq, requestLen + 1);
		}
		if(hConnect = WinHttpConnect( hSession, wUrl, port, 0 ))
		{
			if(hRequest = WinHttpOpenRequest( hConnect, L"GET", wRequest, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0 ))
			{
				if(WinHttpSendRequest( hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0 ))
				{
					if(WinHttpReceiveResponse( hRequest, NULL ))
					{
						DWORD bUsed = 0, bFree = HTTP_MAX_FRAME_BUFFER, bSize = 0, bRead = 0;
						do
						{
							WinHttpQueryDataAvailable( hRequest, &bSize );
							if(bSize > 0)
							{								
								if(bSize > bFree)
									bSize = bFree;
								if( WinHttpReadData( hRequest, (LPVOID)(httpData + bUsed), bSize, &bRead ) )
								{
									bUsed += bRead;
									bFree = HTTP_MAX_FRAME_BUFFER - bUsed;
								}								
							}
						}while(bSize > 0 && bFree > 0);
					}
				}
			}
		}
		SAFE_DEL_ARRAY(wUrl);
		SAFE_DEL_ARRAY(wRequest);
	}
	// Close any open handles.
	if( hRequest ) WinHttpCloseHandle( hRequest );
	if( hConnect ) WinHttpCloseHandle( hConnect );
	if( hSession ) WinHttpCloseHandle( hSession );
	return httpData;
}

char* EncodeAndSendHttpRequest(const char* url, UINT16 port, StringList* req_keys, StringList* req_values, char* receive_data)
{
	return NULL;
}

void SendAsynchronousHttpRequest(const char* url, UINT16 port, const char* request)
{
}

void EncodeAndSendAsynchronousHttpRequest(const char* url, UINT16 port, StringList* req_keys, StringList* req_values)
{
}

extern "C" void EncodeAndSendAsynchronousHttpRequestByPostMethod(const char* url, UINT16 port, StringList* req_keys, StringList* req_values)
{
}
#endif