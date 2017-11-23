#include "../common.h"

#if !defined(_PINE_ENGINE_DEBUG_CNETWORK_LOGITEM)
#define _PINE_ENGINE_DEBUG_CNETWORK_LOGITEM

#include "Interface/ILogItem.h"
#include "ENetworkLogItemState.h"
#include "../Network/CSocket.h"
namespace Pine 
{
	namespace Debug
	{
		class PE_EXPORT CNetworkLogItemInside;
		class PE_EXPORT CNetworkLogItem: public Interface::ILogItem, protected Network::CSocket
		{
		protected:
			CNetworkLogItemInside *_inside;
		public:
			CNetworkLogItem(const char* sv_url, int port, const char* log);
			~CNetworkLogItem();

			void send();
			bool isCompleted();
		};
	}
}
#endif//_PINE_ENGINE_DEBUG_CNETWORK_LOGITEM