#include "pch.h"
#include "PineImage.h"
#include "../GameCore/CGame.h"
#include <string.h>
#include <math.h>
#include "../GameCore/Os_Functions.h"
#include "Sprite/CSprite.h"
#if defined(_WINDOWS_PHONE8) || defined(ANDROID_OS)|| defined(_WIN8)
#include "png/png.h"
#endif
#include "../Lib/png/lodepng.h"
#include "DataType/BufferStream.h"


float*	PineImage::_uv = new float[4];
UINT8*	PineImage::_unzip_buff = NULL;
UINT32	PineImage::_unzip_buff_length = 0;

int PineImage::CreateImageMix2(Graphic* g,PineImage* image1,PineImage* image2,int type_mix)
{
	int width = image1->GetWidth();
	int height = image1->GetHeight();

	int width1 = image1->GetWidth();
	int width2 = image2->GetWidth();

	int height1 = image1->GetHeight();
	int height2 = image2->GetHeight();
	BYTE *buffer1 = image1->GetBufferImage();
	BYTE *buffer2 = image2->GetBufferImage();
	if(_buffer == NULL)
	{
		_buffer = new BYTE[width*height*4];
		_width = width;
		_height = height;
	}
	if(_buffer != NULL && (width*height) != (_width*_height))
	{
		delete[] _buffer;
		_buffer = new BYTE[width*height*4];
		_width = width;
		_height = height;
	}
	if(type_mix == 1)
	{
		for(int i = 0; i < height; i++)
		{
			for(int j = 0; j < width; j++)
			{
				_buffer[(i*width+j)*4]   = buffer1[(i*width+j)*4];
				_buffer[(i*width+j)*4+1] = buffer1[(i*width+j)*4+1];
				_buffer[(i*width+j)*4+2] = buffer1[(i*width+j)*4+2];
				_buffer[(i*width+j)*4+3] = buffer1[(i*width+j)*4+3];
			}
		}
		CannyEdgeDetect(_buffer,width,height);
	}
	else
	{
		for(int i = 0; i < height; i++)
		{
			for(int j = 0; j < width; j++)
			{
				if(j < width1 && j < width2 && i < height1 && i < height2)
				{
					if(type_mix == 0)
					{
						_buffer[(i*width+j)*4] =	(buffer2[(i*width2+j)*4] *	buffer2[(i*width2+j)*4+3] / 255) + (buffer1[(i*width+j)*4] * buffer1[(i*width+j)*4+3] * (255 - buffer2[(i*width2+j)*4+3]) / (255*255));
						_buffer[(i*width+j)*4+1] =	(buffer2[(i*width2+j)*4+1] * buffer2[(i*width2+j)*4+3] / 255) + (buffer1[(i*width+j)*4+1] * buffer1[(i*width+j)*4+3] * (255 - buffer2[(i*width2+j)*4+3]) / (255*255));
						_buffer[(i*width+j)*4+2] =	(buffer2[(i*width2+j)*4+2] * buffer2[(i*width2+j)*4+3] / 255) + ( buffer1[(i*width+j)*4+2] * buffer1[(i*width+j)*4+3] * (255 - buffer2[(i*width2+j)*4+3]) / (255*255));
						_buffer[(i*width+j)*4+3] =	buffer2[(i*width2+j)*4+3] + (buffer1[(i*width+j)*4+3] * (255 - buffer2[(i*width2+j)*4+3]) / 255);
					} 
				}
				else if(j < width1 && i < height1)
				{
					_buffer[(i*width+j)*4]   = buffer1[(i*width+j)*4];
					_buffer[(i*width+j)*4+1] = buffer1[(i*width+j)*4+1];
					_buffer[(i*width+j)*4+2] = buffer1[(i*width+j)*4+2];
					_buffer[(i*width+j)*4+3] = buffer1[(i*width+j)*4+3];
				}
			}
		}
	}
	_isCache = true;
	return CreateImageWithRGBA(g,_buffer,width,height);
}

int PineImage::CreateImageFromPNG_URL(Graphic* g,ResourceItem resitem,bool isCacheBuffer,bool optimized, bool isInThread)
{
#if OPTIMIZE_ALL_WHEN_DEVICEWEAK
    if(Device::IsDeviceWeak())
    {
        optimized = true;
    }
#endif
    _loadingState = enumImageLoadingState::IMAGE_LOADING_STATE_BEGIN;
    unsigned error;
   // unsigned char* rgba;
    unsigned int width, height;
    int texture = 0;
	_isCache = isCacheBuffer;    

    error = lodepng_decode32_file(&_buffer, &width, &height, resitem.Path);
	if(error)
    {
        printf("\n\nfile %s error %u: %s\n ", resitem.Path,error, lodepng_error_text(error));
    }
    else
    {
       texture = CreateImageWithRGBA(g,_buffer,width,height,1,8,optimized, isInThread);
    }
	if(!isCacheBuffer)
	{
		delete[] _buffer;
		// free(_buffer);
		 _buffer = NULL;
	}
    _loadingState = enumImageLoadingState::IMAGE_LOADING_STATE_COMPLETE;
    return texture;
}
int PineImage::CreateImageFromPNGinBundle(Graphic* g,const char *file_name,bool IsCache,bool optimized)
{
	ResourceItem resitem = OS_GetResourceItem(file_name);
	int img = CreateImageFromPNG_URL(g, resitem,IsCache,optimized, false);
    return img;
}

