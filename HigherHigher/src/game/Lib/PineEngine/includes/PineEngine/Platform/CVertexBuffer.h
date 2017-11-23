#pragma once
#if !defined(_H_PINE_ENGINE_PLATFORM_CVERTEXBUFFER)
#define _H_PINE_ENGINE_PLATFORM_CVERTEXBUFFER

#include "../common.h"
#include "EBufferType.h"
#include <memory>


namespace Pine 
{
	namespace Platform
	{
		class PE_EXPORT CGraphicDevice;
		class PE_EXPORT CVertexBufferInside;
		class PE_EXPORT CVertexBuffer
		{

		public:
			CVertexBuffer();
			~CVertexBuffer();

			void bind();

		protected:
			CVertexBufferInside* _inside;
			std::weak_ptr<CGraphicDevice> _g;

			friend class CGraphicDevice;
		};
	}
}

#endif//_H_PINE_ENGINE_PLATFORM_CVERTEXBUFFER