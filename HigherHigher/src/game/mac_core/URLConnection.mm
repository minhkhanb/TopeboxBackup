// for MD5 hash
#import <CommonCrypto/CommonDigest.h>

#import <SystemConfiguration/SCNetworkReachability.h>
#import <netinet/in.h>

#import "URLConnection.h"
#import "PineGameDelegate.h"
#import "Defines.h"
#import "CGame.h"

extern CGame* g_pGame;

@implementation URLConnectionDelegate

- (void) init
{
	_url_conn = nil;
}

- (void) dealloc
{
	[super dealloc];
}

- (void) sendAsynchronousHttpGetRequest: (NSString*) req
{
	NSMutableURLRequest *request = [[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString: req]]; 
	[request setHTTPMethod: @"GET"];
	
	_url_conn = [[NSURLConnection alloc] initWithRequest: request delegate: self];
	
	if (!_url_conn)
	{
		//NSLog(@"send score error");
	}
}

- (void) sendAsynchronousHttpPostRequest: (NSString*) url HTTPBody: (NSString*)http_body
{
	NSMutableURLRequest *request = [[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString: url]];
	[request setHTTPMethod: @"POST"];
	[request setHTTPBody: [http_body dataUsingEncoding: NSUTF8StringEncoding]];
	
	_url_conn = [[NSURLConnection alloc] initWithRequest: request delegate: self];
	
	if (!_url_conn)
	{
		//NSLog(@"send score error");
	}
}

- (void) connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{

	//g_pGame->_http_online->ConnectionReceiveResponseNotify();
}

- (void) connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
	int len = [data length];
	BYTE* online_data = new BYTE[len];
	
	[data getBytes:online_data length:len];
	
	NSString* string = [[NSString alloc] initWithBytes:online_data length:len encoding:NSASCIIStringEncoding];
	NSLog(string);
	
	//g_pGame->_http_online->ConnectionReceiveDataNotify(online_data, len);
}

- (void) connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    // release the connection, and the data object
    [connection release];
	
	//g_pGame->_http_online->ConnectionReceiveErrorNotify([[error localizedDescription] UTF8String]);
	
	[self release];
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{	
    [connection release];
	
	//g_pGame->_http_online->ConnectionFinishLoadingNotify();
	
	[self release];
}

@end

NSString* HashDataWithSecretKey(StringList* req_values)
{
	CC_MD5_CTX		md5_ctx;
	
	req_values->ResetSearchPointer();
	StringListItem* value = req_values->GetCurrentItem();
	
	CC_MD5_Init(&md5_ctx);
	while (value != NULL)
	{
		CC_MD5_Update(&md5_ctx, value->_val, value->_len);
		
		req_values->NextItem();
		value = req_values->GetCurrentItem();
	}
	
	//CC_MD5_Update(&md5_ctx, FB_APPS_SECRET_KEY, FB_APPS_KEY_LEN);
	
	// then get the hash
	unsigned char hash_tmp[16];
	CC_MD5_Final(hash_tmp, &md5_ctx);
	
	NSString* hash = [NSString stringWithString:@""];
	for (int i = 0; i < 16; i++) 
	{
		hash = [hash stringByAppendingFormat:@"%02x", hash_tmp[i]];
	}
	
	return hash;
}

NSString* EncodeURL(const char* str)
{
	NSString* encoded_str = [NSString stringWithUTF8String: str];
	
	encoded_str = [encoded_str stringByAddingPercentEscapesUsingEncoding: NSUTF8StringEncoding];
	
	// '&' and '=' should be encoded manually
	encoded_str = [encoded_str stringByReplacingOccurrencesOfString:@"&" withString:@"%26"];
	encoded_str = [encoded_str stringByReplacingOccurrencesOfString:@"=" withString:@"%3D"];
	
	return encoded_str;
}

NSString* BuildURLRequest(StringList* req_keys, StringList* req_values)
{
	NSString* url_request = [NSString stringWithString:@""];
	
	req_keys->ResetSearchPointer();
	req_values->ResetSearchPointer();
	StringListItem* key = req_keys->GetCurrentItem();
	StringListItem* value = req_values->GetCurrentItem();
	
	while (key != NULL && value != NULL)
	{
		//encode
		//url_request = [url_request stringByAppendingFormat:@"%s=%@", key->_val, EncodeURL(value->_val)];
		//non encode
		url_request = [url_request stringByAppendingFormat:@"%s=%s", key->_val, value->_val];
		
		req_keys->NextItem();
		key = req_keys->GetCurrentItem();
		
		req_values->NextItem();
		value = req_values->GetCurrentItem();
		
		if (key != NULL && value != NULL)
		{
			url_request = [url_request stringByAppendingFormat:@"&"];
		}
	}
		
	return url_request;
}

