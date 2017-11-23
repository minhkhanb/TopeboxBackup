 //#ifndef MAC_OS
#include "pch.h"
#include <stdio.h>
#include <string.h>
//#endif
#include "CSprite.h"

#include "../../GameCore/CGame.h"

#if defined(_USE_TEST)
#include "../Debug/Test.h"
#endif

#if defined(MAC_OS) || defined(ANDROID_OS)
#include <stdarg.h>
#endif

//INT32	CSprite::_line_spacing = 3;
UINT8* CSprite::_unzip_buff = NULL;
UINT32	CSprite::_unzip_buff_length = 0;
//float*	CSprite::_uv = new float[4];
//UNICHAR* CSprite::_uni_buffer = new UNICHAR[102400];
//char* CSprite::_ansi_buffer = new char[102400];
 UINT8* CSprite::_rle_ind_buffer = new UINT8[1000*1024*2];
 UINT8* CSprite::_rle_alp_buffer = new UINT8[1000*1024*2];
 UINT8* CSprite::_ind_buffer = new UINT8[1000*1024*2];
UINT8* CSprite::_alp_buffer = new UINT8[1000*1024*2];

#if defined(_DEBUG)
UINT32 _debug_sprite_count = 0;
#endif

CSprite::CSprite()
{
	_fixed_clip_bug = false;
	_word_spacing = 0;
	_char_spacing=0;
	_line_spacing = 3;
	_num_texture_images = 0;
	_backup_pos = NULL;
	_image_format = NULL;
	_backup_pallete = 0;
	_backup_antialias = false;

	_current_pal = 0;
	_num_modules = 0;
	_texture_image = NULL;
	_texture_image_data = NULL;
	_module_img_id = NULL;
	_module_x = NULL;
	_module_y = NULL;
	_module_w = NULL;
	_module_h = NULL;
	_module_type = NULL;
	_module_color = NULL;
	_module_thickness = NULL;
	_module_ext_param1 = NULL;
	_num_frames = 0;
	_frame_offset = NULL;
	_num_fmodules = 0;
	_fmodule_id = NULL;
	_fmodule_x = NULL;
	_fmodule_y = NULL;
	_fmodule_transf = NULL;
	_fmodule_ox = NULL;
	_fmodule_oy = NULL;
	_fmodule_m11 = NULL;
	_fmodule_m12 = NULL;
	_fmodule_m21 = NULL;
	_fmodule_m22 = NULL;
	_num_anims = 0;
	_num_aframes = 0;
	_is_current_anim_stop = true;
	_current_anim = -1;
	_use_true_color_texture = true;
	_use_palette_with_optimize_texture = false;
	_has_alpha = false;
	_texture_image = NULL;
	_palette_table_loaded = false;

    _module_replay_data = NULL;
    
	for(int i = 0; i < _NUM_PAL_MAX; i++)
	{
		_pal_loaded[i] = false;
	}
#ifdef USE_COLOR_MASK
	ResetTextColorMask();
#endif
	_texture_mask = NULL;
	ClearReplace();
	_fontSpaceWidth = 0;

	_texture_scale = 1.0f;
	_texture_texcoord_delta = 0;
	
	_has_texture = false;
}

CSprite::~CSprite()
{	
	for(int i = 0; i < _NUM_PAL_MAX; i++)
	{
		_pal_loaded[i] = false;
	}
}

void CSprite::LoadFromRGBA(Graphic* g,DataInputStream* is,BYTE* rgba,int width,int height, bool is_create_texture , bool linear_texture, INT8 palette_mask, bool load_trueColor_texture,int withPalette)
{	
	_backup_pallete = palette_mask;
	_backup_antialias = linear_texture;
	if(withPalette>=0)
	{
		_pal_loaded[withPalette] = true;
	}
	_use_true_color_texture = load_trueColor_texture;
	// read flag;
	_flag = is->ReadInt8();
	_fixed_clip_bug = false;
	if ( (_flag&_FLAG_CLIP_BUG_FIXED) != 0)
	{
		_fixed_clip_bug = true;
	}
	//read export info
	_export_quality = is->ReadInt8();
	_export_image_type = is->ReadInt8();
	_export_pixel_type = is->ReadInt8();
	
	//palette reading
	_image_nums = is->ReadInt8();
	//PDEBUG("img num:%d\n", _image_nums);
	if(!_palette_table_loaded)
	{
		_palette_image_num = new int[_image_nums];
		_palette_table =  new UINT8**[_image_nums];
	}
	//printf("\n_image_nums = %d",_image_nums);
	bool has_palette = false;
	for(int i = 0; i < _image_nums; i++)
	{
		_palette_image_num[i] = is->ReadInt8();
		if(!_palette_table_loaded)
		{
			_palette_table[i] = new UINT8*[_palette_image_num[i]];
		}
		//printf("\n_image %d num palette =%d\n",i,_palette_image_num[i]);
		for(int j = 0; j< _palette_image_num[i]; j++)
		{
			if(!_palette_table_loaded)
			{
				_palette_table[i][j] = new UINT8[256*3+4];
			}
			has_palette = true;
			for(int k = 0; k < 256; k++)
			{

				_palette_table[i][j][k*3] = is->ReadInt8();
				_palette_table[i][j][k*3+1] = is->ReadInt8();
				_palette_table[i][j][k*3+2] = is->ReadInt8();
				//printf("%d, %d, %d\t",_palette_table[i][j][k*3],_palette_table[i][j][k*3+1],_palette_table[i][j][k*3+2]);
			}
			is->ReadInt32();
		}
	}
	if(!_use_true_color_texture && has_palette)
	{
		_use_true_color_texture = true;
		_use_palette_with_optimize_texture = true;
	}
	_palette_table_loaded = true;
	//read num clip
	_num_modules  = is->ReadInt16();
	//PDEBUG("_num_modules:%d\n", _num_modules);
	if (_export_image_type == _EXPORT_TYPE_3D_TEXTURE)
	{
		_module_x = new INT16[_num_modules];
		_module_y = new INT16[_num_modules];
		_module_img_id = new BYTE[_num_modules];

		_module_replay_data = new INT16[_num_modules*3];
	}
#ifdef USE_MULTI_MODULE_TYPE
	_module_type = new BYTE[_num_modules];
	_module_color =  new UINT32[_num_modules];
#endif
	_module_w = new INT16[_num_modules];
	_module_h = new INT16[_num_modules];
	
	//read clip info
	for (INT32 i = 0; i < _num_modules; i++)
	{
	#ifdef USE_MULTI_MODULE_TYPE
		if ( (_flag&_FLAG_MULTI_MODULE_TYPE) != 0)
		{
			_module_type[i] = is->ReadInt8();
		}
	#endif
		
		_module_w[i] = is->ReadInt16();
		_module_h[i] = is->ReadInt16();
	#ifdef USE_MULTI_MODULE_TYPE
		if (_module_type[i] != 0)
		{
			_module_color[i] = is->ReadInt32();
			is->ReadInt8();
			is->ReadInt16();
		}
		else
	#endif
		if (_export_image_type == _EXPORT_TYPE_3D_TEXTURE)
		{
			if ((_module_w[i]&0xFFFF) > 0 && (_module_h[i]&0xFFFF) > 0)
			{
				_module_x[i] = is->ReadInt16();
				_module_y[i] = is->ReadInt16();
				
				_module_img_id[i] = is->ReadInt8();
			}
		}
	}
	
	//num frame
	_num_frames = is->ReadInt16();
	//PDEBUG("_num_frames:%d\n", _num_frames);
	if (_num_frames > 0)
	{
		//anim pos in aframe list
		_frame_offset = new INT32[_num_frames];
		_num_fmodules = 0;
		for (INT32 i = 0; i < _num_frames; i++)
		{
			INT32 fmodules = is->ReadInt16();
			
			_frame_offset[i] = (INT32)_num_fmodules;
			
			_num_fmodules += fmodules;
		}
		
		_fmodule_id = new INT16[_num_fmodules];
		_fmodule_x = new INT16[_num_fmodules];
		_fmodule_y = new INT16[_num_fmodules];
		_fmodule_transf = new BYTE[_num_fmodules];

#ifdef USE_FM_OPACITY
		_fmodule_opacity = new BYTE[_num_fmodules];
#endif
		
		
		for (INT32 i = 0; i < _num_fmodules; i++)
		{
			_fmodule_id[i] = (INT16)(is->ReadInt16());
			
			_fmodule_transf[i] = is->ReadInt8();
			if ((_fmodule_transf[i] & _FREE_TRANSFORM) != 0)
			{
			#ifdef USE_FREE_TRANSFORM
				if (!_fmodule_ox)
				{
					_fmodule_ox = new float[_num_fmodules];
					_fmodule_oy = new float[_num_fmodules];
					_fmodule_m11 = new float[_num_fmodules];
					_fmodule_m12 = new float[_num_fmodules];
					_fmodule_m21 = new float[_num_fmodules];
					_fmodule_m22 = new float[_num_fmodules];
				}
				_fmodule_ox[i] = is->ReadFloat32();
				_fmodule_oy[i] = is->ReadFloat32();
				_fmodule_m11[i] = is->ReadFloat32();
				_fmodule_m12[i] = is->ReadFloat32();
				_fmodule_m21[i] = is->ReadFloat32();
				_fmodule_m22[i] = is->ReadFloat32();
				_fmodule_x[i] = 0;		
				_fmodule_y[i] = 0;
			#else
				_fmodule_x[i] = (INT16) is->ReadFloat32();		
				_fmodule_y[i] = (INT16) is->ReadFloat32();
				 is->ReadInt32();
				 is->ReadInt32();
				 is->ReadInt32();
				 is->ReadInt32();
			#endif

			}
			else
			{
				_fmodule_x[i] = (INT16)(is->ReadInt16());
				_fmodule_y[i] = (INT16)(is->ReadInt16());
			}
	
#ifdef USE_FM_OPACITY
			if ( (_flag&_FLAG_EXPORT_FM_OPACITY) != 0)
			{
				_fmodule_opacity[i] = is->ReadInt8();
			}
			else
				_fmodule_opacity[i] = 100;
#else
			if ( (_flag&_FLAG_EXPORT_FM_OPACITY) != 0)
				is->ReadInt8();
#endif			

		}
	}
	
	//num anim
	_num_anims = is->ReadInt16();
	//PDEBUG("_num_anims:%d\n", _num_anims);
	if (_num_anims > 0)
	{
		//anim pos in aframe list
		_anim_offset = new INT16[_num_anims];
		_num_aframes = 0;
		for (INT32 i = 0; i < _num_anims; i++)
		{
			INT32 aframes = is->ReadInt16();
			
			_anim_offset[i] = (INT16)_num_aframes;
			
			_num_aframes += aframes;
		}
		
		//aframe list
		_aframe_id = new INT16[_num_aframes];
		_aframe_x = new INT16[_num_aframes];
		_aframe_y = new INT16[_num_aframes];
		_aframe_transf = new BYTE[_num_aframes];
		_aframe_time = new BYTE[_num_aframes];
		for (INT32 i = 0; i < _num_aframes; i++)
		{
			_aframe_id[i] = (INT16)(is->ReadInt16());
			
			_aframe_x[i] = (INT16)(is->ReadInt16());
			
			_aframe_y[i] = (INT16)(is->ReadInt16());
		
			_aframe_transf[i] = is->ReadInt8();
			_aframe_time[i] = is->ReadInt8();
		}
	}
	

	{
		//texture image
		_num_texture_images = is->ReadInt8();
		_image_format = new BYTE[_num_texture_images];
		_texture_image_w = new INT32[_num_texture_images];
		_texture_image_h = new INT32[_num_texture_images];

		if (_palette_image_num[0] >= 1)
			_num_pal = _palette_image_num[0];
		else
			_num_pal = 1;
		if(_texture_image == NULL)
		{
			_texture_image = new HTEXTURE*[_num_pal];
		}
		
		if (is_create_texture)
		{
			_texture_image[withPalette] = new HTEXTURE[_num_texture_images];
		}

		_texture_image_data = new BYTE*[_num_texture_images];
		for (int k = 0; k < _num_texture_images; k++)
		{
			_texture_image_data[k] = NULL;
		}
		
		for (int tex_id = 0; tex_id < _num_texture_images; tex_id++)
		{
			_image_format[tex_id] = is->ReadInt8();
					
			if (_image_format[tex_id] == _IMAGE_FORMAT_RAW_8888 || 
				_image_format[tex_id] == _IMAGE_FORMAT_RAW_0888 || 
				_image_format[tex_id] == _IMAGE_FORMAT_RAW_1888)
			{
				if (_image_format[tex_id] == _IMAGE_FORMAT_RAW_8888 || _image_format[tex_id] == _IMAGE_FORMAT_RAW_1888)
				{
					_has_alpha = true;
				}
							
				//read image width
				_texture_image_w[tex_id] = width;
				
				//read image height
				_texture_image_h[tex_id] = height;
				
				//printf("\nis_create_texture = %d",is_create_texture);
				if (is_create_texture)
				{
					//printf("Cache Texture with palette %d",withPalette);
					_texture_image[withPalette][tex_id] = CreateTextureImage(g,tex_id, rgba, _texture_image_w[tex_id], _texture_image_h[tex_id], linear_texture, withPalette,withPalette);
					
				}
				else
				{
					CacheTextureData(tex_id);
				}
			}
		}
	}

	ClearReplace();
}

