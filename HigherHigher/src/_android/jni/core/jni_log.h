#ifndef _H_LOG
#define _H_LOG


class JniLog{
	public:
		static void error(const char* pmessage, ...);
		static void warn(const char* pmessage, ...);
		static void info(const char* pmessage, ...);
		static void debug(const char* pmessage, ...);
};

#ifdef _DEBUG
	#define PDEBUG(...) JniLog::debug(__VA_ARGS__)

#else
	#define PDEBUG(...)
#endif



#endif