#if !defined(_H_PINE_FREE_TYPE)
#define _H_PINE_FREE_TYPE
#include "../DataType/CType.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "PineFreeTypePen.h"
#include "PineFreeTypeCanvas.h"

class PineFreeType
{
private:
	static bool _is_ft_init;
	static FT_Library _ft_library;
public:
	
	static void InitFreeType();
	static PineFreeTypePen* LoadPen(ResourceItem resitem);

	
};

#endif//_H_PINE_FREE_TYPE
