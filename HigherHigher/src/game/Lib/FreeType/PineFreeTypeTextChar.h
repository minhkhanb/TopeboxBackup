#if !defined(_H_PINE_FREETYPE_TEXT_CHAR)
#define _H_PINE_FREETYPE_TEXT_CHAR

#include "../DataType/CType.h"

class PineFreeTypeTextChar
{
private:
	int _chr_index;
	PineRectangle _chr_rect;
	
	PineFreeTypeTextChar();

	friend class PineFreeTypeText;
};

#endif//_H_PINE_FREETYPE_TEXT_CHAR