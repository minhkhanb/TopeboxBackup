#pragma once
#ifndef _H_PINE_ENGINE_COMMON
#define _H_PINE_ENGINE_COMMON

#if defined (_WINDOWS)
//disable warnings on 255 char debug symbols
#	pragma warning (disable : 4786)
//disable warnings on extern before template instantiation
#	pragma warning (disable : 4231)
#	pragma warning (disable : 4251)
#	pragma warning (disable : 4275)
#	define _WINSOCK_DEPRECATED_NO_WARNINGS
#	define WIN32_LEAN_AND_MEAN
#	define _CRT_SECURE_NO_DEPRECATE
#	define _SCL_SECURE_NO_DEPRECATE
#	define _CRT_SECURE_NO_WARNINGS
#	define _SCL_SECURE_NO_WARNINGS
#	define _WINSOCKAPI_ 
#	include <WinSock2.h>
#	include <Windows.h>
#endif

#include <cstdint>
#include <cstring>

#if defined(_UNITY)
#	define PE_BUILD_UNITY
#endif

#	define PE_EXPORT_TEMPLATE 

#if defined (_WINDOWS)
#	if !defined (PE_EXPORT)
#		if defined (PINEENGINE_EXPORTS)// DLL export
#			define PE_EXPORT __declspec(dllexport)
#			define PE_EXPORT_TEMPLATE
#		else// EXE import
#			define PE_EXPORT __declspec(dllimport)
#			define PE_EXPORT_TEMPLATE extern
#		endif
#	endif
//#	define PE_DLLCALL	__declspec(dllexport)//__stdcall//__cdecl
#	define PE_DLLCALL	PE_EXPORT
#   define PE_EXTERN_C  extern "C"

#	if defined(_UNITY)
#		define PE_BUILD_UNITY_WINDOWS_DESKTOP
#	else
#		define PE_BUILD_WINDOWS_DESKTOP
#	endif

#elif defined(_IOS)
#   define PE_EXPORT
#   define PE_DLLCALL

#	if defined(_UNITY)
#		define PE_BUILD_UNITY_IOS
#	else
#		define PE_BUILD_IOS
#	endif
#   define PE_EXTERN_C  extern "C"

#elif defined(_ANDROID)
#   define PE_EXPORT
#   define PE_DLLCALL
#	if defined(_UNITY)
#		define PE_BUILD_UNITY_ANDROID
#	else
#		define PE_BUILD_ANDROID
#	endif
#   define PE_EXTERN_C	extern "C"

#elif defined(_OSX)
#   define PE_EXPORT
#   define PE_DLLCALL
#   define PE_BUILD_OSX
#   define PE_EXTERN_C  extern "C"

#elif defined(_DEBIAN)
#   define PE_EXPORT
#   define PE_DLLCALL
#   define PE_BUILD_DEBIAN
#   define PE_EXTERN_C  extern "C"

#elif defined(_WP8)
#	if !defined (PE_EXPORT)
#		if defined (PINEENGINE_EXPORTS)// DLL export
#			define PE_EXPORT __declspec(dllexport)
#		else// EXE import
#			define PE_EXPORT __declspec(dllimport)
#		endif
#	endif

#	define PE_DLLCALL	PE_EXPORT

#	if defined(_UNITY)
#		define PE_BUILD_UNITY_WP8
#	else
#		define PE_BUILD_WP8
#	endif
#   define PE_EXTERN_C  extern "C"

#elif defined(_EM)
#	define PE_EXPORT
#   define PE_DLLCALL PE_EXPORT
#   define PE_BUILD_EMSCRIPTEN
#   define PE_EXTERN_C  extern "C"
#else
#	define PE_EXPORT
#	define PE_DLLCALL
#   define PE_EXTERN_C  extern "C"
#endif

#if defined(_DEBUG_FULL)
#   define PE_BUILD_DEBUG_FULL
#endif

typedef uint8_t		BYTE, UINT8;
typedef int8_t		INT8;
typedef uint16_t	UINT16;
typedef int16_t		INT16;

typedef uint32_t	UINT32;
typedef int32_t		INT32;
typedef int64_t		INT64;
typedef uint64_t	UINT64;

#define PE_COLOR_RAY(a,r,g,b) {(r),(g),(b),(a)}




#if !defined(NULL)
#	define NULL 0
#endif

#define PE_ASSERT assert

typedef void(*ACTION_CALLBACK_FUNC)(const char*, int len);

#endif //_H_PINE_ENGINE_COMMON
