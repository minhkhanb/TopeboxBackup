#include "../../common.h"

#if !defined(_PINE_ENGINE_DEBUG_ILOG_ITEM)
#define _PINE_ENGINE_DEBUG_ILOG_ITEM

namespace Pine
{
	namespace Debug
	{
		namespace Interface
		{
			class PE_EXPORT ILogItemInside;

			class PE_EXPORT ILogItem
			{
			private:
				ILogItemInside *_inside;
				
			public:
				ILogItem();
				ILogItem(const char* log_str);
				~ILogItem();

				void init(const char* log_str);
				
			protected:
				const char* getData();
			};
		}
	}
}

#endif //_PINE_ENGINE_DEBUG_ILOG_ITEM