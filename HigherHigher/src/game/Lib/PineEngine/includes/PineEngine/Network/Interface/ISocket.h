#if !defined(_H_PINE_ENGINE_NETWORK_ISOCKET)
#define _H_PINE_ENGINE_NETWORK_ISOCKET
#include "../../common.h"

namespace Pine 
{
	namespace Network
	{
		enum class PE_EXPORT ESocketStatus 
		{
			Unknown, //just born and doesn't connect to any host
			Connecting, //trying to connect to host
			Connected, //connected and enable to make transaction
			Disponsed, //not enable to use forever
			Writing,
			Reading,
		};

		namespace Interface
		{
			class PE_EXPORT ISocket
			{
			public:
				
				virtual void asClient(const char* server_address, int server_port) = 0;
				virtual int send(const char* buffer, unsigned int len) = 0;
				virtual ESocketStatus getStatus() = 0;
			};
		}
	}
}

#endif//_H_PINE_ENGINE_NETWORK_ISOCKET