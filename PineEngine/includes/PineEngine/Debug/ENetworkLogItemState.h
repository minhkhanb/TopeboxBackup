#include "../common.h"

#if defined(BUILD_DEBUG_FULL) && !defined(_PINE_ENGINE_DEBUG_ENETWORK_LOGITEM_STATE)
#define _PINE_ENGINE_DEBUG_ENETWORK_LOGITEM_STATE

namespace PineEngine 
{
	namespace Debug
	{
		enum class PE_EXPORT ENetworkLogItemState
		{
			Init,
			Connected,
			Sending,
			Receiving
		};
	}
}

#endif//_PINE_ENGINE_DEBUG_ENETWORK_LOGITEM_STATE