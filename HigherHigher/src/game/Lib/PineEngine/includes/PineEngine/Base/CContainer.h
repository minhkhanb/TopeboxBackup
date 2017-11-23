#if !defined(_H_PINE_ENGINE_BASE_CCONTAINER)
#define _H_PINE_ENGINE_BASE_CCONTAINER
#include "../common.h"
#include "../Io/CBuffer.h"

namespace Pine
{
	class PE_EXPORT CContainer
	{
	public:
		char _name[255];
		Io::CBuffer _content;
	};
}

#endif//_H_PINE_ENGINE_BASE_CCONTAINER