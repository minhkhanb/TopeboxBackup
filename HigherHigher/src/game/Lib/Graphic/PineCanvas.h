#pragma once
#if !defined(_H_PINE_CANVAS)
#define _H_PINE_CANVAS
#include "../DataType/CType.h"
#include "../OpenGLES/Graphic.h"
#include "PineCanvasResource.h"
#include <vector>

class PineCanvas
{
private:
	std::vector<HTEXTURE> _tex;
	PineSize _tex_size;
	std::vector<std::vector<PineRectangle>> _free_space;
	std::vector<PineCanvasResource> _res;
	PineCanvas(PineSize base_size);
	Graphic* _g;

	HTEXTURE createNewTexture();
public:
	void release();
	static PineCanvas* createCanvas(Graphic* g, PineSize base_size);
	PineCanvasResource putResource(Buffer* buff, int w, int h);

	void renderResource(PineCanvasResource res, float x, float y);
};
#endif//_H_PINE_CANVAS