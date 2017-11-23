#if !defined(_PINE_ENGINE_PLATFORM_IRENDER_STATE)
#define _PINE_ENGINE_PLATFORM_IRENDER_STATE

#include "../../common.h"
#include "../EBlend.h"
#include "../ECullFace.h"
#include "../EDepthFunction.h"

namespace Pine
{
	namespace Platform
	{
		namespace Interface
		{
			class IRenderState
			{
			public:
				EBlend BlendSrc;
				EBlend BlendDst;
				ECullFace CullFace;
				EDepthFunction DepthFunction;
				virtual void bind() = 0;
			};
		}
	}
}
#endif//_PINE_ENGINE_PLATFORM_IRENDER_STATE