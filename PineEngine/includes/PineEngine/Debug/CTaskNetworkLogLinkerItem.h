#include "../common.h"

#if !defined(_PINE_ENGINE_DEBUG_CTASK_NETWORKLOG_LINKER_ITEM)
#define _PINE_ENGINE_DEBUG_CTASK_NETWORKLOG_LINKER_ITEM

#include "CNetworkLogItem.h"
#include "../Schedule/Interface/ITask.h"
#include "Interface/ITaskLogLinkerItem.h"

namespace Pine
{
	namespace Debug
	{
		class PE_EXPORT CTaskNetworkLogLinkerItem: public
			Interface::ITaskLogLinkerItem < Schedule::Interface::ITask <CNetworkLogItem> >
		{

		};
	}
}

#endif //_PINE_ENGINE_DEBUG_CTASK_NETWORKLOG_LINKER_ITEM
