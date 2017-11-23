#pragma once
#if !defined(_H_PINE_ENGINE_STRING_CBASE64)
#define _H_PINE_ENGINE_STRING_CBASE64

#include "../common.h"
#include <string>

namespace Pine
{
	namespace String
	{
		class PE_EXPORT CBase64
		{
		public:
			static std::string encode(std::string str);
			static std::string encode(const char* buff, int len);
		};
	}
}


#endif//_H_PINE_ENGINE_STRING_CBASE64