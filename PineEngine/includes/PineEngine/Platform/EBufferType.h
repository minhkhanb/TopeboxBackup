#pragma once
#if !defined(_H_PINE_ENGINE_PLATFORM_EBUFFER_TYPE)
#define _H_PINE_ENGINE_PLATFORM_EBUFFER_TYPE

#include "../common.h"
namespace Pine
{
	namespace Platform
	{
		enum class PE_EXPORT EBufferType
		{
			VERTEX = 0,
			INDICES = 1
		};

	}
}

#endif//_H_PINE_ENGINE_PLATFORM_EBUFFER_TYPE