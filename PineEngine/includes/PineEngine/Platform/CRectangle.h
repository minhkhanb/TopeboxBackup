#if !defined(_H_PINE_ENGINE_CRECTANGLE)
#define _H_PINE_ENGINE_CRECTANGLE
#include "../common.h"
#include "Interface/IRectangle.h"
namespace Pine
{
	namespace Platform
	{
		class PE_EXPORT CRectangle :public Interface::IRectangle
		{
		public:
			CRectangle(float x, float y, float w, float h);
		};
	}
}

#endif//_H_PINE_ENGINE_CRECTANGLE