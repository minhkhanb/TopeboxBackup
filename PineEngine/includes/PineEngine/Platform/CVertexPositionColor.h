#pragma once
#if !defined(_H_PINE_ENGINE_PLATFORM_CVERTEX_POSITION_COLOR)
#define _H_PINE_ENGINE_PLATFORM_CVERTEX_POSITION_COLOR

#include "../common.h"
#include "CElement.h"

namespace Pine
{
	namespace Platform
	{
		class PE_EXPORT CVertexPositionColor
		{
		public:
			float Position[3];
			BYTE Color[4];
			static TVertexFormat getElements();
		};
	}
}


#endif//_H_PINE_ENGINE_PLATFORM_CVERTEX_POSITION_COLOR