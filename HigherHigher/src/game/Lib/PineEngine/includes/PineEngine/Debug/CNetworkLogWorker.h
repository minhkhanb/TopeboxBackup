#include "../common.h"

#if !defined(PINE_ENGINE_CNETWORK_LOG_WORKER)
#define PINE_ENGINE_CNETWORK_LOG_WORKER


#include "../Schedule/Interface/IWorker.h"
#include "Interface/ILogger.h"
#include "CNetworkLogItem.h"

namespace Pine
{
	namespace Debug
	{
		class PE_EXPORT CNetworkLogWorkerInside;

		class PE_EXPORT CNetworkLogWorker:
			public Schedule::Interface::IWorker<CNetworkLogItem>,
			public Interface::ILogger
		{
		private:
			CNetworkLogWorkerInside* _inside;

		public:
			CNetworkLogWorker();
			~CNetworkLogWorker();
			void setServer(const char* svurl, int port);

			//implement IWorker
			virtual void addTask(Schedule::Interface::ITask<CNetworkLogItem> *task);
			virtual void run();

			//implement ILogger
			virtual void log(const char* format, ...);

			//
			void doTask(Schedule::Interface::ITask<CNetworkLogItem> *task);
		};
	}
}


#endif //PINE_ENGINE_CNETWORK_LOG_WORKER