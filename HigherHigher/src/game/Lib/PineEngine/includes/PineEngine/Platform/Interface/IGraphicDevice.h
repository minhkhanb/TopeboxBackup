#pragma once
#if !defined(_H_PLATFORM_GRAPHIC_DEVICE)
#define _H_PLATFORM_GRAPHIC_DEVICE
#include "IDeviceContext.h"
#include "IRectangle.h"
#include <memory>
namespace Pine 
{
	namespace Platform 
	{
		namespace Interface
		{
			class PE_EXPORT IGraphicDevice
			{
				virtual void init(std::shared_ptr< Interface::IDeviceContext> icontext) = 0;
				virtual void beginRender() = 0;
				virtual void endRender() = 0;
				virtual bool isRenderReady() = 0;
				virtual void setViewPort(IRectangle viewport) = 0;

				
#if defined(_DEBUG)
				virtual void getDebug() = 0;
#endif
			};
		}
	}
}
#endif//_H_PLATFORM_GRAPHIC_DEVICE
