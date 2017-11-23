#if !defined(_H_PINE_ENGINE_CLOUD_ECLOUD_KEYVALUE_CHANGE_REASON)
#define _H_PINE_ENGINE_CLOUD_ECLOUD_KEYVALUE_CHANGE_REASON

#include "../common.h"

namespace Pine
{
    namespace Cloud
    {
        enum class PE_EXPORT ECloudKeyValueChangeReason
        {
            ServerChange,
            InitialSyncChange,
            QuotaViolationChange,
            AccountChange,
        };
    }
}

#endif//_H_PINE_ENGINE_CLOUD_ECLOUD_KEYVALUE_CHANGE_REASON