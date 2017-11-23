#pragma once
#if !defined(_H_PINE_ENGINE_NETWORK_CWEBSOCKET)
#define _H_PINE_ENGINE_NETWORK_CWEBSOCKET

#include "../common.h"
#include <string>

#include "../wrapper_helper.h"

namespace Pine
{
	namespace Network
	{

		class PE_EXPORT CWebSocketInside;

		class PE_EXPORT CWebSocket
		{
		private:
			CWebSocketInside* _inside;
		public:

			CWebSocket(std::string uri);
			~CWebSocket();
			int getId();
			bool connect();
			void setMessageHandleFunction(WsMessageHandleFuncPtr msg_funcptr);
			void setConnectedHandleFunction(WsConnectEventFuncPtr connected_funcptr);
			void setConnectFailHandleFunction(WsConnectEventFuncPtr fail_funcptr);
			void setConnectCloseHandleFunction(WsConnectEventFuncPtr close_funcptr);
			void setConnectInterruptHandleFunction(WsConnectEventFuncPtr interrupt_funcptr);
			void setConnectPongTimeOutHandleFunction(WsConnectEventFuncPtr pong_timeout_funcptr);
			void ping();
			bool isConnecting();
			void close();
			void setHeader(std::string key, std::string value);
			void setProxy(std::string url);
			void send(std::string msg);
		};
	}
}

#endif//_H_PINE_ENGINE_NETWORK_CWEBSOCKET