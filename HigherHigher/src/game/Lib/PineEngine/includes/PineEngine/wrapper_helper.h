#pragma once
#if !defined(_H_PINE_ENGINE_WRAPPER_HELPER)
#define _H_PINE_ENGINE_WRAPPER_HELPER

typedef int WebSocketHandle, WrapperHandle;
typedef void(*OsWriteConsoleFuncPtr)(const char *);
typedef void(*WsMessageHandleFuncPtr)(const char *);
typedef void(*WsConnectEventFuncPtr)();


#endif