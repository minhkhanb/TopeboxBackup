#if !defined(_H_PINE_ENGINE_SCHEDULE_IWORKER)
#define _H_PINE_ENGINE_SCHEDULE_IWORKER
#include "../../common.h"
#include "ITask.h"
namespace Pine
{
	namespace Schedule
	{
		namespace Interface
		{
			template<class T>
			class PE_EXPORT IWorker
			{
			public:
				virtual void addTask(ITask<T> *task) = 0;
				virtual void run() = 0;
			};
		}
	}
	
}

#endif //_H_PINE_ENGINE_SCHEDULE_IWORKER
