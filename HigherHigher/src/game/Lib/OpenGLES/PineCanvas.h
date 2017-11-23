#pragma once
#if !defined(_H_PINE_TEXTURE)
#define _H_PINE_TEXTURE
#include "../DataType/CType.h"
#include "Graphic.h"
#include <vector>

class PineCanvas
{
private:
	std::vector<HTEXTURE> _tex;
	PineSize _tex_size;
	std::vector<std::vector<PineRectangle>> _free_space;
	PineCanvas(PineSize base_size);
	Graphic* _g;

	HTEXTURE PineCanvas::createNewTexture();
public:
	static PineCanvas* createCanvas(Graphic* g, PineSize base_size);
	void freeTexture();
	void reload();
	//draw function
	void putResource(Buffer* buff, int w, int h);
};
#endif//_H_PINE_TEXTURE