int PineImage::CreateImageFromPNGinBundleInSubThread(Graphic* g, const char *file_name, bool IsCache, bool optimized)
{
    ResourceItem resitem = OS_GetResourceItem(file_name);
    int img = CreateImageFromPNG_URL(g, resitem,IsCache,optimized, true);
    return img;
}

int PineImage::CreateImageFromPNGinDocument(Graphic* g, const char*file_image, bool IsCache, bool optimized)
{
#if defined(MAC_OS) || defined(_WINDOWS_PHONE8) || defined(_WIN8) || defined (ANDROID_OS)
	char* sFile = OS_GetDocumentFilePath(file_image);
#else
	const char* sFile = file_image;
#endif

	ResourceItem resitem(sFile, 0, -1);
    int rs = CreateImageFromPNG_URL(g, resitem,IsCache,optimized, false);
#if defined(_WINDOWS_PHONE8) || defined(_WIN8) || defined (ANDROID_OS)
	delete[] sFile;
#endif
	return rs;
}

int PineImage::CreateImageFromPNGinDocumentInSubThread(Graphic* g, const char*file_image, bool IsCache, bool optimized)
{
#if defined(MAC_OS) || defined(_WINDOWS_PHONE8) || defined(_WIN8) || defined (ANDROID_OS)
    char* sFile = OS_GetDocumentFilePath(file_image);
#else
    const char* sFile = file_image;
#endif
    //printf("\nsFile  =%s",sFile);
    ResourceItem resitem(sFile, 0, -1);
    int rs = CreateImageFromPNG_URL(g, resitem,IsCache,optimized, true);
#if defined(_WINDOWS_PHONE8) || defined(_WIN8) || defined (ANDROID_OS)
    delete[] sFile;
#endif
    return rs;
}