extern "C" char* SendHttpRequest(const char* url, UINT16 port, const char* strReq, char* httpData)
{
	NSString* strUrl = [NSString stringWithFormat:@"http://%s?%s",url, strReq];
	NSMutableURLRequest *request = [ [ NSMutableURLRequest alloc ] initWithURL: [ NSURL URLWithString: strUrl ] ]; 
	[request setHTTPMethod: @"GET"];
	NSData* returnData = [ NSURLConnection sendSynchronousRequest: request returningResponse: nil error: nil ];
	if (returnData)
	{
		strcpy(httpData, (char*)[returnData bytes]);
	}
	
	[request release];
	[returnData release];
	[strUrl release];

	//NSString* string = [[NSString alloc] initWithBytes:httpData length:strlen(httpData) encoding:NSASCIIStringEncoding];
	//NSLog(string);
	//[string release];

	return httpData;
}

extern "C" char* EncodeAndSendHttpRequest(const char* url, UINT16 port, StringList* req_keys, StringList* req_values, char* httpData)
{
	//Get secret hash code
	NSString* str_hashcode = HashDataWithSecretKey(req_values);
	
	//Append hash code to request string
	req_keys->AddNewString("ssikey");
	req_values->AddNewString([str_hashcode UTF8String]);
	
	//Encode URL and build request string
	NSString* strReq = BuildURLRequest(req_keys, req_values);
	
	NSString* strUrl = [ NSString stringWithFormat:@"http://%s?%@", url, strReq];
	NSMutableURLRequest *request = [ [ NSMutableURLRequest alloc ] initWithURL: [ NSURL URLWithString: strUrl ]]; 
	[request setHTTPMethod: @"GET"];
	NSData *returnData = [ NSURLConnection sendSynchronousRequest: request returningResponse: nil error: nil ];
	if (returnData)
	{
		strcpy(httpData, (char*)[returnData bytes]);
	}
	
	[request release];
	[returnData release];
	[strUrl release];

	//NSString* string = [[NSString alloc] initWithBytes:httpData length:strlen(httpData) encoding:NSASCIIStringEncoding];
	//NSLog(string);
	//[string release];

	return httpData;
}

extern "C" void SendAsynchronousHttpRequest(const char* url, UINT16 port, const char* strReq)
{
	NSString* strUrl = [NSString stringWithFormat:@"http://%s?%s", url, strReq];
	
	URLConnectionDelegate* url_conn = [[URLConnectionDelegate alloc] init];
	[url_conn sendAsynchronousHttpGetRequest: strUrl];
}

extern "C" void EncodeAndSendAsynchronousHttpRequest(const char* url, UINT16 port, StringList* req_keys, StringList* req_values)
{
	//Get secret hash code
	NSString* str_hashcode = HashDataWithSecretKey(req_values);
	
	//Append hash code to request string
	req_keys->AddNewString("ssikey");
	req_values->AddNewString([str_hashcode UTF8String]);
		
	//Encode URL and build request string
	NSString* strReq = BuildURLRequest(req_keys, req_values);
	
	NSString* strUrl = [NSString stringWithFormat:@"http://%s?%@", url, strReq];
	
	URLConnectionDelegate* url_conn = [[URLConnectionDelegate alloc] init];
	[url_conn sendAsynchronousHttpGetRequest: strUrl];
}

extern "C" void EncodeAndSendAsynchronousHttpRequestByPostMethod(const char* url, UINT16 port, StringList* req_keys, StringList* req_values)
{
	//Get secret hash code
	NSString* str_hashcode = HashDataWithSecretKey(req_values);
	
	//Append hash code to request string
	req_keys->AddNewString("ssikey");
	req_values->AddNewString([str_hashcode UTF8String]);
	
	//Encode URL and build request string
	NSString* strReq = BuildURLRequest(req_keys, req_values);
	
	NSString* strUrl = [NSString stringWithFormat:@"http://%s", url];
	
	URLConnectionDelegate* url_conn = [[URLConnectionDelegate alloc] init];
	
	[url_conn sendAsynchronousHttpPostRequest: strUrl HTTPBody: strReq];
}


extern "C" void OpenURLInSafari(char* url) {
     NSString* strUrl = [ NSString stringWithFormat:@"http://%s",url];

     [[UIApplication sharedApplication] openURL:[NSURL URLWithString:strUrl]];

}