void CSprite::Load(Graphic* g, DataInputStream* is, bool is_create_texture, int linear_texture, INT8 palette_mask, bool load_trueColor_texture, int withPalette, bool isInThread)
{
#if OPTIMIZE_ALL_WHEN_DEVICEWEAK
    if(Device::IsDeviceWeak())
    {
        load_trueColor_texture = false;
    }
#endif
	_backup_pallete = palette_mask;
	_backup_antialias = linear_texture;

	ENGINE_DEBUG_SPRITE("[SPRITE] load");
	_fontHeight = 0;
	_fontSpaceWidth = 0;
	if(withPalette>=0)
	{
		_pal_loaded[withPalette] = true;
	}
	_use_true_color_texture = load_trueColor_texture;
	// read flag;
	_flag = is->ReadInt8();
	_fixed_clip_bug = false;
	if ( (_flag&_FLAG_CLIP_BUG_FIXED) != 0)
	{
		//printf("\nclip bug fixed clip bug fixed clip bug fixed");
		_fixed_clip_bug = true;
	}
	//read export info
	_export_quality = is->ReadInt8();
	_export_image_type = is->ReadInt8();
	_export_pixel_type = is->ReadInt8();
	
	//palette reading
	_image_nums = is->ReadInt8();
	ENGINE_DEBUG_SPRITE("[SPRITE] [_image_nums:%d]\n", _image_nums);
	if(!_palette_table_loaded)
	{
		_palette_image_num = new int[_image_nums];
		_palette_table =  new UINT8**[_image_nums];
	}
	//TEST_LOG("\n_image_nums = %d _flag:%d", _image_nums, _flag);
	bool has_palette = false;
	for(int i = 0; i < _image_nums; i++)
	{
		_palette_image_num[i] = is->ReadInt8();
		if(!_palette_table_loaded)
		{
			_palette_table[i] = new UINT8*[_palette_image_num[i]];
		}
		//printf("\n_image %d num palette =%d\n",i,_palette_image_num[i]);
		for(int j = 0; j< _palette_image_num[i]; j++)
		{
			if(!_palette_table_loaded)
			{
				_palette_table[i][j] = new UINT8[256*3+4];
			}
			has_palette = true;
			for(int k = 0; k < 256; k++)
			{
				_palette_table[i][j][k*3] = is->ReadInt8();
				_palette_table[i][j][k*3+1] = is->ReadInt8();
				_palette_table[i][j][k*3+2] = is->ReadInt8();
				//printf("%d, %d, %d\t",_palette_table[i][j][k*3],_palette_table[i][j][k*3+1],_palette_table[i][j][k*3+2]);
			}
			is->ReadInt32();
		}
	}
	if(!_use_true_color_texture && has_palette)
	{
		_use_true_color_texture = true;
		_use_palette_with_optimize_texture = true;
	}
	_palette_table_loaded = true;
	//read num clip
	_num_modules  = is->ReadInt16();
	ENGINE_DEBUG_SPRITE("[SPRITE] [_num_modules:%d]\n", _num_modules);
	//TEST_LOG("_num_modules:%d\n", _num_modules);
	if (_export_image_type == _EXPORT_TYPE_3D_TEXTURE)
	{
		_module_x = new INT16[_num_modules];
		_module_y = new INT16[_num_modules];
		for (int m = 0; m < _num_modules; m++){
			_module_x[m] = 0;
			_module_y[m] = 0;
		}
		_module_img_id = new BYTE[_num_modules];
		_module_replay_data = new INT16[_num_modules*3];
	}
#ifdef USE_MULTI_MODULE_TYPE
	_module_type = new BYTE[_num_modules];
	_module_color =  new UINT32[_num_modules];
#endif
	_module_w = new INT16[_num_modules];
	_module_h = new INT16[_num_modules];
	
	//read clip info
	for (INT32 i = 0; i < _num_modules; i++)
	{
	#ifdef USE_MULTI_MODULE_TYPE
		if ( (_flag&_FLAG_MULTI_MODULE_TYPE) != 0)
		{
			_module_type[i] = is->ReadInt8();
		}
	#endif
		
		_module_w[i] =is->ReadInt16() ;
		_module_h[i] = is->ReadInt16() ;

	#ifdef USE_MULTI_MODULE_TYPE
		if (_module_type[i] != 0)
		{
			_module_color[i] = is->ReadInt32();
			is->ReadInt8();
			is->ReadInt16();
		}
		else
	#endif
		if (_export_image_type == _EXPORT_TYPE_3D_TEXTURE)
		{
			if ((_module_w[i]&0xFFFF) > 0 && (_module_h[i]&0xFFFF) > 0)
			{
				_module_x[i] = is->ReadInt16();
				_module_y[i] = is->ReadInt16();
				
				_module_img_id[i] = is->ReadInt8();
			}
		}
	}
	
	//num frame
	_num_frames = is->ReadInt16();
	ENGINE_DEBUG_SPRITE("[SPRITE] [_num_frames:%d]\n", _num_frames);
	//TEST_LOG("_num_frames:%d\n", _num_frames);
	if (_num_frames > 0)
	{
		//anim pos in aframe list
		_frame_offset = new INT32[_num_frames];
		_num_fmodules = 0;
		for (INT32 i = 0; i < _num_frames; i++)
		{
			INT32 fmodules = is->ReadInt16();
			
			_frame_offset[i] = (INT32)_num_fmodules;
			_num_fmodules += fmodules;
		}
		
		_fmodule_id = new INT16[_num_fmodules];
		_fmodule_x = new INT16[_num_fmodules];
		_fmodule_y = new INT16[_num_fmodules];
		for (int fm = 0; fm < _num_fmodules; fm++){
			_fmodule_x[fm] = 0;
			_fmodule_y[fm] = 0;
		}
		_fmodule_transf = new BYTE[_num_fmodules];

#ifdef USE_FM_OPACITY
		_fmodule_opacity = new BYTE[_num_fmodules];
#endif
		
		
		for (INT32 i = 0; i < _num_fmodules; i++)
		{
			_fmodule_id[i] = (INT16)(is->ReadInt16());
			//TEST_LOG("_num_frames_module:%d:%d\n",i, _fmodule_id[i]);
			_fmodule_transf[i] = is->ReadInt8();
			if ((_fmodule_transf[i] & _FREE_TRANSFORM) != 0)
			{
			#ifdef USE_FREE_TRANSFORM
				if (!_fmodule_ox)
				{
					_fmodule_ox = new float[_num_fmodules];
					_fmodule_oy = new float[_num_fmodules];
					_fmodule_m11 = new float[_num_fmodules];
					_fmodule_m12 = new float[_num_fmodules];
					_fmodule_m21 = new float[_num_fmodules];
					_fmodule_m22 = new float[_num_fmodules];
				}
				_fmodule_ox[i] = is->ReadFloat32();
				_fmodule_oy[i] = is->ReadFloat32();
				_fmodule_m11[i] = is->ReadFloat32();
				_fmodule_m12[i] = is->ReadFloat32();
				_fmodule_m21[i] = is->ReadFloat32();
				_fmodule_m22[i] = is->ReadFloat32();
				_fmodule_x[i] = 0;		
				_fmodule_y[i] = 0;
			#else
				_fmodule_x[i] = (INT16) is->ReadFloat32();		
				_fmodule_y[i] = (INT16) is->ReadFloat32();
				 is->ReadInt32();
				 is->ReadInt32();
				 is->ReadInt32();
				 is->ReadInt32();
			#endif

			}
			else
			{
				_fmodule_x[i] = (INT16)(is->ReadInt16());
				_fmodule_y[i] = (INT16)(is->ReadInt16());
			}
	
#ifdef USE_FM_OPACITY
			if ( (_flag&_FLAG_EXPORT_FM_OPACITY) != 0)
			{
				_fmodule_opacity[i] = is->ReadInt8();
			}
			else
				_fmodule_opacity[i] = 100;
#else
			if ( (_flag&_FLAG_EXPORT_FM_OPACITY) != 0)
				is->ReadInt8();
#endif			

		}
	}
	
	//num anim
	_num_anims = is->ReadInt16();
	ENGINE_DEBUG_SPRITE("[SPRITE] [_num_anims:%d]\n", _num_anims);
	//TEST_LOG("_num_anims:%d\n", _num_anims);
	if (_num_anims > 0)
	{
		//anim pos in aframe list
		_anim_offset = new INT16[_num_anims];
		_num_aframes = 0;
		for (INT32 i = 0; i < _num_anims; i++)
		{
			INT32 aframes = is->ReadInt16();
			
			_anim_offset[i] = (INT16)_num_aframes;
			
			_num_aframes += aframes;
		}
		
		//aframe list
		_aframe_id = new INT16[_num_aframes];
		_aframe_x = new INT16[_num_aframes];
		_aframe_y = new INT16[_num_aframes];
		_aframe_transf = new BYTE[_num_aframes];
		_aframe_time = new BYTE[_num_aframes];
		for (INT32 i = 0; i < _num_aframes; i++)
		{
			_aframe_id[i] = (INT16)(is->ReadInt16());
			
			_aframe_x[i] = (INT16)(is->ReadInt16());
			
			_aframe_y[i] = (INT16)(is->ReadInt16());
		
			_aframe_transf[i] = is->ReadInt8();
			_aframe_time[i] = is->ReadInt8();
		}
	}
	
	
	
	//only support _EXPORT_QUALITY_TRUE_COLOR and _EXPORT_TYPE_3D_TEXTURE
	if (_export_quality == _EXPORT_QUALITY_INDEX_COLOR )
	{
		if (_export_image_type == _EXPORT_TYPE_3D_TEXTURE)
		{
			//texture image
			//printf("Module 3D texture");
			_num_texture_images = is->ReadInt8();
			
			_image_format = new BYTE[_num_texture_images];
			_texture_image_w = new INT32[_num_texture_images];
			_texture_image_h = new INT32[_num_texture_images];

			_backup_pos = new int[_num_texture_images];
			
			
			_texture_image_data = new BYTE*[_num_texture_images];
			for (int k = 0; k < _num_texture_images; k++)
			{
				_texture_image_data[k] = NULL;
			}
			
			for (int tex_id = 0; tex_id < _num_texture_images; tex_id++)
			{

				_image_format[tex_id] = is->ReadInt8();
				_image_format[tex_id] = _IMAGE_FORMAT_RAW_8888;
				BYTE num_color = is->ReadInt8();
				_num_pal = is->ReadInt8();
				_current_pal = 0;
				_texture_image = new HTEXTURE*[_num_pal];
				_texture_mask = new INT32*[_num_pal];
				int color = 0;
				// read palette
				for (int i = 0; i < _num_pal; i++)
				{
					_texture_image[i] = new HTEXTURE[_num_modules];
					_texture_mask[i] = new INT32[num_color];
					for (int j = 0; j < num_color; j++)
					{
						color = is->ReadInt24();
						_texture_mask[i][j] = color;
						//printf("pal[%d][%d] = %d\n", i , j , color);
					}
					//printf("\n");
				}
				
				_has_alpha = true;
				
				//read image width
				_texture_image_w[tex_id] = is->ReadInt16();
				
				//read image height
				_texture_image_h[tex_id] = is->ReadInt16();
				//printf("w %d	h %d", _texture_image_w[tex_id], _texture_image_h[tex_id] );
				int m_size = _texture_image_w[tex_id] * _texture_image_h[tex_id];
				int ind_size = 0;
				int alp_size = 0;
				_backup_pos[tex_id] = is->GetPos();
				//size = is->ReadInt24();
				ind_size = is->ReadInt24();
				alp_size = is->ReadInt24();
				
				//printf("m_size %d	%d	%d\n", m_size, ind_size, alp_size );
				//UINT8* _rle_ind_buffer = new UINT8[ind_size];
				is->ReadData(_rle_ind_buffer, ind_size);
				
				//UINT8* _rle_alp_buffer = new UINT8[alp_size];
				is->ReadData(_rle_alp_buffer, alp_size);
				
				// decode RLE
				//UINT8* _ind_buffer = new UINT8[m_size];
				//UINT8* _alp_buffer = new UINT8[m_size];
				
				DecodeRLE(_rle_ind_buffer, _ind_buffer, ind_size, 0xFF);
				DecodeRLE(_rle_alp_buffer, _alp_buffer, alp_size, 0xFE);
				
				// build bmp
				if (is_create_texture)
				{
					for (int h = 0; (palette_mask>>h) > 0; h++)
					{
						if (((palette_mask>>h) & 1) == 1)
						{
							int offset = 0;
							for (int j =0; j< m_size; j++)
							{
								_unzip_buff[offset++] = (_texture_mask[h][_ind_buffer[j]] >> 16) & 0xff;
								_unzip_buff[offset++] = (_texture_mask[h][_ind_buffer[j]] >> 8) & 0xff;
								_unzip_buff[offset++] = (_texture_mask[h][_ind_buffer[j]]) & 0xff;
								_unzip_buff[offset++] = _alp_buffer[j];

								//printf("CGame::_this->_unzip_buff[%d] = %d	%d	%d	%d\n", offset, CGame::_this->_unzip_buff[offset-4], CGame::_this->_unzip_buff[offset-3], CGame::_this->_unzip_buff[offset-2], h);
							}
							//printf("\n");
							//_texture_image[0][i] = CreateTextureImage(i, _unzip_buff, _texture_image_w[i], _texture_image_h[i], 0);
							//TEST_LOG("create tex2 %d w:%d h:%d\n", _texture_image[h][tex_id], _texture_image_w[tex_id], _texture_image_h[tex_id]);
							_texture_image[h][tex_id] = CreateTextureImage(g,tex_id, _unzip_buff, _texture_image_w[tex_id], _texture_image_h[tex_id], linear_texture, h, 0, -1, isInThread);
							
						}
					}
				}
			}
		}
		else // module image
		{
			//printf("Module Image");
			BYTE img_format = is->ReadInt8();
			
			_image_format = new BYTE[_num_modules];
			_texture_image_w = new INT32[_num_modules];
			_texture_image_h = new INT32[_num_modules];
			_backup_pos = new int[_num_texture_images];
			
			
			
			BYTE num_color = is->ReadInt8();
			_num_pal = is->ReadInt8();
			_current_pal = 0;
			if(_texture_image == NULL)
			{
				_texture_image = new HTEXTURE*[_num_pal];
			}
			_texture_mask = new INT32*[_num_pal];
			int color = 0;
			// read palette
			for (int i = 0; i < _num_pal; i++)
			{
				_texture_image[i] = new HTEXTURE[_num_modules];
				_texture_mask[i] = new INT32[num_color];
				for (int j = 0; j < num_color; j++)
				{
					color = is->ReadInt24();
					_texture_mask[i][j] = color;
					//("pal[%d][%d] = %d\n", i , j , color);
				}
				//printf("\n");
			}
			
			_has_alpha = true;
			// read modules image data
			for (int i = 0; i < _num_modules; i++)
			{
				int m_size = _module_w[i]*_module_h[i];
				//_module_x[i] = 0;
				//_module_y[i] = 0;
				
				//int size = 0;
				int ind_size = 0;
				int alp_size = 0;
				_backup_pos[i] = is->GetPos();
				//size = is->ReadInt24();
				ind_size = is->ReadInt24();
				alp_size = is->ReadInt24();
				
				//printf("size %d	%d	%d\n", size, ind_size, alp_size );

				//UINT8* _rle_ind_buffer = new UINT8[ind_size];
				is->ReadData(_rle_ind_buffer, ind_size);
				
				//UINT8* _rle_alp_buffer = new UINT8[alp_size];
				is->ReadData(_rle_alp_buffer, alp_size);
				
				// decode RLE
				//UINT8* _ind_buffer = new UINT8[m_size];
				//UINT8* _alp_buffer = new UINT8[m_size];
				
				_texture_image_w[i] = _module_w[i];
				_texture_image_h[i] = _module_h[i];
				_image_format[i] = _IMAGE_FORMAT_RAW_8888;
				
				DecodeRLE(_rle_ind_buffer, _ind_buffer, ind_size, 0xFF);
				DecodeRLE(_rle_alp_buffer, _alp_buffer, alp_size, 0xFE);
				
				
				// merge color
				if (is_create_texture)
				{
					for (int h = 0; (palette_mask>>h) > 0; h++)
					{
						if (((palette_mask>>h) & 1) == 1)
						{
							//int pal = h;
							
							int offset = 0;
							for (int j =0; j< m_size; j++)
							{
								_unzip_buff[offset++] = (_texture_mask[h][_ind_buffer[j]] >> 16) & 0xff;
								_unzip_buff[offset++] = (_texture_mask[h][_ind_buffer[j]] >> 8) & 0xff;
								_unzip_buff[offset++] = (_texture_mask[h][_ind_buffer[j]]) & 0xff;
								_unzip_buff[offset++] = _alp_buffer[j];

								//printf("CGame::_this->_unzip_buff[%d] = %d	%d	%d	%d\n", offset, CGame::_this->_unzip_buff[offset-4], CGame::_this->_unzip_buff[offset-3], CGame::_this->_unzip_buff[offset-2], h);
							}
							//printf("\n");
							//_texture_image[0][i] = CreateTextureImage(i, _unzip_buff, _texture_image_w[i], _texture_image_h[i], 0);
							//TEST_LOG("create tex1 %d w:%d h:%d\n", _texture_image[h][i], _texture_image_w[i], _texture_image_h[i]);
							_texture_image[h][i] = CreateTextureImage(g,i, _unzip_buff, _texture_image_w[i], _texture_image_h[i], linear_texture, h, 0, -1, isInThread);
							
						}
					}
				}
			}
		}
	}
	else // rgb color
	{
		_backup_pallete = withPalette;
		//texture image
		_num_texture_images = is->ReadInt8();
		_image_format = new BYTE[_num_texture_images];
		_texture_image_w = new INT32[_num_texture_images];
		_texture_image_h = new INT32[_num_texture_images];
		_backup_pos = new int[_num_texture_images];

		if (_palette_image_num[0] >= 1)
			_num_pal = _palette_image_num[0];
		else
			_num_pal = 1;
		if(_texture_image == NULL)
		{
			_texture_image = new HTEXTURE*[_num_pal];
		}
		
		if (is_create_texture)
		{
			_texture_image[withPalette] = new HTEXTURE[_num_texture_images];
		}

		_texture_image_data = new BYTE*[_num_texture_images];
		for (int k = 0; k < _num_texture_images; k++)
		{
			_texture_image_data[k] = NULL;
		}
		
		for (int tex_id = 0; tex_id < _num_texture_images; tex_id++)
		{
			_image_format[tex_id] = is->ReadInt8();
					
			if (_image_format[tex_id] == _IMAGE_FORMAT_RAW_8888 || 
				_image_format[tex_id] == _IMAGE_FORMAT_RAW_0888 || 
				_image_format[tex_id] == _IMAGE_FORMAT_RAW_1888)
			{
				if (_image_format[tex_id] == _IMAGE_FORMAT_RAW_8888 || _image_format[tex_id] == _IMAGE_FORMAT_RAW_1888)
				{
					_has_alpha = true;
				}
				int w = 0;
				int h = 0;
				w = is->ReadInt16();
				h = is->ReadInt16();
				//read image width
				_texture_image_w[tex_id] = w;// is->ReadInt16();
				
				//read image height
				_texture_image_h[tex_id] = h;// is->ReadInt16();

				_backup_pos[tex_id] = is->GetPos();
				//read image zipped data
				UnzipRAWImage(g->ColorType(), is, _image_format[tex_id], _unzip_buff, _texture_image_w[tex_id], _texture_image_h[tex_id]);
				
				if (is_create_texture)
				{
					//TEST_LOG("create tex %d w:%d h:%d\n", tex_id, w,h);
					//printf("Cache Texture with palette %d",withPalette);
					_texture_image[withPalette][tex_id] = CreateTextureImage(g, tex_id, _unzip_buff, _texture_image_w[tex_id], _texture_image_h[tex_id], linear_texture, withPalette,withPalette, -1, isInThread);
					
				}
				else
				{
					CacheTextureData(tex_id);
				}
			}
		}
	}

	ClearReplace();

	//set fontHeight
	_fontHeight = GetFModuleHeight(0, 0);
	_fontSpaceWidth = GetFModuleWidth(0, 0);
	
	_has_texture = true;
}

	
void CSprite::Unload(Graphic* g)
{	
	if (_num_texture_images > 0)
	{
		//printf("\n  _image_nums = %d ",  _image_nums);
		for (int i =0; i<_num_pal ; i++)
		{
			for (int j = 0; j < _num_texture_images; j++){
				if (_texture_image[i][j] != NULL){
					g->freeTexture(_texture_image[i][j]);
				}
				_texture_image[i][j] = NULL;
			}
		}
		SAFE_DEL_ARRAY_ARRAY(_texture_mask, _num_pal);
		//SAFE_DEL_ARRAY_ARRAY(_texture_image, _num_pal);
		SAFE_DEL_ARRAY(_image_format);
		SAFE_DEL_ARRAY(_backup_pos);
		
		SAFE_DEL_ARRAY(_texture_image_w);
		SAFE_DEL_ARRAY(_texture_image_h);
		
		if (_texture_image_data != NULL)
		{
			SAFE_DEL_ARRAY_ARRAY(_texture_image_data, _num_texture_images);
		}

		//@@@@
		//if (_texture_image != NULL)
		//{
			SAFE_DEL_ARRAY_ARRAY(_texture_image, _num_pal);
		//}
		if (_palette_table_loaded)
		{			
			for(int i = 0; i < _image_nums; i++)
			{
				for (int j = 0; j < _palette_image_num[i]; j++)
				{
					SAFE_DEL_ARRAY(_palette_table[i][j]);
				}
				SAFE_DEL_ARRAY(_palette_table[i]);
			}
			SAFE_DEL_ARRAY(_palette_table);
			SAFE_DEL_ARRAY(_palette_image_num);
		}
		
		
	}

	SAFE_DEL_ARRAY(_module_img_id);
	SAFE_DEL_ARRAY(_module_x);
	SAFE_DEL_ARRAY(_module_y);	
	SAFE_DEL_ARRAY(_module_w);
	SAFE_DEL_ARRAY(_module_h);
    SAFE_DEL_ARRAY(_module_replay_data);
	SAFE_DEL_ARRAY(_module_type);	
	SAFE_DEL_ARRAY(_module_color);	
	SAFE_DEL_ARRAY(_module_thickness);	
	SAFE_DEL_ARRAY(_module_ext_param1);		 
	SAFE_DEL_ARRAY(_frame_offset);	
	SAFE_DEL_ARRAY(_fmodule_id);
	SAFE_DEL_ARRAY(_fmodule_x);
	SAFE_DEL_ARRAY(_fmodule_y);
	SAFE_DEL_ARRAY(_fmodule_transf);
	SAFE_DEL_ARRAY(_fmodule_ox);
	SAFE_DEL_ARRAY(_fmodule_oy);
	SAFE_DEL_ARRAY(_fmodule_m11);
	SAFE_DEL_ARRAY(_fmodule_m12);
	SAFE_DEL_ARRAY(_fmodule_m21);
	SAFE_DEL_ARRAY(_fmodule_m22);
/*#ifdef USE_FM_ROTATION
	SAFE_DEL_ARRAY(_fmodule_angle);
#endif*/
#ifdef USE_FM_OPACITY
	SAFE_DEL_ARRAY(_fmodule_opacity);
#endif
/*#ifdef USE_FM_SCALE
	SAFE_DEL_ARRAY(_fmodule_scale);
#endif*/
	if (_num_anims > 0)
	{
		SAFE_DEL_ARRAY(_anim_offset);
		SAFE_DEL_ARRAY(_aframe_id);
		SAFE_DEL_ARRAY(_aframe_x);
		SAFE_DEL_ARRAY(_aframe_y);
		SAFE_DEL_ARRAY(_aframe_transf);
		SAFE_DEL_ARRAY(_aframe_time);
	}
	//CGame::Sleep(500);
	
	_has_texture = false;
}

void CSprite::unloadTexture(Graphic* g){
	if (_num_texture_images > 0)
	{
		for (int i = 0; i < _num_pal; i++)
		{
			for (int j = 0; j < _num_texture_images; j++){
				if (_texture_image[i][j] != NULL){
					g->freeTexture(_texture_image[i][j]);
				}
				_texture_image[i][j] = NULL;
			}
		}
	}
	_has_texture = false;
}

void CSprite::reloadTexture(Graphic* g, DataInputStream* is, bool isInThread){

	_has_texture = true;
	if (_num_texture_images > 0)
	{
		for (int i = 0; i < _num_pal; i++)
		{
			for (int j = 0; j < _num_texture_images; j++){
				if (_texture_image[i][j] != NULL){
					return;
				}
			}
		}
	}

	if (_export_quality == _EXPORT_QUALITY_INDEX_COLOR)
	{
		if (_export_image_type == _EXPORT_TYPE_3D_TEXTURE)
		{
			for (int tex_id = 0; tex_id < _num_texture_images; tex_id++)
			{
				int m_size = _texture_image_w[tex_id] * _texture_image_h[tex_id];
				int ind_size = 0;
				int alp_size = 0;
				is->Seek(SEEK_SET,_backup_pos[tex_id]);
				//size = is->ReadInt24();
				ind_size = is->ReadInt24();
				alp_size = is->ReadInt24();

				//printf("m_size %d	%d	%d\n", m_size, ind_size, alp_size );
				//UINT8* _rle_ind_buffer = new UINT8[ind_size];
				is->ReadData(_rle_ind_buffer, ind_size);

				//UINT8* _rle_alp_buffer = new UINT8[alp_size];
				is->ReadData(_rle_alp_buffer, alp_size);

				// decode RLE
				//UINT8* _ind_buffer = new UINT8[m_size];
				//UINT8* _alp_buffer = new UINT8[m_size];

				DecodeRLE(_rle_ind_buffer, _ind_buffer, ind_size, 0xFF);
				DecodeRLE(_rle_alp_buffer, _alp_buffer, alp_size, 0xFE);

				// build bmp
				for (int h = 0; (_backup_pallete >> h) > 0; h++)
				{
					if (((_backup_pallete >> h) & 1) == 1)
					{
						int offset = 0;
						for (int j = 0; j< m_size; j++)
						{
							_unzip_buff[offset++] = (_texture_mask[h][_ind_buffer[j]] >> 16) & 0xff;
							_unzip_buff[offset++] = (_texture_mask[h][_ind_buffer[j]] >> 8) & 0xff;
							_unzip_buff[offset++] = (_texture_mask[h][_ind_buffer[j]]) & 0xff;
							_unzip_buff[offset++] = _alp_buffer[j];

							//printf("CGame::_this->_unzip_buff[%d] = %d	%d	%d	%d\n", offset, CGame::_this->_unzip_buff[offset-4], CGame::_this->_unzip_buff[offset-3], CGame::_this->_unzip_buff[offset-2], h);
						}
						//printf("\n");
						//_texture_image[0][i] = CreateTextureImage(i, _unzip_buff, _texture_image_w[i], _texture_image_h[i], 0);
						//TEST_LOG("create tex2 %d w:%d h:%d\n", _texture_image[h][tex_id], _texture_image_w[tex_id], _texture_image_h[tex_id]);
						_texture_image[h][tex_id] = CreateTextureImage(g, tex_id, _unzip_buff, _texture_image_w[tex_id], _texture_image_h[tex_id], _backup_antialias, h, 0, -1, isInThread);

					}
				}
			}
		}
		else // module image
		{
			for (int i = 0; i < _num_modules; i++)
			{
				int m_size = _module_w[i] * _module_h[i];
				//_module_x[i] = 0;
				//_module_y[i] = 0;

				//int size = 0;
				int ind_size = 0;
				int alp_size = 0;
				is->Seek(SEEK_SET, _backup_pos[i]);
				//size = is->ReadInt24();
				ind_size = is->ReadInt24();
				alp_size = is->ReadInt24();

				//printf("size %d	%d	%d\n", size, ind_size, alp_size );

				//UINT8* _rle_ind_buffer = new UINT8[ind_size];
				is->ReadData(_rle_ind_buffer, ind_size);

				//UINT8* _rle_alp_buffer = new UINT8[alp_size];
				is->ReadData(_rle_alp_buffer, alp_size);

				// decode RLE
				//UINT8* _ind_buffer = new UINT8[m_size];
				//UINT8* _alp_buffer = new UINT8[m_size];

				DecodeRLE(_rle_ind_buffer, _ind_buffer, ind_size, 0xFF);
				DecodeRLE(_rle_alp_buffer, _alp_buffer, alp_size, 0xFE);


				// merge color

					for (int h = 0; (_backup_pallete >> h) > 0; h++)
					{
						if (((_backup_pallete >> h) & 1) == 1)
						{
							//int pal = h;

							int offset = 0;
							for (int j = 0; j< m_size; j++)
							{
								_unzip_buff[offset++] = (_texture_mask[h][_ind_buffer[j]] >> 16) & 0xff;
								_unzip_buff[offset++] = (_texture_mask[h][_ind_buffer[j]] >> 8) & 0xff;
								_unzip_buff[offset++] = (_texture_mask[h][_ind_buffer[j]]) & 0xff;
								_unzip_buff[offset++] = _alp_buffer[j];

								//printf("CGame::_this->_unzip_buff[%d] = %d	%d	%d	%d\n", offset, CGame::_this->_unzip_buff[offset-4], CGame::_this->_unzip_buff[offset-3], CGame::_this->_unzip_buff[offset-2], h);
							}
							//printf("\n");
							//_texture_image[0][i] = CreateTextureImage(i, _unzip_buff, _texture_image_w[i], _texture_image_h[i], 0);
							//TEST_LOG("create tex1 %d w:%d h:%d\n", _texture_image[h][i], _texture_image_w[i], _texture_image_h[i]);
							_texture_image[h][i] = CreateTextureImage(g, i, _unzip_buff, _texture_image_w[i], _texture_image_h[i], _backup_antialias, h, 0, -1, isInThread);

						}
					}

			}
		}
	}
	else // rgb color
	{
		
		for (int tex_id = 0; tex_id < _num_texture_images; tex_id++)
		{
			if (_image_format[tex_id] == _IMAGE_FORMAT_RAW_8888 ||
				_image_format[tex_id] == _IMAGE_FORMAT_RAW_0888 ||
				_image_format[tex_id] == _IMAGE_FORMAT_RAW_1888)
			{
				is->Seek(SEEK_SET, _backup_pos[tex_id]);
				//read image zipped data
				UnzipRAWImage(g->ColorType(), is, _image_format[tex_id], _unzip_buff, _texture_image_w[tex_id], _texture_image_h[tex_id]);

				//if (is_create_texture)
				//{
					//TEST_LOG("create tex %d w:%d h:%d\n", tex_id, w,h);
					//printf("Cache Texture with palette %d",withPalette);
				_texture_image[_backup_pallete][tex_id] = CreateTextureImage(g, tex_id, _unzip_buff, _texture_image_w[tex_id], _texture_image_h[tex_id], _backup_antialias, _backup_pallete, _backup_pallete, -1, isInThread);

				//}
				//else
				//{
				//	CacheTextureData(tex_id);
				//}
			}
		}
	}
}

void CSprite::CacheTextureData(int tex_id)
{
	int w = _texture_image_w[tex_id];
	int h = _texture_image_h[tex_id];
	INT32 img_size = 0;
	
	if (_use_true_color_texture)
	{
		if (_has_alpha)
		{
			img_size = w*h*4;
		}
		else
		{
			img_size = w*h*3;
		}
	}
	else
	{
		img_size = w*h*2;
	}
	
	_texture_image_data[tex_id] = new BYTE[img_size];
	
	for (int i = 0; i < img_size; i++)
	{
		_texture_image_data[tex_id][i] = _unzip_buff[i];
	}
}

