#include "pch.h"
#include "PineFreeTypeText.h"
#include <string.h>
#include "PineFreeTypeCanvas.h"
#include "PineFreeTypeChar.h"
float PineFreeTypeText::infox[PAGARAP_MAX],PineFreeTypeText::infoy[PAGARAP_MAX];
PineFreeTypeText::PineFreeTypeText()
{

}
void PineFreeTypeText::Init(const char* str)
{
	int str_len = strlen(str);
	INT32 chr;
	int offset = 0;
	for (int c = 0; c < str_len; c++)
	{
		chr = str[c];

		PineFreeTypeTextChar tchar;
		tchar._chr_index = _cvas->insertChar(chr);
		if (tchar._chr_index > -1)
		{
			PineFreeTypeChar* chr_desc = &_cvas->_loaded_char[tchar._chr_index];
			tchar._chr_rect.X = offset;
			tchar._chr_rect.Y = 0 - chr_desc->_metrics.horiBearingY/64;
			tchar._chr_rect.W = chr_desc->_w;
			tchar._chr_rect.H = chr_desc->_h;

			offset += chr_desc->_w;
			//PDEBUG("load success:%c w:%d h:%d tex:%d\n", str[c], chr_desc->_w, chr_desc->_h, chr_desc->_tex);
		}
		else
		{
			//load char fail
			//PDEBUG("load fail:%c\n", str[c]);
			offset += _cvas->_pen->getSpaceSize();
		}
		_chr.push_back(tchar);
	}
}
void PineFreeTypeText::Init(INT32* str, int len)
{
	int offset = 0;
	for (int c = 0; c < len; c++)
	{

		PineFreeTypeTextChar tchar;
		tchar._chr_index = _cvas->insertChar(str[c]);
		if (tchar._chr_index > -1)
		{
			PineFreeTypeChar* chr_desc = &_cvas->_loaded_char[tchar._chr_index];
			tchar._chr_rect.X = offset;
			tchar._chr_rect.Y = 0 - chr_desc->_metrics.horiBearingY / 64;
			tchar._chr_rect.W = chr_desc->_w;
			tchar._chr_rect.H = chr_desc->_h;

			offset += chr_desc->_w;
			//PDEBUG("load success:%c w:%d h:%d tex:%d\n", str[c], chr_desc->_w, chr_desc->_h, chr_desc->_tex);
		}
		else
		{
			//load char fail
			//PDEBUG("load fail:%c\n", str[c]);
			offset += _cvas->_pen->getSpaceSize();
		}
		_chr.push_back(tchar);
	}
}

