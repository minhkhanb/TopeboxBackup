#if !defined(_H_PINE_ENGINE_RESOURCE_CRESOURCE_SEGMENT)
#define _H_PINE_ENGINE_RESOURCE_CRESOURCE_SEGMENT

#include "../common.h"
#include "Interface/IResource.h"
#include "../Io/Interface/IStream.h"
#include "../Io/Interface/IBuffer.h"
#include <string>

namespace Pine
{
	namespace Resource
	{
		class PE_EXPORT CResourceSegmentInside;
		class PE_EXPORT CResourceSegment: 
			public Interface::IResource
		{
		public:
			CResourceSegment();
			~CResourceSegment();
            std::wstring _path;
			size_t _offset;
			int _length; //if _length < 0 it's mean is read whole of file from offset
		};
	}
}

#endif//_H_PINE_ENGINE_RESOURCE_CRESOURCE_SEGMENT
