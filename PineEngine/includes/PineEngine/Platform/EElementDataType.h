#pragma once
#if !defined (_H_PINE_ENGINE_PLATFORM_EELEMENT_DATA_TYPE)
#define _H_PINE_ENGINE_PLATFORM_EELEMENT_DATA_TYPE

#include "../common.h"

namespace Pine
{
	namespace Platform
	{
		enum class PE_EXPORT EElementDataType
		{
			UNSIGNED_BYTE,
			UNSIGNED_SHORT,
			UNSIGNED_INT,
			FLOAT,
			BYTE,
			SHORT,
			INT,
		};

		namespace ElementDataType
		{
			int getSize(EElementDataType type);
		};
	}
}

#endif//_H_PINE_ENGINE_PLATFORM_EELEMENT_DATA_TYPE