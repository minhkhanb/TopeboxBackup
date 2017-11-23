#if !defined(_H_PINE_FREETYPE_PEN)
#define _H_PINE_FREETYPE_PEN

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../DataType/CType.h"

class PineFreeTypePen
{
private:
	Buffer* _pen_buff;
	FT_Face _face;
	FT_Error _face_error;
	FT_Bitmap_Size _pen_size;
	float _space_size;
	
	PineFreeTypePen();

	int _ref_count;

public:
	void AddRef();
	void Release();

	void setCharSize(int size, bool on_vertical = false);
	void setSpaceSize(float size);
	float getSpaceSize();

	friend class PineFreeType;
	friend class PineFreeTypeCanvas;
};

#endif//_H_PINE_FREETYPE_PEN