void CSprite::CreateSprite(BYTE* argb, INT32 w, INT32 h)
{
	//export
	_export_quality = 1;
	_export_image_type = 1;
	_export_pixel_type = 1;

	_image_nums = 1;
	_palette_image_num = new int[_image_nums];
	_palette_table =  new UINT8**[_image_nums];
	//module
	_num_modules = 1;
	_module_x = new INT16[_num_modules];
	_module_y = new INT16[_num_modules];
	_module_img_id = new BYTE[_num_modules];

	_module_replay_data = new INT16[_num_modules*3];

#ifdef USE_MULTI_MODULE_TYPE
	_module_type = new BYTE[_num_modules];
	_module_color =  new UINT32[_num_modules];
#endif
	_module_w = new INT16[_num_modules];
	_module_h = new INT16[_num_modules];

	for (INT32 i = 0; i < _num_modules; i++)
	{
#ifdef USE_MULTI_MODULE_TYPE
		_module_type[i] = _MODULE_TYPE_IMAGE;
#endif		
		_module_w[i] = w;
		_module_h[i] = h;

		_module_x[i] = 0;
		_module_y[i] = 0;
		
		_module_img_id[i] = 0;

	}

//frame
	_num_frames = 1;
	
	if (_num_frames > 0)
	{
		//anim pos in aframe list
		_frame_offset = new INT32[_num_frames];
		_num_fmodules = 0;
		for (INT32 i = 0; i < _num_frames; i++)
		{
			INT32 fmodules = 1;
			
			_frame_offset[i] = (INT32)_num_fmodules;
			
			_num_fmodules += fmodules;
		}
		
		_fmodule_id = new INT16[_num_fmodules];
		_fmodule_x = new INT16[_num_fmodules];
		_fmodule_y = new INT16[_num_fmodules];
		_fmodule_transf = new BYTE[_num_fmodules];

#ifdef USE_FM_OPACITY
		_fmodule_opacity = new BYTE[_num_fmodules];
#endif

		
		for (INT32 i = 0; i < _num_fmodules; i++)
		{
			_fmodule_id[i] = 0;
			
			_fmodule_transf[i] = 0;
			
			_fmodule_x[i] = 0;
			_fmodule_y[i] = 0;		
			_fmodule_opacity[i] = 100;
		}
	}

	//num anim
	_num_anims = 0;

	//texture
	_num_texture_images = 1;
			
	_image_format = new BYTE[_num_texture_images];
	_texture_image_w = new INT32[_num_texture_images];
	_texture_image_h = new INT32[_num_texture_images];

	
	_num_pal = 1;
	if(_texture_image == NULL)
	{
		_texture_image = new HTEXTURE*[_num_pal];
	}
	
	_texture_image[0] = new HTEXTURE[_num_texture_images];

	_texture_image_data = new BYTE*[_num_texture_images];
	for (int k = 0; k < _num_texture_images; k++)
	{
		_texture_image_data[k] = NULL;
	}

	for (int tex_id = 0; tex_id < _num_texture_images; tex_id++)
	{
		_image_format[tex_id] = _IMAGE_FORMAT_RAW_8888;
		_has_alpha = true;

		//read image width
		_texture_image_w[tex_id] = w;
		
		//read image height
		_texture_image_h[tex_id] = h;

		////_texture_image[0][tex_id] = CreateTextureImage(tex_id, argb, _texture_image_w[tex_id], _texture_image_h[tex_id], true, 0,0);
	}
}
void CSprite::PreplaceTexture(BYTE* argb,int width, int height,int tex_id,int palette)
{
	////GLuint  texname = -1;
	////if (_texture_image[palette][tex_id] )
	////{
	////	texname = _texture_image[0][tex_id];
	////	glDeleteTextures(1, &texname);
	////}
	////if(texname > 0)
	////{
	////	//_texture_image_w[tex_id] = width;
	////	//_texture_image_h[tex_id] = height;
	////	BYTE *temp = new BYTE[_texture_image_w[tex_id]*_texture_image_h[tex_id]*4];
	////	//printf("\n_texture_image_w[tex_id] = %d,_texture_image_h[tex_id] = %d",_texture_image_w[tex_id],_texture_image_h[tex_id]);
 ////       for(int i = 0; i < _texture_image_w[tex_id]; i++)
 ////       {
 ////           for(int j = 0; j < _texture_image_h[tex_id];j++)
 ////           {
 ////               if(i < width && j < height)
 ////               {
 ////                   temp[(j*_texture_image_w[tex_id] + i)*4] = argb[(j*width+i)*4];
	////				temp[(j*_texture_image_w[tex_id] + i)*4+1] = argb[(j*width+i)*4+1];
	////				temp[(j*_texture_image_w[tex_id] + i)*4+2] = argb[(j*width+i)*4+2];
	////				temp[(j*_texture_image_w[tex_id] + i)*4+3] = argb[(j*width+i)*4+3];

 ////               }
 ////           }
 ////           
 ////       }
	////	GLint data_format = GL_RGBA;
	////	GLint pixel_type = GL_UNSIGNED_BYTE;
	////	glEnable(GL_TEXTURE_2D);
	////	//glGenTextures(1, &texname);
	////	glBindTexture(GL_TEXTURE_2D, texname);
	////	bool linear_texture = true;
	////	#ifdef USE_LINEAR_TEXTURE_MAPPING
	////		if (linear_texture)
	////		{
	////			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_LINEAR
	////			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_LINEAR
	////		}
	////		else
	////	#endif //USE_LINEAR_TEXTURE_MAPPING
	////		{
	////			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_NEAREST
	////			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_NEAREST
	////		}
	////	//#endif //USE_LINEAR_TEXTURE_MAPPING

	////		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	////		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	////	 glTexImage2D(GL_TEXTURE_2D, 0, data_format, _texture_image_w[tex_id], _texture_image_h[tex_id], 0, data_format, pixel_type, temp);
	////	_texture_image[0][tex_id] = texname;
	////	 delete[] temp;
	////	glColor4f(1,1,1,1);
	////}
}
HTEXTURE CSprite::CreateTextureImage(Graphic*g, INT32 tex_id, UINT8* argb, INT32 w, INT32 h, int linear_texture, INT8 palette, int withPalette, int texture_name, bool isInThread)
{
	int off = 0;
	PineColorFormat colorformat;
	if(_use_true_color_texture && _image_format[tex_id] == _IMAGE_FORMAT_RAW_8888)
	{
		for(int i = 0; i < w*h;i++)
		{
			
			int r = argb[off];
			int g = argb[off+1];
			int b = argb[off+2];
			int a = argb[off+3];
			if((r==0xFF && !g && b == 0xFF)|| (a == 0x0))
			{
				argb[off] = 0x0;
				argb[off+1] = 0x0;
				argb[off+2] = 0x0;
				argb[off+3] = 0x0;
			}
			else
			if(withPalette >= 1)
			{
				for(int p = 0; p < 256; p++)
				{
					int rr =_palette_table[tex_id][0][p*3];
					int gg =_palette_table[tex_id][0][p*3+1];
					int bb =_palette_table[tex_id][0][p*3+2];

					if(rr ==r && gg == g && bb == b)
					{
						//printf("\nquals");
						argb[off] = _palette_table[tex_id][withPalette][p*3];
						argb[off+1] = _palette_table[tex_id][withPalette][p*3+1];
						argb[off+2] = _palette_table[tex_id][withPalette][p*3+2];
						
						break;
					}
				}
			}
			off += 4;
		}
	}
////
////	
////	UINT32 texname;
////	
////	glEnable(GL_TEXTURE_2D);
////	if(texture_name >= 0)
////	{
////		texname = texture_name;
////	}
////	else
////	{
////		glGenTextures(1, &texname);
////	}
////	glBindTexture(GL_TEXTURE_2D, texname);
////	
////#ifdef USE_LINEAR_TEXTURE_MAPPING
////	if (linear_texture)
////	{
////		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_LINEAR
////		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_LINEAR
////
////	}
////	else
////#endif //USE_LINEAR_TEXTURE_MAPPING
////	{
////		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_NEAREST
////		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST
////
////		
////	}
////    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
////    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
////
////
////	
////
////
	colorformat = PineColorFormat::RGBA_1BYTE;
////	GLint data_format = GL_RGBA;
////	GLint pixel_type = GL_UNSIGNED_BYTE;
	if (_image_format[tex_id] == _IMAGE_FORMAT_RAW_8888)
	{
		if (_use_true_color_texture)
		{
			if(_use_palette_with_optimize_texture)
			{

				//printf("\nUsing palette");
				UINT8 *argb_clone = new UINT8[w*h*2];
				int off = 0;
				int off1 = 0;
				for(int i = 0; i < w*h;i++)
				{
					
					int r = argb[off];
					int g = argb[off+1];
					int b = argb[off+2];
					int a = argb[off+3];
					
					argb_clone[off1] = ((b&0xF0) | ((a>>4)&0x0F));
					argb_clone[off1+1] = ((r&0xF0) | ((g>>4)&0x0F));
					off+=4;
					off1+=2;
				}
				colorformat = PineColorFormat::RGBA_4444;
				PineSize size = scaleBuffer(argb, colorformat, w, h);
                HTEXTURE texture = 0;
#if defined(MAC_OS)
                if (isInThread)
                {
                    
                    int lengthPointer = sizeof(int*);
                    int dataLength = 1 + lengthPointer + 4 + 4 + 4;
                    BufferStream bufferStream;
                    bufferStream.Init(dataLength);
                    
                    
                    bufferStream.WriteByte(lengthPointer);
                    BYTE **temp = NULL;
                    temp = &argb_clone;
                    BYTE* pointer = (BYTE*)temp;

                    for (int i = 0; i < lengthPointer; i++)
                    {
                        BYTE b = *(pointer+i);
                        bufferStream.WriteByte(b);
                    }
                    bufferStream.WriteInt32(w);
                    bufferStream.WriteInt32(h);
                    bufferStream.WriteInt32(colorformat);
                    
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
                                texture = bufferStreamResponse.ReadInt32();
                                GAME()->CleanEvent(evt);
                                complete = true;
                                break;
                            }
                            
                            evt = GAME()->NextEvent(EVENT_RESPONSE_CREATE_TEXTURE, &evt_pointer);
                        }
                        //GAME()->Sleep(FPS_DELAY_TIME);
                    }
                }
                else
                {
                    texture = g->createTexture(argb_clone, size.W, size.H, colorformat, linear_texture);
                }
#else
				texture = g->createTexture(argb_clone, size.W, size.H, colorformat, linear_texture);
#endif
                
					//(GL_TEXTURE_2D, 0, data_format, w, h, 0, data_format, pixel_type, argb_clone);
				delete[] argb_clone;
				//return texname;
				return texture;
			}
		}
		else
		{
			colorformat = PineColorFormat::RGBA_4444;
		}
	}
	else if (	_image_format[tex_id] == _IMAGE_FORMAT_RAW_0888 ||
				_image_format[tex_id] == _IMAGE_FORMAT_RAW_1888)
	{
		if (_use_true_color_texture)
		{
			if (_has_alpha)
			{
				colorformat = PineColorFormat::RGBA_1BYTE;
			}
			else
			{
				colorformat = PineColorFormat::RGB_1BYTE;
			}
		}
		else
		{
			if (_has_alpha)
			{
				colorformat = PineColorFormat::RGBA_5551;
			}
			else
			{
				colorformat = PineColorFormat::RGB_565;
			}
		}
	}
	PineSize size = scaleBuffer(argb, colorformat, w, h);
    HTEXTURE texName = 0;