void PineImage::SetTextureMaxSize(int width,int height)
{
	TEXTURE_CELL_W = width-2;
	TEXTURE_CELL_H = height-2;
}
PineImage::PineImage()
{
	 TEXTURE_CELL_W = (512-2);
	 TEXTURE_CELL_H = (512-2);
	_width = 0;
	_height = 0;
	_texnames = NULL;
	_cols_texture_num = 0;
	_rows_texutre_num = 0;
	_pixel_type = _EXPORT_PIXEL_32BITS;
	_pixel_format = _IMAGE_FORMAT_RAW_8888;
	_isCache = false;
	_buffer = NULL;
    _loadingState = enumImageLoadingState::IMAGE_LOADING_STATE_NONE;
}
void PineImage::FreeTexture(Graphic* g)
{
	if (_texnames != NULL) {
		for (int i = 0; i < _rows_texutre_num; i++)
		{
			for (int j = 0; j < _cols_texture_num; j++)
			{
				HTEXTURE texname = _texnames[i][j];
				if (texname >= 0)
				{
					g->freeTexture(texname);

#if TEXTURE_MANAGER_MAX
					CGame::_this->DeleteTicket(texname);
#endif
					_texnames[i][j] = NULL;
				}
			}
			delete[] _texnames[i];
			_texnames[i] = NULL;
		}
		delete[] _texnames;
		_texnames = NULL;
	}
}
PineImage::~PineImage()
{
	//FreeTexture();
	if(_isCache)
	{
		_isCache = false;
		if(_buffer != NULL)
		{
			delete[] _buffer;
			_buffer = NULL;
		}
	}

}
/*int PineImage::CreateImageFromScreen(int x, int y, int width, int height,bool isCache)
{
	int texture = -1;
	if(_buffer != NULL)
	{
		delete[] _buffer;
	}

	_buffer = new BYTE[width*height*4];
	
	G()->ReadScreenPixels(x,y,width,height,_buffer);
	texture = CreateImageWithRGBA(_buffer,width,height);
	if(!isCache)
	{
		delete[] _buffer;
		_buffer = NULL;
	}
	return texture;
}*/
HTEXTURE PineImage::CreateSubImageWithRGBA(Graphic* graphic, UINT8* rgba, int x, int y, int width, int height, int scan_width, int scan_height, int pixel_type, int pixel_format, bool optimized, bool isInThread)
{
	BYTE* buffer = NULL;

	PineColorFormat color_format;
	color_format = PineColorFormat::RGBA_1BYTE;
	width= width+2;
    height= height+ 2;
    if(optimized)
    {
        buffer = new BYTE[width*height*2];
		for(int i = 0; i <width*height*2; i++)
		{
			buffer[i] = 0;
		}
		color_format = PineColorFormat::RGBA_4444;
        for(int i =0; i < width && i+x < scan_width; i++)
        {
            for(int j = 0; j < height && j+y < scan_height; j++)
            {
                int r = rgba[((y+j)*scan_width+i+x)*4];
                int g = rgba[((y+j)*scan_width+i+x)*4+1];
                int b = rgba[((y+j)*scan_width+i+x)*4+2];
                int a = rgba[((y+j)*scan_width+i+x)*4+3];
				if (graphic->ColorType() == PineColorType::CTYPE_DIRECTX)
				{
					buffer[((j)*width + i) * 2] = ((g & 0xF0) | ((b >> 4) & 0x0F));
					buffer[((j)*width + i) * 2 + 1] = ((a & 0xF0) | ((r >> 4) & 0x0F));
				}
				else if (graphic->ColorType() == PineColorType::CTYPE_OPENGL)
				{
					buffer[((j)*width + i) * 2] = ((b & 0xF0) | ((a >> 4) & 0x0F));
					buffer[((j)*width + i) * 2 + 1] = ((r & 0xF0) | ((g >> 4) & 0x0F));
				}

            }
        }
        for(int i =0; i < width -2 && i+x < scan_width; i++)
        {
            for(int j = 0; j < height -2 && j+y < scan_height; j++)
            {
                int r = rgba[((y+j)*scan_width+i+x)*4];
                int g = rgba[((y+j)*scan_width+i+x)*4+1];
                int b = rgba[((y+j)*scan_width+i+x)*4+2];
                int a = rgba[((y+j)*scan_width+i+x)*4+3];
				if (graphic->ColorType() == PineColorType::CTYPE_DIRECTX){
					buffer[((j + 1)*width + i + 1) * 2] = ((g & 0xF0) | ((b >> 4) & 0x0F));
					buffer[((j + 1)*width + i + 1) * 2 + 1] = ((a & 0xF0) | ((r >> 4) & 0x0F));
				}
				else if (graphic->ColorType() == PineColorType::CTYPE_OPENGL)
				{
					buffer[((j + 1)*width + i + 1) * 2] = ((b & 0xF0) | ((a >> 4) & 0x0F));
					buffer[((j + 1)*width + i + 1) * 2 + 1] = ((r & 0xF0) | ((g >> 4) & 0x0F));
				}
            }
        }
    }
    else
    {
        buffer = new BYTE[width*height*4];
		for(int i = 0; i <width*height*4; i++)
		{
			buffer[i] = 0;
		}
        for(int i =0; i < width && i+x < scan_width; i++)
        {
            for(int j = 0; j < height && j+y < scan_height; j++)
            {
				buffer[((j)*width + i) * 4] = rgba[((j + y)*scan_width + i + x) * 4];
				buffer[((j)*width + i) * 4 + 1] = rgba[((j + y)*scan_width + i + x) * 4 + 1];
				buffer[((j)*width + i) * 4 + 2] = rgba[((j + y)*scan_width + i + x) * 4 + 2];
				buffer[((j)*width + i) * 4 + 3] = rgba[((j + y)*scan_width + i + x) * 4 + 3];
            }
        }
        for(int i =0; i < width-2 && i+x < scan_width; i++)
        {
            for(int j = 0; j < height-2 && j+y < scan_height; j++)
            {
				buffer[((j + 1)*width + i + 1) * 4] = rgba[((j + y)*scan_width + i + x) * 4];
				buffer[((j + 1)*width + i + 1) * 4 + 1] = rgba[((j + y)*scan_width + i + x) * 4 + 1];
				buffer[((j + 1)*width + i + 1) * 4 + 2] = rgba[((j + y)*scan_width + i + x) * 4 + 2];
				buffer[((j + 1)*width + i + 1) * 4 + 3] = rgba[((j + y)*scan_width + i + x) * 4 + 3];
            }
        }
    }
    HTEXTURE texname = 0;
#if defined(MAC_OS)
    if (isInThread)
    {
        int lengthPointer = sizeof(int*);
        int dataLength = 1 + lengthPointer + 4 + 4 + 4;
        BufferStream bufferStream;
        bufferStream.Init(dataLength);
        
        
        bufferStream.WriteByte(lengthPointer);
        BYTE **temp = NULL;
        temp = &buffer;
        BYTE* pointer = (BYTE*)temp;
        //printf("\n &buffer = %d ", buffer);
        //printf("\n pointer = %d ", pointer);
        for (int i = 0; i < lengthPointer; i++)
        {
            BYTE b = *(pointer+i);
            //printf("\n write b[%d] = %d", i, b);
            bufferStream.WriteByte(b);
        }
        bufferStream.WriteInt32(width);
        bufferStream.WriteInt32(height);
        bufferStream.WriteInt32(color_format);
        
        GAME()->addEvent(EVENT_REQUEST_CREATE_TEXTURE, (const char*)bufferStream.Data(), dataLength);
        
        bool complete = false;
        
        while (!complete)
        {
            EVENT_POINTER evt_pointer = 0; //tao ra mot con tro duyet
            const PineEvent* evt = GAME()->NextEvent(EVENT_RESPONSE_CREATE_TEXTURE, &evt_pointer);
            
            while (evt != NULL)
            {
                //printf("\n searching evt");
                BufferStream bufferStreamResponse;
                bufferStreamResponse.Init(evt->Data->Data(), evt->Data->Length());
                
                bool isCorrectEvent = true;
                for (int i = 0; i < lengthPointer; i++)
                {
                    if (bufferStream.Data()[1 + i] != bufferStreamResponse.Data()[1 + i])
                    {
                        isCorrectEvent = false;
                        break;
                    }
                }
                
                if (isCorrectEvent)
                {
                    bufferStreamResponse.ReadByte();//length
                    for (int i = 0; i < lengthPointer; i++)
                    {
                        bufferStreamResponse.ReadByte();
                    }
                    texname = bufferStreamResponse.ReadInt32();
                    GAME()->CleanEvent(evt);
                    complete = true;
                    break;
                }

                evt = GAME()->NextEvent(EVENT_RESPONSE_CREATE_TEXTURE, &evt_pointer);
            }
            GAME()->Sleep(FPS_DELAY_TIME);
        }
    }
    else
    {
        texname = graphic->createTexture(buffer, width, height, color_format);
    }
#else
	texname = graphic->createTexture(buffer, width, height, color_format);
#endif
	delete[] buffer;
	return texname;
}
int PineImage::CreateImageWithRGBA(Graphic* g, UINT8* rgba, int width, int height, int pixel_type, int pixel_format, bool optimized, bool isInThread)
{

	_width = width;
	_height = height;
	FreeTexture(g);
	_cols_texture_num = width/TEXTURE_CELL_W + (width%TEXTURE_CELL_W > 0 ?1:0);
	_rows_texutre_num = height/TEXTURE_CELL_H + (height%TEXTURE_CELL_H > 0?1:0);
	_texnames = new HTEXTURE*[_rows_texutre_num];
	for(int i = 0; i < _rows_texutre_num; i++)
	{
		_texnames[i] = new HTEXTURE[_cols_texture_num];
		for(int j = 0; j < _cols_texture_num; j++)
		{
			int x = j*TEXTURE_CELL_W;
			int y = i*TEXTURE_CELL_H;
			_texnames[i][j] = CreateSubImageWithRGBA(g, rgba,x,y,TEXTURE_CELL_W,TEXTURE_CELL_H,width,height,pixel_type,pixel_format,optimized, isInThread);
		}
	}
	return true;
}

