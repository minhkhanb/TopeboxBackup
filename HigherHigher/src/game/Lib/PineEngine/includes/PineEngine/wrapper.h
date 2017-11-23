#if !defined (_H_PINE_ENGINE_WRAPPER)
#define _H_PINE_ENGINE_WRAPPER

#include "common.h"
#include "wrapper_helper.h"
#include <string>

extern "C" 
{
	//init background environment
	void PE_DLLCALL PineEngineInit();
	void PE_DLLCALL PineEngineRelease();

    //os
	void PE_DLLCALL PineOsSetWriteConsoleHandleFunction(OsWriteConsoleFuncPtr funcptr);
	void PE_DLLCALL PineOsResetWriteConsoleHandleFunction();
    void PE_DLLCALL PineOsWriteConsole(const char* format, ...);

	//cloud
	bool PE_DLLCALL PineCloudIsSupport(int cloud_provider, int cloud_service);
    bool PE_DLLCALL PineCloudKeyValueSynchronize(WrapperHandle handle);

	//security
	WrapperHandle PE_DLLCALL PineSecurityBasicAuthorizeV1Open(const char* secret_key);
	void PE_DLLCALL PineSecurityBasicAuthorizeV1Close(WrapperHandle handle);
	void PE_DLLCALL PineSecurityBasicAuthorizeV1SimpleEncrypt(WrapperHandle handle, char* output, int len, const char* content);
	void PE_DLLCALL PineSecurityBasicAuthorizeV1GetPass(WrapperHandle handle, char* output, int len, const char* id);

	//network http
	//WrapperHandle PE_DLLCALL PineNetworkHttpOpen(const char* url);
	//void PE_DLLCALL PineNetworkHttpClose(WrapperHandle handle);
	//void PE_DLLCALL PineNetworkHttpSetHeader(WrapperHandle handle, const char* key, const char* value);

	//network websocket
	int PE_DLLCALL PineTestInt();
	WebSocketHandle PE_DLLCALL PineNetworkWebSocketOpen(const char* url);
	void PE_DLLCALL PineNetworkWebSocketPing(WebSocketHandle handle);
	void PE_DLLCALL PineNetworkWebSocketClose(WebSocketHandle handle);
	bool PE_DLLCALL PineNetworkWebSocketConnect(WebSocketHandle handle);
	void PE_DLLCALL PineNetworkWebSocketSetMessageHandleFunction(WebSocketHandle handle, WsMessageHandleFuncPtr funcptr);
	void PE_DLLCALL PineNetworkWebSocketSetConnectedHandleFunction(WebSocketHandle handle, WsConnectEventFuncPtr funcptr);
	void PE_DLLCALL PineNetworkWebSocketSetConnectFailHandleFunction(WebSocketHandle handle, WsConnectEventFuncPtr funcptr);
	void PE_DLLCALL PineNetworkWebSocketSetConnectInterruptHandleFunction(WebSocketHandle handle, WsConnectEventFuncPtr funcptr);
	void PE_DLLCALL PineNetworkWebSocketSetConnectPongTimeOutHandleFunction(WebSocketHandle handle, WsConnectEventFuncPtr funcptr);
	void PE_DLLCALL PineNetworkWebSocketSetConnectCloseHandleFunction(WebSocketHandle handle, WsConnectEventFuncPtr funcptr);
	
	void PE_DLLCALL PineNetworkWebSocketSetHeader(WebSocketHandle handle, const char* key, const char* value);
	void PE_DLLCALL PineNetworkWebSocketSetProxy(WebSocketHandle handle, const char* url);
	void PE_DLLCALL PineNetworkWebSocketSend(WebSocketHandle handle, const char* msg);
	bool PE_DLLCALL PineNetworkWebSocketIsConnecting(WebSocketHandle handle);

	//SignalR
	void PE_DLLCALL PineNetworkSignalRGetNegotiate(char* output, int len, const char* url, const char* prefix, const char* version_string, const char* authorize_string);
	bool PE_DLLCALL	PineNetworkSignalRSendStart(const char* url, const char* prefix, const char* token, const char* connection_string, const char* version_string, const char* authorize_string);
	bool PE_DLLCALL	PineNetworkSignalRSendAbort(const char* url, const char* prefix, const char* token, const char* connection_string, const char* version_string, const char* authorize_string);
	bool PE_DLLCALL	PineNetworkSignalRSendReconnect(const char* url, const char* prefix, const char* token, const char* version_string, const char* authorize_string);

#if defined(PE_BUILD_EMSCRIPTEN)
	void PE_DLLCALL pine_emscripten_socket_onMessage(WebSocketHandle id);
	void PE_DLLCALL pine_emscripten_socket_onError(WebSocketHandle id);
	void PE_DLLCALL pine_emscripten_socket_onOpen(WebSocketHandle id);
	void PE_DLLCALL pine_emscripten_socket_onClose(WebSocketHandle id);
#endif
}
#endif//_H_PINE_ENGINE_WRAPPER