#if defined(MAC_OS)
    if (isInThread)
    {
        
        int lengthPointer = sizeof(int*);
        int dataLength = 1 + lengthPointer + 4 + 4 + 4;
        BufferStream bufferStream;
        bufferStream.Init(dataLength);
        
        
        bufferStream.WriteByte(lengthPointer);
        BYTE **temp = NULL;
        temp = &argb;
        BYTE* pointer = (BYTE*)temp;
        
        for (int i = 0; i < lengthPointer; i++)
        {
            BYTE b = *(pointer+i);
            bufferStream.WriteByte(b);
        }
        bufferStream.WriteInt32(w);
        bufferStream.WriteInt32(h);
        bufferStream.WriteInt32(colorformat);
        
        GAME()->addEvent(EVENT_REQUEST_CREATE_TEXTURE, (const char*)bufferStream.Data(), dataLength);
        
        bool complete = false;
        
        while (!complete)
        {
            EVENT_POINTER evt_pointer = 0; //tao ra mot con tro duyet
            const PineEvent* evt = GAME()->NextEvent(EVENT_RESPONSE_CREATE_TEXTURE, &evt_pointer);
            
            while (evt != NULL)
            {
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
                    texName = bufferStreamResponse.ReadInt32();
                    
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
        texName = g->createTexture(argb, size.W, size.H, colorformat, linear_texture);
    }
#else
	texName = g->createTexture(argb, size.W, size.H, colorformat, linear_texture);
#endif
    return texName;
}

INT32 CSprite::UnzipRAWImage(PineColorType color_type, BYTE* data, INT32 offset, BYTE format, UINT8* output, INT32 w, INT32 h)
{
	//unzip RGB data
	INT32 i;
	INT32 img_size = 0;
	
	if (_use_true_color_texture)
	{
		if (_has_alpha)
		{
			img_size = w*h*4;
		}
		else
		{
			img_size = w*h*3;
		}
	}
	else
	{
		img_size = w*h*2;
	}
	
	INT32 rlen = 0;
	INT32 val = 0;
	UINT8 a, r, g, b;
	
	i = 0;
	while (i < img_size)
	{
		val = data[offset++]&0xFF;
		
		if (val == 0xFE)
		{
			rlen = data[offset++]&0xFF;
			
			if (format == _IMAGE_FORMAT_RAW_8888)
			{
				if (_export_pixel_type == _EXPORT_PIXEL_16BITS)
				{
					b = data[offset]&0x0F;
					b = b | (b<<4);
					
					g = data[offset]&0xF0;
					g = g | (g>>4);
					
					r = data[offset+1]&0x0F;
					r = r | (r<<4);
					
					a = data[offset+1]&0xF0;
					a = a | (a>>4);
					
					offset += 2;
				}
				//_EXPORT_PIXEL_32BITS
				else
				{
					b = data[offset]&0xFF;
					g = data[offset+1]&0xFF;
					r = data[offset+2]&0xFF;
					a = data[offset+3]&0xFF;
					
					offset += 4;
				}
			}
			//_IMAGE_FORMAT_RAW_0888 or _IMAGE_FORMAT_RAW_1888
			else
			{
				if (_export_pixel_type == _EXPORT_PIXEL_16BITS)
				{
					b = data[offset]&0x1F;
					b = (b&0x07) | (b<<3);
					
					g = (data[offset]&0xE0)>>5;
					g = (g&0x03) | (g<<2) | ((data[offset+1]&0x07)<<5);
					
					r = data[offset+1]&0xF8;
					r = r | ((r>>3)&0x07);
					
					if (r == 0xFF && g == 0 && b == 0xFF)
					{
						a = 0;
					}
					else
					{
						a = 0xFF;
					}

					offset += 2;
				}
				else
				{
					b = data[offset]&0xFF;
					g = data[offset+1]&0xFF;
					r = data[offset+2]&0xFF;
					
					if (r == 0xFF && g == 0 && b == 0xFF)
					{
						a = 0;
					}
					else
					{
						a = 0xFF;
					}
					
					offset += 3;
				}
			}
			
			for (INT32 j = 0; j < rlen; j++)
			{
				if (_use_true_color_texture)
				{
					//use 8888
					if (_has_alpha)
					{
						output[i++] = r;
						output[i++] = g;
						output[i++] = b;
						output[i++] = a;
					}
					//use 0888
					else
					{
						output[i++] = r;
						output[i++] = g;
						output[i++] = b;
					}
				}
				else
				{
					//use 4444 or 1555
					if (_has_alpha)
					{
						//use 4444
						if (format == _IMAGE_FORMAT_RAW_8888)
						{
							if (color_type == PineColorType::CTYPE_OPENGL){
								output[i++] = (b & 0xF0) | ((a >> 4) & 0x0F);
								output[i++] = (r & 0xF0) | ((g >> 4) & 0x0F);
							}
							else if (color_type == PineColorType::CTYPE_DIRECTX){
								output[i++] = (g & 0xF0) | ((b >> 4) & 0x0F);
								output[i++] = (a & 0xF0) | ((r >> 4) & 0x0F);
							}
						}
						//use 5551
						else if (format == _IMAGE_FORMAT_RAW_1888)
						{
							if (color_type ==  PineColorType::CTYPE_OPENGL){
								output[i++] = ((g << 3) & 0xC0) | ((b >> 2) & 0x3E) | (a & 0x01);
								output[i++] = (r & 0xF8) | ((g >> 5) & 0x07);
							}
							else if (color_type == PineColorType::CTYPE_DIRECTX)
							{
								output[i++] = ((g << 3) & 0xC0) | ((r >> 2) & 0x3E) | (a & 0x01);
								output[i++] = (b & 0xF8) | ((g >> 5) & 0x07);
							}
						}
					}
					else
					//use 565
					{
						if (color_type == PineColorType::CTYPE_OPENGL){
							output[i++] = ((g << 3) & 0xE0) | ((b >> 3) & 0x1F);
							output[i++] = (r & 0xF8) | ((g >> 5) & 0x07);
						}
						else if (color_type == PineColorType::CTYPE_DIRECTX){
							output[i++] = ((g << 3) & 0xE0) | ((r >> 3) & 0x1F);
							output[i++] = (b & 0xF8) | ((g >> 5) & 0x07);
						}
					}
				}
			}
		}
		else
		{
			if (format == _IMAGE_FORMAT_RAW_8888)
			{
				if (_export_pixel_type == _EXPORT_PIXEL_16BITS)
				{
					b = val&0x0F;
					b = b | (b<<4);
					
					g = val&0xF0;
					g = g | (g>>4);
					
					r = data[offset]&0x0F;
					r = r | (r<<4);
					
					a = data[offset]&0xF0;
					a = a | (a>>4);
					
					offset += 1;
				}
				//_EXPORT_PIXEL_32BITS
				else
				{
					b = val&0xFF;
					g = data[offset]&0xFF;
					r = data[offset+1]&0xFF;
					a = data[offset+2]&0xFF;
					
					offset += 3;
				}
			}
			else
			{
				if (_export_pixel_type == _EXPORT_PIXEL_16BITS)
				{
					b = val&0x1F;
					b = (b&0x07) | (b<<3);
					
					g = (val&0xE0)>>5;
					g = (g&0x03) | (g<<2) | ((data[offset]&0x07)<<5);
					
					r = data[offset]&0xF8;
					r = r | ((r>>3)&0x07);
					
					if (r == 0xFF && g == 0 && b == 0xFF)
					{
						a = 0;
					}
					else
					{
						a = 0xFF;
					}

					offset += 1;
				}
				else
				{
					b = val&0xFF;
					g = data[offset]&0xFF;
					r = data[offset+1]&0xFF;
					
					if (r == 0xFF && g == 0 && b == 0xFF)
					{
						a = 0;
					}
					else
					{
						a = 0xFF;
					}
					
					offset += 2;
				}
			}

			if (_use_true_color_texture)
			{
				//use 8888
				if (_has_alpha)
				{
					output[i++] = r;
					output[i++] = g;
					output[i++] = b;
					output[i++] = a;
				}
				//use 0888
				else
				{
					output[i++] = r;
					output[i++] = g;
					output[i++] = b;
				}
			}
			else
			{
				//use 4444 or 1555
				if (_has_alpha)
				{
					//use 4444
					if (format == _IMAGE_FORMAT_RAW_8888)
					{
						if (color_type == PineColorType::CTYPE_OPENGL){
							output[i++] = (b & 0xF0) | ((a >> 4) & 0x0F);
							output[i++] = (r & 0xF0) | ((g >> 4) & 0x0F);
						}
						else if (color_type == PineColorType::CTYPE_DIRECTX){
							output[i++] = (g & 0xF0) | ((b >> 4) & 0x0F);
							output[i++] = (a & 0xF0) | ((r >> 4) & 0x0F);
						}
					}
					//use 5551
					else if (format == _IMAGE_FORMAT_RAW_1888)
					{
						if (color_type == PineColorType::CTYPE_OPENGL){
							output[i++] = ((g << 3) & 0xC0) | ((b >> 2) & 0x3E) | (a & 0x01);
							output[i++] = (r & 0xF8) | ((g >> 5) & 0x07);
						}
						else if (color_type == PineColorType::CTYPE_DIRECTX)
						{
							output[i++] = ((g << 3) & 0xC0) | ((r >> 2) & 0x3E) | (a & 0x01);
							output[i++] = (b & 0xF8) | ((g >> 5) & 0x07);
						}
					}
				}
				else
					//use 565
				{
					if (color_type == PineColorType::CTYPE_OPENGL){
						output[i++] = ((g << 3) & 0xE0) | ((b >> 3) & 0x1F);
						output[i++] = (r & 0xF8) | ((g >> 5) & 0x07);
					}
					else if (color_type == PineColorType::CTYPE_DIRECTX){
						output[i++] = ((g << 3) & 0xE0) | ((r >> 3) & 0x1F);
						output[i++] = (b & 0xF8) | ((g >> 5) & 0x07);
					}
				}
			}
		}
	}
	
	return offset;
}

void CSprite::UnzipRAWImage(PineColorType color_type, DataInputStream* is, BYTE format, UINT8* output, INT32 w, INT32 h)
{
	//unzip RGB data
	INT32 i;
	INT32 img_size = 0;
	
	if (_use_true_color_texture)
	{
		if (_has_alpha)
		{
			img_size = w*h*4;
		}
		else
		{
			img_size = w*h*3;
		}
	}
	else
	{
		img_size = w*h*2;
	}
	
	INT32 rlen = 0;
	INT32 val = 0;
	UINT8 a, r, g, b, d;
	
	i = 0;
	while (i < img_size)
	{
		val = is->ReadInt8();
		
		if (val == 0xFE)
		{
			rlen = is->ReadInt8();
			
			if (format == _IMAGE_FORMAT_RAW_8888)
			{
				if (_export_pixel_type == _EXPORT_PIXEL_16BITS)
				{
					d = is->ReadInt8();
					
					b = d&0x0F;
					b = b | (b<<4);
					
					g = d&0xF0;
					g = g | (g>>4);
					
					d = is->ReadInt8();
					
					r = d&0x0F;
					r = r | (r<<4);
					
					a = d&0xF0;
					a = a | (a>>4);
				}
				//_EXPORT_PIXEL_32BITS
				else
				{
					b = is->ReadInt8();
					g = is->ReadInt8();
					r = is->ReadInt8();
					a = is->ReadInt8();
				}
			}
			//_IMAGE_FORMAT_RAW_0888 or _IMAGE_FORMAT_RAW_1888
			else
			{
				if (_export_pixel_type == _EXPORT_PIXEL_16BITS)
				{
					d = is->ReadInt8();
					
					b = d&0x1F;
					b = (b&0x07) | (b<<3);
					
					g = (d&0xE0)>>5;
					
					d = is->ReadInt8();
					
					g = (g&0x03) | (g<<2) | ((d&0x07)<<5);
					
					r = d&0xF8;
					r = r | ((r>>3)&0x07);
					
					if (r == 0xFF && g == 0 && b == 0xFF)
					{
						a = 0;
					}
					else
					{
						a = 0xFF;
					}
				}
				else
				{
					b = is->ReadInt8();
					g = is->ReadInt8();
					r = is->ReadInt8();
					
					if (r == 0xFF && g == 0 && b == 0xFF)
					{
						a = 0;
					}
					else
					{
						a = 0xFF;
					}
				}
			}
			
			for (INT32 j = 0; j < rlen; j++)
			{
				if (_use_true_color_texture)
				{
					//use 8888
					if (_has_alpha)
					{
						output[i++] = r;
						output[i++] = g;
						output[i++] = b;
						output[i++] = a;
					}
					//use 0888
					else
					{
						output[i++] = r;
						output[i++] = g;
						output[i++] = b;
					}
				}
				else
				{
					//use 4444 or 1555
					if (_has_alpha)
					{
						//use 4444
						if (format == _IMAGE_FORMAT_RAW_8888)
						{
							if (color_type == PineColorType::CTYPE_OPENGL){
								output[i++] = (b & 0xF0) | ((a >> 4) & 0x0F);
								output[i++] = (r & 0xF0) | ((g >> 4) & 0x0F);
							}
							else if (color_type == PineColorType::CTYPE_DIRECTX){
								output[i++] = (g & 0xF0) | ((b >> 4) & 0x0F);
								output[i++] = (a & 0xF0) | ((r >> 4) & 0x0F);
							}
						}
						//use 5551
						else if (format == _IMAGE_FORMAT_RAW_1888)
						{
							if (color_type == PineColorType::CTYPE_OPENGL){
								output[i++] = ((g << 3) & 0xC0) | ((b >> 2) & 0x3E) | (a & 0x01);
								output[i++] = (r & 0xF8) | ((g >> 5) & 0x07);
							}
							else if (color_type == PineColorType::CTYPE_DIRECTX)
							{
								output[i++] = ((g << 3) & 0xC0) | ((r >> 2) & 0x3E) | (a & 0x01);
								output[i++] = (b & 0xF8) | ((g >> 5) & 0x07);
							}
						}
					}
					else
					//use 565
					{
						if (color_type == PineColorType::CTYPE_OPENGL){
							output[i++] = ((g << 3) & 0xE0) | ((b >> 3) & 0x1F);
							output[i++] = (r & 0xF8) | ((g >> 5) & 0x07);
						}
						else if (color_type == PineColorType::CTYPE_DIRECTX){
							output[i++] = ((g << 3) & 0xE0) | ((r >> 3) & 0x1F);
							output[i++] = (b & 0xF8) | ((g >> 5) & 0x07);
						}
					}
				}
			}
		}
		else
		{
			if (format == _IMAGE_FORMAT_RAW_8888)
			{
				if (_export_pixel_type == _EXPORT_PIXEL_16BITS)
				{
					b = val&0x0F;
					b = b | (b<<4);
					
					g = val&0xF0;
					g = g | (g>>4);
					
					d = is->ReadInt8();
					
					r = d&0x0F;
					r = r | (r<<4);
					
					a = d&0xF0;
					a = a | (a>>4);
				}
				//_EXPORT_PIXEL_32BITS
				else
				{
					b = val&0xFF;
					g = is->ReadInt8();
					r = is->ReadInt8();
					a = is->ReadInt8();
				}
			}
			else
			{
				if (_export_pixel_type == _EXPORT_PIXEL_16BITS)
				{
					b = val&0x1F;
					b = (b&0x07) | (b<<3);
					
					g = (val&0xE0)>>5;
					
					d = is->ReadInt8();
					
					g = (g&0x03) | (g<<2) | ((d&0x07)<<5);
					
					r = d&0xF8;
					r = r | ((r>>3)&0x07);
					
					if (r == 0xFF && g == 0 && b == 0xFF)
					{
						a = 0;
					}
					else
					{
						a = 0xFF;
					}
				}
				else
				{
					b = val&0xFF;
					g = is->ReadInt8();
					r = is->ReadInt8();
					
					if (r == 0xFF && g == 0 && b == 0xFF)
					{
						a = 0;
					}
					else
					{
						a = 0xFF;
					}
				}
			}

			if (_use_true_color_texture)
			{
				//use 8888
				if (_has_alpha)
				{
					output[i++] = r;
					output[i++] = g;
					output[i++] = b;
					output[i++] = a;
				}
				//use 0888
				else
				{
					output[i++] = r;
					output[i++] = g;
					output[i++] = b;
				}
			}
			else
			{
				//use 4444 or 1555
				if (_has_alpha)
				{
					//use 4444
					if (format == _IMAGE_FORMAT_RAW_8888)
					{
						if (color_type == PineColorType::CTYPE_OPENGL){
							output[i++] = (b & 0xF0) | ((a >> 4) & 0x0F);
							output[i++] = (r & 0xF0) | ((g >> 4) & 0x0F);
						}
						else if (color_type == PineColorType::CTYPE_DIRECTX){
							output[i++] = (g & 0xF0) | ((b >> 4) & 0x0F);
							output[i++] = (a & 0xF0) | ((r >> 4) & 0x0F);
						}
					}
					//use 5551
					else if (format == _IMAGE_FORMAT_RAW_1888)
					{
						if (color_type == PineColorType::CTYPE_OPENGL){
							output[i++] = ((g << 3) & 0xC0) | ((b >> 2) & 0x3E) | (a & 0x01);
							output[i++] = (r & 0xF8) | ((g >> 5) & 0x07);
						}
						else if (color_type == PineColorType::CTYPE_DIRECTX)
						{
							output[i++] = ((g << 3) & 0xC0) | ((r >> 2) & 0x3E) | (a & 0x01);
							output[i++] = (b & 0xF8) | ((g >> 5) & 0x07);
						}
					}
				}
				else
				//use 565
				{
					if (color_type == PineColorType::CTYPE_OPENGL){
						output[i++] = ((g << 3) & 0xE0) | ((b >> 3) & 0x1F);
						output[i++] = (r & 0xF8) | ((g >> 5) & 0x07);
					}
					else if (color_type == PineColorType::CTYPE_DIRECTX){
						output[i++] = ((g << 3) & 0xE0) | ((r >> 3) & 0x1F);
						output[i++] = (b & 0xF8) | ((g >> 5) & 0x07);
					}
				}
			}
		}
	}
}
void CSprite::DecodeRLE(UINT8* in, UINT8* out, int rle_size, int rle_flag)
{
	int offset = 0;
	int out_offset = 0;
	//printf("decode RLE\n");
	while (offset < rle_size)
	{
		if (in[offset++] == (BYTE)rle_flag)
		{
			//printf("in[%d] %d \n",offset, rle_flag);
			int count = in[offset++];
			//printf("in[%d] %d \n",offset, count);
			int val = in[offset++];
			//printf("in[%d] %d \n",offset, val);

			//printf("count %d val: %d\n", count, val);
			for (int j =0; j < count; j ++)
			{
				//printf("decode RLE\n");
				out[out_offset++] = val;
			}
			
		}
		else
		{
			//printf("in[%d] %d \n",offset,  in[offset]);
			out[out_offset++] = in[offset-1];
		}
	}
}

void CSprite::SetCurrentPalette(INT8 pal)
{
	if (pal >= _num_pal)
		return;
	
	_current_pal = pal;
}

void CSprite::DrawModule(Graphic* g, INT32 module_id, float x, float y, INT32 transform, INT32 opacity, INT8 palette, int withpalette)
{
	
	INT32 w = _module_w[module_id]&0xFFFF;
	INT32 h = _module_h[module_id]&0xFFFF;
	CGame* game = CGame::_this;
	GraphicContext* gcontext = &CGame::_this->_graphic_context;
	//Use _fullscreen_clip at Draw2DTexture in GLESLib1.cpp instead
	/*
	switch (GAME()->_typeOtimizeDrawModule)
	{
	case 0: //normal optimize
		{
		float scalex = G()->GetCurrentScaleX();// > 1.0f ? G()->GetCurrentScaleX() : 1.0f;
		float scaley = G()->GetCurrentScaleY();// > 1.0f ? G()->GetCurrentScaleY() : 1.0f;
		if (scalex > 1.0f && scaley > 1.0f)
		{
			float ww = gcontext->Viewport.W;
			float hh = gcontext->Viewport.H;

			if ((x + w*scalex < gcontext->Viewport.X || x > ww + gcontext->Viewport.X || y + h*scaley < gcontext->Viewport.Y || y > hh + gcontext->Viewport.Y)
#ifdef USE_FREE_TRANSFORM
				&& (transform & _FREE_TRANSFORM) == 0
#endif
				)
			{
				return;
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

	if (palette == -1)
		palette = _current_pal;

	if (w == 0 || h == 0)
	{
		return;
	}

	INT32 tex_id = module_id;
#ifdef USE_FM_OPACITY
	int opacity_sys = g->GetCurrentOpacityPercent();
	if (opacity != 100)
		g->SetOpacity(opacity*opacity_sys/100);
#endif

#ifdef USE_MULTI_MODULE_TYPE
	if (_module_type[module_id] == _MODULE_TYPE_FILL_RECT)
	{
		g->SetColor(_module_color[module_id]);
		g->FillRect(x, y, w, h, true);
	}
#ifdef USE_DRAW_LINE
	else if(_module_type[module_id] == _MODULE_TYPE_RECT)
	{
		g->SetColor(_module_color[module_id]);
		g->DrawRect(x, y, w, h, true);
	}
#endif
	else if (_module_type[module_id] == _MODULE_TYPE_IMAGE)
#endif
	{
		if (_export_image_type == _EXPORT_TYPE_3D_TEXTURE)
		{
			tex_id = _module_img_id[module_id]&0xFF;
		}
		//if (_texture_image[tex_id] == 0)
		{
		//	return;
		}
		
		_uv[0] = _uv[2] = _uv[1] = _uv[3] = 0;
		float offset_clip  = 0.0f;
        //_texture_texcoord_delta = 5.0f;
		
		if (_export_image_type == _EXPORT_TYPE_3D_TEXTURE)
		{
			_uv[0] = _uv[2] = _module_x[module_id] + _texture_texcoord_delta;
			_uv[1] = _uv[3] = _module_y[module_id] + _texture_texcoord_delta;
			//x-=2;
			//y-=0;
		}
		
		if ((transform & _FLIP_H) != 0)
		{
			_uv[0] += (w - _texture_texcoord_delta);
		}
		else
		{
			_uv[2] += (w - _texture_texcoord_delta);
		}

		if ((transform & _FLIP_V) != 0)
		{
			_uv[1] += (h - _texture_texcoord_delta);
		}
		else
		{
			_uv[3] += (h - _texture_texcoord_delta);
		}
		
		//printf("\tw:%f \th:%f\n", (float)_texture_image_w[tex_id], (float)_texture_image_h[tex_id]);
		_uv[0] /= _texture_image_w[tex_id];
		_uv[1] /= _texture_image_h[tex_id];
		_uv[2] /= _texture_image_w[tex_id];
		_uv[3] /= _texture_image_h[tex_id];

		//_uv[1] = 1.0f - _uv[1];
		//_uv[3] = 1.0f - _uv[3];
	/*#ifdef USE_FM_ROTATION
		if (angle != 0)
		{
			g->SetRotate(angle, x + ((w*scale/100)>>1), y + ((h*scale/100)>>1));
		}
	#endif*/

	/*#ifdef USE_FM_SCALE
		if (scale != 100)
		{
			g->SetScale((float)scale/100, (float)scale/100, x, y);
			//x * 100 / scale;
			//y * 100 / scale;
		}
	#endif*/
		g->Draw2DTexture(x, y, w, h, _texture_image[withpalette][tex_id], _uv, 0, _has_alpha);

	/*#ifdef USE_FM_ROTATION
		if (angle != 0)
			g->ClearRotate();
	#endif*/
	}
#ifdef USE_FM_OPACITY
	if (opacity != 100)
		g->SetOpacity(opacity_sys);
#endif
/*#ifdef USE_FM_SCALE
	if (scale != 100)
		g->ClearScale();
#endif*/
}
void CSprite::DrawModule(Graphic* g, INT32 module_id, float x, float y,float scale_x, float scale_y, INT32 transform, INT32 opacity, INT8 palette, int withpalette)
{

	INT32 w = _module_w[module_id] & 0xFFFF;
	INT32 h = _module_h[module_id] & 0xFFFF;
	//Use _fullscreen_clip at Draw2DTexture in GLESLib1.cpp instead
	/*
	switch (GAME()->_typeOtimizeDrawModule)
	{
	case 0: //normal optimize
	{
		float scalex = G()->GetCurrentScaleX();// > 1.0f ? G()->GetCurrentScaleX() : 1.0f;
		float scaley = G()->GetCurrentScaleY();// > 1.0f ? G()->GetCurrentScaleY() : 1.0f;

		if (scalex > 1.0f && scaley > 1.0f)
		{
			if (x + w*scalex < 0 || x > CGame::_this->_device.ScreenSize.W || y + h*scaley < 0 || y > CGame::_this->_device.ScreenSize.H)
			{
				return;
			}
		}
	}
		break;
	case 1: //none optimize
		break;
	}
	*/

	if (palette == -1)
		palette = _current_pal;

	if (w == 0 || h == 0)
	{
		return;
	}

	INT32 tex_id = module_id;
#ifdef USE_FM_OPACITY
	int opacity_sys = g->GetCurrentOpacityPercent();
	if (opacity != 100)
		g->SetOpacity(opacity*opacity_sys / 100);
#endif

#ifdef USE_MULTI_MODULE_TYPE
	if (_module_type[module_id] == _MODULE_TYPE_FILL_RECT)
	{
		g->SetColor(_module_color[module_id]);
		g->FillRect(x, y, w, h, true);
	}
#ifdef USE_DRAW_LINE
	else if (_module_type[module_id] == _MODULE_TYPE_RECT)
	{
		g->SetColor(_module_color[module_id]);
		g->DrawRect(x, y, w, h, true);
	}
#endif
	else if (_module_type[module_id] == _MODULE_TYPE_IMAGE)
#endif
	{
		if (_export_image_type == _EXPORT_TYPE_3D_TEXTURE)
		{
			tex_id = _module_img_id[module_id] & 0xFF;
		}
		//if (_texture_image[tex_id] == 0)
		{
			//	return;
		}

		_uv[0] = _uv[2] = _uv[1] = _uv[3] = 0;
		float offset_clip = 0.0f;

		if (_export_image_type == _EXPORT_TYPE_3D_TEXTURE)
		{
			_uv[0] = _uv[2] = _module_x[module_id] + _texture_texcoord_delta;
			_uv[1] = _uv[3] = _module_y[module_id] + _texture_texcoord_delta;
			//x-=2;
			//y-=0;
		}

		if ((transform & _FLIP_H) != 0)
		{
			_uv[0] += (w - _texture_texcoord_delta);
		}
		else
		{
			_uv[2] += (w - _texture_texcoord_delta);
		}

		if ((transform & _FLIP_V) != 0)
		{
			_uv[1] += (h - _texture_texcoord_delta);
		}
		else
		{
			_uv[3] += (h - _texture_texcoord_delta);
		}

		_uv[0] /= _texture_image_w[tex_id];
		_uv[1] /= _texture_image_h[tex_id];
		_uv[2] /= _texture_image_w[tex_id];
		_uv[3] /= _texture_image_h[tex_id];

		
		//_uv[1] = 1.0f - _uv[1];
		//_uv[3] = 1.0f - _uv[3];
		/*#ifdef USE_FM_ROTATION
		if (angle != 0)
		{
		g->SetRotate(angle, x + ((w*scale/100)>>1), y + ((h*scale/100)>>1));
		}
		#endif*/

		/*#ifdef USE_FM_SCALE
		if (scale != 100)
		{
		g->SetScale((float)scale/100, (float)scale/100, x, y);
		//x * 100 / scale;
		//y * 100 / scale;
		}
		#endif*/

		g->Draw2DTexture(x, y, w, h, _texture_image[withpalette][tex_id], _uv, 0, _has_alpha);

		/*#ifdef USE_FM_ROTATION
		if (angle != 0)
		g->ClearRotate();
		#endif*/
	}
#ifdef USE_FM_OPACITY
	if (opacity != 100)
		g->SetOpacity(opacity_sys);
#endif
	/*#ifdef USE_FM_SCALE
	if (scale != 100)
	g->ClearScale();
	#endif*/
}
int CSprite::GetModuleOfFrame(int frame_id)
{
	INT32 fmodule_min = 0;
	INT32 fmodule_max = 0;
	
	if (frame_id < (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _frame_offset[frame_id + 1] - 1;
	}
	else if (frame_id == (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _num_fmodules - 1;
	}
	
	return (fmodule_max -fmodule_min+1);
}
void CSprite::DrawFrame(Graphic* g, INT32 frame_id, float x, float y, INT32 transform, INT8 palette, int withpalette, bool hasOverlay, int overlay_from, CSprite* sprite_add, int overlay_add)
{
	INT32 fmodule_min = 0;
	INT32 fmodule_max = 0;
	if (frame_id < (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _frame_offset[frame_id + 1] - 1;
	}
	else if (frame_id == (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _num_fmodules - 1;
	}
	
	if (transform == 0)
	{
		for (INT32 i = fmodule_min; i <= fmodule_max; i++)
		{
			//draw fmodule	
			g->CleanCurrentFlag();
		#ifdef USE_FREE_TRANSFORM
			if ((_fmodule_transf[i] & _FREE_TRANSFORM) != 0)
			{
				//g->CleanCurrentFlag();
				float xx = _fmodule_ox[i] + x;
				float yy = _fmodule_oy[i] + y;
				
				
				g->LoadModel(xx,yy, _fmodule_m11[i], _fmodule_m12[i], _fmodule_m21[i], _fmodule_m22[i]);
				if(_num_of_module_replay <= 0)
				{					
					
					
					if(hasOverlay && _fmodule_id[i] == overlay_from)
					{
						DrawModule(g,
							_fmodule_id[i],
							0,
							0,
							_fmodule_transf[i],
#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
#else
							100,
#endif	
							palette,
							withpalette
							);
						//printf("\n _fmodule_id[i] = %d, overlay_from = %d ", _fmodule_id[i], overlay_from);
						//g->LoadModel(xx,yy, _fmodule_m11[i], _fmodule_m12[i], _fmodule_m21[i], _fmodule_m22[i]);
						/*sprite_add->DrawModule(	g,
								overlay_add,
								0,
								100,
								_fmodule_transf[i],
								#ifdef USE_FM_OPACITY
								_fmodule_opacity[i],
								#else
								100,
								#endif	
								palette,
								withpalette
								);*/
						g->LoadModel(xx, yy, _fmodule_m11[i], _fmodule_m12[i], _fmodule_m21[i], _fmodule_m22[i]);
						sprite_add->DrawModule(	g,
							overlay_add,
							0,
							0,
							_fmodule_transf[i],
							#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
							#else
							100,
							#endif	
							palette,
							withpalette
							);
					}
					else
					{
						DrawModule(	g,
							_fmodule_id[i],
							0,
							0,
							_fmodule_transf[i],
							#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
							#else
							100,
							#endif	
							palette,
							withpalette
							);

					}
					
				}
				else
				{
					
					if(_module_replay_data[_fmodule_id[i]*3] >= 0)
					{						
						CGame::_this->GetSprite(_module_replay_data[_fmodule_id[i]*3+1])->DrawModule(	g,
								_module_replay_data[_fmodule_id[i]*3+2],
								0,
								0,
								_fmodule_transf[i],
								#ifdef USE_FM_OPACITY
								_fmodule_opacity[i],
								#else
								100,
								#endif	
								palette,
								withpalette
								);
						
						
					}
					else
					{
						DrawModule(	g,
							_fmodule_id[i],
							0,
							0,
							_fmodule_transf[i],
							#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
							#else
							100,
							#endif	
							palette,
							withpalette
							);
						
						if(hasOverlay && _fmodule_id[i] == overlay_from)
						{
							g->LoadModel(xx,yy, _fmodule_m11[i], _fmodule_m12[i], _fmodule_m21[i], _fmodule_m22[i]);
							sprite_add->DrawModule(	g,
									overlay_add,
									0,
									0,
									_fmodule_transf[i],
									#ifdef USE_FM_OPACITY
									_fmodule_opacity[i],
									#else
									100,
									#endif	
									palette,
									withpalette
									);
						}
					
					}
				}
			}
			else
			{
				if(_num_of_module_replay <= 0)
				{
					DrawModule(	g,
						_fmodule_id[i],
						_fmodule_x[i] + x,
						_fmodule_y[i] + y,
						_fmodule_transf[i],
						#ifdef USE_FM_OPACITY
						_fmodule_opacity[i],
						#else
						100,
						#endif	
						palette,
						withpalette
						);
				}
				else
				{
					if(_module_replay_data[_fmodule_id[i]*3] >= 0)
					{
						CGame::_this->GetSprite(_module_replay_data[_fmodule_id[i]*3+1])->DrawModule(	g,
							_module_replay_data[_fmodule_id[i]*3+2],
							_fmodule_x[i] + x,
							_fmodule_y[i] + y,
							_fmodule_transf[i],
							#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
							#else
							100,
							#endif	
							palette,
							withpalette
							);
						
						if(hasOverlay && _fmodule_id[i] == overlay_from)
						{
							sprite_add->DrawModule(g,
										overlay_add,
										_fmodule_x[i] + x,
										_fmodule_y[i] + y,
										_fmodule_transf[i],
										#ifdef USE_FM_OPACITY
										_fmodule_opacity[i],
										#else
										100,
										#endif	
										palette,
										withpalette
									);
						}
						
					}
					else
					{
						DrawModule(	g,
							_fmodule_id[i],
							_fmodule_x[i] + x,
							_fmodule_y[i] + y,
							_fmodule_transf[i],
							#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
							#else
							100,
							#endif	
							palette,
							withpalette
							);
							if(hasOverlay && _fmodule_id[i] == overlay_from)
							{
								sprite_add->DrawModule(g,
											overlay_add,
											_fmodule_x[i] + x,
											_fmodule_y[i] + y,
											_fmodule_transf[i],
											#ifdef USE_FM_OPACITY
											_fmodule_opacity[i],
											#else
											100,
											#endif	
											palette,
											withpalette
										);
							}

					}
				}
				
			}
		#else	
		{
			DrawModule(	g,
						_fmodule_id[i],
						_fmodule_x[i] + x,
						_fmodule_y[i] + y,
						_fmodule_transf[i],
					/*#ifdef USE_FM_ROTATION
						_fmodule_angle[i],
					#else
						0,
					#endif*/
					#ifdef USE_FM_OPACITY
						_fmodule_opacity[i],
					#else
						100,
					#endif
					/*#ifdef USE_FM_SCALE
						_fmodule_scale[i],
					#else
						100,
					#endif*/
						palette,
						withpalette
						);
		}
			#endif
		}
	}
	else
	{		

		for (INT32 i = fmodule_min; i <= fmodule_max; i++)
		{
			//draw fmodule
			INT32 module = _fmodule_id[i];
			INT32 fmodule_transf = _fmodule_transf[i];
		#ifdef USE_FM_ROTATION
			INT32 fmodule_angle = _fmodule_angle[i];
		#endif
		#ifdef USE_FM_OPACITY
			INT32 fmodule_opacity = _fmodule_opacity[i];
		#endif
		#ifdef USE_FM_SCALE
			INT32 fmodule_scale = _fmodule_scale[i];
		#endif
			INT32 ox = _fmodule_x[i];
			INT32 oy = _fmodule_y[i];
			INT32 xx = x;
			INT32 yy = y;
			
			
			if ((transform & _FLIP_H) != 0)
			{
				fmodule_transf ^= _FLIP_H;
				xx -= (ox + _module_w[module]&0xFFFF);
			}
			else
			{
				xx += ox;
			}
			
			if ((transform & _FLIP_V) != 0)
			{
				fmodule_transf ^= _FLIP_V;
				//#yy -= (oy + _module_h[module]&0xFFFF);
			}
			else
			{
				yy += oy;
			}
			#ifdef USE_FREE_TRANSFORM
			if ((_fmodule_transf[i] & _FREE_TRANSFORM) != 0)
			{
				g->CleanCurrentFlag();
				g->LoadModel(_fmodule_ox[i] * g->GetCurrentScaleX() + xx, _fmodule_oy[i] * g->GetCurrentScaleY() + yy, _fmodule_m11[i], _fmodule_m12[i], _fmodule_m21[i], _fmodule_m22[i]);
				if(_num_of_module_replay <= 0)
				{
					DrawModule(	g,
							module,
							0,
							0,
							fmodule_transf,
							#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
							#else
							100,
							#endif	
							palette,
							withpalette
							);	
						if(hasOverlay && _fmodule_id[i] == overlay_from)
						{
							g->LoadModel(xx,yy, _fmodule_m11[i], _fmodule_m12[i], _fmodule_m21[i], _fmodule_m22[i]);
							DrawModule(	g,
							overlay_add,
							0,
							0,
							fmodule_transf,
							#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
							#else
							100,
							#endif	
							palette,
							withpalette
							);	
						}
				}
				else
				{
					if(_module_replay_data[module*3] >= 0)
					{
						
						CGame::_this->GetSprite(_module_replay_data[module*3+1])->DrawModule(	g,
								_module_replay_data[module*3+2],
								0,
								0,
								fmodule_transf,
								#ifdef USE_FM_OPACITY
								_fmodule_opacity[i],
								#else
								100,
								#endif	
								palette,
								withpalette
								);	
							if(hasOverlay && _fmodule_id[i] == overlay_from)
							{
								g->LoadModel(xx,yy, _fmodule_m11[i], _fmodule_m12[i], _fmodule_m21[i], _fmodule_m22[i]);
								DrawModule(	g,
								overlay_add,
								0,
								0,
								fmodule_transf,
								#ifdef USE_FM_OPACITY
								_fmodule_opacity[i],
								#else
								100,
								#endif	
								palette,
								withpalette
								);	
							}
					}
					else
					{
						DrawModule(	g,
							module,
							0,
							0,
							fmodule_transf,
							#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
							#else
							100,
							#endif	
							palette,
							withpalette
							);	
							if(hasOverlay && _fmodule_id[i] == overlay_from)
							{
								g->LoadModel(xx,yy, _fmodule_m11[i], _fmodule_m12[i], _fmodule_m21[i], _fmodule_m22[i]);
								DrawModule(	g,
								overlay_add,
								0,
								0,
								fmodule_transf,
								#ifdef USE_FM_OPACITY
								_fmodule_opacity[i],
								#else
								100,
								#endif	
								palette,
								withpalette
								);	
							}
					}
				}
				
			}
			else
			{
				if(_num_of_module_replay <= 0)
				{
					DrawModule(	g,
							module,
							xx,
							yy,
							fmodule_transf,
							#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
							#else
							100,
							#endif	
							palette,
							withpalette
							);	
						if(hasOverlay && _fmodule_id[i] == overlay_from)
						{
							DrawModule(	g,
							overlay_add,
							xx,
							yy,
							fmodule_transf,
							#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
							#else
							100,
							#endif	
							palette,
							withpalette
							);	
						}
				}
				else
				{
					if(_module_replay_data[module*3] >= 0)
					{
						
						CGame::_this->GetSprite(_module_replay_data[module*3+1])->DrawModule(	g,
							_module_replay_data[module*3+1],
							xx,
							yy,
							fmodule_transf,
							#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
							#else
							100,
							#endif	
							palette,
							withpalette
							);	
							if(hasOverlay && _fmodule_id[i] == overlay_from)
							{
								DrawModule(	g,
								overlay_add,
								xx,
								yy,
								fmodule_transf,
								#ifdef USE_FM_OPACITY
								_fmodule_opacity[i],
								#else
								100,
								#endif	
								palette,
								withpalette
								);	
							}
					}
					else
					{
						DrawModule(	g,
							module,
							xx,
							yy,
							fmodule_transf,
							#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
							#else
							100,
							#endif	
							palette,
							withpalette
							);	
						if(hasOverlay && _fmodule_id[i] == overlay_from)
						{
							DrawModule(	g,
							overlay_add,
							xx,
							yy,
							fmodule_transf,
							#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
							#else
							100,
							#endif	
							palette,
							withpalette
							);	
						}

					}
				}
				
			}
			#else
			DrawModule(	g,
						module,
						xx,
						yy,
						fmodule_transf,
					/*#ifdef USE_FM_ROTATION
						fmodule_angle,
					#else
						0,
					#endif*/
					#ifdef USE_FM_OPACITY
						fmodule_opacity,
					#else
						100,
					#endif
					/*#ifdef USE_FM_SCALE
						fmodule_scale,
					#else
						0,
					#endif*/
						palette,
						withpalette
						);
			#endif
		}
	}
	
}

void CSprite::DrawFrame(Graphic* g, INT32 frame_id, float x, float y, float scale_x, float scale_y, INT32 transform, INT8 palette, int withpalette, bool hasOverlay, int overlay_from, CSprite* sprite_add, int overlay_add)
{
	INT32 fmodule_min = 0;
	INT32 fmodule_max = 0;
	if (frame_id < (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _frame_offset[frame_id + 1] - 1;
	}
	else if (frame_id == (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _num_fmodules - 1;
	}

	if (transform == 0)
	{
		for (INT32 i = fmodule_min; i <= fmodule_max; i++)
		{
			//draw fmodule	
			g->CleanCurrentFlag();
#ifdef USE_FREE_TRANSFORM
			if ((_fmodule_transf[i] & _FREE_TRANSFORM) != 0)
			{
				//g->CleanCurrentFlag();
				float xx = (_fmodule_ox[i] + x) * scale_x;
				float yy = (_fmodule_oy[i] + y) * scale_y;

				g->LoadModel(xx, yy, _fmodule_m11[i], _fmodule_m12[i], _fmodule_m21[i], _fmodule_m22[i]);
				if (_num_of_module_replay <= 0)
				{


					if (hasOverlay && _fmodule_id[i] == overlay_from)
					{
						DrawModule(g,
							_fmodule_id[i],
							0,
							0,
							scale_x,
							scale_y,
							_fmodule_transf[i],
#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
#else
							100,
#endif	
							palette,
							withpalette
							);
						//printf("\n _fmodule_id[i] = %d, overlay_from = %d ", _fmodule_id[i], overlay_from);
						//g->LoadModel(xx,yy, _fmodule_m11[i], _fmodule_m12[i], _fmodule_m21[i], _fmodule_m22[i]);
						/*sprite_add->DrawModule(	g,
						overlay_add,
						0,
						100,
						_fmodule_transf[i],
						#ifdef USE_FM_OPACITY
						_fmodule_opacity[i],
						#else
						100,
						#endif
						palette,
						withpalette
						);*/
						g->LoadModel(xx, yy, _fmodule_m11[i], _fmodule_m12[i], _fmodule_m21[i], _fmodule_m22[i]);
						sprite_add->DrawModule(g,
							overlay_add,
							0,
							0,
							scale_x,
							scale_y,
							_fmodule_transf[i],
#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
#else
							100,
#endif	
							palette,
							withpalette
							);
					}
					else
					{
						DrawModule(g,
							_fmodule_id[i],
							0,
							0,
							scale_x,
							scale_y,
							_fmodule_transf[i],
#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
#else
							100,
#endif	
							palette,
							withpalette
							);

					}

				}
				else
				{

					if (_module_replay_data[_fmodule_id[i] * 3] >= 0)
					{
						CGame::_this->GetSprite(_module_replay_data[_fmodule_id[i] * 3 + 1])->DrawModule(g,
							_module_replay_data[_fmodule_id[i] * 3 + 2],
							0,
							0,
							scale_x,
							scale_y,
							_fmodule_transf[i],
#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
#else
							100,
#endif	
							palette,
							withpalette
							);

						/*if(hasOverlay && _fmodule_id[i] == overlay_from)
						{

						g->LoadModel(xx,yy, _fmodule_m11[i], _fmodule_m12[i], _fmodule_m21[i], _fmodule_m22[i]);
						sprite_add->DrawModule(	g,
						overlay_add,
						0,
						0,
						_fmodule_transf[i],
						#ifdef USE_FM_OPACITY
						_fmodule_opacity[i],
						#else
						100,
						#endif
						palette,
						withpalette
						);
						}*/

					}
					else
					{
						DrawModule(g,
							_fmodule_id[i],
							0,
							0,
							scale_x,
							scale_y,
							_fmodule_transf[i],
#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
#else
							100,
#endif	
							palette,
							withpalette
							);

						if (hasOverlay && _fmodule_id[i] == overlay_from)
						{
							g->LoadModel(xx, yy, _fmodule_m11[i], _fmodule_m12[i], _fmodule_m21[i], _fmodule_m22[i]);
							sprite_add->DrawModule(g,
								overlay_add,
								0,
								0,
								scale_x,
								scale_y,
								_fmodule_transf[i],
#ifdef USE_FM_OPACITY
								_fmodule_opacity[i],
#else
								100,
#endif	
								palette,
								withpalette
								);
						}

					}
				}
			}
			else
			{
				if (_num_of_module_replay <= 0)
				{
					DrawModule(g,
						_fmodule_id[i],
						(_fmodule_x[i]*scale_x) + x,
						(_fmodule_y[i]*scale_y) + y,
						scale_x,
						scale_y,
						_fmodule_transf[i],
#ifdef USE_FM_OPACITY
						_fmodule_opacity[i],
#else
						100,
#endif	
						palette,
						withpalette
						);
				}
				else
				{
					if (_module_replay_data[_fmodule_id[i] * 3] >= 0)
					{
						CGame::_this->GetSprite(_module_replay_data[_fmodule_id[i] * 3 + 1])->DrawModule(g,
							_module_replay_data[_fmodule_id[i] * 3 + 2],
							(_fmodule_x[i]*scale_x) + x,
							(_fmodule_y[i]*scale_y) + y,
							scale_x,
							scale_y,
							_fmodule_transf[i],
#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
#else
							100,
#endif	
							palette,
							withpalette
							);

						if (hasOverlay && _fmodule_id[i] == overlay_from)
						{
							sprite_add->DrawModule(g,
								overlay_add,
								(_fmodule_x[i]*scale_x) + x,
								(_fmodule_y[i]*scale_y) + y,
								scale_x,
								scale_y,
								_fmodule_transf[i],
#ifdef USE_FM_OPACITY
								_fmodule_opacity[i],
#else
								100,
#endif	
								palette,
								withpalette
								);
						}

					}
					else
					{
						DrawModule(g,
							_fmodule_id[i],
							(_fmodule_x[i]*scale_x) + x,
							(_fmodule_y[i]*scale_y) + y,
							scale_x,
							scale_y,
							_fmodule_transf[i],
#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
#else
							100,
#endif	
							palette,
							withpalette
							);
						if (hasOverlay && _fmodule_id[i] == overlay_from)
						{
							sprite_add->DrawModule(g,
								overlay_add,
								(_fmodule_x[i]*scale_x) + x,
								(_fmodule_y[i]*scale_y) + y,
								scale_x,
								scale_y,
								_fmodule_transf[i],
#ifdef USE_FM_OPACITY
								_fmodule_opacity[i],
#else
								100,
#endif	
								palette,
								withpalette
								);
						}

					}
				}

			}
#else	
			{
				DrawModule(g,
					_fmodule_id[i],
					(_fmodule_x[i]*scale_x) + x,
					(_fmodule_y[i]*scale_y) + y,
					scale_x,
					scale_y,
					_fmodule_transf[i],
					/*#ifdef USE_FM_ROTATION
					_fmodule_angle[i],
					#else
					0,
					#endif*/
#ifdef USE_FM_OPACITY
					_fmodule_opacity[i],
#else
					100,
#endif
					/*#ifdef USE_FM_SCALE
					_fmodule_scale[i],
					#else
					100,
					#endif*/
					palette,
					withpalette
					);
			}
#endif
		}
	}
	else
	{

		for (INT32 i = fmodule_min; i <= fmodule_max; i++)
		{
			//draw fmodule
			INT32 module = _fmodule_id[i];
			INT32 fmodule_transf = _fmodule_transf[i];
#ifdef USE_FM_ROTATION
			INT32 fmodule_angle = _fmodule_angle[i];
#endif
#ifdef USE_FM_OPACITY
			INT32 fmodule_opacity = _fmodule_opacity[i];
#endif
#ifdef USE_FM_SCALE
			INT32 fmodule_scale = _fmodule_scale[i];
#endif
			INT32 ox = _fmodule_x[i] * scale_x;
			INT32 oy = _fmodule_y[i] * scale_y;
			INT32 xx = x;
			INT32 yy = y;


			if ((transform & _FLIP_H) != 0)
			{
				fmodule_transf ^= _FLIP_H;
				xx -= (ox + _module_w[module] & 0xFFFF);
			}
			else
			{
				xx += ox;
			}

			if ((transform & _FLIP_V) != 0)
			{
				fmodule_transf ^= _FLIP_V;
				yy -= (oy + _module_h[module] & 0xFFFF);
			}
			else
			{
				yy += oy;
			}
#ifdef USE_FREE_TRANSFORM
			if ((_fmodule_transf[i] & _FREE_TRANSFORM) != 0)
			{
				g->CleanCurrentFlag();
				g->LoadModel(_fmodule_ox[i] * g->GetCurrentScaleX() + xx, _fmodule_oy[i] * g->GetCurrentScaleY() + yy, _fmodule_m11[i], _fmodule_m12[i], _fmodule_m21[i], _fmodule_m22[i]);
				if (_num_of_module_replay <= 0)
				{
					DrawModule(g,
						module,
						0,
						0,
						scale_x,
						scale_y,
						fmodule_transf,
#ifdef USE_FM_OPACITY
						_fmodule_opacity[i],
#else
						100,
#endif	
						palette,
						withpalette
						);
					if (hasOverlay && _fmodule_id[i] == overlay_from)
					{
						g->LoadModel(xx, yy, _fmodule_m11[i], _fmodule_m12[i], _fmodule_m21[i], _fmodule_m22[i]);
						DrawModule(g,
							overlay_add,
							0,
							0,
							scale_x,
							scale_y,
							fmodule_transf,
#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
#else
							100,
#endif	
							palette,
							withpalette
							);
					}
				}
				else
				{
					if (_module_replay_data[module * 3] >= 0)
					{

						CGame::_this->GetSprite(_module_replay_data[module * 3 + 1])->DrawModule(g,
							_module_replay_data[module * 3 + 2],
							0,
							0,
							scale_x,
							scale_y,
							fmodule_transf,
#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
#else
							100,
#endif	
							palette,
							withpalette
							);
						if (hasOverlay && _fmodule_id[i] == overlay_from)
						{
							g->LoadModel(xx, yy, _fmodule_m11[i], _fmodule_m12[i], _fmodule_m21[i], _fmodule_m22[i]);
							DrawModule(g,
								overlay_add,
								0,
								0,
								scale_x,
								scale_y,
								fmodule_transf,
#ifdef USE_FM_OPACITY
								_fmodule_opacity[i],
#else
								100,
#endif	
								palette,
								withpalette
								);
						}
					}
					else
					{
						DrawModule(g,
							module,
							0,
							0,
							scale_x,
							scale_y,
							fmodule_transf,
#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
#else
							100,
#endif	
							palette,
							withpalette
							);
						if (hasOverlay && _fmodule_id[i] == overlay_from)
						{
							g->LoadModel(xx, yy, _fmodule_m11[i], _fmodule_m12[i], _fmodule_m21[i], _fmodule_m22[i]);
							DrawModule(g,
								overlay_add,
								0,
								0,
								scale_x,
								scale_y,
								fmodule_transf,
#ifdef USE_FM_OPACITY
								_fmodule_opacity[i],
#else
								100,
#endif	
								palette,
								withpalette
								);
						}
					}
				}

			}
			else
			{
				if (_num_of_module_replay <= 0)
				{
					DrawModule(g,
						module,
						xx,
						yy,
						scale_x,
						scale_y,
						fmodule_transf,
#ifdef USE_FM_OPACITY
						_fmodule_opacity[i],
#else
						100,
#endif	
						palette,
						withpalette
						);
					if (hasOverlay && _fmodule_id[i] == overlay_from)
					{
						DrawModule(g,
							overlay_add,
							xx,
							yy,
							scale_x,
							scale_y,
							fmodule_transf,
#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
#else
							100,
#endif	
							palette,
							withpalette
							);
					}
				}
				else
				{
					if (_module_replay_data[module * 3] >= 0)
					{

						CGame::_this->GetSprite(_module_replay_data[module * 3 + 1])->DrawModule(g,
							_module_replay_data[module * 3 + 1],
							xx,
							yy,
							scale_x,
							scale_y,
							fmodule_transf,
#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
#else
							100,
#endif	
							palette,
							withpalette
							);
						if (hasOverlay && _fmodule_id[i] == overlay_from)
						{
							DrawModule(g,
								overlay_add,
								xx,
								yy,
								scale_x,
								scale_y,
								fmodule_transf,
#ifdef USE_FM_OPACITY
								_fmodule_opacity[i],
#else
								100,
#endif	
								palette,
								withpalette
								);
						}
					}
					else
					{
						DrawModule(g,
							module,
							xx,
							yy,
							scale_x,
							scale_y,
							fmodule_transf,
#ifdef USE_FM_OPACITY
							_fmodule_opacity[i],
#else
							100,
#endif	
							palette,
							withpalette
							);
						if (hasOverlay && _fmodule_id[i] == overlay_from)
						{
							DrawModule(g,
								overlay_add,
								xx,
								yy,
								scale_x,
								scale_y,
								fmodule_transf,
#ifdef USE_FM_OPACITY
								_fmodule_opacity[i],
#else
								100,
#endif	
								palette,
								withpalette
								);
						}

					}
				}

			}
#else
			DrawModule(g,
				module,
				xx,
				yy,
				scale_x,
				scale_y,
				fmodule_transf,
				/*#ifdef USE_FM_ROTATION
				fmodule_angle,
				#else
				0,
				#endif*/
#ifdef USE_FM_OPACITY
				fmodule_opacity,
#else
				100,
#endif
				/*#ifdef USE_FM_SCALE
				fmodule_scale,
				#else
				0,
				#endif*/
				palette,
				withpalette
				);
#endif
		}
	}

}
	
void CSprite::DrawCurrentAFrame(Graphic* g, INT32 anim_id, float x, float y, INT32 transform, INT8 palette, bool hasOverlay, int overlay_from, CSprite* sprite_add, int overlay_add)
{
	if (_current_anim != anim_id)
	{
		_current_anim = anim_id;
		_min_aframes = _anim_offset[_current_anim];
		
		if (_current_anim == (_num_anims - 1))
		{
			_max_aframes = _num_aframes - 1;
		}
		else
		{
			_max_aframes = _anim_offset[_current_anim + 1] - 1;
		}
		
		_current_aframes = _min_aframes;
		_current_aframes_time = _aframe_time[_current_aframes];
		_is_current_anim_stop = false;
	}
	
	if (!_is_current_anim_stop)
	{
		INT32 aframe_transf = _aframe_transf[_current_aframes];
		INT32 xx = _aframe_x[_current_aframes];
		INT32 yy = _aframe_y[_current_aframes];
		
		if (transform != 0)
		{
			if ((transform & _FLIP_H) != 0)
			{
				aframe_transf ^= _FLIP_H;
			}
			
			if ((transform & _FLIP_V) != 0)
			{
				aframe_transf ^= _FLIP_V;
			}
		}

		DrawFrame(	g, 
					_aframe_id[_current_aframes], 
					xx + x, 
					yy + y, 
					aframe_transf, palette,0,
							hasOverlay, 
							overlay_from,
							sprite_add,
							overlay_add);
	}
}
void CSprite::DrawAFrame(Graphic*g, INT32 aframe, float x, float y, INT32 transform){
	
	INT32 aframe_transf = _aframe_transf[aframe];
	INT32 xx = _aframe_x[aframe];
	INT32 yy = _aframe_y[aframe];

	if (transform != 0)
	{
		if ((transform & _FLIP_H) != 0)
		{
			aframe_transf ^= _FLIP_H;
		}

		if ((transform & _FLIP_V) != 0)
		{
			aframe_transf ^= _FLIP_V;
		}
	}

	DrawFrame(g,
		_aframe_id[aframe],
		xx + x,
		yy + y,
		aframe_transf);
}
void CSprite::DrawCurrentAFrame(Graphic* g, INT32 anim_id, float x, float y)
{
	if (_current_anim != anim_id)
	{
		_current_anim = anim_id;
		_min_aframes = _anim_offset[_current_anim];
		if (_current_anim == (_num_anims - 1))
		{
			_max_aframes = _num_aframes - 1;
		}
		else
		{
			_max_aframes = _anim_offset[_current_anim + 1] - 1;
		}
		_current_aframes = _min_aframes;
		_current_aframes_time = _aframe_time[_current_aframes];
		_is_current_anim_stop = false;
	}
	
	if (!_is_current_anim_stop)
	{
		DrawFrame(	g, 
					_aframe_id[_current_aframes], 
					_aframe_x[_current_aframes] + x, 
					_aframe_y[_current_aframes] + y, 
					_aframe_transf[_current_aframes]);
	}
}

void CSprite::SetAnim(INT32 anim, INT32 aframe, INT32 cur_aframe_time, bool is_loop)
{
	//set current anim
	_current_anim = anim;
	
	//get min max aframe
	_min_aframes = _anim_offset[_current_anim];
	if (_current_anim == (_num_anims - 1))
	{
		_max_aframes = _num_aframes - 1;
	}
	else
	{
		_max_aframes = _anim_offset[_current_anim + 1] - 1;
	}
	
	//set aframe and aframe time
	_current_aframes = aframe;
	_current_aframes_time = cur_aframe_time;
	
	_is_current_anim_stop = false;
	//check is anim stop
	if (_current_aframes_time == 0)
	{
		_current_aframes++;
		
		if (_current_aframes <= _max_aframes)
		{
			_current_aframes_time = _aframe_time[_current_aframes];
		}
		else if (is_loop)
		{
			_current_aframes = _min_aframes;
			_current_aframes_time = _aframe_time[_current_aframes];
		}
		else
		{
			_is_current_anim_stop = true;
		}
	}
}

INT32 CSprite::GetFirstAFrameID(INT32 anim)
{
	//get min aframe
	return _anim_offset[anim]&0xFFFF;
}

INT32 CSprite::GetFirstAFrameTime(INT32 anim)
{
	INT32 aframe = _anim_offset[anim]&0xFFFF;
	return _aframe_time[aframe];
}

INT32 CSprite::GetTotalAnimTime(INT32 anim)
{
	if (anim < 0 || anim >= _num_anims)
	{
		return 0;
	}
	
	//get min max aframe
	int min_aframes = _anim_offset[anim];
	int max_aframes = 0;
	if (anim == (_num_anims - 1))
	{
		max_aframes = _num_aframes - 1;
	}
	else
	{
		max_aframes = _anim_offset[anim + 1] - 1;
	}
	
	int total_anim_time = 0;
	for (int i = min_aframes; i <= max_aframes; i++)
	{
		total_anim_time += _aframe_time[i];
	}
	
	return total_anim_time;
}
	
void CSprite::NextAFrame(bool is_loop)
{	
	_current_aframes_time--;
	
	if (_current_aframes_time == 0)
	{
		_current_aframes++;
		
		if (_current_aframes <= _max_aframes)
		{
			_current_aframes_time = _aframe_time[_current_aframes];
		}
		else if (is_loop)
		{
			_current_aframes = _min_aframes;
			_current_aframes_time = _aframe_time[_current_aframes];
		}
		else
		{
			_is_current_anim_stop = true;
		}
	}
}
		
bool CSprite::IsAnimStop(INT32 anim_id)
{
	if (_current_anim != anim_id)
	{
		return true;
	}
	
	return _is_current_anim_stop;
}
	
bool CSprite::IsInside(int x, int y, INT32 aframe) // check if the point is inside current frame
{	
	INT32 transform = _aframe_transf[aframe];
	INT32 xx = _aframe_x[aframe];
	INT32 yy = _aframe_y[aframe];

	INT32 frame_id = _aframe_id[aframe];
	INT32 fmodule_min = 0;
	INT32 fmodule_max = 0;
	
	if (frame_id < (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _frame_offset[frame_id + 1] - 1;
	}
	else if (frame_id == (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _num_fmodules - 1;
	}	
	for (INT32 i = fmodule_min; i <= fmodule_max; i++)
	{
		INT32 mx = _fmodule_x[i] + xx;
		INT32 my = _fmodule_y[i] + yy;
		INT32 mi = _fmodule_id[i];
		INT32 mw = _module_w[mi]&0xFFFF;
		INT32 mh = _module_h[mi]&0xFFFF;
		if( x>mx && x<mx+mw && y>my && y<my+mh )
			return true;
	}
	return false;
}

//-----------------------------------------------------------------
// GetAFCollisionCircle: get a boundary circle of module which collide the circle (x, y, r)
// ricky.nguyen added
// 28,June, 2009
// @param : aframe - aframe which used for checking
// @param : (x, y, r): define a circle which used for checking
//-----------------------------------------------------------------
int* CSprite::GetAFCollisionCircle(int aframe, int x, int y, int r)
{
	int R = 0;
	int Ox = 0;
	int Oy = 0;

	INT32 transform = _aframe_transf[aframe];
	INT32 xx = _aframe_x[aframe];
	INT32 yy = _aframe_y[aframe];

	INT32 frame_id = _aframe_id[aframe];
	INT32 fmodule_min = 0;
	INT32 fmodule_max = 0;
	
	if (frame_id < (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _frame_offset[frame_id + 1] - 1;
	}
	else if (frame_id == (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _num_fmodules - 1;
	}	
	for (INT32 i = fmodule_min; i <= fmodule_max; i++)
	{
		INT32 mx = _fmodule_x[i] + xx;
		INT32 my = _fmodule_y[i] + yy;
		INT32 mi = _fmodule_id[i];
		INT32 mw = _module_w[mi]&0xFFFF;
		INT32 mh = _module_h[mi]&0xFFFF;
		
		Ox = mx + (mw>>1);
		Oy = my + (mh>>1);
		R = ((mw>mh)?(mw):(mh))>>1;
		
		int dx = Ox - x;
		int dy = Oy - y;
		int D = dx*dx + dy*dy;
		if (D < (r + R)*(r + R))
		{
			int *circle = new int[3];
			circle[0] = Ox;
			circle[1] = Oy;
			circle[2] = R;
			return circle;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------
// GetAFBoundaryCircle: get a boundary circle a AFrame
// ricky.nguyen added
// 28,June, 2009
// @param : aframe - aframe which used for checking
//-----------------------------------------------------------------
int* CSprite::GetAFBoundaryCircle(int aframe)
{
	int _x1 = 99999;
	int _y1 = 99999;
	int _x2 = -99999;
	int _y2 = -99999;

	INT32 transform = _aframe_transf[aframe];
	INT32 xx = _aframe_x[aframe];
	INT32 yy = _aframe_y[aframe];

	INT32 frame_id = _aframe_id[aframe];
	INT32 fmodule_min = 0;
	INT32 fmodule_max = 0;
	
	if (frame_id < (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _frame_offset[frame_id + 1] - 1;
	}
	else if (frame_id == (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _num_fmodules - 1;
	}	
	for (INT32 i = fmodule_min; i <= fmodule_max; i++)
	{
		INT32 mx = _fmodule_x[i] + xx;
		INT32 my = _fmodule_y[i] + yy;
		INT32 mi = _fmodule_id[i];
		INT32 mw = _module_w[mi]&0xFFFF;
		INT32 mh = _module_h[mi]&0xFFFF;
		
		if (_x1 > mx)
		{
			_x1 = mx;
		}
		if (_y1 > my)
		{
			_y1 = my;
		}
		if (_x2 < mx + mw)
		{
			_x2 = mx + mw;
		}
		if (_y2 < my + mh)
		{
			_y2 = my + mh;
		}
	}
	int *circle = new int[3];
	
	int dx = _x2 - _x1;
	int dy = _y2 - _y1;
	
	circle[0] = (dx>>1) + _x1;
	circle[1] = (dy>>1) + _y1;
	
	int __r1 = (dx>0)?dx:-dx;
	int __r2 = (dy>0)?dy:-dy;
	circle[2] = ((__r1 > __r2)?__r1:__r2)>>1;
	return circle;
}

void CSprite::CalcPage(INT32 data[], UNICHAR* text, INT32 length, INT32 w)
{
	INT32 num_of_line = 0;


	INT32 c;
	INT32 width = 0;
	INT32 start_off = 0;
	INT32 end_off = 0;

	INT32 blockwidth = 0;
	INT32 start_block = 0;
	INT32 end_block = 0;
	
	for (INT32 i = 0; i < length; i++)
	{
		c = text[i]&0xFFFF;
		//printf("%c", c);
		if(c > 7000)c-=7000;
		if(c-33 < 0)
		{
			c = 33;
		}
		int fmodule_min = _frame_offset[c-33];
		if (c < 33)
		{
			fmodule_min = _frame_offset[0];
			
		}
		blockwidth += _fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]]&0xFFFF) + _char_spacing;
		if (c == ' ' || c == '|' || i >= length-1)
		{	
			width += blockwidth;
			if (c == '|')
			{
				data[1 + num_of_line*2 ] = start_off;
				data[1 + num_of_line*2 + 1] = i - start_off;
				num_of_line++;
				width = 0;
				start_off = i+1;
			}
			else if (width >= w )
			{
				
				data[1 + num_of_line*2 ] = start_off;
				data[1 + num_of_line*2 + 1] = start_block - start_off;
				
				num_of_line++;
				width = blockwidth;
				start_off = start_block;
			}
			
			start_block = i+1;
			blockwidth = 0;
		}
	}

	// last line
	data[1 + num_of_line*2 ] = start_off;
	data[1 + num_of_line*2 + 1] = length - start_off;

	data[0] = num_of_line+1;
}

void CSprite::DrawPage(Graphic* g, UNICHAR* text, INT32 data[], float x, float y, INT32 anchor)
{
	INT32 num_of_line = data[0];
	float yy = y;

	if ((anchor&_VCENTER) != 0)
	{
		yy -= ((_module_h[0]&0xFFFF)* num_of_line + _line_spacing * (num_of_line-1) )/2;
	}


	for (INT32 i = 0; i<num_of_line; i++ )
	{
		//printf("get  %d  %d \n", data[1+i*2], data[2+i*2]);
		DrawText(g, text, data[1+i*2], data[2+i*2], x, yy, anchor);
		yy += (_module_h[0]&0xFFFF) + _line_spacing;
	}

}
//hao.ly add TEXT color mask
#ifdef USE_COLOR_MASK
void CSprite::ResetTextColorMask()
{
	for (int i = 0; i < TOTAL_TEXT_MASK_SUPPORT; i++)
	{
		_maskIndex[i] = -1;
		_maskColor[i] = 0x00FFFFFF;
	}
	_colorMaskIndex = 0;
}
#endif
//hao.ly add
void CSprite::DrawAnsiTextPageFormat(Graphic* g, float x, float y, int w, INT32 anchor, const char* text)
{
#ifdef USE_COLOR_MASK
	ResetTextColorMask();
#endif
	int maskIndex = 0;
	
	int length = strlen(text);
	int index = 0;
	for (int i = 0; i < length; i++)
	{
#ifdef USE_COLOR_MASK
		if (text[i] == '<' && i <= length - 3 && text[i+2] == '>')
		{
			if (maskIndex < 10)
			{
				_maskIndex[maskIndex] = index;
				if (text[i+1] == 'C')
				{
					_maskColor[maskIndex] = 0xFFFFFFFF;
				}
				else if (text[i+1] == 'R')
				{
					_maskColor[maskIndex] = 0xFFFF0000;
				}
				else if (text[i+1] == 'G')
				{
					_maskColor[maskIndex] = 0xFF0000FF;
				}
				else if (text[i+1] == 'B')
				{
					_maskColor[maskIndex] = 0xFF00FF00;
				}
				else if (text[i+1] == 'Y')
				{
					_maskColor[maskIndex] = 0xFFF4FF77;
				}
				else if (text[i+1] == 'P')
				{
					_maskColor[maskIndex] = 0xFFFF4848;
				}
				else if (text[i+1] == 'D')
				{
					_maskColor[maskIndex] = 0xFF262626;
				}
				else if (text[i+1] == 'O')
				{
					_maskColor[maskIndex] = 0xFFCCBDAB;
				}
				maskIndex++;
			}
			i += 3;
		}
#endif
		_uni_buffer[index] = text[i]&0x00FF;
		index++;
	}
	length -= maskIndex*3;


	
	/*INT32 c;
	INT32 yy = y;
	INT32 width = 0;
	INT32 start_off = 0;
	INT32 end_off = 0;
	
	INT32 blockwidth = 0;
	INT32 start_block = 0;
	INT32 end_block = 0;
	int hText = 0;
	
	for (INT32 i = 0; i < length; i++)
	{
		c = (_uni_buffer[i]&0xFFFF) - 33;
		
		int ww = 0;
		
		int fmodule_min = _frame_offset[0];
		if (c >= 0)
		{
			fmodule_min = _frame_offset[c];
		}
		ww = _fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]]&0xFFFF) + _char_spacing;
		
		blockwidth += ww;
		
		
		if ((_uni_buffer[i]&0xFFFF) == ' ' || i == length -1 || ((_uni_buffer[i]&0xFFFF) == '\\' &&  (_uni_buffer[i+1]&0xFFFF) == 'n') || (_uni_buffer[i]&0xFFFF) == 10)
		{
			end_block = i;
			if (i >= length -1)
			{
				end_off = i;
			}
			//printf("\n width = %d, blockwidth = %d, w = %d", width, blockwidth, w);
			if (width + blockwidth > w)
			{
				int len = end_off - start_block;
				//len += (i>=length-1?1:0);
				//printf("\n start_block = %d, len = %d, width = %d, blockwidth = %d", start_block, len, width, blockwidth);
				DrawText(g, _uni_buffer, start_block, len, x, yy, anchor);
				start_block = end_off+1;
				width = blockwidth;
				yy += GetFModuleHeight(0, 0) + _line_spacing;
			}
			else if (((_uni_buffer[i]&0xFFFF) == '\\' &&  (_uni_buffer[i+1]&0xFFFF) == 'n'))
			{    
				int len = i - start_block;
				//printf("\n i = %d, start_block = %d, end_off = %d, len = %d", i, start_block, end_off, len);
				DrawText(g, _uni_buffer, start_block, len, x, yy, anchor);
				start_block =start_block+len + 2;
				width = blockwidth;
				yy += GetFModuleHeight(0, 0) + _line_spacing;    
				i += 2;
			}
			else if((_uni_buffer[i]&0xFFFF) == 10)
			{
				int len = i - start_block;
				//printf("\n i = %d, start_block = %d, end_off = %d, len = %d", i, start_block, end_off, len);
				DrawText(g, _uni_buffer, start_block, len, x, yy, anchor);
				start_block =start_block+len + 1;
				width = blockwidth;
				yy += GetFModuleHeight(0, 0) + _line_spacing;
				i += 1;
			}
			else if (i == length -1)
			{
				int len = i - start_block + 1;
				DrawText(g, _uni_buffer, start_block, len, x, yy, anchor);
			}
			else
			{    
				width += blockwidth;
				end_off = i;
			}
			blockwidth = 0;
		}
	}*/
	
	
	int line_begin = 0;
	int line_width = 0;
	int line_height = GetFModuleHeight(0, 0) + _line_spacing;
	int yy = y;
	int word_count = 0;

	for (INT32 i = 0; i < length; i++)
	{
		
		int c = (_uni_buffer[i] & 0xFFFF) - 33;

		int ww = 0;

		int fmodule_min = _frame_offset[0];
		if (c >= 0)
		{
			fmodule_min = _frame_offset[c];
		}
		ww = _fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]] & 0xFFFF) + _char_spacing;

		bool special_count = (i< length - 1 && (_uni_buffer[i] & 0xFFFF) == '\\' && (_uni_buffer[i + 1] & 0xFFFF) == 'n') ? 2 : (_uni_buffer[i] & 0xFFFF) == 10 ? 1: 0;
		

		if (special_count > 0) {

			//draw last line
			DrawText(g, _uni_buffer, line_begin, i-line_begin + 1, x, yy, anchor);

			//begin new line
			yy += line_height;
			line_width = 0;
			i += special_count;
			line_begin = i;

		}
		else if ((line_width + ww > w) && (_uni_buffer[i] & 0xFFFF) != ' ')
		{
			//draw last line
			DrawText(g, _uni_buffer, line_begin, i - line_begin - word_count, x, yy, anchor);

			//begin new line
			yy += line_height;
			line_width = 0;
			i -= word_count;
			line_begin = i;
		}
		line_width += ww;

		if ((_uni_buffer[i] & 0xFFFF) == ' ')
		{
			word_count = 0;
			continue;
		}
		word_count++;
	}

	//finaly draw the last line

	if (line_begin < length - 1)
	{
		DrawText(g, _uni_buffer, line_begin, length - line_begin  , x, yy, anchor);
	}
	
#ifdef USE_COLOR_MASK
	G()->ClearColorMask();
#endif
}
void CSprite::DrawAnsiNTextPageFormatCustomColorRange(Graphic* g, float x, float y,int iRenderBegin, int iRenderEnd, int width, INT32 anchor, const char* format, ...)
{
    va_list	argptr;
    va_start(argptr, format);
    vsprintf(_ansi_buffer, format, argptr);
    va_end(argptr);
    
#ifdef USE_COLOR_MASK
    ResetTextColorMask();
#endif
    int maskIndex = 0;
    
    int len = strlen(_ansi_buffer);
    int index = 0;
    for (int i = 0; i < len; i++)
    {
#ifdef USE_COLOR_MASK
        if (_ansi_buffer[i] == '<' && i <= len - 8 && _ansi_buffer[i + 7] == '>')
        {
            if (maskIndex < 10)
            {
                _maskIndex[maskIndex] = index;
                
                _maskColor[maskIndex] = 0xff000000;
                char hexstring[7];
                for (int c = 0; c < 6; c++)
                {
                    hexstring[c] = _ansi_buffer[i + 1 + c];
                }
                hexstring[6] = '\0';
                _maskColor[maskIndex] += strtol(hexstring, NULL, 16);
                maskIndex++;
            }
            i += 8;
        }
#endif
        _uni_buffer[index] = _ansi_buffer[i] & 0x00FF;
        index++;
    }
    len -= maskIndex * 8;
    
    DrawPageNTextRange(g, _uni_buffer, len,iRenderBegin,  iRenderEnd, x, y, width, anchor);
    
#ifdef USE_COLOR_MASK
    G()->ClearColorMask();
#endif

}
void CSprite::DrawAnsiNTextPageFormatCustomColor(Graphic* g, float x, float y, int length, int w, INT32 anchor, const char* text, ...){
    va_list	argptr;
    va_start(argptr, text);
    vsprintf(_ansi_buffer, text, argptr);
    va_end(argptr);
    
#ifdef USE_COLOR_MASK
    ResetTextColorMask();
#endif
    int maskIndex = 0;
    
    int len = strlen(_ansi_buffer);
    int index = 0;
    for (int i = 0; i < len; i++)
    {
#ifdef USE_COLOR_MASK
        if (_ansi_buffer[i] == '<' && i <= len - 8 && _ansi_buffer[i + 7] == '>')
        {
            if (maskIndex < 10)
            {
                _maskIndex[maskIndex] = index;
                
                _maskColor[maskIndex] = 0xff000000;
                char hexstring[7];
                for (int c = 0; c < 6; c++)
                {
                    hexstring[c] = _ansi_buffer[i + 1 + c];
                }
                hexstring[6] = '\0';
                _maskColor[maskIndex] += strtol(hexstring, NULL, 16);
                maskIndex++;
            }
            i += 8;
        }
#endif
        _uni_buffer[index] = _ansi_buffer[i] & 0x00FF;
        index++;
    }
    len -= maskIndex * 8;
    
    DrawPageNText(g, _uni_buffer, len,length, x, y, w, anchor);
    
#ifdef USE_COLOR_MASK
    G()->ClearColorMask();
#endif
}
void CSprite::DrawAnsiTextPageFormatCustomColor(Graphic* g, float x, float y, int w, INT32 anchor, const char* text, ...)
{
	va_list	argptr;
	va_start(argptr, text);
	vsprintf(_ansi_buffer, text, argptr);
	va_end(argptr);

#ifdef USE_COLOR_MASK
	ResetTextColorMask();
#endif
	int maskIndex = 0;

	int len = strlen(_ansi_buffer);
	int index = 0;
	for (int i = 0; i < len; i++)
	{
#ifdef USE_COLOR_MASK
		if (_ansi_buffer[i] == '<' && i <= len - 8 && _ansi_buffer[i + 7] == '>')
		{
			if (maskIndex < 10)
			{
				_maskIndex[maskIndex] = index;

				_maskColor[maskIndex] = 0xff000000;
				char hexstring[7];
				for (int c = 0; c < 6; c++)
				{
					hexstring[c] = _ansi_buffer[i + 1 + c];
				}
				hexstring[6] = '\0';
				_maskColor[maskIndex] += strtol(hexstring, NULL, 16);
				maskIndex++;
			}
			i += 8;
		}
#endif
		_uni_buffer[index] = _ansi_buffer[i] & 0x00FF;
		index++;
	}
	len -= maskIndex * 8;

	DrawPage(g, _uni_buffer, len, x, y, w, anchor);

#ifdef USE_COLOR_MASK
	G()->ClearColorMask();
#endif
}
//nhut.hoang.add
float CSprite::DrawAnsiTextPageFormatScroll(Graphic* g, float x, float y, int w, INT32 anchor, const char* text)
{
	float textHeight = 0;
	
#ifdef USE_COLOR_MASK
	ResetTextColorMask();
#endif
	int maskIndex = 0;
	
	int length = strlen(text);
	int index = 0;
	for (int i = 0; i < length; i++)
	{
#ifdef USE_COLOR_MASK
		if (text[i] == '<' && i <= length - 3 && text[i+2] == '>')
		{
			if (maskIndex < 10)
			{
				_maskIndex[maskIndex] = index;
				if (text[i+1] == 'C')
				{
					_maskColor[maskIndex] = 0xFFFFFFFF;
				}
				else if (text[i+1] == 'R')
				{
					_maskColor[maskIndex] = 0xFFFF0000;
				}
				else if (text[i+1] == 'G')
				{
					_maskColor[maskIndex] = 0xFF0000FF;
				}
				else if (text[i+1] == 'B')
				{
					_maskColor[maskIndex] = 0xFF00FF00;
				}
				else if (text[i+1] == 'Y')
				{
					_maskColor[maskIndex] = 0xFFF4FF77;
				}
				else if (text[i+1] == 'P')
				{
					_maskColor[maskIndex] = 0xFFFF6767;
				}
				else if (text[i+1] == 'D')
				{
					_maskColor[maskIndex] = 0xFF262626;
				}
				else if (text[i+1] == 'O')
				{
					_maskColor[maskIndex] = 0xFFCCBDAB;
				}
				maskIndex++;
			}
			i += 3;
		}
#endif
		_uni_buffer[index] = text[i]&0x00FF;
		index++;
	}
	length -= maskIndex*3;
	INT32 c;
	INT32 yy = y;
	INT32 width = 0;
	INT32 start_off = 0;
	INT32 end_off = 0;
	
	INT32 blockwidth = 0;
	INT32 start_block = 0;
	INT32 end_block = 0;
	int hText = 0;
	
	for (INT32 i = 0; i < length; i++)
	{
		c = _uni_buffer[i]&0xFFFF;
		
		int ww = 0;
		
		if (c < 33)
		{
			int fmodule_min = _frame_offset[c];
			ww = _fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]]&0xFFFF);
		}
		else
		{
			int fmodule_min = _frame_offset[c-33];
			ww = _fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]]&0xFFFF);
		}
		ww += _char_spacing;
		//printf("\n %d c = %c , ww = %d", i, c, ww);
		
		blockwidth += ww;
		
		if (c == ' ' || i == length -1  || c == '\n')
		{
			end_block = i;
			if (width + blockwidth > w || c == '\n')
			{
				int len = end_off - start_block;
				//len += (i>=length-1?1:0);
				//printf("\n start_block = %d, len = %d, width = %d, blockwidth = %d", start_block, len, width, blockwidth);
				DrawText(g, _uni_buffer, start_block, len, x, yy, anchor);
				start_block = end_off+1;
				width = blockwidth;
				yy += GetFModuleHeight(0, 0) + _line_spacing;
				
				
				if (i == length -1)
				{
					len = i - start_block + 1;
					DrawText(g, _uni_buffer, start_block, len, x, yy, anchor);
				}
			}
			else if (i == length -1)
			{
				int len = i - start_block + 1;
				DrawText(g, _uni_buffer, start_block, len, x, yy, anchor);
			}
			else
			{
				width += blockwidth;
				end_off = i;
			}
			blockwidth = 0;
		}
		
		/*if (c == ' ' || i >= length -1)
		 {
		 end_block = i;
		 if (i >= length -1)
		 {
		 end_off = i;
		 }
		 if (width + blockwidth > w || i >= length -1)
		 {
		 int len = end_off - start_block;
		 len += (i>=length-1?1:0);
		 //printf("\n start_block = %d, len = %d, width = %d, blockwidth = %d", start_block, len, width, blockwidth);
		 DrawText(g, _uni_buffer, start_block, len, x, yy, anchor);
		 start_block = end_off+1;
		 width = blockwidth;
		 yy += GetFModuleHeight(0, 0) + _line_spacing;
		 }
		 else
		 {
		 width += blockwidth;
		 end_off = i;
		 }
		 blockwidth = 0;
		 }*/
	}
	
#ifdef USE_COLOR_MASK
	G()->ClearColorMask();
#endif

	textHeight = yy - y;
	return textHeight;
}

//nghia.dothanh
void CSprite::DrawAnsiPageFormat(Graphic* g, float x, float y, int w, INT32 anchor, const char* text, ...)
{
	va_list	argptr;
	va_start (argptr, text);
	vsprintf (_ansi_buffer, text, argptr);
	va_end (argptr);

#ifdef USE_COLOR_MASK
	ResetTextColorMask();
#endif
	int maskIndex = 0;

	int len = strlen(_ansi_buffer);
	int index = 0;
	for (int i = 0; i < len; i++)
	{
#ifdef USE_COLOR_MASK
		if (_ansi_buffer[i] == '<' && i <= len - 3 && _ansi_buffer[i + 2] == '>')
		{
			if (maskIndex < 10)
			{
				_maskIndex[maskIndex] = index;
				if (_ansi_buffer[i+1] == 'C')
				{
					_maskColor[maskIndex] = 0xFFFFFFFF;
				}
				else if (_ansi_buffer[i+1] == 'R')
				{
					_maskColor[maskIndex] = 0xFFFF0000;
				}
				else if (_ansi_buffer[i+1] == 'G')
				{
					_maskColor[maskIndex] = 0xFF0000FF;
				}
				else if (_ansi_buffer[i+1] == 'B')
				{
					_maskColor[maskIndex] = 0xFF00FF00;
				}
				else if (_ansi_buffer[i+1] == 'Y')
				{
					_maskColor[maskIndex] = 0xFFF4FF77;
				}
				else if (_ansi_buffer[i+1] == 'P')
				{
					_maskColor[maskIndex] = 0xFFFF6767;
				}
				else if (_ansi_buffer[i+1] == 'D')
				{
					_maskColor[maskIndex] = 0xFF494949;
				}
				else if (_ansi_buffer[i+1] == 'O')
				{
					_maskColor[maskIndex] = 0xFFda3310;
				}
				maskIndex++;
			}
			i += 3;
		}
#endif
		_uni_buffer[index] = _ansi_buffer[i] & 0x00FF;
		index++;
	}
	len -= maskIndex * 3;

	/*int len = strlen(_ansi_buffer);
	for (int i = 0; i < len; i++)
	{
		_uni_buffer[i] = _ansi_buffer[i]&0x00FF;
	}*/
	DrawPage( g, _uni_buffer, len,  x,  y,  w,  anchor);

#ifdef USE_COLOR_MASK
	G()->ClearColorMask();
#endif
}
void CSprite::DrawPage(Graphic* g, UNICHAR* text, INT32 data[], INT32 lenght, float x, float y, INT32 anchor)
{
	INT32 num_of_line = data[0];
	float yy = y;

	if ((anchor&_VCENTER) != 0)
	{
		yy -= ((_module_h[0]&0xFFFF)* num_of_line + _line_spacing * (num_of_line-1) )/2;
	}

	int chars = 0;
	for (INT32 i = 0; i<num_of_line; i++ )
	{
		chars = chars + data[2+i*2];
		//printf("get  %d  %d \n", data[1+i*2], data[2+i*2]);
		//printf("\ni = %d chars = %d , limit = %d, ",i,chars,limit);
		if(chars >= lenght)
		{
			DrawText(g, text, data[1+i*2], lenght - (chars- data[2+i*2]), x, yy, anchor);
			break;
		}
		else
		{
			DrawText(g, text, data[1+i*2], data[2+i*2], x, yy, anchor);
			//printf("here kdjkfdf");
		}
		yy += (_module_h[0]&0xFFFF) + _line_spacing;
	}
}
UNICHAR* CSprite::StringToUnichar(const char* buffer)
{
	int len = strlen(buffer);
	UNICHAR * unichar = new UNICHAR[len];
	for(int i = 0; i < len; i++)
	{
		unichar[i] = buffer[i];
	}
	return unichar;
}

const char* CSprite::UnicharToString(UNICHAR* Text, int len)
{
	char *str = new char[len+1];
	for(int i = 0; i < len; i++)
	{
		str[i] = (char)Text[i];
	}
	str[len]='\0';
	return str;
}
void CSprite::DrawPageNTextRange(Graphic* g, UNICHAR* text, INT32 length, int iRenderBegin, int iRenderEnd,float x, float y, int w, INT32 anchor)
{
    float yy = y;
    INT32 c;
    INT32 width = 0;
    INT32 start_off = 0;
    INT32 end_off = 0;
    
    INT32 blockwidth = 0;
    INT32 start_block = 0;
    INT32 end_block = 0;
    int hText = 0;
    
    for (INT32 i = 0; i < length; i++)
    {
        c = text[i] & 0xFFFF;
        int ww = 0;
        if (c < 33)
        {
            ww = _fontSpaceWidth + _char_spacing + _word_spacing;
        }
        else
        {
            if (c > 7000)c -= 7000;
            int fmodule_min = _frame_offset[c - 33];
            ww = (_fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]] & 0xFFFF) + _char_spacing);
            
        }
        blockwidth += ww;
        //printf("\n c = %c ww = %d", c, ww);
        if (c == '\n')
        {
            blockwidth = 0;
        }
        
        if (c == ' ' || i >= length - 1 || c == '\n')
        {
            width += blockwidth;
            if (c == '\n')
            {
                start_block = i + 1;
                blockwidth = 0;
            }
            //printf("\n blockwidth = %d, width = %d", blockwidth, width);
            if (width >= w || c == '\n')
            {
                //printf("\n blockwidth = %d, width = %d, start_block = %d, start_off = %d", blockwidth, width, start_block, start_off);
                //width -= blockwidth;
                
                if ((anchor & _VCENTER) == 0)
                {
                    //if( start_off <= iRenderEnd)
                    {
                        int begin = iRenderBegin;
                        DrawNTextRange(g, text, start_off, start_block - start_off, x, yy, anchor,begin,iRenderEnd);
                    }
                }
                yy += _fontHeight + _line_spacing;
                hText += _fontHeight + _line_spacing;
                width = blockwidth;
                start_off = start_block;
            }
            
            start_block = i + 1;
            blockwidth = 0;
        }
    }
    if ((anchor & _VCENTER) == 0)
    {
        //if( start_off <= iRenderEnd)
        {
            int begin = iRenderBegin;
            DrawNTextRange(g, text, start_off, length - start_off, x, yy, anchor,begin, iRenderEnd);
        }
    }
    else
    {
        
        yy = y - hText / 2;
        width = 0;
        start_off = 0;
        end_off = 0;
        
        blockwidth = 0;
        start_block = 0;
        end_block = 0;
        for (INT32 i = 0; i < length; i++)
        {
            c = text[i] & 0xFFFF;
            int ww = 0;
            if (c < 33)
            {
                ww = _fontSpaceWidth + _char_spacing + _word_spacing;
            }
            else
            {
                /*if (c >= 0)
                 {
                 DrawFrame(g, c, xx, yy, 0);
                 fmodule_min = _frame_offset[c];
                 xx += _fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]]&0xFFFF) + _char_spacing;
                 }
                 else
                 {
                 xx += _fontSpaceWidth + _char_spacing + _word_spacing;
                 //fmodule_min = _frame_offset[0];
                 //xx += _fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]]&0xFFFF) + _char_spacing + _word_spacing;
                 }*/
                if (c > 7000)c -= 7000;
                int fmodule_min = _frame_offset[c - 33];
                ww = (_fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]] & 0xFFFF) + _char_spacing);
                
            }
            blockwidth += ww;
            if (c == '\n')
            {
                blockwidth = 0;
            }
            if (c == ' ' || i >= length - 1 || c == '\n')
            {
                width += blockwidth;
                if (c == '\n')
                {
                    start_block = i + 1;
                    blockwidth = 0;
                }
                
                if (width >= w || c == '\n')
                {
                    //width -= blockwidth;
                    //if( start_off <= iRenderEnd)
                    {
                        int begin = iRenderBegin;
                        DrawNTextRange(g, text, start_off, start_block - start_off, x, yy, anchor, begin,iRenderEnd);
                    }
                    
                    yy += _fontHeight + _line_spacing;
                    
                    width = blockwidth;
                    start_off = start_block;
                }
                
                start_block = i + 1;
                blockwidth = 0;
            }
        }
        //if( start_off <= iRenderEnd)
        {
            int begin = iRenderBegin;
            DrawNTextRange(g, text, start_off, length - start_off, x, yy, anchor,begin,iRenderEnd);
        }
        
        
    }
}
void CSprite::DrawPageNText(Graphic* g, UNICHAR* text, INT32 length, int render_len,float x, float y, int w, INT32 anchor)
{
    float yy = y;
    INT32 c;
    INT32 width = 0;
    INT32 start_off = 0;
    INT32 end_off = 0;
    
    INT32 blockwidth = 0;
    INT32 start_block = 0;
    INT32 end_block = 0;
    int hText = 0;
    
    for (INT32 i = 0; i < length; i++)
    {
        c = text[i] & 0xFFFF;
        int ww = 0;
        if (c < 33)
        {
            ww = _fontSpaceWidth + _char_spacing + _word_spacing;
        }
        else
        {
            if (c > 7000)c -= 7000;
            int fmodule_min = _frame_offset[c - 33];
            ww = (_fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]] & 0xFFFF) + _char_spacing);
            
        }
        blockwidth += ww;
        //printf("\n c = %c ww = %d", c, ww);
        if (c == '\n')
        {
            blockwidth = 0;
        }
        
        if (c == ' ' || i >= length - 1 || c == '\n')
        {
            width += blockwidth;
            if (c == '\n')
            {
                start_block = i + 1;
                blockwidth = 0;
            }
            //printf("\n blockwidth = %d, width = %d", blockwidth, width);
            if (width >= w || c == '\n')
            {
                //printf("\n blockwidth = %d, width = %d, start_block = %d, start_off = %d", blockwidth, width, start_block, start_off);
                //width -= blockwidth;
                
                if ((anchor & _VCENTER) == 0)
                {
                    if( start_off <= render_len)
                    {
                        DrawNText(g, text, start_off, start_block - start_off, x, yy, anchor,render_len-start_off);
                    }
                }
                yy += _fontHeight + _line_spacing;
                hText += _fontHeight + _line_spacing;
                width = blockwidth;
                start_off = start_block;
            }
            
            start_block = i + 1;
            blockwidth = 0;
        }
    }
    if ((anchor & _VCENTER) == 0)
    {
        if( start_off <= render_len)
        {
            DrawNText(g, text, start_off, length - start_off, x, yy, anchor, render_len - start_off);
        }
    }
    else
    {
        
        yy = y - hText / 2;
        width = 0;
        start_off = 0;
        end_off = 0;
        
        blockwidth = 0;
        start_block = 0;
        end_block = 0;
        for (INT32 i = 0; i < length; i++)
        {
            c = text[i] & 0xFFFF;
            int ww = 0;
            if (c < 33)
            {
                ww = _fontSpaceWidth + _char_spacing + _word_spacing;
            }
            else
            {
                /*if (c >= 0)
                 {
                 DrawFrame(g, c, xx, yy, 0);
                 fmodule_min = _frame_offset[c];
                 xx += _fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]]&0xFFFF) + _char_spacing;
                 }
                 else
                 {
                 xx += _fontSpaceWidth + _char_spacing + _word_spacing;
                 //fmodule_min = _frame_offset[0];
                 //xx += _fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]]&0xFFFF) + _char_spacing + _word_spacing;
                 }*/
                if (c > 7000)c -= 7000;
                int fmodule_min = _frame_offset[c - 33];
                ww = (_fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]] & 0xFFFF) + _char_spacing);
                
            }
            blockwidth += ww;
            if (c == '\n')
            {
                blockwidth = 0;
            }
            if (c == ' ' || i >= length - 1 || c == '\n')
            {
                width += blockwidth;
                if (c == '\n')
                {
                    start_block = i + 1;
                    blockwidth = 0;
                }
                
                if (width >= w || c == '\n')
                {
                    //width -= blockwidth;
                    if( start_off <= render_len)
                    {
                        DrawNText(g, text, start_off, start_block - start_off, x, yy, anchor, render_len - start_off);
                    }
                    
                    yy += _fontHeight + _line_spacing;
                    
                    width = blockwidth;
                    start_off = start_block;
                }
                
                start_block = i + 1;
                blockwidth = 0;
            }
        }
        if( start_off <= render_len)
        {
            DrawNText(g, text, start_off, length - start_off, x, yy, anchor,render_len - start_off);
        }
        
        
    }
}
void CSprite::DrawPage(Graphic* g, UNICHAR* text, INT32 length, float x, float y, INT32 w, INT32 anchor)
{
	float yy = y;
	INT32 c;
	INT32 width = 0;
	INT32 start_off = 0;
	INT32 end_off = 0;
	
	INT32 blockwidth = 0;
	INT32 start_block = 0;
	INT32 end_block = 0;
	int hText = 0;
	for (INT32 i = 0; i < length; i++)
	{
		c = text[i] & 0xFFFF;
		int ww = 0;
		if (c < 33)
		{
			ww = _fontSpaceWidth + _char_spacing + _word_spacing;
		}
		else
		{
			if (c > 7000)c -= 7000;
			int fmodule_min = _frame_offset[c - 33];
			ww = (_fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]] & 0xFFFF) + _char_spacing);

		}
		blockwidth += ww;
		//printf("\n c = %c ww = %d", c, ww);
		if (c == '\n')
		{
			blockwidth = 0;
		}

		if (c == ' ' || i >= length - 1 || c == '\n')
		{
			width += blockwidth;
			if (c == '\n')
			{
				start_block = i + 1;
				blockwidth = 0;
			}
			//printf("\n blockwidth = %d, width = %d", blockwidth, width);
			if (width >= w || c == '\n')
			{
				//printf("\n blockwidth = %d, width = %d, start_block = %d, start_off = %d", blockwidth, width, start_block, start_off);
				//width -= blockwidth;

				if ((anchor & _VCENTER) == 0)
				{
					DrawText(g, text, start_off, start_block - start_off, x, yy, anchor);
				}
				yy += _fontHeight + _line_spacing;
				hText += _fontHeight + _line_spacing;
				width = blockwidth;
				start_off = start_block;
			}

			start_block = i + 1;
			blockwidth = 0;
		}
	}
    _current_page_height = hText;
	if ((anchor & _VCENTER) == 0)
	{
		DrawText(g, text, start_off, length - start_off, x, yy, anchor);
	}
	else
	{

		yy = y - hText / 2;
		c;
		width = 0;
		start_off = 0;
		end_off = 0;

		blockwidth = 0;
		start_block = 0;
		end_block = 0;
		for (INT32 i = 0; i < length; i++)
		{
			c = text[i] & 0xFFFF;
			int ww = 0;
			if (c < 33)
			{
				ww = _fontSpaceWidth + _char_spacing + _word_spacing;
			}
			else
			{
				/*if (c >= 0)
				{
				DrawFrame(g, c, xx, yy, 0);
				fmodule_min = _frame_offset[c];
				xx += _fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]]&0xFFFF) + _char_spacing;
				}
				else
				{
				xx += _fontSpaceWidth + _char_spacing + _word_spacing;
				//fmodule_min = _frame_offset[0];
				//xx += _fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]]&0xFFFF) + _char_spacing + _word_spacing;
				}*/
				if (c > 7000)c -= 7000;
				int fmodule_min = _frame_offset[c - 33];
				ww = (_fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]] & 0xFFFF) + _char_spacing);

			}
			blockwidth += ww;
			if (c == '\n')
			{
				blockwidth = 0;
			}
			if (c == ' ' || i >= length - 1 || c == '\n')
			{
				width += blockwidth;
				if (c == '\n')
				{
					start_block = i + 1;
					blockwidth = 0;
				}

				if (width >= w || c == '\n')
				{
					//width -= blockwidth;

					DrawText(g, text, start_off, start_block - start_off, x, yy, anchor);

					yy += _fontHeight + _line_spacing;

					width = blockwidth;
					start_off = start_block;
				}

				start_block = i + 1;
				blockwidth = 0;
			}
		}

		DrawText(g, text, start_off, length - start_off, x, yy, anchor);


	}
}

