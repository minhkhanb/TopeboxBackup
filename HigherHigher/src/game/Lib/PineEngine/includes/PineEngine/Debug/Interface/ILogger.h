#include "../../common.h"

#if !defined(_PINE_ENGINE_DEBUG_ILOGGER)
#define _PINE_ENGINE_DEBUG_ILOGGER


namespace Pine 
{
	namespace Debug 
	{
		namespace Interface 
		{
			class PE_EXPORT ILogger 
			{
			public:
				static ILogger* Logger;
				static void tryLog(const char* format, ...);
				virtual void log(const char* format, ...) = 0;
			};
		}
	}
}


#if defined(PE_BUILD_DEBUG_FULL)

#define PE_LOG Pine::Debug::Interface::ILogger::tryLog
#define PE_SET_LOGGER(a) Pine::Debug::Interface::ILogger::Logger = a

#else

#define PE_LOG(...)
#define PE_SET_LOGGER(...)


#endif


#endif //_PINE_ENGINE_DEBUG_ILOGGER
