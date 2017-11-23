#if !defined (_H_PINE_ENGINE_ERESULT_CODE)
#define _H_PINE_ENGINE_ERESULT_CODE

#include "../common.h"

namespace Pine
{
	enum class EResultCode
	{
		OK = 0,
		NOT_SUPPORT,
		OUT_OF_RANGE,
		INVALID_PARAM
	};
}

#endif//_H_PINE_ENGINE_ERESULT_CODE