#pragma once
#if !defined(_H_PINE_ENGINE_NETWORK_CSIGNALR)
#define _H_PINE_ENGINE_NETWORK_CSIGNALR

#include "../common.h"
#include <string>

namespace Pine
{
	namespace Network
	{
		class PE_EXPORT CSignalR
		{
		public:
			static std::string getNegotiate(std::string host, std::string prefix, std::string version_string, std::string authorize_string);
			static bool sendStart(std::string host, std::string prefix, std::string token, std::string connection_data, std::string version_string, std::string authorize_string);
			static bool sendAbort(std::string host, std::string prefix, std::string token, std::string connection_data, std::string version_string, std::string authorize_string);
			static bool sendReconnect(std::string host, std::string prefix, std::string token, std::string version_string, std::string authorize_string);
		};
	}
}

#endif//_H_PINE_ENGINE_NETWORK_CSIGNALR