void CSprite::SetLineSpacing(INT32 space)
{
	_line_spacing = space;
}
void CSprite::SetCharSpacing(INT32 space)
{
	_char_spacing = space;
}
void CSprite::SetWordSpacing(INT32 space)
{
	_word_spacing = space;
}
int CSprite::GetWidthAnsiText(const char* text)
{
	int len = strlen(text);
	
	for (int i = 0; i < len; i++)
	{
		_uni_buffer[i] = text[i]&0xFFFF;
	}

	return GetWidthText(_uni_buffer, 0, len);
}
void CSprite::DrawAnsiText(Graphic* g, float x, float y, INT32 anchor, const char* text)
{
	int len = strlen(text);
	
	for (int i = 0; i < len; i++)
	{
		_uni_buffer[i] = text[i]&0x00FF;
	}
	_uni_buffer[len] = '\0';
	DrawText(g, _uni_buffer, 0, len, x, y, anchor);
}
void CSprite::DrawAnsiNText(Graphic* g, float x, float y, int length, INT32 anchor, const char* text)
{
	int len = strlen(text);
	
	for (int i = 0; i < len; i++)
	{
		_uni_buffer[i] = text[i]&0x00FF;
	}
	_uni_buffer[len] = '\0';
	DrawNText(g, _uni_buffer, 0, len, x, y, anchor,length);
}
void CSprite::DrawAnsiNTextRange(Graphic* g, float x, float y, int iRenderBegin, int iRenderEnd, INT32 anchor, const char* text)
{
    int len = strlen(text);
    
    for (int i = 0; i < len; i++)
    {
        _uni_buffer[i] = text[i]&0x00FF;
    }
    _uni_buffer[len] = '\0';
    DrawNTextRange(g, _uni_buffer, 0, len, x, y, anchor, iRenderBegin,  iRenderEnd);
}
void CSprite::DrawAnsiNTextRangeFormat(Graphic* g, float x, float y, int iRenderBegin, int iRenderEnd, INT32 anchor, const char* text, ...)
{
    va_list	argptr;
    
    va_start (argptr, text);
    
    vsprintf (_ansi_buffer, text, argptr);
    
    va_end (argptr);
    
    DrawAnsiNTextRange(g, x, y, iRenderBegin,  iRenderEnd,anchor, _ansi_buffer);
}
void CSprite::DrawAnsiNTextFormat(Graphic* g, float x, float y, int length, INT32 anchor, const char* text, ...)
{
	va_list	argptr;

	va_start (argptr, text);

	vsprintf (_ansi_buffer, text, argptr);

	va_end (argptr);

	DrawAnsiNText(g, x, y, length,anchor, _ansi_buffer);
}
void CSprite::DrawNText(Graphic* g, UNICHAR* text, INT32 length, INT32 x, INT32 y, int lenght_render, INT32 anchor)
{
	DrawNText(g, text, 0, length, x, y, anchor,lenght_render);
}
void CSprite::DrawNTextRange(Graphic* g, UNICHAR* text, INT32 length, INT32 x, INT32 y, int iRenderBegin, int iRenderEnd, INT32 anchor)
{
    DrawNTextRange(g, text, 0, length, x, y, anchor, iRenderBegin,  iRenderEnd);
}
	
