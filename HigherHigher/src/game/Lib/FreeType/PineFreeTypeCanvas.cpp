#include "pch.h"
#include "PineFreeTypeCanvas.h"
void debugFreeSpace(PineRectangle* rect_array, int num)
{
	for (int r = 0; r < num; r++)
	{
		PDEBUG("space:x:%0.2f y:%0.2f w:%0.2f h:%0.2f\n", rect_array[r].X, rect_array[r].Y, rect_array[r].W, rect_array[r].H);
	}
}

PineFreeTypeCanvas::PineFreeTypeCanvas()
{

}


//static
PineFreeTypeCanvas* PineFreeTypeCanvas::create(Graphic*g, PineFreeTypePen* pen, int size)
{
	if (size != 256 && size != 512 && size != 1024) return NULL;
	PineFreeTypeCanvas* cvas = new PineFreeTypeCanvas();

	pen->AddRef();
	cvas->_g = g;
	cvas->_pen = pen;

	cvas->_tex_size = size;

	return cvas;
}

HTEXTURE PineFreeTypeCanvas::createNewTexture()
{
	Buffer* buff = new Buffer(_tex_size * _tex_size * 4);
	BYTE* buff_data = buff->Data();

	for (int i = 0; i < _tex_size*_tex_size * 4; i++)
	{
		buff_data[i] = 0;
	}

	HTEXTURE tex = _g->createTexture(buff_data, _tex_size, _tex_size, PineColorFormat::RGBA_1BYTE);

	delete buff;

	return tex;
}

void PineFreeTypeCanvas::putCharImage(PineFreeTypeChar* chr, BYTE* buff, int w, int h)
{
	//find the freespace that big enough
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
		space_rect = {0, 0, (float)_tex_size, (float)_tex_size };
		std::vector<PineRectangle> vec;
		vec.push_back(space_rect);
		space_index = 0;
		_free_space.push_back(vec);
		_tex.push_back(createNewTexture());
		tex_index = _free_space.size() - 1;
	}

	//create new 2 space
	PineRectangle rest_right = { (float)space_rect.X + w, (float)space_rect.Y, (float)space_rect.W - w, (float)h };
	PineRectangle rest_bottom = { (float)space_rect.X, (float)space_rect.Y + h, (float)w, (float)space_rect.H - h };

	//remove old one
	_free_space[tex_index].erase(_free_space[tex_index].begin() + space_index);

	_free_space[tex_index].push_back(rest_right);
	_free_space[tex_index].push_back(rest_bottom);

	chr->_tex = _tex[tex_index];

	float* uv = chr->_uv;

	uv[0] = uv[2] = space_rect.X;
	uv[1] = uv[3] = space_rect.Y;

	uv[2] += w;
	uv[3] += h;

	uv[0] /= _tex_size;
	uv[1] /= _tex_size;
	uv[2] /= _tex_size;
	uv[3] /= _tex_size;

	chr->_x = space_rect.X;
	chr->_y = space_rect.Y;

	_g->updateTexture(chr->_tex, buff, w, h, space_rect.X, space_rect.Y, PineColorFormat::RGBA_1BYTE);
	PDEBUG("use space at: x:%0.2f y:%0.2f w:%d h:%d\n", space_rect.X, space_rect.Y, w, h);
	for (int i = 0; i < _free_space.size(); i++)
	{
		PDEBUG("freespace at texture:%d\n", i);
		debugFreeSpace(&_free_space[i][0], _free_space[i].size());
	}
	PDEBUG("\n");
	
}

