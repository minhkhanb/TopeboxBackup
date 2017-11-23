#if !defined(_H_PINE_ENGINE_IRECTANGLE)
#define _H_PINE_ENGINE_IRECTANGLE
#include "../../common.h"
namespace Pine
{
	namespace Platform 
	{
		namespace Interface
		{
			class PE_EXPORT IRectangle
			{
			public:
				float _x;
				float _y;
				float _w;
				float _h;
			};
		}
	}
}

#endif//_H_PINE_ENGINE_IRECTANGLE