void CSprite::DrawNText(Graphic* g, UNICHAR* text, INT32 offset, INT32 length, INT32 x, INT32 y, INT32 anchor, int lenght_render)
{
	INT32 textwidth = GetTextWidth(text, offset, length);
	
	DrawNText(g, text, offset, length, textwidth, x, y, anchor,lenght_render);
}

void CSprite::DrawNTextRange(Graphic* g, UNICHAR* text, INT32 offset, INT32 length, INT32 x, INT32 y, INT32 anchor,int iRenderBegin, int iRenderEnd)
{
    INT32 textwidth = GetTextWidth(text, offset, length);
    DrawNTextRange(g, text, offset, length, textwidth, x, y, anchor,iRenderBegin,iRenderEnd);
}
	
//support TOP LEFT, TOP RIGHT, TOP HCENTER
void CSprite::DrawNTextRange(Graphic* g, UNICHAR* text, INT32 offset, INT32 length, INT32 textwidth, INT32 x, INT32 y, INT32 anchor, int iRenderBegin, int iRenderEnd)
{
    INT32 xx = x;
    INT32 yy = y;
    
    if ((anchor&_RIGHT) != 0)
    {
        xx -= textwidth;
    }
    else if ((anchor&_HCENTER) != 0)
    {
        xx -= (textwidth>>1);
    }
    
    if ((anchor&_BOTTOM) != 0)
    {
        yy -= (_module_h[0]&0xFFFF);
    }
    else if ((anchor&_VCENTER) != 0)
    {
        yy += ((_module_h[0]&0xFFFF)>>1);
    }
    
    INT32 end_offset = offset + length;
    
    INT32 fmodule_min = 0;
    int count = 0;
    for (INT32 i = offset; i < end_offset; i++)
    {
        count++;
        
        INT32 c = (text[i]&0xFFFF) - 33;
        
        if (c >= 0)
        {
            if(!(i > iRenderEnd || (i) < iRenderBegin))
            {
                DrawFrame(g, c, xx, yy, 0);
            }
            fmodule_min = _frame_offset[c];
            xx += _fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]]&0xFFFF) + _char_spacing;
        }
        else
        {
            fmodule_min = _frame_offset[0];
            xx += _fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]]&0xFFFF) + _char_spacing;
        }
        
    }
}
void CSprite::DrawNText(Graphic* g, UNICHAR* text, INT32 offset, INT32 length, INT32 textwidth, INT32 x, INT32 y, INT32 anchor, int lenght_render)
{
	INT32 xx = x;
	INT32 yy = y;
	
	if ((anchor&_RIGHT) != 0)
	{
		xx -= textwidth;
	}
	else if ((anchor&_HCENTER) != 0)
	{
		xx -= (textwidth>>1);
	}
	
	if ((anchor&_BOTTOM) != 0)
	{
		yy -= (_module_h[0]&0xFFFF);
	}
	else if ((anchor&_VCENTER) != 0)
	{
		yy += ((_module_h[0]&0xFFFF)>>1);
	}
	
	INT32 end_offset = offset + length;
	
	INT32 fmodule_min = 0;	
	int count = 0;
    int root_opa = g->GetCurrentOpacity();
	for (INT32 i = offset; i < end_offset; i++)
	{
		count++;
		if(count > lenght_render)
		{
			g->SetOpacity(0);
		}
		INT32 c = (text[i]&0xFFFF) - 33;			
		
		if (c >= 0)
		{
			DrawFrame(g, c, xx, yy, 0);
			fmodule_min = _frame_offset[c];
			xx += _fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]]&0xFFFF) + _char_spacing;
		}
		else
		{
			fmodule_min = _frame_offset[0];
			xx += _fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]]&0xFFFF) + _char_spacing;
		}
		if(count > lenght_render)
		{
			g->SetOpacity(root_opa);
		}
	}
}
int CSprite::GetWidthAnsiTextFormat(const char* text, ...)
{
	va_list	argptr;

	va_start (argptr, text);

	vsprintf (_ansi_buffer, text, argptr);

	va_end (argptr);

	return GetWidthAnsiText(_ansi_buffer);
}
void CSprite::DrawAnsiTextFormat(Graphic* g, float x, float y, INT32 anchor, const char* text, ...)
{
	va_list	argptr;

	va_start (argptr, text);

	vsprintf (_ansi_buffer, text, argptr);

	va_end (argptr);

	DrawAnsiText(g, x, y, anchor, _ansi_buffer);
}