int PineFreeTypeCanvas::insertChar(INT32 chr)
{
	int index = getCharIndex(chr);
	if (index == -1)
	{
		FT_GlyphSlot slot = _pen->_face->glyph;
		FT_Error error;
		FT_UInt glyph_index = FT_Get_Char_Index(_pen->_face, chr);
		error = FT_Load_Char(_pen->_face, chr, FT_LOAD_RENDER);

		if (error)
		{
			return -1;
		}

		index = _loaded_char.size();

		PineFreeTypeChar chrdesc;
		chrdesc._chr = chr;
		chrdesc._metrics = slot->metrics;
		
		//FT_Get_Bitm
		int bl = 0;// slot->bitmap_left;
		int bt = 0;// slot->bitmap_top;
		int w = slot->bitmap.width - bl;
		int h = slot->bitmap.rows - bt;

		if (w == 0 || h == 0) return -1;

		chrdesc._w = w;
		chrdesc._h = h;

		BYTE* buff = new BYTE[w * h * 4];

		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{

				int offset = ((y + bt) *slot->bitmap.width) + ((bl + x));

				buff[y * w * 4 + x * 4 + 0] = 0xFF;// slot->bitmap.buffer[offset + 1];
				buff[y * w * 4 + x * 4 + 1] = 0xFF;// slot->bitmap.buffer[offset + 2];
				buff[y * w * 4 + x * 4 + 2] = 0xFF;// slot->bitmap.buffer[offset + 3];
				buff[y * w * 4 + x * 4 + 3] = slot->bitmap.buffer[offset];

			}
		}
		putCharImage(&chrdesc, buff, w, h);

		delete buff;
		chrdesc._ref_count++;

		_loaded_char.push_back(chrdesc);
	}
	else
	{
		_loaded_char[index]._ref_count++;
	}
	return index;
}

void PineFreeTypeCanvas::reloadChar(PineFreeTypeChar* chr)
{

		FT_GlyphSlot slot = _pen->_face->glyph;
		FT_Error error;
		FT_UInt glyph_index = FT_Get_Char_Index(_pen->_face, chr->_chr);
		error = FT_Load_Char(_pen->_face, chr->_chr, FT_LOAD_RENDER);

		if (error)
		{
			return;
		}


		//FT_Get_Bitm
		int bl = 0;// slot->bitmap_left;
		int bt = 0;// slot->bitmap_top;
		int w = slot->bitmap.width - bl;
		int h = slot->bitmap.rows - bt;

		if (w == 0 || h == 0) return;

		BYTE* buff = new BYTE[w * h * 4];

		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{

				int offset = ((y + bt) *slot->bitmap.width) + ((bl + x));

				buff[y * w * 4 + x * 4 + 0] = 0xFF;// slot->bitmap.buffer[offset + 1];
				buff[y * w * 4 + x * 4 + 1] = 0xFF;// slot->bitmap.buffer[offset + 2];
				buff[y * w * 4 + x * 4 + 2] = 0xFF;// slot->bitmap.buffer[offset + 3];
				buff[y * w * 4 + x * 4 + 3] = slot->bitmap.buffer[offset];

			}
		}
		_g->updateTexture(chr->_tex, buff, w, h, chr->_x, chr->_y, PineColorFormat::RGBA_1BYTE);

		delete buff;

}

int PineFreeTypeCanvas::getCharIndex(INT32 chr)
{
	for (int i = 0; i < _loaded_char.size(); i++)
	{
		if (chr == _loaded_char[i]._chr) return i;
	}
	return -1;
}

PineFreeTypeText* PineFreeTypeCanvas::createNewText()
{
	PineFreeTypeText* text = new PineFreeTypeText();
	text->_cvas = this;
	return text;
}

PineFreeTypeCanvas::~PineFreeTypeCanvas()
{
	for (int i = 0; i < _tex.size(); i++)
	{
		_g->freeTexture(_tex[i]);
	}
	_pen->Release();
}

void PineFreeTypeCanvas::free()
{
	_tmp_tex.clear();
	for (int i = 0; i < _loaded_char.size(); i++)
	{
		for (int j = 0; j < _tex.size(); j++)
		{
			if (_loaded_char[i]._tex == _tex[j])
			{
				_tmp_tex.push_back(j);
				break;
			}
		}
	}
	for (int i = 0; i < _tex.size(); i++)
	{
		_g->freeTexture(_tex[i]);
	}
}
void PineFreeTypeCanvas::reload()
{
	std::vector<HTEXTURE> tmp;

	for (int i = 0; i < _tex.size(); i++)
	{
		tmp.push_back(createNewTexture());
	}

	for (int i = 0; i < _loaded_char.size(); i++)
	{
		_loaded_char[i]._tex = tmp[_tmp_tex[i]];
		reloadChar(&_loaded_char[i]);
	}
}