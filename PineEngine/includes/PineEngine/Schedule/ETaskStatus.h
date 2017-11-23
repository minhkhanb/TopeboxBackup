#if !defined(_PINE_ENGINE_PLATFORM_SCHEDULE_ETASK_STATUS)
#define _PINE_ENGINE_PLATFORM_SCHEDULE_ETASK_STATUS
#include "../common.h"

namespace Pine
{
	namespace Schedule
	{
		enum class PE_EXPORT ETaskStatus
		{
			Init,
			Doing,
			Done,
			Canceled,
		};

	}
}
#endif//_PINE_ENGINE_PLATFORM_SCHEDULE_ETASK_STATUS