void CSprite::DrawText(Graphic* g, UNICHAR* text, INT32 length, float x, float y, INT32 anchor)
{
	DrawText(g, text, 0, length, x, y, anchor);
}

int CSprite::GetWidthText(UNICHAR* text, INT32 offset, INT32 length)
{
	return GetTextWidth(text, offset, length);
}
void CSprite::DrawText(Graphic* g, UNICHAR* text, INT32 offset, INT32 length, float x, float y, INT32 anchor)
{
	INT32 textwidth = GetTextWidth(text, offset, length);
	
	DrawText(g, text, offset, length, textwidth, x, y, anchor);
}
	
//support TOP LEFT, TOP RIGHT, TOP HCENTER
void CSprite::DrawText(Graphic* g, UNICHAR* text, INT32 offset, INT32 length, INT32 textwidth, float x, float y, INT32 anchor)
{
	float xx = x;
	float yy = y;
	if ((anchor&_RIGHT) != 0)
	{
		xx -= textwidth;
	}
	else if ((anchor&_HCENTER) != 0)
	{
		xx -= (textwidth >> 1);

		//triming the text
		/*while (length > 0 && (text[offset] & 0xFFFF) == ' '){
			offset++;
			length--;
		}

		while (length > 0 && (text[offset + length - 1] & 0xFFFF) == ' ') length--;
		*/
	}
	
	if ((anchor&_BOTTOM) != 0)
	{
		yy -= (_module_h[0]&0xFFFF);
	}
	else if ((anchor&_VCENTER) != 0)
	{
		yy += ((_module_h[0]&0xFFFF)>>1);
	}
	
	INT32 end_offset = offset + length;
	
	INT32 fmodule_min = 0;	
	
	for (INT32 i = offset; i < end_offset; i++)
	{		
#ifdef USE_COLOR_MASK
		if (_colorMaskIndex < TOTAL_TEXT_MASK_SUPPORT)
		{			
			if (_maskIndex[_colorMaskIndex] == i)
			{				
				G()->SetColorMask(_maskColor[_colorMaskIndex]);					
				_colorMaskIndex++;
			}
		}
#endif
		INT32 c = text[i];			
		if (c>7000)c -= 7000;
		c -= 33;
		if (c >= 0)
		{
			DrawFrame(g, c, xx, yy, 0);
			fmodule_min = GetFrameFirstModule(c);
			xx += _fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]] & 0xFFFF) + _char_spacing;
		}
		else
		{
			fmodule_min = _frame_offset[0];
			xx += _fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]]&0xFFFF) + _char_spacing + _word_spacing;
		}
	}	
}

INT32 CSprite::GetAnsiTextWidth(const char* text, INT32 offset, INT32 length)
{
	int len = strlen(text);
	
	for (int i = 0; i < len; i++)
	{
		_uni_buffer[i] = text[i]&0xFFFF;
	}
	return GetTextWidth(_uni_buffer, offset, length);
}
	
INT32 CSprite::GetTextWidth(UNICHAR* text, INT32 offset, INT32 length)
{
	INT32 width = 0;
	
	INT32 end_offset = offset + length;
	INT32 fmodule_min = 0;
	
	for (INT32 i = offset; i < end_offset; i++)
	{
		INT32 c = (text[i] & 0xFFFF);
		if (c < 33)
		{
			width += _fontSpaceWidth + _char_spacing + _word_spacing;
		}
		else
		{
			if (c > 7000)c -= 7000;
			fmodule_min = GetFrameFirstModule(c - 33);
			width += (_fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]] & 0xFFFF) + _char_spacing);
		}	
	}
	
	return width;
}
	
INT32 CSprite::GetCharWidth(UNICHAR c)
{
	INT32 i = (c&0xFFFF) - 33;
	if(c > 7000)c-=7000;
	if (i < 0)
	{
		i = 0;
	}
	
	INT32 fmodule_min = _frame_offset[i];
	
	return _fmodule_x[fmodule_min] + (_module_w[_fmodule_id[fmodule_min]]&0xFFFF) + _char_spacing;
}
	
