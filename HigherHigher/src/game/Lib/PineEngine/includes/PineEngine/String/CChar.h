#if !defined(_H_PINE_ENGINE_STRING_CCHAR)
#define _H_PINE_ENGINE_STRING_CCHAR
#include "../common.h"
namespace Pine {
	namespace String {
		union PE_EXPORT CChar {
			BYTE ascii;
			UINT16 utf16;
			UINT32 ucs;
		};
	}
}
#endif //_H_PINE_ENGINE_STRING_CCHAR