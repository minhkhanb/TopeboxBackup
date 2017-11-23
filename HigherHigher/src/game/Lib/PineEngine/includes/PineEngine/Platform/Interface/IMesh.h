#pragma once
#if !defined(_H_PINE_ENGINE_PLATFORM_INTERFACE_MESH)
#define _H_PINE_ENGINE_PLATFORM_INTERFACE_MESH

#include "../../common.h"
#include <cstring>

namespace Pine
{
	namespace Platform
	{
		namespace Interface
		{
			class PE_EXPORT IMesh
			{
			public:
				virtual void setDirty() = 0;
				virtual size_t getVertexCount() = 0;
				virtual size_t getIndexCount() = 0;
				virtual void bind() = 0;
				virtual void render() = 0;
			};
		}
	}
}

#endif//_H_PINE_ENGINE_PLATFORM_INTERFACE_MESH