void PineFreeTypeText::Render(float x, float y,int anchor)
{
	/*
	float uv[4];
	uv[0] = uv[1] = 0.0f;
	uv[2] = uv[3] = 1.0f;

 	if (_chr.size() > 0)
	{
		PineFreeTypeChar* chr_desc = &_cvas->_loaded_char[_chr[0]._chr_index];
		_cvas->_g->Draw2DTexture(10, 50,300, 300, chr_desc->_tex,uv, 0, true);
	}
	return;
	*/
    float string_w = 0;
    float string_h = 0;
    
    float offx = 0;
    float offy = 0;
    long size = _chr.size();
    int line_space = 1.0f;
    if(size > 0)
    {
        if (_chr[size-1]._chr_index >= 0)
        {
            PineFreeTypeChar* chr_desc = &_cvas->_loaded_char[_chr[size-1]._chr_index];
            string_w = _chr[size-1]._chr_rect.X + line_space*(size-1) + chr_desc->_w;
            string_h = chr_desc->_h;
        }
    }
    if((anchor&_RIGHT) != 0)
    {
        offx -= string_w;
    }
    if((anchor & _HCENTER) != 0)
    {
        offx -= string_w/2;
    }
    if((anchor & _VCENTER) != 0)
    {
        offy -= string_h/2;
    }
    

	for (int c = 0; c < size; c++)
	{
		if (_chr[c]._chr_index >= 0)
		{
			PineFreeTypeChar* chr_desc = &_cvas->_loaded_char[_chr[c]._chr_index];

			//_cvas->_g->SetColor(0xFF00FFFF);
			//_cvas->_g->FillRect(x + _chr[c]._chr_rect.X, y + _chr[c]._chr_rect.Y, chr_desc->_w, chr_desc->_h);

			_cvas->_g->Draw2DTexture(offx+line_space*c + x + _chr[c]._chr_rect.X, offy+ y + _chr[c]._chr_rect.Y, chr_desc->_w, chr_desc->_h, chr_desc->_tex, &chr_desc->_uv[0], 0, true);

		}
		else
		{
			//PDEBUG("here\n");
		}
	}
	
}
void PineFreeTypeText::RenderPage(float x, float y,int width, int anchor)
{
    int line = 0;
    //calc line;
    long size = _chr.size();
    float xx = 0;
    float char_space = 1.0f;
    float line_space = 5.0f;
    int begin = 0;
    int true_end = 0;
    int line_w;
    int line_h = 0;
    
    for(int i = 0; i < size; i++)
    {
        if (_chr[i]._chr_index >= 0)
        {
            PineFreeTypeChar* chr_desc = &_cvas->_loaded_char[_chr[i]._chr_index];
            xx += (char_space + chr_desc->_w);
            bool isbreak = false;
            if(xx >= width)
            {
                int end = i;
                true_end = end;
                isbreak = true;
                for(end = i; end > begin; end--)
                {
                    if (_chr[end]._chr_index >= 0)
                    {
                        PineFreeTypeChar* chr_desc_end = &_cvas->_loaded_char[_chr[end]._chr_index];
                        if(chr_desc_end->_chr == ' ')
                        {
                            true_end = end;
							break;
                        }
                    }
                    else
                    {
                        true_end = end;
                        break;
                    }
                        
                }
            }
            else if(chr_desc->_chr == '\n' || i+1 >= size)
            {
                true_end = i;
                isbreak = true;
            }
            
            if(isbreak)
            {
                line_w = 0;
                int h_max = 0;
                for(int r = begin; r <= true_end; r++)
                {
                    if (_chr[r]._chr_index >= 0)
                    {
                        PineFreeTypeChar* chr_desc_end = &_cvas->_loaded_char[_chr[r]._chr_index];
                        line_w += (char_space+chr_desc_end->_w);
                        if(h_max < chr_desc_end->_h)
                        {
                            h_max = chr_desc_end->_h;
                        }
                    }
                    else
                    {
                        line_w += (char_space + _cvas->_pen->getSpaceSize());
                    }
                }
                int xxx = 0;
                float offsetx = 0;
                if((anchor&_RIGHT) != 0)
                {
                    offsetx -= line_w;
                }
                if((anchor & _HCENTER) != 0)
                {
                    offsetx -= line_w/2;
                }
                for(int r = begin; r <= true_end; r++)
                {
					if (_chr[r]._chr_index >= 0)
					{
						PineFreeTypeChar* chr_desc_end = &_cvas->_loaded_char[_chr[r]._chr_index];
						infox[r] = xxx + offsetx;
						infoy[r] = line_h;
						xxx = (xxx + char_space + chr_desc_end->_w);
					}
                    else
                    {
                        xxx += (char_space + _cvas->_pen->getSpaceSize());
                    }
                   
                    
                }
                line_h += (h_max+line_space);
                i = true_end;
                begin = true_end+1;
                line++;
				xx = 0;
                
            }
        }
        else
        {
            xx += (char_space + _cvas->_pen->getSpaceSize());
        }
        
    }
    float offy = 0;
    if((anchor & _VCENTER) != 0)
    {
        offy -= line_h/2;
    }
    int count = 0;
    for (int c = 0; c < size; c++)
    {
        if (_chr[c]._chr_index >= 0)
        {
            PineFreeTypeChar* chr_desc = &_cvas->_loaded_char[_chr[c]._chr_index];
            
            //_cvas->_g->SetColor(0xFF00FFFF);
            //_cvas->_g->FillRect(x + _chr[c]._chr_rect.X, y + _chr[c]._chr_rect.Y, chr_desc->_w, chr_desc->_h);
            
			_cvas->_g->Draw2DTexture(infox[c] + x, offy + infoy[c] + y + _chr[c]._chr_rect.Y, chr_desc->_w, chr_desc->_h, chr_desc->_tex, &chr_desc->_uv[0], 0, true);
           // count++;
            
        }
        else
        {
            //PDEBUG("here\n");
        }
    }
    
    
}

PineFreeTypeText::~PineFreeTypeText()
{
	
}