#include "pch.h"
#include "PineTexture.h"
UINT8*	PineTexture::_unzip_buff = NULL;
UINT32	PineTexture::_unzip_buff_length = 0;

HTEXTURE PineTexture::getTexture()
{
	return _tex;
}

void PineTexture::BindUnzipBuffer(UINT8* buff, UINT32 len)
{
	PineTexture::_unzip_buff = buff;
	PineTexture::_unzip_buff_length = len;
}

PineTexture* PineTexture::createTexture(Graphic* g, PineSize base_size)
{
	PineSize use_size = { 0,0 };
	
	//calc texture width
	int size = 2;
	for (int i = 0; i < 15; i++)
	{
		if (size >= base_size.W)
		{
			use_size.W = size;
			break;
		}
		size *= 2;
	}
	//calc texture height
	size = 2;
	for (int i = 0; i < 15; i++)
	{
		if (size >= base_size.H) {
			use_size.H = size;
			break;
		}
		size *= 2;
	}
	//create texture
	if (g != NULL && use_size.W > 0 && use_size.H > 0) 
	{
		
		PineTexture* tex = new PineTexture(g, use_size);
		return tex;
	}
	//printf("create texture fail:%f :%f", use_size.W, use_size.H);
	return NULL;
}
PineTexture::PineTexture(Graphic* g, PineSize base_size)
{
	_g = g;
	_tex_valid = false;
	_tex = NULL;
	_tex_size = base_size;
	//PDEBUG("create Texture:%f :%f", _tex_size.W, _tex_size.H);
}
PineTexture::~PineTexture()
{
	free();
}

void PineTexture::free()
{
	if (_tex_valid) 
	{
		_g->freeTexture(_tex);
		_tex = NULL;
		_tex_valid = false;
	}
}

void PineTexture::beginDraw()
{
	free();
	int tex_data_size = _tex_size.W * _tex_size.H * 4;
	if (_g != NULL && PineTexture::_unzip_buff != NULL && PineTexture::_unzip_buff_length >= tex_data_size)
	{
		for (int i = 0; i < tex_data_size; i++)
		{
			PineTexture::_unzip_buff[i] = 0;
		}
	}
}

void PineTexture::endDraw()
{
	_tex = _g->createTexture(PineTexture::_unzip_buff, _tex_size.W, _tex_size.H, PineColorFormat::RGBA_1BYTE);
	_tex_valid = true;
}

void PineTexture::drawResource(Buffer* buff, int x, int y, int w, int h)
{
	if (PineTexture::_unzip_buff != NULL)
	{
		BYTE* pbuff = (BYTE*)buff->Data();

		BYTE* pbuff_bg = (BYTE*)PineTexture::_unzip_buff;

		if (x < 0) {
			w -= 0 - x;
			x = 0;
		}
		if (y < 0) {
			h -= 0 - y;
			y = 0;
		}

		int tmp_w = (x + w > _tex_size.W) ? _tex_size.W - x : w;
		int tmp_h = (y + h > _tex_size.H) ? _tex_size.H - y: h;
		
		int tex_w = (int)_tex_size.W;
		
		for (int by = 0; by < tmp_h; by++)
		{
			int bg_off = ((y + by) * tex_w) * 4 + x*4;
			int buff_off = by * w * 4;
			//PDEBUG("bg_off:%d y:%d by:%d x:%d tex_w:%d", bg_off, y, by, x, tex_w);
			for (int bx = 0; bx < tmp_w; bx++, bg_off += 4, buff_off += 4)
			{
				//int off = off_y + (x + bx) * 4;
				//int off_buff = by * w * 4 + bx * 4;

				BYTE a = pbuff[buff_off + 3];

				if (a == 0) continue;
				
				BYTE bg_a = pbuff_bg[bg_off + 3];
				BYTE bg_r = pbuff_bg[bg_off];
				BYTE bg_g = pbuff_bg[bg_off + 1];
				BYTE bg_b = pbuff_bg[bg_off + 2];
				/*
				float ratio_bg = bg_a * (255.0f - a);
				float ratio_a = a;

				float merge_ratio = ratio_bg + a;

				float ratio_1 = a / merge_ratio;
				float ratio_2 = ratio_bg / merge_ratio;

				pbuff_bg[bg_off] = pbuff[buff_off] * ratio_1 + bg_r * ratio_2;
				pbuff_bg[bg_off + 1] = pbuff[buff_off + 1] * ratio_1 + bg_g * ratio_2;
				pbuff_bg[bg_off + 2] = pbuff[buff_off + 2] * ratio_1 + bg_b * ratio_2;

				if (hasTransparentPixel)
				{
					pbuff_bg[bg_off + 3] = (BYTE)bg_a == 0 ? merge_ratio : 255;
				}
				else
				{
					pbuff_bg[bg_off + 3] = (BYTE)merge_ratio;
				}*/
				
				pbuff_bg[bg_off] = ((pbuff[buff_off] * a) + (bg_r * (255 - a))) >> 8;
				pbuff_bg[bg_off + 1] = ((pbuff[buff_off + 1] * a) + ( bg_g * (255 - a))) >> 8;
				pbuff_bg[bg_off + 2] = ((pbuff[buff_off + 2] * a) + (bg_b * (255 - a))) >> 8;
				pbuff_bg[bg_off + 3] = bg_a + a >= 255 ? 255 : bg_a + a;

			}
		}
	}
	
}

void PineTexture::render(float x, float y, float tx, float ty, float tw, float th)
{
	if (_g != NULL && _tex_valid)
	{
		float uv[4];
		uv[0] = tx; 
		uv[1] = ty;
		uv[2] = tx + tw;
		uv[3] = ty + th;
		//uv[0] = uv[1] = 0;
		//uv[2] = uv[3] = 1;
		//_g->setDebugFlagOn(DEBUG_GRAPHIC_DRAWTEX);
		_g->Draw2DTexture(x, y, _tex_size.W * (tw), _tex_size.H * th, _tex, uv, 0, true);
		//_g->setDebugFlagOff(DEBUG_GRAPHIC_DRAWTEX);
	}
}