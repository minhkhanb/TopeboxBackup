#pragma once
#if !defined(_H_PINE_ENGINE_CLOUD_CCLOUD)
#define _H_PINE_ENGINE_CLOUD_CCLOUD
#include "../common.h"
#include "ECloudService.h"
#include "ECloudProvider.h"
#include "CCloudKeyValueService.h"
#include "ECloudKeyValueChangeReason.h"
#include <iostream>
#include <memory>
#include <functional>
#include <string>
#include <vector>

namespace Pine
{
	namespace Cloud
	{
		class PE_EXPORT CCloud
		{
        public:
			static bool isSupport(ECloudProvider provider, ECloudService service);
            static std::shared_ptr<CCloudKeyValueService> regKeyValueService(ECloudProvider provider, std::function<void(ECloudKeyValueChangeReason,std::vector<std::string> keys)> onChanged);
			static std::shared_ptr<CCloudKeyValueService> getKeyValueService(ECloudProvider provider);
		};
	}
}

#endif//_H_PINE_ENGINE_CLOUD_CCLOUD