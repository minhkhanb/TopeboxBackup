#if !defined(_H_GRAPHIC_UTIL)
#define _H_GRAPHIC_UTIL

#include "../DataType/CType.h"

class GraphicUtil
{
public:
	static void FillTransparent(Buffer buff);
	static void FillTransparent(BYTE* buff, int len);
	static void MergeColorRgbaNormal(COLOR bg_color, COLOR fg_color, COLOR& out);

	//merge foreground to background
	static void MergeColorRgbaNormal(BYTE* bg_color, BYTE* fg_color);

	static void DrawImage(BYTE* bg_buff, PineSize bg_size, BYTE* fg_buff, PineSize fg_size, PineRectangle fg_rect, int tx = 0, int ty = 0, BYTE color_size = 4, BYTE flip = 0);
};
#endif//_H_GRAPHIC_UTIL