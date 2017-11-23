#if !defined(_H_PINE_ENGINE_PLATFORM_CEFFECT)
#define _H_PINE_ENGINE_PLATFORM_CEFFECT

#include "../common.h"
#include "CGraphicDevice.h"
#include "CElement.h"
#include <memory>
#include <vector>
#include "Interface/IMesh.h"

namespace Pine
{
	namespace Platform
	{
		class PE_EXPORT CEffectInside;
		class PE_EXPORT CEffect
		{
		private:
			CEffectInside* _inside;
			std::shared_ptr<CGraphicDevice> _g; //graphic device from that effect has been created;
		public:
			static std::shared_ptr<CEffect> fromString(std::shared_ptr<CGraphicDevice> gdevice, const char* vsh, const char* fsh, const char* defines, TVertexFormat vertex_format);
			//static std::shared_ptr<CEffect> createDefaultEffect(std::shared_ptr<CGraphicDevice> gdevice);
			static std::shared_ptr<CEffect> createSimpleEffect(std::shared_ptr<CGraphicDevice> gdevice);
			void bind();
			void bindAndRender(std::shared_ptr<Interface::IMesh> mesh);

			CEffect();
			~CEffect();

			friend class CVertexFormat;
		};
	}
}

#endif//_H_PINE_ENGINE_PLATFORM_CEFFECT