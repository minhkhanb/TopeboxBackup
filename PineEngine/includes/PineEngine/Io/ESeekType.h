#if !defined(_PINE_ENGINE_IO_ESEEKTYPE)
#define _PINE_ENGINE_IO_ESEEKTYPE
#include "../common.h"

namespace Pine
{
	namespace Io
	{
		enum class PE_EXPORT ESeekType
		{
			AtBegin,//seek from begin of stream
			AtEnd,//seek from the end of stream to the beginning
			AtCurrentToEnd,//seek from current pos to end
			AtCurrentToBegin,//seek from current pos to end
		};
	}
}

#endif //_PINE_ENGINE_IO_ESEEKTYPE