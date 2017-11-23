#ifndef _H_PINE_ENGINE_PLATFORM_CSERVICE_SHELL
#define _H_PINE_ENGINE_PLATFORM_CSERVICE_SHELL
#include <memory>

#include "../common.h"
#include "CRequestStream.h"

namespace Pine
{
    namespace ServiceShell
    {
        class PE_EXPORT CServiceShell
        {
        public:
            //return true if platform is supported, false if otherwise
            static bool isEnable();

            static int request(std::shared_ptr<CMiddleRequest> request);
            static void responsed(int32_t id);
        };
    }
}


#endif//_H_PINE_ENGINE_PLATFORM_CSERVICE_SHELL