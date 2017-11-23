#include "../../common.h"

#if !defined(_PINE_ENGINE_DEBUG_ITASK_LOG_LINKER_ITEM)
#define _PINE_ENGINE_DEBUG_ITASK_LOG_LINKER_ITEM

namespace Pine
{
	namespace Debug
	{
		namespace Interface
		{
			template<class T>
			class PE_EXPORT ITaskLogLinkerItem
			{
			public:
				T * _data_item;
				ITaskLogLinkerItem<T>* _next;

				ITaskLogLinkerItem<T>() : _data_item(NULL), _next(NULL){};
			};
		}
	}
}

#endif//_PINE_ENGINE_DEBUG_ITASK_LOG_LINKER_ITEM