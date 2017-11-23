#include "pch.h"
#include "GraphicUtil.h"

void GraphicUtil::FillTransparent(Buffer buff)
{
	BYTE* p = buff.Data();
	int len = buff.Length();
	while (--len >= 0)
	{
		*(p + len) = 0;
	}
}

void GraphicUtil::FillTransparent(BYTE* buff, int len)
{
	while (--len >= 0)
	{
		*(buff + len) = 0;
	}
}

void GraphicUtil::MergeColorRgbaNormal(COLOR bg_color, COLOR fg_color, COLOR& out)
{
	out = bg_color;
	BYTE a = fg_color & 0xFF;
	if (a == 0) return;



	BYTE r = (fg_color >> 24) & 0xFF;
	BYTE g = (fg_color >> 16) & 0xF;
	BYTE b = (fg_color >> 8) & 0xF;



	BYTE bg_a = bg_color & 0xFF;
	BYTE bg_r = (bg_color >> 24) & 0xFF;
	BYTE bg_g = (bg_color >> 24) & 0xFF;
	BYTE bg_b = (bg_color >> 24) & 0xFF;


	float ratio_bg = bg_a;
	float ratio = a;

	ratio_bg *= (255.0f - ratio);


	float merge_ratio = ratio_bg + ratio;

	float ratio_1 = ratio / merge_ratio;
	float ratio_2 = ratio_bg / merge_ratio;


	r = r * ratio_1 + bg_r * ratio_2;
	g = g * ratio_1 + bg_g * ratio_2;
	b = b * ratio_1 + bg_b * ratio_2;

	a = (BYTE)merge_ratio;

	out =  a | r << 24 | g << 16 | b << 8;
}

void GraphicUtil::MergeColorRgbaNormal(BYTE* bg_color, BYTE* fg_color)
{
	BYTE a = *(fg_color + 3);

	if (a == 0) return;

	BYTE r = *fg_color;
	BYTE g = *(fg_color + 1);
	BYTE b = *(fg_color + 2);



	BYTE bg_a = *(bg_color + 3);
	BYTE bg_r = *bg_color;
	BYTE bg_g = *(bg_color + 1);
	BYTE bg_b = *(bg_color + 2);


	float ratio_bg = bg_a;
	float ratio = a;

	ratio_bg *= (255.0f - ratio);


	float merge_ratio = ratio_bg + ratio;

	float ratio_1 = ratio / merge_ratio;
	float ratio_2 = ratio_bg / merge_ratio;


	r = r * ratio_1 + bg_r * ratio_2;
	g = g * ratio_1 + bg_g * ratio_2;
	b = b * ratio_1 + bg_b * ratio_2;

	a = (BYTE)merge_ratio;

	*bg_color = r;
	*(bg_color + 1) = g;
	*(bg_color + 2) = b;
	*(bg_color + 3) = a;
}
static int debug_2_id = 0;
void GraphicUtil::DrawImage(BYTE* bg_buff, PineSize bg_size, BYTE* fg_buff, PineSize fg_size, PineRectangle fg_rect, int tx, int ty, BYTE color_size, BYTE flip)
{
	printf("draw image: bgw:%f bgh:%f fgw:%f fgh:%f clipx:%f clipy:%f clipw:%f cliph:%f\n", bg_size.W, bg_size.H, fg_size.W, fg_size.H, fg_rect.X, fg_rect.Y, fg_rect.W, fg_rect.H);
	int fg_offy = 0;
	int bg_off = 0;
	int fg_off = 0;
	int rx = 0, ry = 0;
	int tmp_w = 0 , tmp_h = 0;

	/*char file_name[255];
	sprintf(file_name, "debug_%d", debug_2_id++);
	FILE* tmp_file = fopen(file_name, "wb");
	if (tmp_file != NULL)
	{
	fwrite(fg_buff, 1, fg_size.W * fg_size.H * 4, tmp_file);
	fclose(tmp_file);
	}*/

	if (flip == 0)
	{
		tmp_w = (fg_rect.W + tx > bg_size.W) ? bg_size.W - tx : fg_rect.W;
		tmp_h = (fg_rect.H + ty > bg_size.H) ? bg_size.H - ty: fg_rect.H;

		for (int y = 0; y < tmp_h; y++)
		{
			ry = (ty + y);
			fg_off = (fg_rect.Y + y) * fg_size.W * 4 + fg_rect.X * 4;
			bg_off = ry * bg_size.W * 4 + tx *4;

			for (int x = 0; x < tmp_w; x++, fg_off += 4, bg_off += 4)
			{
				
				//rx = (tx + x);

				//if (ry < 0 || rx < 0 || ry >= bg_size.H || rx >= bg_size.W)continue;

				//bg_off = (ry * bg_size.W + rx) * 4;

				//fg_off = fg_offy + (fg_rect.X + x) * 4;

				/*BYTE* p = bg_buff + bg_off;
				BYTE* p_buff = fg_buff + fg_off;

				*p++ = *p_buff++;
				*p++ = *p_buff++;
				*p++ = *p_buff++;
				*p++ = *p_buff++;*/

				MergeColorRgbaNormal(bg_buff + bg_off, fg_buff + fg_off);
			}
		}

		/*char file_name[255];
		sprintf(file_name, "debug_%d", debug_2_id++);
		FILE* tmp_file = fopen(file_name, "wb");
		if (tmp_file != NULL)
		{
			fwrite(bg_buff, 1, bg_size.W * bg_size.H * 4, tmp_file);
			fclose(tmp_file);
		}*/
	}
	//not check
	else if (flip == 2)
	{
		for (int y = fg_rect.H - 1; y >= 0; y--)
		{

			fg_offy = (fg_rect.Y + y) * fg_size.W * 4;
			for (int x = 0; x < fg_rect.W; x++)
			{
				ry = (ty + y);
				rx = (tx + x);

				if (ry < 0 || rx < 0 || ry >= bg_size.H || rx >= bg_size.W)continue;

				bg_off = (ry * bg_size.W + rx) * 4;
				fg_off = fg_offy + (fg_rect.X + x) * 4;

				MergeColorRgbaNormal(bg_buff + bg_off, fg_buff + fg_off);
			}
		}
	}
	//not check
	else if (flip == 1)
	{
		for (int y = 0; y < fg_rect.H; y++)
		{
			fg_offy = (fg_rect.Y + y) * fg_size.W * 4;
			for (int x = fg_rect.W - 1; x >= 0; x--)
			{
				ry = (ty + y);
				rx = (tx + x);

				if (ry < 0 || rx < 0 || ry >= bg_size.H || rx >= bg_size.W)continue;

				bg_off = (ry * bg_size.W + rx) * 4;
				fg_off = fg_offy + (fg_rect.X + x) * 4;

				MergeColorRgbaNormal(bg_buff + bg_off, fg_buff + fg_off);
			}
		}
	}
	//not check
	else
	{
		for (int y = fg_rect.H - 1; y >= 0; y--)
		{
			fg_offy = (fg_rect.Y + y) * fg_size.W * 4;
			for (int x = fg_rect.W - 1; x >= 0; x--)
			{
				ry = (ty + y);
				rx = (tx + x);

				if (ry < 0 || rx < 0 || ry >= bg_size.H || rx >= bg_size.W)continue;

				bg_off = (ry * bg_size.W + rx) * 4;
				fg_off = fg_offy + (fg_rect.X + x) * 4;

				MergeColorRgbaNormal(bg_buff + bg_off, fg_buff + fg_off);
			}
		}
	}
}