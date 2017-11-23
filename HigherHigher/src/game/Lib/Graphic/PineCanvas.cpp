#include "pch.h"
#include "PineCanvas.h"

PineCanvas::PineCanvas(PineSize base_size)
{
	_tex_size = base_size;
}

PineCanvas* PineCanvas::createCanvas(Graphic* g, PineSize base_size)
{
	if (g != NULL 
		&& (base_size.W == 256 || base_size.W == 512 || base_size.W == 1024) 
		&& (base_size.H == 256 || base_size.H == 512 || base_size.H == 1024) )
	{
		PineCanvas* cvas = new PineCanvas(base_size);
		cvas->_g = g;
		return cvas;
	}
	return NULL;
}

void PineCanvas::release()
{
	for (int i = 0; i < _tex.size(); i++)
	{
		_g->freeTexture(_tex[i]);
	}
	_tex.clear();
	_free_space.clear();
	_res.clear();
}


HTEXTURE PineCanvas::createNewTexture()
{
	Buffer* buff = new Buffer(_tex_size.W * _tex_size.H * 4);
	BYTE* buff_data = buff->Data();

	for (int i = 0; i < _tex_size.W * _tex_size.H * 4; i++)
	{
		buff_data[i] = 0;
	}

	HTEXTURE tex = _g->createTexture(buff_data, _tex_size.W, _tex_size.H, PineColorFormat::RGBA_1BYTE);

	delete buff;

	return tex;
}


PineCanvasResource PineCanvas::putResource(Buffer* buff, int w, int h)
{
	// find the freespace that big enough
		int space_index = -1;
	int tex_index = -1;
	PineRectangle space_rect;

	if (_free_space.size() > 0)
	{
		tex_index = _free_space.size() - 1;
		int tmpw, tmph;

		for (int t = 0; t < _free_space[tex_index].size(); t++)
		{
			tmpw = _free_space[tex_index][t].W;
			tmph = _free_space[tex_index][t].H;
			if (tmpw >= w && tmph >= h) //for simple, we only support one direction now
			{
				space_index = t;
				space_rect = _free_space[tex_index][t];
				break;
			}
		}
	}
	if (space_index == -1)
	{
		//create new tex
		space_rect.X = space_rect.Y = 0;
		space_rect.W = _tex_size.W;
		space_rect.H = _tex_size.H;

		std::vector<PineRectangle> vec;
		vec.push_back(space_rect);
		space_index = 0;
		_free_space.push_back(vec);
		_tex.push_back(createNewTexture());
		tex_index = _free_space.size() - 1;
	}

	//create new 2 space
	PineRectangle rest_right;
	rest_right.X = (float)space_rect.X + w;
	rest_right.Y = (float)space_rect.Y;
	rest_right.W = (float)space_rect.W - w;
	rest_right.H = (float)space_rect.H;
	
	PineRectangle rest_bottom;
	rest_bottom.X = (float)space_rect.X;
	rest_bottom.Y = (float)space_rect.Y + h;
	rest_bottom.W = (float)w;
	rest_bottom.H = (float)space_rect.H - h;

	//remove old one
	_free_space[tex_index].erase(_free_space[tex_index].begin() + space_index);

	_free_space[tex_index].push_back(rest_right);
	_free_space[tex_index].push_back(rest_bottom);

	PineCanvasResource res;
	res._id = _res.size();

	res._tex = _tex[tex_index];
	
	float* uv = res._uv;

	uv[0] = uv[2] = space_rect.X;
	uv[1] = uv[3] = space_rect.Y;

	uv[2] += w;
	uv[3] += h;

	uv[0] /= _tex_size.W;
	uv[1] /= _tex_size.H;
	uv[2] /= _tex_size.W;
	uv[3] /= _tex_size.H;

	res._w = w;
	res._h = h;

	//chr->_x = space_rect.X;
	//chr->_y = space_rect.Y;

	_g->updateTexture(res._tex, buff->Data(), w, h, space_rect.X, space_rect.Y, PineColorFormat::RGBA_1BYTE);
	
	_res.push_back(res);
	PDEBUG("use space at: x:%0.2f y:%0.2f w:%d h:%d\n", space_rect.X, space_rect.Y, w, h);
	for (int i = 0; i < _free_space.size(); i++)
	{
		PDEBUG("freespace at texture:%d\n", i);
		//debugFreeSpace(&_free_space[i][0], _free_space[i].size());
	}
	PDEBUG("\n");

	return res;
}

void PineCanvas::renderResource(PineCanvasResource res, float x, float y)
{
	_g->Draw2DTexture(x , y , res._w, res._h ,res._tex, res._uv, 0, true);

}