#include "pch.h"
#include "PineFreeType.h"
#include <stdio.h>
#include <string.h>

FT_Library PineFreeType::_ft_library;
bool PineFreeType::_is_ft_init = false;


void PineFreeType::InitFreeType()
{
	if (!_is_ft_init)
	{
		FT_Error error = FT_Init_FreeType(&_ft_library);
		if (!error)
		{
			_is_ft_init = true;
		}
	}
}

PineFreeTypePen* PineFreeType::LoadPen(ResourceItem resitem)
{
	if (_is_ft_init)
	{
		FILE* file = fopen(resitem.Path, "rb");
		if (file)
		{
			int len = resitem.Length;

			if (resitem.Length < 0)
			{
				fseek(file, 0, SEEK_END);
				len = ftell(file);
			}

			fseek(file, resitem.Offset, SEEK_SET);
			Buffer* buff = new Buffer(len);//buff will be free on UnloadPen;

			int readed = fread(buff->Data(), sizeof(char), len, file);

			while (readed < len && readed > 0)
			{
				int tmp = fread(buff->Data() + readed, sizeof(char), len - readed, file);
				if (tmp == 0)break;
				readed += tmp;
			}

			fclose(file);
			
			PineFreeTypePen* pen = new PineFreeTypePen();
			pen->_pen_buff = buff; 

			FT_Error err = FT_New_Memory_Face(_ft_library, buff->Data(), buff->Length(), 0, &pen->_face);

			if (err)
			{
				delete buff;
				delete pen;
				return NULL;
			}

			//set default size;
			int num_size = pen->_face->num_fixed_sizes;
			if (num_size > 0)
			{
				pen->_pen_size = pen->_face->available_sizes[0];
				FT_Set_Pixel_Sizes(pen->_face, pen->_pen_size.width, pen->_pen_size.height);
			}
			pen->AddRef();
			return pen;
		}
	}
	return NULL;
}