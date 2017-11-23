#if !defined(_H_PINE_FREETYPE_CANVAS)
#define _H_PINE_FREETYPE_CANVAS
#include "../DataType/CType.h"
#include "../OpenGLES/Graphic.h"
#include "PineFreeTypeChar.h"
#include "PineFreeTypePen.h"
#include "PineFreeTypeText.h"

#include <vector>

class PineFreeTypeCanvas
{
private:

	PineFreeTypeCanvas();
	Graphic* _g;
	PineFreeTypePen* _pen;
	int _tex_size;
	std::vector<PineFreeTypeChar> _loaded_char;
	std::vector<std::vector<PineRectangle>> _free_space;
	std::vector<HTEXTURE> _tex;

	std::vector<INT32> _tmp_tex;

	void putCharImage(PineFreeTypeChar* chr, BYTE* buff, int w, int h);
	void reloadChar(PineFreeTypeChar* chr);
	HTEXTURE createNewTexture();

	int getCharIndex(INT32 chr);
	int insertChar(INT32 chr);

public:

	static PineFreeTypeCanvas* create(Graphic*g, PineFreeTypePen* pen, int size);//256 512 1024
	PineFreeTypeText* createNewText();

	void free();
	void reload();

	~PineFreeTypeCanvas();

	friend class PineFreeType;
	friend class PineFreeTypeText;
};

#endif//_H_PINE_FREETYPE_CANVAS