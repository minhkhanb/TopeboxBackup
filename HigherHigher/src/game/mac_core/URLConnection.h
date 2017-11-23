#ifndef _URL_CONNECTION_H_
#define _URL_CONNECTION_H_

#import <Foundation/NSObject.h>
#import <UIKit/UIKit.h>
#import <Foundation/NSURLConnection.h>
#import "CGame.h"


@interface URLConnectionDelegate : NSObject
{
	NSURLConnection*	_url_conn;
}

@end

extern "C" char* SendHttpRequest(const char* url, UINT16 port, const char* request, char* httpData);
extern "C" char* EncodeAndSendHttpRequest(const char* url, UINT16 port, StringList* req_keys, StringList* req_values, char* receive_data);
extern "C" void SendAsynchronousHttpRequest(const char* url, UINT16 port, const char* request);
extern "C" void EncodeAndSendAsynchronousHttpRequest(const char* url, UINT16 port, StringList* req_keys, StringList* req_values);
extern "C" void EncodeAndSendAsynchronousHttpRequestByPostMethod(const char* url, UINT16 port, StringList* req_keys, StringList* req_values);
extern "C" bool OS_IsNetworkAvailable();
extern "C" void OpenURLInSafari(char* url);

#endif