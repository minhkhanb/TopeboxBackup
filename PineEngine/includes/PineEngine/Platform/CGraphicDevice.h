#if !defined(_H_PINE_ENGINE_PLATFORM_CGRAPHIC_DEVICE)
#define _H_PINE_ENGINE_PLATFORM_CGRAPHIC_DEVICE

#include "../common.h"
#include "Interface/IGraphicDevice.h"
#include "Interface/IDeviceContext.h"
#include "CVertexBuffer.h"
#include <memory>

namespace Pine
{
	namespace Os {
		class PE_EXPORT OS;
	}
	namespace Platform
	{
		class PE_EXPORT CGraphicDeviceInside;
		class PE_EXPORT CGraphicDevice: public Interface::IGraphicDevice, public std::enable_shared_from_this<CGraphicDevice>
		{
		private:
			CGraphicDevice();
		protected:
			CGraphicDeviceInside* _inside;
		public:
			
			~CGraphicDevice();
			//implement IGraphicDevice
			virtual void init(std::shared_ptr< Interface::IDeviceContext> context);
			virtual void beginRender();
			virtual void endRender();
			virtual bool isRenderReady();
			virtual void setViewPort(Interface::IRectangle viewport);

			std::shared_ptr<CVertexBuffer> createBuffer(EBufferType type, size_t col_bsize, size_t row_size, const void* data);
#if defined(_DEBUG)
			virtual void getDebug();
#endif
			friend class CEffect;
			friend class CVertexFormat;
			friend class CVertexBuffer;
			friend class Os::OS;
		};
	}
}
#endif//_H_PINE_ENGINE_PLATFORM_CGRAPHIC_DEVICE