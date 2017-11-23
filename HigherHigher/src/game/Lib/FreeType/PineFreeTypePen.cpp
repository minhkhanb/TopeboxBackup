#include "pch.h"
#include "PineFreeTypePen.h"

PineFreeTypePen::PineFreeTypePen():_pen_buff(NULL), _ref_count(0), _space_size(0)
{

}

void PineFreeTypePen::AddRef()
{
	_ref_count++;
}

void PineFreeTypePen::Release()
{
	_ref_count--;

	if (_ref_count == 0)
	{
		FT_Done_Face(_face);

		if (_pen_buff != NULL)
		{
			delete _pen_buff;
		}
		delete this;
	}
}

void PineFreeTypePen::setCharSize(int size, bool on_vertical)
{
	if (on_vertical)
	{
		FT_Error error;
		error = FT_Set_Char_Size(_face, 0, size * 64, 300, 300);
	}
	else
	{
		FT_Error error;
		error = FT_Set_Char_Size(_face, size * 64, 0 , 300, 300);
	}
}

void PineFreeTypePen::setSpaceSize(float size)
{
	_space_size = size;
}
float PineFreeTypePen::getSpaceSize()
{
	return _space_size;
}