INT32 CSprite::GetFontHeight()
{
	return _fmodule_y[0] + _module_h[_fmodule_id[0]]&0xFFFF;
}
INT32 CSprite::GetFrameFirstModule(INT32 frame_id)
{
	INT32 fmodule_min = 0;
	if (frame_id < (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
	}
	else if (frame_id == (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
	}
	return fmodule_min;
}
INT32 CSprite::GetFrameLastModule(INT32 frame_id)
{
	INT32 fmodule_max = 0;
	if (frame_id < (_num_frames - 1))
	{
		fmodule_max = _frame_offset[frame_id + 1] - 1;
	}
	else if (frame_id == (_num_frames - 1))
	{
		fmodule_max = _num_fmodules - 1;
	}
	return fmodule_max;
}
INT32 CSprite::GetFrameWidth(INT32 frame_id)
{
	INT32 fmodule_min = 0;
	INT32 fmodule_max = 0;
	if (frame_id < (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _frame_offset[frame_id + 1] - 1;
	}
	else if (frame_id == (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _num_fmodules - 1;
	}

	INT32 min_x = _fmodule_x[fmodule_min];
	INT32 max_x = _fmodule_x[fmodule_min]; 
	for (INT32 i = fmodule_min; i <= fmodule_max; i++)
	{
		
		if ( _fmodule_x[i] < min_x)
			min_x = _fmodule_x[i];

		if ( _fmodule_x[i]+_module_w[_fmodule_id[i]] > max_x)
			max_x =  _fmodule_x[i]+_module_w[_fmodule_id[i]];
	}

	return max_x - min_x;
}

//Nghia.dothanh add
INT32 CSprite::GetFrameHeight(INT32 frame_id)
{
	INT32 fmodule_min = 0;
	INT32 fmodule_max = 0;
	if (frame_id < (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _frame_offset[frame_id + 1] - 1;
	}
	else if (frame_id == (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _num_fmodules - 1;
	}

	INT32 min_y = _fmodule_y[fmodule_min];
	INT32 max_y = _fmodule_y[fmodule_min]; 
	for (INT32 i = fmodule_min; i <= fmodule_max; i++)
	{
		
		if ( _fmodule_y[i] < min_y)
			min_y = _fmodule_y[i];

		if ( _fmodule_y[i]+_module_h[_fmodule_id[i]] > max_y)
			max_y =  _fmodule_y[i]+_module_h[_fmodule_id[i]];
	}

	return max_y - min_y;
}
PineRectangle CSprite::GetFrameBound(int frame_id){
	INT32 fmodule_min = 0;
	INT32 fmodule_max = 0;
	if (frame_id < (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _frame_offset[frame_id + 1] - 1;
	}
	else if (frame_id == (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _num_fmodules - 1;
	}

	PineRectangle rect;
	rect.X = (float)_fmodule_x[fmodule_min];
	rect.Y = (float)_fmodule_y[fmodule_min];
	rect.W = (float)(_module_w[_fmodule_id[fmodule_min]] & 0xFFFF);
	rect.H = (float)(_module_h[_fmodule_id[fmodule_min]] & 0xFFFF);

#if defined(USE_FREE_TRANSFORM)
	if ((_fmodule_transf[fmodule_min] & _FREE_TRANSFORM) != 0)
	{
		PineTransformParam tparam;
		tparam.m11 = _fmodule_m11[fmodule_min];
		tparam.m12 = _fmodule_m12[fmodule_min];
		tparam.m21 = _fmodule_m21[fmodule_min];
		tparam.m22 = _fmodule_m22[fmodule_min];
		tparam.TranslateX = _fmodule_ox[fmodule_min];
		tparam.TranslateY = _fmodule_oy[fmodule_min];
		float right = rect.X + rect.W;
		float bottom = rect.Y + rect.H;
		PinePoint point = tparam.Transform(rect.X, rect.Y);
		rect.X = point.X;
		rect.Y = point.Y;
		point = tparam.Transform(right, bottom);
		rect.W = point.X - rect.X;
		rect.H = point.Y - rect.Y;
	}
#endif


	float min_x = rect.X;
	float max_x = rect.X;
	float min_y = rect.Y;
	float max_y = rect.Y;
	for (INT32 i = fmodule_min; i <= fmodule_max; i++)
	{
		rect.X = (float)_fmodule_x[i];
		rect.Y = (float)_fmodule_y[i];
		rect.W = (float)(_module_w[_fmodule_id[i]] & 0xFFFF);
		rect.H = (float)(_module_h[_fmodule_id[i]] & 0xFFFF);

#if defined(USE_FREE_TRANSFORM)
		if ((_fmodule_transf[i] & _FREE_TRANSFORM) != 0)
		{
			PineTransformParam tparam;
			tparam.m11 = _fmodule_m11[i];
			tparam.m12 = _fmodule_m12[i];
			tparam.m21 = _fmodule_m21[i];
			tparam.m22 = _fmodule_m22[i];
			tparam.TranslateX = _fmodule_ox[i];
			tparam.TranslateY = _fmodule_oy[i];
			float right = rect.X + rect.W;
			float bottom = rect.Y + rect.H;
			PinePoint point = tparam.Transform(rect.X, rect.Y);
			rect.X = point.X;
			rect.Y = point.Y;
			point = tparam.Transform(right, bottom);
			rect.W = point.X - rect.X;
			rect.H = point.Y - rect.Y;
		}
#endif
		if (rect.X < min_x)
			min_x = rect.X;

		if (rect.X + rect.W > max_x)
			max_x = rect.X + rect.W;
		if (rect.Y < min_y)
			min_y = rect.Y;
		if (rect.Y + rect.H > max_y)
			max_y = rect.Y + rect.H;

	}
	PineRectangle rs_rect;
	rs_rect.X = min_x;
	rs_rect.Y = min_y;
	rs_rect.W = max_x - min_x;
	rs_rect.H = max_y - min_y;
	return rs_rect;
}
int CSprite::GetCurrentAFrameWidth(INT32 anim_id)
{
	if (_current_anim != anim_id)
	{
		_current_anim = anim_id;
		_min_aframes = _anim_offset[_current_anim];
		
		if (_current_anim == (_num_anims - 1))
		{
			_max_aframes = _num_aframes - 1;
		}
		else
		{
			_max_aframes = _anim_offset[_current_anim + 1] - 1;
		}
		
		_current_aframes = _min_aframes;
		_current_aframes_time = _aframe_time[_current_aframes];
		_is_current_anim_stop = false;
	}

	if (!_is_current_anim_stop)
	{
		return GetFrameWidth(_aframe_id[_current_aframes]);
	}
	return -1;
}

int CSprite::GetCurrentAFrameHeight(INT32 anim_id)
{
	if (_current_anim != anim_id)
	{
		_current_anim = anim_id;
		_min_aframes = _anim_offset[_current_anim];
		
		if (_current_anim == (_num_anims - 1))
		{
			_max_aframes = _num_aframes - 1;
		}
		else
		{
			_max_aframes = _anim_offset[_current_anim + 1] - 1;
		}
		
		_current_aframes = _min_aframes;
		_current_aframes_time = _aframe_time[_current_aframes];
		_is_current_anim_stop = false;
	}

	if (!_is_current_anim_stop)
	{
		return GetFrameHeight(_aframe_id[_current_aframes]);
	}
	return -1;
}
int CSprite::GetFModuleNum(int frame_id)
{
	INT32 fmodule_min = 0;
	INT32 fmodule_max = 0;
	
	if (frame_id < (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _frame_offset[frame_id + 1] - 1;
	}
	else if (frame_id == (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _num_fmodules - 1;
	}

	return (fmodule_max - fmodule_min + 1);
}

int CSprite::GetFModuleX(int frame_id, int fmodule_id)
{
	INT32 fmodule_min = 0;
	fmodule_min = _frame_offset[frame_id];
	float fmx = _fmodule_x[fmodule_min+fmodule_id];
#if defined(USE_FREE_TRANSFORM)
#endif
	return fmx;
}
int CSprite::GetFModuleY(int frame_id, int fmodule_id)
{
	INT32 fmodule_min = 0;
	fmodule_min = _frame_offset[frame_id];
	return _fmodule_y[fmodule_min+fmodule_id];
}

int CSprite::GetFModuleWidth(int frame_id, int module)
{
	INT32 fmodule_min = 0;
	fmodule_min = _frame_offset[frame_id];
	return (_module_w[_fmodule_id[fmodule_min+module]]&0xFFFF);
}
int CSprite::GetFModuleHeight(int frame_id, int module)
{
	INT32 fmodule_min = 0;
	fmodule_min = _frame_offset[frame_id];
	return (_module_h[_fmodule_id[fmodule_min+module]]&0xFFFF);
}
PinePoint CSprite::GetFModulePos(int frame_id, int fmodule_id){
	INT32 fmodule_min = 0;
	fmodule_min = _frame_offset[frame_id];
	PinePoint point;
	point.X = _fmodule_x[fmodule_min + fmodule_id];
	point.Y = _fmodule_y[fmodule_min + fmodule_id];
#if defined(USE_FREE_TRANSFORM)
	if ((_fmodule_transf[fmodule_min + fmodule_id] & _FREE_TRANSFORM) != 0)
	{
		PineTransformParam tparam;
		tparam.m11 = _fmodule_m11[fmodule_min + fmodule_id];
		tparam.m12 = _fmodule_m12[fmodule_min + fmodule_id];
		tparam.m21 = _fmodule_m21[fmodule_min + fmodule_id];
		tparam.m22 = _fmodule_m22[fmodule_min + fmodule_id];
		tparam.TranslateX = _fmodule_ox[fmodule_min + fmodule_id];
		tparam.TranslateY = _fmodule_oy[fmodule_min + fmodule_id];
		point.X = point.X;
		point.Y = point.Y;
	}
#endif
	return point;
}
PineRectangle CSprite::GetFmoduleBound(int frame_id, int fmodule_id){
	PineRectangle rect;
	INT32 fmodule_min = 0;
	fmodule_min = _frame_offset[frame_id];
	rect.X = _fmodule_x[fmodule_min + fmodule_id];
	rect.Y = _fmodule_y[fmodule_min + fmodule_id];
	rect.W = (float)(_module_w[_fmodule_id[fmodule_min + fmodule_id]] & 0xFFFF);
	rect.H = (float)(_module_h[_fmodule_id[fmodule_min + fmodule_id]] & 0xFFFF);

#if defined(USE_FREE_TRANSFORM)
	if ((_fmodule_transf[fmodule_min + fmodule_id] & _FREE_TRANSFORM) != 0)
	{
		PineTransformParam tparam;
		tparam.m11 = _fmodule_m11[fmodule_min + fmodule_id];
		tparam.m12 = _fmodule_m12[fmodule_min + fmodule_id];
		tparam.m21 = _fmodule_m21[fmodule_min + fmodule_id];
		tparam.m22 = _fmodule_m22[fmodule_min + fmodule_id];
		tparam.TranslateX = _fmodule_ox[fmodule_min + fmodule_id];
		tparam.TranslateY = _fmodule_oy[fmodule_min + fmodule_id];
		float right = rect.X + rect.W;
		float bottom = rect.Y + rect.H;
		PinePoint point = tparam.Transform(rect.X, rect.Y);
		rect.X = point.X;
		rect.Y = point.Y;
		point = tparam.Transform(right, bottom);
		rect.W = point.X - rect.X;
		rect.H = point.Y - rect.Y;
	}
#endif
	
	return rect;
}


PinePolygon CSprite::GenPolygonFromFrame(int frame_id){
	PinePolygon poly;
	INT32 fmodule_min = 0;
	INT32 fmodule_max = 0;
	if (frame_id < (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _frame_offset[frame_id + 1] - 1;
	}
	else if (frame_id == (_num_frames - 1))
	{
		fmodule_min = _frame_offset[frame_id];
		fmodule_max = _num_fmodules - 1;
	}
	int fmodule_num = fmodule_max - fmodule_min + 1;
	if (fmodule_num > 0){
		poly.init(fmodule_num + 1);
		for (int i = 0; i < fmodule_num; i++)
		{
			PinePoint pos = GetFModulePos(frame_id, i);
			poly.setVertex(i, pos);
		}
		PinePoint pos = GetFModulePos(frame_id, 0);
		poly.setVertex(fmodule_num, pos);
	}
	return poly;
}

#if defined(USE_FM_OPACITY)
int CSprite::GetFModuleOpacity(int frame_id, int fmodule_id){
	INT32 fmodule_min = 0;
	fmodule_min = _frame_offset[frame_id];
	return _fmodule_opacity[fmodule_min + fmodule_id];
}
#endif
void CSprite::CalculateCurrentAFrame(INT32 anim_id)
{
	if (_current_anim != anim_id)
	{
		_current_anim = anim_id;
		_min_aframes = _anim_offset[_current_anim];
		if (_current_anim == (_num_anims - 1))
		{
			_max_aframes = _num_aframes - 1;
		}
		else
		{
			_max_aframes = _anim_offset[_current_anim + 1] - 1;
		}
		_current_aframes = _min_aframes;
		_current_aframes_time = _aframe_time[_current_aframes];
		_is_current_anim_stop = false;
	}
	
}
int CSprite::GetFrameIDOfAFrame(int aframe_id){
	return _aframe_id[aframe_id];
}
PinePoint CSprite::GetAFramePos(int aframe_id){
	PinePoint point;
	point.X = _aframe_x[aframe_id];
	point.Y = _aframe_y[aframe_id];
	return point;
}

void CSprite::FreePalette(int pal)
{

	if (_texture_image[pal] != NULL)
	{
		_pal_loaded[pal] = false;
		////::glDeleteTextures(_num_texture_images, _texture_image[pal]);
	}
}
bool CSprite::PaletteLoaded(int pal)
{
	return _pal_loaded[pal];
}



void CSprite::ClearReplace()
{
	if (_module_replay_data != NULL)
	{
		_num_of_module_replay = 0;
		for (int i = 0; i < _num_modules * 3; i++)
		{
			_module_replay_data[i] = -1;
		}
	}
}
void CSprite::ReplaceModule(int module,int sprite,int module_replace)
{
	_module_replay_data[module*3] = module;
	_module_replay_data[module*3+1] = sprite;
	_module_replay_data[module*3+2] = module_replace;
	_num_of_module_replay++;
}

void CSprite::ClearReplace(int module)
{
	if(_module_replay_data[module*3] >= 0 )
	{
		_module_replay_data[module*3] = -1;
		_module_replay_data[module*3+1] = -1;
		 _module_replay_data[module*3+2] = -1;
		_num_of_module_replay--;
	}
}

void CSprite::Embossing(int* pixels,int width,int height)
{
	for (int x = 2; x < width - 1; x++)
	{
		for (int y = 2; y < height - 1; y++)
		{  
			int red = ((pixels[(x + 1) + y * width + 1] & 0xFF) - (pixels[x + y * width] & 0xFF)) + 128;  
			int green = (((pixels[(x + 1) + y * width + 1] & 0xFF00) / 0x100) % 0x100 - ((pixels[x + y * width] & 0xFF00) / 0x100) % 0x100) + 128;
			int blue = (((pixels[(x + 1) + y * width + 1] & 0xFF0000) / 0x10000) % 0x100 - ((pixels[x + y * width] & 0xFF0000) / 0x10000) % 0x100) + 128;
			int avg = (red + green + blue) / 3;
			pixels[x + y * width] = (0xff000000 | avg << 16 | avg << 8 | avg);
		}
	}
}

void CSprite::NegateImage(int* pixels,int width,int height)
{
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{  
			int red = -((pixels[x + y * width] >>16)&0xff) + 255;
			int green = -((pixels[x + y * width]>>8)&0xff) + 255;
			int blue = -((pixels[x + y * width])&0xff) + 255;
			int alpha = (pixels[x + y * width])&0xff000000/2;
			pixels[x + y * width] = (alpha | red << 16 | green << 8 | blue);
		}
	}
}

int CSprite::GetGray(int color)
{
	int R = (color>>16)&&0xff;
	int G = (color>>8)&&0xff;
	int B = (color)&&0xff;
	return (int)(R * 0.11 + G * 0.59 + B * 0.3);
}
int CSprite::GetCorrectChannel(int value)
{
	if ((value < 256)&(value>=0))
	{
		return value;
	}
	if (value > 255)
	{
		return 255;
	}
	return 0;
}
int CSprite::GetCorrectColor(int r, int g, int b)
{
	return 0xff000000 + (GetCorrectChannel(r)<<16) + (GetCorrectChannel(g)<<8)+ GetCorrectChannel(b);
}
 void CSprite::ConvertColorToARGB(int* color, BYTE* argb,int width,int height)
{
	for(int i = 0; i < width*height; i++)
	{
		int c = color[i];
		argb[i*4]		= (c << 24)&0xff; 
		argb[i*4 + 1]	= (c << 16)&0xff;
		argb[i*4 + 2]	= (c << 8)&0xff;
		argb[i*4 + 3]	= (c)&0xff;
	}
}
 void CSprite::ConvertARGBToColor(BYTE* argb,int* color,int width, int height)
{
	for(int i = 0; i < width*height; i++)
	{
		color[i] = (argb[i*4+3]<<24) | (argb[i*4+2]<<16) | (argb[i*4+1]<<8) | (argb[i*4]);
	}
}

 PineSize CSprite::scaleBuffer(BYTE* argb, PineColorFormat colorformat, int src_width, int src_height) {

	 PineSize size;
	 size.W = src_width;
	 size.H = src_height;
#if defined(_USE_TEST)
	 //TEST_LOG_FILE("texture.raw", (const char*)argb, src_width * src_height * 4);
#endif
	 if (_texture_scale > 0.0f && _texture_scale < 1.0f && src_width > 0 && src_height >0) {
		 int width = (int)(src_width * _texture_scale);
		 int height = (int)(src_height * _texture_scale);
		 float dpx_size = 1.0f / _texture_scale;
		 float value_reduce = 1.0f / (dpx_size * dpx_size);
		 int des_buff_rnum = (int)(dpx_size + 1);
		 ColorFloatARGB* src_buff = new ColorFloatARGB[src_width];
		 ColorFloatARGB* des_buff = new ColorFloatARGB[width * 2];
		 //int src_buff_ron = 0;
		 int des_buff_ron = 0;
 
		 float dx = 0;
		 float dy = 0;
		 float px = 0;
		 float py = 0;

		 int tmp_r = 0;
		 int tmp_c = 0;
		 int des_r = 0;
		 int des_c = 0;

		 ColorFloatARGB* des_color = NULL;
		 int src_line_width = src_width * 4;
		 int des_line_width = width * 4;
		 switch (colorformat)
		 {
		 case PineColorFormat::RGBA_4444:
			 src_line_width = src_width * 2;
			 des_line_width = width * 2;
			 break;
		 case PineColorFormat::RGB_1BYTE:
			 src_line_width = src_width * 3;
			 des_line_width = width * 3;
			 break;
		 case PineColorFormat::RGBA_5551:
			 src_line_width = src_width * 2;
			 des_line_width = width * 2;
			 break;
		 case PineColorFormat::RGB_565:
			 src_line_width = src_width * 2;
			 des_line_width = width * 2;
			 break;
		 default:
			 break;
		 }
		 
		 for (int r = 0; r < src_height; r++) {
			 for (int c = 0; c < src_width; c++) {
				 //src_buff[c]
				 switch (colorformat)
				 {
				 case PineColorFormat::RGBA_1BYTE: {
					 src_buff[c].A = (float)argb[r* src_line_width + c * 4];
					 src_buff[c].R = (float)argb[r* src_line_width + c * 4 + 1];
					 src_buff[c].G = (float)argb[r* src_line_width + c * 4 + 2];
					 src_buff[c].B = (float)argb[r* src_line_width + c * 4 + 3];
				 }break;
				 case PineColorFormat::RGBA_4444: {
					 BYTE byte1 = argb[r* src_line_width + c * 2];
					 BYTE byte2 = argb[r* src_line_width + c * 2 + 1];
					 BYTE a = (byte1 >> 4) & 0xFF;
					 BYTE r = byte1 & 0x0F;
					 BYTE g = (byte2 >> 4) & 0xFF;
					 BYTE b = byte2 & 0x0F;
					 src_buff[c].A = (float)a;
					 src_buff[c].R = (float)r;
					 src_buff[c].G = (float)g;
					 src_buff[c].B = (float)b;
				 }break;
				 case PineColorFormat::RGB_1BYTE: {
					 src_buff[c].A = 0;
					 src_buff[c].R = (float)argb[r* src_line_width + c * 3];
					 src_buff[c].G = (float)argb[r* src_line_width + c * 3 + 1];
					 src_buff[c].B = (float)argb[r* src_line_width + c * 3 + 2];
				 }break;
				 case PineColorFormat::RGBA_5551: {
					 BYTE byte1 = argb[r* src_line_width + c * 2];
					 BYTE byte2 = argb[r* src_line_width + c * 2 + 1];
					 BYTE a = byte2 & 0x01;
					 BYTE r = (byte1 >> 3) & 0xFF;
					 BYTE g = (byte1 << 5) | (byte2 >> 6 & 0xFF);
					 BYTE b = (byte2)& 0x3E;
					 src_buff[c].A = (float)a;
					 src_buff[c].R = (float)r;
					 src_buff[c].G = (float)g;
					 src_buff[c].B = (float)b;
				 }break;
				 case PineColorFormat::RGB_565: {
					 BYTE byte1 = argb[r* src_line_width + c * 2];
					 BYTE byte2 = argb[r* src_line_width + c * 2 + 1];
					 BYTE r = (byte1 >> 3) & 0xFF;
					 BYTE g = (byte1 << 5) | (byte2 >> 5 & 0xFF);
					 BYTE b = (byte2)& 0x1F;
					 src_buff[c].A = 0;
					 src_buff[c].R = (float)r;
					 src_buff[c].G = (float)g;
					 src_buff[c].B = (float)b;
				 }break;
				 default:
					 break;
				 }
			 }
			 dx = 0;
			 des_c = 0;
			 bool roll = false;
			 if ((float)(r + 1) > dy + dpx_size) {
				 py = (float)(r + 1) - (dy + dpx_size);
				 dy += dpx_size;
				 des_r++;
				 
				 roll = true;
			 }
			 for (int c = 0; c < src_width; c++) {
				 //src color
				 ColorFloatARGB src_color = src_buff[c];
				 src_color.A *= value_reduce;
				 src_color.R *= value_reduce;
				 src_color.G *= value_reduce;
				 src_color.B *= value_reduce;
				 
				 if ((float)(c + 1) > (dx + dpx_size)) {
					 px = (float)(c + 1) - (dx + dpx_size);
					 dx += dpx_size;
					 des_c++;
					 if (py > 0) {
						 tmp_r = 0;
						 tmp_c = des_c - 1;
						 des_color = des_buff + (tmp_r * width) + tmp_c;
						 float part_1 =  (1.0f - px) * (1.0f - py);
						 des_color->A += src_color.A * part_1;
						 des_color->R += src_color.R * part_1;
						 des_color->G += src_color.G * part_1;
						 des_color->B += src_color.B * part_1;

						 tmp_r = 0;
						 tmp_c = des_c;
						 des_color = des_buff + tmp_r * width + tmp_c;
						 float part_2 = (1.0f - py) * px;
						 des_color->A += src_color.A * part_2;
						 des_color->R += src_color.R * part_2;
						 des_color->G += src_color.G * part_2;
						 des_color->B += src_color.B * part_2;


						 tmp_r = 1;
						 tmp_c = des_c;
						 des_color = des_buff + tmp_r * width + tmp_c;
						 float part_3 = px * py;
						 des_color->A += src_color.A * part_3;
						 des_color->R += src_color.R * part_3;
						 des_color->G += src_color.G * part_3;
						 des_color->B += src_color.B * part_3;


						 tmp_r = 1;
						 tmp_c = des_c - 1;
						 des_color = des_buff + tmp_r * width + tmp_c;
						 float part_4 = (1.0f - px) * py;
						 des_color->A += src_color.A * part_4;
						 des_color->R += src_color.R * part_4;
						 des_color->G += src_color.G * part_4;
						 des_color->B += src_color.B * part_4;

					 }
					 else {
						 
						 tmp_r = 0;
						 tmp_c = des_c - 1;
						 des_color = des_buff + tmp_r * width + tmp_c;
						 float part_1 = (1.0f - px);
						 des_color->A += src_color.A * part_1;
						 des_color->R += src_color.R * part_1;
						 des_color->G += src_color.G * part_1;
						 des_color->B += src_color.B * part_1;

						 tmp_r = 0;
						 tmp_c = des_c;
						 des_color = des_buff + tmp_r * width + tmp_c;
						 des_color->A += src_color.A * px;
						 des_color->R += src_color.R * px;
						 des_color->G += src_color.G * px;
						 des_color->B += src_color.B * px;
					 }
				 }
				 else {
					 if (py > 0) {
						 tmp_r = 0;
						 tmp_c = des_c;

						 des_color = des_buff + tmp_r * width + tmp_c;
						 float part_2 = (1.0f - py);
						 des_color->A += src_color.A * part_2;
						 des_color->R += src_color.R * part_2;
						 des_color->G += src_color.G * part_2;
						 des_color->B += src_color.B * part_2;
						

						 tmp_r = 1;
						 tmp_c = des_c;
						 des_color = des_buff + tmp_r * width + tmp_c;
						 des_color->A += src_color.A * py;
						 des_color->R += src_color.R * py;
						 des_color->G += src_color.G * py;
						 des_color->B += src_color.B * py;
						 
					 }
					 else {
						 tmp_r = 0;
						 tmp_c = des_c;
						 des_color = des_buff + tmp_r * width + tmp_c;
						 des_color->A += src_color.A;
						 des_color->R += src_color.R;
						 des_color->G += src_color.G;
						 des_color->B += src_color.B;
					 }
					 
				 }

			 }
			 if (roll) {
				 for (int c = 0; c < width; c++) {

					 switch (colorformat)
					 {
					 case PineColorFormat::RGBA_1BYTE: {
						 argb[des_buff_ron* width * 4 + c * 4] = (BYTE)des_buff[c].A;
						 argb[des_buff_ron* width * 4 + c * 4 + 1] = (BYTE)des_buff[c].R;
						 argb[des_buff_ron* width * 4 + c * 4 + 2] = (BYTE)des_buff[c].G;
						 argb[des_buff_ron* width * 4 + c * 4 + 3] = (BYTE)des_buff[c].B;
					 }break;
					 case PineColorFormat::RGBA_4444: {
						 BYTE a = (BYTE)des_buff[c].A;
						 BYTE r = (BYTE)des_buff[c].R;
						 BYTE g = (BYTE)des_buff[c].G;
						 BYTE b = (BYTE)des_buff[c].B;

						 BYTE byte1 = ((a << 4) & 0xF0) | (r & 0x0F);
						 BYTE byte2 = ((g << 4) & 0xF0) | (b & 0x0F);

						 argb[des_buff_ron* des_line_width + c * 2] = byte1;
						 argb[des_buff_ron* des_line_width + c * 2 + 1] = byte2;
					 }break;
					 case PineColorFormat::RGB_1BYTE: {
						 argb[des_buff_ron* des_line_width + c * 3] = (BYTE)des_buff[c].R;
						 argb[des_buff_ron* des_line_width + c * 3 + 1] = (BYTE)des_buff[c].G;
						 argb[des_buff_ron* des_line_width + c * 3 + 2] = (BYTE)des_buff[c].B;
					 }break;
					 case PineColorFormat::RGBA_5551: {
						 BYTE a = (BYTE)des_buff[c].A;
						 BYTE r = (BYTE)des_buff[c].R;
						 BYTE g = ((BYTE)des_buff[c].G) & 0x1F;
						 BYTE b = (BYTE)des_buff[c].B;

						 BYTE byte1 = ((r << 3) & 0xF8) | (g >> 3 & 0x07);
						 BYTE byte2 = ((g << 6) & 0xC0) | ((b << 1) & 0x3E) | (a & 0x01);

						 argb[des_buff_ron* des_line_width + c * 2] = byte1;
						 argb[des_buff_ron* des_line_width + c * 2 + 1] = byte2;
					 }break;
					 case PineColorFormat::RGB_565: {
						 BYTE a = (BYTE)des_buff[c].A;
						 BYTE r = (BYTE)des_buff[c].R;
						 BYTE g = ((BYTE)des_buff[c].G) & 0x1F;
						 BYTE b = (BYTE)des_buff[c].B;

						 BYTE byte1 = ((r << 3) & 0xF8) | (g >> 3 & 0x07);
						 BYTE byte2 = ((g << 5) & 0xE0) | (b & 0x1F);

						 argb[des_buff_ron* des_line_width + c * 2] = byte1;
						 argb[des_buff_ron* des_line_width + c * 2 + 1] = byte2;
					 }break;
					 default:
						 break;
					 }
					 des_buff[c] = des_buff[width + c];
					 des_buff[width + c].A = 0;
					 des_buff[width + c].R = 0;
					 des_buff[width + c].G = 0;
					 des_buff[width + c].B = 0;
				 }
				 
				 des_buff_ron++;
			 }
		 }
		 delete[] des_buff;
		 delete[] src_buff;
		 
		 size.W = width;
		 size.H = height;

#if defined(_USE_TEST)
		 //TEST_LOG_FILE("scale_texture.raw", (const char*)argb, des_buff_ron * des_line_width);
#endif
	 }

	 return size;
 }
 void CSprite::SetTextureScale(Graphic* g, float scale) {
	 if (scale > 0 && scale < 1) {
		 if (g->IsTexturePowOf2()) {
			 if (scale <= 0.25f) {
				 _texture_scale = 0.25f;
			 }
			 else{
				 _texture_scale = 0.5f;
			 }
		 }
		 else {
			 _texture_scale = scale;
		 }
		 _texture_texcoord_delta = (int)(1.0f / _texture_scale);
	 }
 }
