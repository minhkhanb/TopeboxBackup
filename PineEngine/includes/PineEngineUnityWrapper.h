#pragma once
#if !defined(_H_PINE_ENGINE_C_WRAPPER)
#define _H_PINE_ENGINE_UNITY_C_WRAPPER

#include "PineEngine/common.h"
#include "PineEngine/wrapper.h"
#include "Unity/IUnityInterface.h"
#include "Unity/IUnityGraphics.h"


extern "C"
{
	PE_EXPORT void regActionCallbackListenner(ACTION_CALLBACK_FUNC action_callback);
}

#endif//_H_PINE_ENGINE_UNITY_C_WRAPPER