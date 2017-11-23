#pragma once
#if !defined(_H_PINE_ENGINE_CLOUD_CKEYVALUE_SERVICE)
#define _H_PINE_ENGINE_CLOUD_CKEYVALUE_SERVICE

#include "../common.h"
#include <string>
#include <vector>

namespace Pine
{
	namespace Cloud
	{
		class PE_EXPORT CCloudKeyValueService
		{
            friend class CCloud;
		private:
			CCloudKeyValueService();
		public:
			bool write(std::string key, const char* buff, int len);
            	bool synchronize();
            	std::vector<BYTE> readBytes(std::string key);
		};
	}
}

#endif//_H_PINE_ENGINE_CLOUD_CKEYVALUE_SERVICE