int PineImage::CreateImageWithMixImage(Graphic* g, const char* mask_file, const char* source_file)
{
    FreeTexture(g);
    unsigned width;
    unsigned height;
    unsigned width1;
    unsigned height1;
    unsigned width2;
    unsigned height2;
    BYTE *buffer1 = NULL;
	BYTE *buffer2 = NULL;
	BYTE *buffer = NULL;
	ResourceItem resitem_mask = OS_GetResourceItem(mask_file);
	ResourceItem resitem_src = OS_GetResourceItem(source_file);
#if defined(_WINDOWS_PHONE8) || defined(ANDROID_OS)|| defined(_WIN8)
	Png* png = new Png();
	if (png->loadPng(resitem_mask, _unzip_buff, _unzip_buff_length, PNG_COLOR_RGBA) == PNG_OK){
		width1 = png->getW();
		height1 = png->getH();
		int bufsize = width1 * height1 * 4;
		buffer1 = new BYTE[bufsize];
		for (int i = 0; i < bufsize; i++)
		{
			buffer1[i] = _unzip_buff[i];
		}
	}
	else{
		delete png;
		PDEBUG("[PINE IMAGE] Reading Error! \n\t filename:%s \n", mask_file);
		return 0;
	}
	if (png->loadPng(resitem_src, _unzip_buff, _unzip_buff_length, PNG_COLOR_RGBA) == PNG_OK){
		width2 = png->getW();
		height2 = png->getH();
		int bufsize = width2 * height2 * 4;
		buffer2 = new BYTE[bufsize];
		for (int i = 0; i < bufsize; i++)
		{
			buffer2[i] = _unzip_buff[i];
		}
	}
	else{
		if (buffer1 != NULL) delete[] buffer1;
		delete png;
		PDEBUG("[PINE IMAGE] Reading Error! \n\t source_file:%s \n", source_file);
		return 0;
	}
	delete png;
#else
    unsigned int error;
    error = lodepng_decode32_file(&buffer1, &width1, &height1, mask_file);
    if(error)
    {
		PDEBUG("\n\nfile %s error %u: %s\n ", mask_file,error, lodepng_error_text(error));
        return 0;
    }
    
    error = lodepng_decode32_file(&buffer2, &width2, &height2, source_file);
    if(error)
    {
		PDEBUG("\n\nfile %s error %u: %s\n ", mask_file,error, lodepng_error_text(error));
        return 0;
    }
#endif
    width = width1;
    height = height1;
    buffer = new BYTE[4*width*height];
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            buffer[(i*width+j)*4]   = 0;
            buffer[(i*width+j)*4+1] = 0;
            buffer[(i*width+j)*4+2] = 0;
            buffer[(i*width+j)*4+3] = 0;
            if(j < width1 && j < width2 && i < height1 && i < height2)
            {
                int a = buffer1[(i*width+j)*4];
                int r = buffer1[(i*width+j)*4+1];
                int g = buffer1[(i*width+j)*4+2];
                int b = buffer1[(i*width+j)*4+3];
                if((a == 255 && r == 255 && g == 255))
                {
                    buffer[(i*width+j)*4]   = buffer2[(i*width+j)*4];
                    buffer[(i*width+j)*4+1] = buffer2[(i*width+j)*4+1];
                    buffer[(i*width+j)*4+2] = buffer2[(i*width+j)*4+2];
                    buffer[(i*width+j)*4+3] = buffer2[(i*width+j)*4+3];
                }
				else if(a != 0 && r != 0 && g != 0)
				{
					//printf("\n a = %d", buffer1[(i*width+j)*4]);
					//printf(" r = %d", buffer1[(i*width+j)*4 + 1]);
					//printf(" g = %d", buffer1[(i*width+j)*4 + 2]);
					//printf(" b = %d", buffer1[(i*width+j)*4 + 3]);

					int finR = ((float)(buffer1[(i*width+j)*4+0] * 100 / 255) / 100) * buffer2[(i*width+j)*4+0];
					int finG = ((float)(buffer1[(i*width+j)*4+1] * 100 / 255) / 100) * buffer2[(i*width+j)*4+1];
					int finB = ((float)(buffer1[(i*width+j)*4+2] * 100 / 255) / 100) * buffer2[(i*width+j)*4+2];

					buffer[(i*width+j)*4]   = finR;
                    buffer[(i*width+j)*4+1] = finG;
                    buffer[(i*width+j)*4+2] = finB;
                    buffer[(i*width+j)*4+3] = 255;
				}

            }
        }
    }
    int texture = CreateImageWithRGBA(g, buffer,width,height,_EXPORT_PIXEL_32BITS,_IMAGE_FORMAT_RAW_8888,false);
    delete[] buffer;
    delete[] buffer1;
    delete[] buffer2;
    return texture;
}

