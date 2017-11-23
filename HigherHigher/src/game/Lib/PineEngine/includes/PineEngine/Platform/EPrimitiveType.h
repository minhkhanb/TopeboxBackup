#if !defined(_H_PINE_ENGINE_PLATFORM_EPRIMITIVE_TYPE)
#define _H_PINE_ENGINE_PLATFORM_EPRIMITIVE_TYPE

#include "../common.h"
namespace Pine
{
	namespace Platform
	{
		enum class EPrimitiveType
		{
			TRIANGLES,
			TRIANGLE_TRIP,
			LINES,
			LINE_TRIP,
			POINTS
		};
	}
}
#endif//_H_PINE_ENGINE_PLATFORM_EPRIMITIVE_TYPE