#pragma once
#if !defined(_H_PLATFORM_IDEVICE_CONTEXT)
#define _H_PLATFORM_IDEVICE_CONTEXT

#include "../../common.h"
#include "../EDeviceType.h"
namespace Pine 
{
	namespace Platform
	{
		namespace Interface 
		{
			class PE_EXPORT IDeviceContext
			{
			public:
				virtual EDeviceType getDeviceType() = 0;
				virtual void show() = 0;

                //Hardware support
                
                //flashlight
                virtual bool isSupportFlashLight() = 0;
                virtual void turnFlashLight(bool on) = 0;
			};
		}
	}
}

#endif//_H_PLATFORM_IDEVICE_CONTEXT
