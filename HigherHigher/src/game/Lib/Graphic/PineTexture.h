#pragma once
#if !defined(_H_PINE_TEXTURE)
#define _H_PINE_TEXTURE
#include "../DataType/CType.h"
#include "../OpenGLES/Graphic.h"
#include <vector>

class PineTexture
{
private:
	static UINT8*	_unzip_buff;
	static UINT32	_unzip_buff_length;

	HTEXTURE _tex;
	PineSize _tex_size;
	PineTexture(Graphic* g, PineSize base_size);
	Graphic* _g;
	bool _tex_valid;

public:
	static void BindUnzipBuffer(UINT8* buff, UINT32 len);
	static PineTexture* createTexture(Graphic* g, PineSize base_size);
	HTEXTURE getTexture();
	~PineTexture();
	void free();
	void beginDraw();
	void endDraw();
	
	void drawResource(Buffer* buff,int x, int y, int w, int h);

	void render(float x, float y, float tx = 0.0f, float ty = 0.0f, float tw = 1.0f, float th = 1.0f);

	PineSize GetSize() { return _tex_size;};
};
#endif//_H_PINE_TEXTURE