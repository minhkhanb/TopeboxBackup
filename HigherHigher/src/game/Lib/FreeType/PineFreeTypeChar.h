#if !defined(_H_PINE_FREETYPE_CHAR)
#define _H_PINE_FREETYPE_CHAR

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../DataType/CType.h"
#include "../OpenGLES/Graphic.h"

class PineFreeTypeChar
{
private:
	INT32 _chr;
	FT_Glyph_Metrics _metrics;
	HTEXTURE _tex;
	float _uv[4];
	int _ref_count;
	int _x;
	int _y;
	int _w;
	int _h;
	PineFreeTypeChar();
	friend class PineFreeTypeCanvas;
	friend class PineFreeTypeText;
};

#endif//_H_PINE_FREETYPE_CHAR