void PineImage::DrawImageWithRect(Graphic* g, float posX, float posY, int transform)
{
	DrawImageWithRect(g,posX,posY,0,0,_width,_height,transform);
}
void PineImage::DrawImageWithRect(Graphic* g, float posX, float posY)
{
	DrawImageWithRect(g,posX,posY,0);
}
void PineImage::DrawImageWithRect(Graphic* g, float posX, float posY, float x, float y, int width, int height, int transform)
{
	for(int i = 0; i < _rows_texutre_num; i++)
	{
		for(int j = 0; j < _cols_texture_num; j++)
		{
			g->CleanCurrentFlag();
			HTEXTURE texname = _texnames[i][j];
			INT32 w = j*TEXTURE_CELL_W > width ? (width-(j-1)*TEXTURE_CELL_W):TEXTURE_CELL_W;
			INT32 h = i*TEXTURE_CELL_H > height? (height-(i-1)*TEXTURE_CELL_H):TEXTURE_CELL_H;
            float xx = posX + j*TEXTURE_CELL_W ;
            float yy = posY + i*TEXTURE_CELL_H;
			/*
            switch (GAME()->_typeOtimizeDrawModule)
            {
                case 0: //normal optimize
                {
					float scalex = G()->GetCurrentScaleX();// > 1.0f ? G()->GetCurrentScaleX() : 1.0f;
					float scaley = G()->GetCurrentScaleY();// > 1.0f ? G()->GetCurrentScaleY() : 1.0f;
					if (scalex > 1.0f && scaley > 1.0f)
					{
						float ww = CGame::_this->_device.ScreenSize.W > k_SCREEN_WIDTH ? CGame::_this->_device.ScreenSize.W : k_SCREEN_WIDTH;
						float hh = CGame::_this->_device.ScreenSize.H > k_SCREEN_HEIGHT ? CGame::_this->_device.ScreenSize.H : k_SCREEN_HEIGHT;

						if (xx + w*scalex < 0 || xx > ww || yy + h*scaley < 0 || yy > hh)
						{
							//printf("\nPasssed");
							continue;
						}
					}
                }
                    break;
                case 1: //none optimize
                    break;
                case 2:
                    break;
            }
			*/
			//printf("\nw = %d, h= %d",w,h);
			_uv[0] = x+1;
            _uv[2] = x;
			_uv[1] = y+1;
            _uv[3] = y;

			if ((transform & _FLIP_H) != 0)
			{
				_uv[0] += w;
			}
			else
			{
				_uv[2] += w;
			}

			if ((transform & _FLIP_V) != 0)
			{
				_uv[1] += h;
			}
			else
			{
				_uv[3] += h;
			}
			
			_uv[0] *= 1.0f / (TEXTURE_CELL_W+2);
			_uv[1] *= 1.0f / (TEXTURE_CELL_H+2);
			_uv[2] *= 1.0f / (TEXTURE_CELL_W+2);
			_uv[3] *= 1.0f / (TEXTURE_CELL_H+2);

			g->Draw2DTexture(posX+j*TEXTURE_CELL_W, posY+TEXTURE_CELL_H*i, w, h, texname, _uv, 0, true);

		}
	}
	
}

