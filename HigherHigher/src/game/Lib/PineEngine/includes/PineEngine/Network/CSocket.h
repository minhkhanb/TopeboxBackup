#if !defined(_H_PINE_ENGINE_NETWORK_CSOCKET)
#define _H_PINE_ENGINE_NETWORK_CSOCKET
#include "../common.h"
#include "Interface/ISocket.h"

namespace Pine
{
	namespace Network
	{
		class PE_EXPORT CSocketInside;
		class PE_EXPORT CSocket: public Interface::ISocket
		{
		private:
			CSocketInside* _data;
			
		public:
			CSocket();
			~CSocket();
			//implement ISocket
			virtual void asClient(const char* server_address, int server_port);
			virtual int send(const char* buffer, unsigned int len);
			virtual ESocketStatus getStatus();
		};
	}
}

#endif//_H_PINE_ENGINE_NETWORK_ISOCKET
