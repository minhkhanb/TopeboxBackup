#pragma once
#if !defined(_H_PINE_ENGINE_PLATFORM_CMESH)
#define _H_PINE_ENGINE_PLATFORM_CMESH

#include "../common.h"
#include "Interface/IMesh.h"
#include "EPrimitiveType.h"
#include <vector>
#include <memory>
#include "CVertexPositionColor.h"
#include "CGraphicDevice.h"

namespace Pine
{
	namespace Platform
	{
		template<class T>
		class PE_EXPORT CMeshInside;

		template<class T>
		class PE_EXPORT CMesh : public Interface::IMesh
		{
		private:
			CMesh();

		protected:
			CMeshInside<T>* _inside;

		public:
			~CMesh();
			static std::shared_ptr<CMesh<T>> createMesh(std::shared_ptr<CGraphicDevice> gdevice, EPrimitiveType primitive_type);

			void add(std::vector<T> vertexs, std::vector<UINT32> index);

			virtual void setDirty();

			virtual size_t getVertexCount();
			virtual size_t getIndexCount();

			virtual void bind();
			virtual void render();
		};

		typedef CMesh<CVertexPositionColor> CMeshPositionColor;
	}
}

#endif