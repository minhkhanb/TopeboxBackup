#if !defined(_H_PINE_ENGINE_PLATFORM_CDEVICE_CONTEXT)

#define _H_PINE_ENGINE_PLATFORM_CDEVICE_CONTEXT

#include "../common.h"
#include "Interface/IDeviceContext.h"
#include <memory>

namespace Pine
{
    namespace Os
    {
        class PE_EXPORT OS;
		class PE_EXPORT OsInside;
    }
	namespace Platform
	{
		class PE_EXPORT CApplication;
		class PE_EXPORT CDeviceContextInside;
		class PE_EXPORT CDeviceContext:public Interface::IDeviceContext,  public std::enable_shared_from_this<CDeviceContext>
		{
		private:
			CDeviceContextInside* _inside;
			CDeviceContext(std::shared_ptr<Pine::Platform::CApplication>);

		public:
			
			~CDeviceContext();


			//void init(ID3D11Device* dx_device, ID3D11DeviceContext* dx_context, IDXGISwapChain* dx_swapchain);

			
			
			//implement IDeviceContext
			virtual EDeviceType getDeviceType();
			virtual void show();
            
            //implement hardware support
            
            //flash light
            virtual bool isSupportFlashLight();
            virtual void turnFlashLight(bool on);
            
            //ambient light sensor
            virtual bool isSupportGetScreenBrightness();
            virtual float getScreenBrightness();
            
            //proximity sensor
            virtual bool isSupportProximitySensor();
            virtual void turnProximitySensor(bool on_off);
            virtual bool isProximityValueNear();


            //friend
			friend class CGraphicDeviceInside;
            friend class Os::OS;
			friend class Os::OsInside;
			friend class CGraphicDevice;
		};
	}
}
#endif//_H_PINE_ENGINE_PLATFORM_CDEVICE_CONTEXT