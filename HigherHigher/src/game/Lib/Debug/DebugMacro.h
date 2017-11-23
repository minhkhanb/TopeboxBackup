#ifndef _DEBUG_MACRO
#define _DEBUG_MACRO

#ifdef _DEBUG
//debug build

	#ifdef MAC_OS
	//on mac os
	
		#define LOG(s)					printf(s)
		#define LOG1(s, x)				printf(s, x)
		#define LOG2(s, x, y)			printf(s, x, y)
		#define LOG3(s, x, y, z)		printf(s, x, y, z)
		
	#else
	//on win os
	
		#define LOG(s)					OutputDebugStringA(s)
		#define LOG1(s, x)				{char sLog[256]; sprintf(sLog, s, x); OutputDebugStringA(sLog);}
		#define LOG2(s, x, y)			{char sLog[256]; sprintf(sLog, s, x, y); OutputDebugStringA(sLog);}
		#define LOG3(s, x, y, z)		{char sLog[256]; sprintf(sLog, s, x, y, z); OutputDebugStringA(sLog);}
		
	#endif


#else
//release build
	
	#define LOG(s)	printf(s)
	#define LOG1(s, x)	
	#define LOG2(s, x, y)	
	#define LOG3(s, x, y, z)	
	
#endif

#endif