/*
	This file is use global for multiple project.
	And is a part of engine.
	If you are coding in gameplay, please don't make change on it.
*/

#if !defined(_H_ENGINE_DEFINES)
#define _H_ENGINE_DEFINES

//Touch State
#define		TOUCH_UP					0
#define		TOUCH_DOWN					1
#define		TOUCH_DRAG					2

#define		ANCHOR_LEFT					1//(1<<0)
#define		ANCHOR_RIGHT				2//(1<<1)
#define		ANCHOR_HCENTER				4//(1<<2)
#define		ANCHOR_TOP					8//(1<<3)
#define		ANCHOR_BOTTOM				16//(1<<4)
#define		ANCHOR_VCENTER				32//(1<<5)
#define		ANCHOR_HFREE				64//(1<<6)
#define		ANCHOR_VFREE				128//(1<<7)


/////////////////////////////////////////
//GRAPHIC ENGINE
/////////////////////////////////////////
#define GVER_GLES1					0
#define GVER_GLES2					2
#define GVER_DIRECTX11				3

//default graphic engine
//can change on defines.h
#define GVER_USING					GVER_GLES1

//Macro to detect current using engine
//these macro had been make to optimizing code purpose
#define ISGVER_GLES1				(GVER_USING == GVER_GLES1)
#define ISGVER_GLES2				(GVER_USING == GVER_GLES2)
#define ISGVER_DIRECTX11			(GVER_USING == GVER_DIRECTX11)


//memcopy (void* src, void* dst, int len)
#define MEMCOPY(src, dst, len) { int l = len; char* p = (char*)src; char* q = (char*)dst; while(l-->0) {*q++ = *p++; } }
#define MEMSET(src,c, len) { int l = len; unsigned char* p=(unsigned char*)src; while(l--)*p++ = (unsigned char)c; }

#define PINE_ACCOUNT_RANGE			100000000000000000
#define PINE_ACCOUNT_CLASS(acc_id)	(acc_id/PINE_ACCOUNT_RANGE)
#define PINE_ACCOUNT_SOCIAL_ID(acc_id) (acc_id%PINE_ACCOUNT_RANGE)

#define PINE_FANPAGE_FACEBOOK		"https://www.facebook.com/345487638795425"
#define PINE_FANPAGE_TWITTER		"https://www.twitter.com/PineGames"

/////////////////////////////////////////
//ENGINE DEBUG
/////////////////////////////////////////
//define for test engine function
//to use it. define apropriate defination in project's property
//ENGINE_DEBUG to print log in engine
#define DEBUG_PRINT PDEBUG
#if defined(_DEBUG_ENGINE)
#	define ENGINE_DEBUG PDEBUG
#else
#	define ENGINE_DEBUG(...)
#endif

#if defined(_DEBUG_ENGINE_SPRITE)
#	define ENGINE_DEBUG_SPRITE PDEBUG
#else
#	define ENGINE_DEBUG_SPRITE(...)
#endif

//////////////////////////////////////////
///EVENT
//////////////////////////////////////////
#define PINE_EVENT_STATE_AVAILABLE		1
#define PINE_EVENT_STATE_TIMEOUT		2
#define PINE_EVENT_STATE_UNAVAILABLE	4

//System = 0xFF,
//GameLower = 0xFE,
//Game = 0x00
#define EVENT_KEYDOWN					0xFF000001
#define EVENT_KEYUP						0xFF000002
#define EVENT_KEYPRESS					0xFF000003
#define EVENT_FRAME_BREAK				0xFF000004 //break current frame
#define EVENT_FRAME_CONTINUE			0xFF000005 //disable current breaking frame event
#define EVENT_REQUEST_CREATE_TEXTURE    0xFF000006 //
#define EVENT_RESPONSE_CREATE_TEXTURE   0xFF000007 //

#define SYSTEM_VERSION_EQUAL_TO(v)                  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedSame)
#define SYSTEM_VERSION_GREATER_THAN(v)              ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedDescending)
#define SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN(v)                 ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN_OR_EQUAL_TO(v)     ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedDescending)

//////////////////////////////////////////
///TMP FILE
//////////////////////////////////////////
#define TMP_FILE_SCREEN_CAPTURE			"tmp_screen_raw"

//////////////////////////////////////////
///DEUG VALUE
//////////////////////////////////////////
//because DEBUG_PRINT(PDEBUG) only valid on debug mode so these macro only valid on debug mode too

#define DEBUG_VALUE_INT(v)			DEBUG_PRINT("%s:%d\n", #v, v)
#define DEBUG_VALUE_INT64(v)		DEBUG_PRINT("%s:%lld\n", #v, v)
#define DEBUG_VALUE_UINT(v)			DEBUG_PRINT("%s:%u\n", #v, v)
#define DEBUG_VALUE_UINT64(v)		DEBUG_PRINT("%s:%llu\n", #v, v)
#define DEBUG_VALUE_STRING(v) 		DEBUG_PRINT("%s:%s\n", #v, v)
#define DEBUG_VALUE_FLOAT(v)		DEBUG_PRINT("%s:%f\n", #v, v)

#define DEBUG_POINTER_EQUAL(p1,p2)	DEBUG_PRINT("%s %s %s\n", #p1, ((p1)==(p2))? "is":" is not", #p2)

#endif//_H_ENGINE_DEFINES