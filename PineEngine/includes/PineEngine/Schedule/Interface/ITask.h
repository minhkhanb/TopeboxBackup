#if !defined(_H_PINE_ENGINE_SCHEDULE_ITASK)
#define _H_PINE_ENGINE_SCHEDULE_ITASK

#include "../../common.h"
#include "../ETaskStatus.h"
#include "../../Platform/Interface/IRef.h"

namespace Pine
{
	namespace Schedule
	{
		namespace Interface
		{
			template <class T>
			class PE_EXPORT ITask:public Pine::Platform::Interface::IRef 
			{
			protected:
				ETaskStatus _status;

			public:
				T *_data_item;

				void(*funcOnDone)(ITask<T>* task);
				void(*funcOnCancel)(ITask<T>* task);
				void(*funcDo)(ITask<T>* task);

				ITask<T>(T* item) : _data_item(NULL), funcDo(NULL), funcOnCancel(NULL), funcOnDone(NULL), _status(ETaskStatus::Init) { _data_item = item; };
					
				inline ETaskStatus getStatus() { return _status; };
				inline void setStatus(ETaskStatus status) { _status = status; };
			};
		}
	}
	
}
#endif //_H_PINE_ENGINE_SCHEDULE_ITASK