BYTE* PineImage::GetBufferImage()
{
	if(!_isCache)
	{
		return NULL;
	}
	return _buffer;
}
void PineImage::FreeBufferImage()
{
	if(_isCache)
	{
		_isCache = false;
		if(_buffer != NULL)
		{
			delete[] _buffer;
			_buffer = NULL;
		}
	}
}

 void PineImage::CannyEdgeDetect(BYTE* buffer,int width, int height)
{
	unsigned int W, H;		// Width and Height of current frame [pixels]
	unsigned int row, col;		// Pixel's row and col positions
	unsigned long i;		// Dummy variable for row-column vector
	int upperThreshold = 60;	// Gradient strength nessicary to start edge
	int lowerThreshold = 30;	// Minimum gradient strength to continue edge
	unsigned long iOffset;		// Variable to offset row-column vector during sobel mask
	int rowOffset;			// Row offset from the current pixel
	int colOffset;			// Col offset from the current pixel
	int rowTotal = 0;		// Row position of offset pixel
	int colTotal = 0;		// Col position of offset pixel
	int Gx;				// Sum of Sobel mask products values in the x direction
	int Gy;				// Sum of Sobel mask products values in the y direction
	float thisAngle;		// Gradient direction based on Gx and Gy
	int newAngle;			// Approximation of the gradient direction
	bool edgeEnd;			// Stores whether or not the edge is at the edge of the possible image
	int GxMask[3][3];		// Sobel mask in the x direction
	int GyMask[3][3];		// Sobel mask in the y direction
	int newPixel;			// Sum pixel values for gaussian
	int gaussianMask[5][5];		// Gaussian mask

	W = width;  // biWidth: number of columns
   	H = height; // biHeight: number of rows
	int** edgeDir = new int*[H];
	int** gradient = new int*[H];
	BYTE* m_destinationBmp  = new BYTE[W*H*3]; 
	for(int i = 0; i < W*H; i++)
	{
		
		int red		= buffer[i*4];
		int green	= buffer[i*4+1];
		int blue	= buffer[i*4+2];
		int grayValue = (BYTE)(0.299*red + 0.587*green + 0.114*blue);
		m_destinationBmp[i*3]	= grayValue;
		m_destinationBmp[i*3+1] = grayValue;
		m_destinationBmp[i*3+2] = grayValue;
		
		/*
		m_destinationBmp[i*3]	= buffer[i*4];
		m_destinationBmp[i*3+1] = buffer[i*4+1];
		m_destinationBmp[i*3+2] = buffer[i*4+2];
		*/

	}
	for (row = 0; row < H; row++) {
		edgeDir[row] = new int[W];
		gradient[row] = new int[W];
		for (col = 0; col < W; col++) {
			edgeDir[row][col] = 0;
		}
	}

	/* Declare Sobel masks */
	GxMask[0][0] = -1; GxMask[0][1] = 0; GxMask[0][2] = 1;
	GxMask[1][0] = -2; GxMask[1][1] = 0; GxMask[1][2] = 2;
	GxMask[2][0] = -1; GxMask[2][1] = 0; GxMask[2][2] = 1;
	
	GyMask[0][0] =  1; GyMask[0][1] =  2; GyMask[0][2] =  1;
	GyMask[1][0] =  0; GyMask[1][1] =  0; GyMask[1][2] =  0;
	GyMask[2][0] = -1; GyMask[2][1] = -2; GyMask[2][2] = -1;

	/* Declare Gaussian mask */
	gaussianMask[0][0] = 2;	 gaussianMask[0][1] = 4;  gaussianMask[0][2] = 5;  gaussianMask[0][3] = 4;  gaussianMask[0][4] = 2;	
	gaussianMask[1][0] = 4;	 gaussianMask[1][1] = 9;  gaussianMask[1][2] = 12; gaussianMask[1][3] = 9;  gaussianMask[1][4] = 4;	
	gaussianMask[2][0] = 5;	 gaussianMask[2][1] = 12; gaussianMask[2][2] = 15; gaussianMask[2][3] = 12; gaussianMask[2][4] = 2;	
	gaussianMask[3][0] = 4;	 gaussianMask[3][1] = 9;  gaussianMask[3][2] = 12; gaussianMask[3][3] = 9;  gaussianMask[3][4] = 4;	
	gaussianMask[4][0] = 2;	 gaussianMask[4][1] = 4;  gaussianMask[4][2] = 5;  gaussianMask[4][3] = 4;  gaussianMask[4][4] = 2;	
	

	/* Gaussian Blur */
	for (row = 2; row < H-2; row++) {
		for (col = 2; col < W-2; col++) {
			newPixel = 0;
			for (rowOffset=-2; rowOffset<=2; rowOffset++) {
				for (colOffset=-2; colOffset<=2; colOffset++) {
					rowTotal = row + rowOffset;
					colTotal = col + colOffset;
					iOffset = (unsigned long)(rowTotal*3*W + colTotal*3);
					newPixel += (*(m_destinationBmp + iOffset)) * gaussianMask[2 + rowOffset][2 + colOffset];
				}
			}
			i = (unsigned long)(row*3*W + col*3);
			*(m_destinationBmp + i) = newPixel / 159;
		}
	}


	/* Determine edge directions and gradient strengths */
	for (row = 1; row < H-1; row++) {
		for (col = 1; col < W-1; col++) {
			i = (unsigned long)(row*3*W + 3*col);
			Gx = 0;
			Gy = 0;
			/* Calculate the sum of the Sobel mask times the nine surrounding pixels in the x and y direction */
			for (rowOffset=-1; rowOffset<=1; rowOffset++) {
				for (colOffset=-1; colOffset<=1; colOffset++) {
					rowTotal = row + rowOffset;
					colTotal = col + colOffset;
					iOffset = (unsigned long)(rowTotal*3*W + colTotal*3);
					Gx = Gx + (*(m_destinationBmp + iOffset) * GxMask[rowOffset + 1][colOffset + 1]);
					Gy = Gy + (*(m_destinationBmp + iOffset) * GyMask[rowOffset + 1][colOffset + 1]);
				}
			}

			gradient[row][col] = sqrt(pow(Gx,2.0) + pow(Gy,2.0));	// Calculate gradient strength			
			thisAngle = (atan2((long double)Gx,(long double)Gy)/3.14159) * 180.0;		// Calculate actual direction of edge
			
			/* Convert actual edge direction to approximate value */
			if ( ( (thisAngle < 22.5) && (thisAngle > -22.5) ) || (thisAngle > 157.5) || (thisAngle < -157.5) )
				newAngle = 0;
			if ( ( (thisAngle > 22.5) && (thisAngle < 67.5) ) || ( (thisAngle < -112.5) && (thisAngle > -157.5) ) )
				newAngle = 45;
			if ( ( (thisAngle > 67.5) && (thisAngle < 112.5) ) || ( (thisAngle < -67.5) && (thisAngle > -112.5) ) )
				newAngle = 90;
			if ( ( (thisAngle > 112.5) && (thisAngle < 157.5) ) || ( (thisAngle < -22.5) && (thisAngle > -67.5) ) )
				newAngle = 135;
				
			edgeDir[row][col] = newAngle;		// Store the approximate edge direction of each pixel in one array
		}
	}
	/* Trace along all the edges in the image */
	for (row = 1; row < H - 1; row++) {
		for (col = 1; col < W - 1; col++) {
			edgeEnd = false;
			if (gradient[row][col] > upperThreshold) {		// Check to see if current pixel has a high enough gradient strength to be part of an edge
				/* Switch based on current pixel's edge direction */
				switch (edgeDir[row][col]){		
					case 0:
						findEdge(m_destinationBmp,edgeDir,gradient,0, 1, row, col, 0, lowerThreshold,width,height);
						break;
					case 45:
						findEdge(m_destinationBmp,edgeDir,gradient,1, 1, row, col, 45, lowerThreshold,width,height);
						break;
					case 90:
						findEdge(m_destinationBmp,edgeDir,gradient,1, 0, row, col, 90, lowerThreshold,width,height);
						break;
					case 135:
						findEdge(m_destinationBmp,edgeDir,gradient,1, -1, row, col, 135, lowerThreshold,width,height);
						break;
					default :
						i = (unsigned long)(row*3*W + 3*col);
						*(m_destinationBmp + i) = 
						*(m_destinationBmp + i + 1) = 
						*(m_destinationBmp + i + 2) = 0;
						break;
					}
				}
			else {
				i = (unsigned long)(row*3*W + 3*col);
					*(m_destinationBmp + i) = 
					*(m_destinationBmp + i + 1) = 
					*(m_destinationBmp + i + 2) = 0;
			}	
		}
	}

	/* Non-maximum Suppression */
	for (row = 1; row < H - 1; row++) {
		for (col = 1; col < W - 1; col++) {
			i = (unsigned long)(row*3*W + 3*col);
			if (*(m_destinationBmp + i) == 255) {		// Check to see if current pixel is an edge
				/* Switch based on current pixel's edge direction */
				switch (edgeDir[row][col]) {		
					case 0:
						suppressNonMax(m_destinationBmp,edgeDir,gradient, 1, 0, row, col, 0, lowerThreshold, width,height);
						break;
					case 45:
						suppressNonMax(m_destinationBmp,edgeDir,gradient, 1, -1, row, col, 45, lowerThreshold, width,height);
						break;
					case 90:
						suppressNonMax(m_destinationBmp,edgeDir,gradient, 0, 1, row, col, 90, lowerThreshold, width,height);
						break;
					case 135:
						suppressNonMax(m_destinationBmp,edgeDir,gradient, 1, 1, row, col, 135, lowerThreshold, width,height);
						break;
					default :
						break;
				}
			}	
		}
	}

	/* Suppress any pixels not changed by the edge tracing */
	for (row = 0; row < H; row++) {
		for (col = 0; col < W; col++) {	
			// Recall each pixel is composed of 3 bytes
			i = (unsigned long)(row*3*W + 3*col);
			// If a pixel's grayValue is not black or white make it black
			if( ((*(m_destinationBmp + i) != 255) && (*(m_destinationBmp + i) != 0)) || ((*(m_destinationBmp + i + 1) != 255) && (*(m_destinationBmp + i + 1) != 0)) || ((*(m_destinationBmp + i + 2) != 255) && (*(m_destinationBmp + i + 2) != 0)) ) 
				*(m_destinationBmp + i) = 
				*(m_destinationBmp + i + 1) = 
				*(m_destinationBmp + i + 2) = 0; // Make pixel black
		}
	}

	

	for(int i = 0; i < W*H; i++)
	{
		buffer[i*4] = m_destinationBmp[i*3];
		buffer[i*4+1] = m_destinationBmp[i*3+1];
		buffer[i*4+2] = m_destinationBmp[i*3+2];
		buffer[i*4+3] = 0xff;
	}
	SAFE_DEL(m_destinationBmp);
	SAFE_DEL(edgeDir);
	SAFE_DEL(gradient);
}


 void PineImage::findEdge(BYTE* m_destinationBmp,int** edgeDir,int** gradient, int rowShift, int colShift, int row, int col, int dir, int lowerThreshold,int width,int height)
{
	int W = width;
	int H = height;
	int newRow;
	int newCol;
	unsigned long i;
	bool edgeEnd = false;

	/* Find the row and column values for the next possible pixel on the edge */
	if (colShift < 0) {
		if (col > 0)
			newCol = col + colShift;
		else
			edgeEnd = true;
	} else if (col < W - 1) {
		newCol = col + colShift;
	} else
		edgeEnd = true;		// If the next pixel would be off image, don't do the while loop
	if (rowShift < 0) {
		if (row > 0)
			newRow = row + rowShift;
		else
			edgeEnd = true;
	} else if (row < H - 1) {
		newRow = row + rowShift;
	} else
		edgeEnd = true;	
		
	/* Determine edge directions and gradient strengths */
	while ( (edgeDir[newRow][newCol]==dir) && !edgeEnd && (gradient[newRow][newCol] > lowerThreshold) ) {
		/* Set the new pixel as white to show it is an edge */
		i = (unsigned long)(newRow*3*W + 3*newCol);
		*(m_destinationBmp + i) =
		*(m_destinationBmp + i + 1) =
		*(m_destinationBmp + i + 2) = 255;
		if (colShift < 0) {
			if (newCol > 0)
				newCol = newCol + colShift;
			else
				edgeEnd = true;	
		} else if (newCol < W - 1) {
			newCol = newCol + colShift;
		} else
			edgeEnd = true;	
		if (rowShift < 0) {
			if (newRow > 0)
				newRow = newRow + rowShift;
			else
				edgeEnd = true;
		} else if (newRow < H - 1) {
			newRow = newRow + rowShift;
		} else
			edgeEnd = true;	
	}	
}

 void PineImage::suppressNonMax(BYTE* m_destinationBmp,int** edgeDir,int** gradient,int rowShift, int colShift, int row, int col, int dir, int lowerThreshold,int width,int height)
{
	int W = width;
	int H = height;
	int newRow = 0;
	int newCol = 0;
	unsigned long i;
	bool edgeEnd = false;
	float** nonMax = new float*[width];			// Temporarily stores gradients and positions of pixels in parallel edges
	for(int i = 0; i < width; i++)
	{
		nonMax[i] =new float[3];
	}
	int pixelCount = 0;					// Stores the number of pixels in parallel edges
	int count;						// A for loop counter
	int max[3];						// Maximum point in a wide edge
	
	if (colShift < 0) {
		if (col > 0)
			newCol = col + colShift;
		else
			edgeEnd = true;
	} else if (col < W - 1) {
		newCol = col + colShift;
	} else
		edgeEnd = true;		// If the next pixel would be off image, don't do the while loop
	if (rowShift < 0) {
		if (row > 0)
			newRow = row + rowShift;
		else
			edgeEnd = true;
	} else if (row < H - 1) {
		newRow = row + rowShift;
	} else
		edgeEnd = true;	
	i = (unsigned long)(newRow*3*W + 3*newCol);
	/* Find non-maximum parallel edges tracing up */
	while ((edgeDir[newRow][newCol] == dir) && !edgeEnd && (*(m_destinationBmp + i) == 255)) {
		if (colShift < 0) {
			if (newCol > 0)
				newCol = newCol + colShift;
			else
				edgeEnd = true;	
		} else if (newCol < W - 1) {
			newCol = newCol + colShift;
		} else
			edgeEnd = true;	
		if (rowShift < 0) {
			if (newRow > 0)
				newRow = newRow + rowShift;
			else
				edgeEnd = true;
		} else if (newRow < H - 1) {
			newRow = newRow + rowShift;
		} else
			edgeEnd = true;	
		nonMax[pixelCount][0] = newRow;
		nonMax[pixelCount][1] = newCol;
		nonMax[pixelCount][2] = gradient[newRow][newCol];
		pixelCount++;
		i = (unsigned long)(newRow*3*W + 3*newCol);
	}

	/* Find non-maximum parallel edges tracing down */
	edgeEnd = false;
	colShift *= -1;
	rowShift *= -1;
	if (colShift < 0) {
		if (col > 0)
			newCol = col + colShift;
		else
			edgeEnd = true;
	} else if (col < W - 1) {
		newCol = col + colShift;
	} else
		edgeEnd = true;	
	if (rowShift < 0) {
		if (row > 0)
			newRow = row + rowShift;
		else
			edgeEnd = true;
	} else if (row < H - 1) {
		newRow = row + rowShift;
	} else
		edgeEnd = true;	
	i = (unsigned long)(newRow*3*W + 3*newCol);
	while ((edgeDir[newRow][newCol] == dir) && !edgeEnd && (*(m_destinationBmp + i) == 255)) {
		if (colShift < 0) {
			if (newCol > 0)
				newCol = newCol + colShift;
			else
				edgeEnd = true;	
		} else if (newCol < W - 1) {
			newCol = newCol + colShift;
		} else
			edgeEnd = true;	
		if (rowShift < 0) {
			if (newRow > 0)
				newRow = newRow + rowShift;
			else
				edgeEnd = true;
		} else if (newRow < H - 1) {
			newRow = newRow + rowShift;
		} else
			edgeEnd = true;	
		nonMax[pixelCount][0] = newRow;
		nonMax[pixelCount][1] = newCol;
		nonMax[pixelCount][2] = gradient[newRow][newCol];
		pixelCount++;
		i = (unsigned long)(newRow*3*W + 3*newCol);
	}

	/* Suppress non-maximum edges */
	max[0] = 0;
	max[1] = 0;
	max[2] = 0;
	for (count = 0; count < pixelCount; count++) {
		if (nonMax[count][2] > max[2]) {
			max[0] = nonMax[count][0];
			max[1] = nonMax[count][1];
			max[2] = nonMax[count][2];
		}
	}
	for (count = 0; count < pixelCount; count++) {
		i = (unsigned long)(nonMax[count][0]*3*W + 3*nonMax[count][1]);
		*(m_destinationBmp + i) = 
		*(m_destinationBmp + i + 1) = 
		*(m_destinationBmp + i + 2) = 0;
	}
}

bool PineImage::IsFree()
{
    bool free = false;
    
    if(_texnames == NULL)
    {
        free = true;
    }
    else if(_texnames[0][0] == NULL)
    {
        free = true;
    }
    return free;
}

//#endif
