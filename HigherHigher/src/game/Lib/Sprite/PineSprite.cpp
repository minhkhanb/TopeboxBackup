#include "pch.h"
#include "PineSprite.h"
#include "../Graphic/GraphicUtil.h"

int PineSpriteInfo::FindModuleIndex(UINT32 frame, UINT32 fmodule)
{
	if (frame >= _frames.size()) return -1;

	PineSpriteFrame frm = _frames[frame];
	
	if (fmodule >= frm._fm_num) return -1;

	int fm_offset = frm._fm_offset + fmodule;

	PineSpriteFModule fm = _fmodules[fm_offset];

	return fm._mid;
}

PineRectangle PineSpriteInfo::GetFrameBound(UINT32 frame)
{
	INT32 fmodule_min = 0;
	INT32 fmodule_max = 0;
	PineRectangle rect;
	if (frame >= _frames.size()) return rect;
	PineSpriteFrame frm = _frames[frame];
	bool trans = _fmodules[frm._fm_offset]._flag & SPRITE_FREE_TRANSFORM;
	rect.X = trans ? 0: _fmodules[frm._fm_offset]._tx;
	rect.Y = trans ? 0:_fmodules[frm._fm_offset]._ty;

	PineSpriteModule module = _modules[_fmodules[frm._fm_offset]._mid];
	if (module._type == 0) {

		rect.W = module.Image.W;
		rect.H = module.Image.H;
	}
	else
	{
		rect.W = 0;
		rect.H = 0;;
	}


#if defined(USE_FREE_TRANSFORM)
	if (trans)
	{
		PineTransformParam tparam;
		tparam.m11 = _fmodules[frm._fm_offset]._m11;
		tparam.m12 = _fmodules[frm._fm_offset]._m12;
		tparam.m21 = _fmodules[frm._fm_offset]._m21;
		tparam.m22 = _fmodules[frm._fm_offset]._m22;
		tparam.TranslateX = _fmodules[frm._fm_offset]._tx;
		tparam.TranslateY = _fmodules[frm._fm_offset]._ty;
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

	for (INT32 i = frm._fm_offset; i < frm._fm_offset + frm._fm_num; i++)
	{
		PineSpriteModule mod = _modules[_fmodules[i]._mid];

		if (mod._type != 0)continue;

		trans = _fmodules[i]._flag & SPRITE_FREE_TRANSFORM;

		rect.X = trans? 0 : _fmodules[i]._tx;
		rect.Y = trans ? 0 : _fmodules[i]._ty;

		rect.W = mod.Image.W;
		rect.H = mod.Image.H;

#if defined(USE_FREE_TRANSFORM)
		if (trans)
		{
			PineTransformParam tparam;
			tparam.m11 = _fmodules[i]._m11;
			tparam.m12 = _fmodules[i]._m12;
			tparam.m21 = _fmodules[i]._m21;
			tparam.m22 = _fmodules[i]._m22;
			tparam.TranslateX = _fmodules[i]._tx;
			tparam.TranslateY = _fmodules[i]._ty;
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


UINT8*	PineSprite::_unzip_buff = NULL;
UINT32	PineSprite::_unzip_buff_length = 0;

void PineSprite::BindUnzipBuffer(UINT8* buff, UINT32 len)
{
	_unzip_buff = buff;
	_unzip_buff_length = len;
}

void PineSprite::LoadInfo(DataInputStream* is, PineSpriteInfo& out_info)
{
	sprintf(out_info._file_path, "%s", is->_file_path);

	out_info._flag = is->ReadInt8();
	out_info._export_quality = is->ReadInt8();
	out_info._export_image_type = is->ReadInt8();
	out_info._export_pixel_type = is->ReadInt8();
	out_info._image_nums = is->ReadInt8();

	//load palette for image if exist.
	for (int i = 0; i < out_info._image_nums; i++)
	{
		std::vector<PineColorPalette> lst_palette;
		int palette_num = is->ReadInt8(); //one image can have multi palette;
		for (int p = 0; p < palette_num; p++)
		{
			PineColorPalette palette;
			//each palette hold 256 color
			for (int c = 0; c < 256; c++)
			{
				PineColor color;
				color.ARGB.A = 0xFF;
				color.ARGB.R = is->ReadInt8();
				color.ARGB.G = is->ReadInt8();
				color.ARGB.B = is->ReadInt8();
			}
			//an unknown component
			is->ReadInt32();
		}
		out_info._img_palette.push_back(lst_palette);
	}
	//load module info
	int num_modules = is->ReadInt16();

	for (INT32 i = 0; i < num_modules; i++)
	{
		PineSpriteModule module;

#ifdef USE_MULTI_MODULE_TYPE
		if ((out_info._flag & SPRITE_FLAG_MULTI_MODULE_TYPE) != 0)
		{
			module._type = is->ReadInt8();
		}
#endif

		UINT16 w = is->ReadInt16();
		UINT16 h = is->ReadInt16();

#ifdef USE_MULTI_MODULE_TYPE
		if (module._type != 0)
		{
			//now only support module types rectangle and image
			module.Rect.Color = is->ReadInt32();
			is->ReadInt8();
			is->ReadInt16();
			
		}
		else
#endif
			if (out_info._export_image_type == SPRITE_EXPORT_TYPE_3D_TEXTURE)
			{
				if ((w & 0xFFFF) > 0 && (h & 0xFFFF) > 0)
				{
					module.Image.X = is->ReadInt16();
					module.Image.Y = is->ReadInt16();
					module.Image.ImageIndex = is->ReadInt8();
					module.Image.W = w;
					module.Image.H = h;
				}
			}
		out_info._modules.push_back(module);
	}

	//load frame infomation
	out_info._num_frame = is->ReadInt16();
	int num_fmodules = 0;

	for (INT32 i = 0; i < out_info._num_frame; i++)
	{
		PineSpriteFrame frame;
		frame._fm_num = is->ReadInt16();

		frame._fm_offset = (INT16)num_fmodules;

		num_fmodules += frame._fm_num;
		out_info._frames.push_back(frame);
	}
	int pos = is->GetPos();
	for (int i = 0; i < num_fmodules; i++)
	{
		PineSpriteFModule fm;
		fm._mid = is->ReadInt16();
		fm._flag = is->ReadInt8();

		if ((fm._flag & SPRITE_FREE_TRANSFORM) != 0)
		{
			fm._tx = is->ReadFloat32();
			fm._ty = is->ReadFloat32();
#ifdef USE_FREE_TRANSFORM
			fm._m11 = is->ReadFloat32();
			fm._m12 = is->ReadFloat32();
			fm._m21 = is->ReadFloat32();
			fm._m22 = is->ReadFloat32();
#else
			is->ReadFloat32();
			is->ReadFloat32();
			is->ReadFloat32();
			is->ReadFloat32();
			fm._m11 = fm._m22 = 1;
			fm._m12 = fm._m21 = 0;
#endif
		}
		else
		{
			fm._tx = (float)is->ReadInt16();
			fm._ty = (float)is->ReadInt16();
		}

#ifdef USE_FM_OPACITY
		if ((out_info._flag & SPRITE_FLAG_EXPORT_FM_OPACITY) != 0)
		{
			fm._opacity = is->ReadInt8();
		}
		else
		{
			fm._opacity = 100;
		}
#else
		if ((out_info._flag & SPRITE_FLAG_EXPORT_FM_OPACITY) != 0)
		{
			is->ReadInt8();
		}
#endif			
		out_info._fmodules.push_back(fm);
	}

	//load anim infomation
	UINT16 num_anims = is->ReadInt16();
	int num_aframes = 0;
	pos = is->GetPos();
	for (int a = 0; a < num_anims; a++)
	{
		PineSpriteAnim anim;
		anim._aframe_num = is->ReadInt16();
		anim._aframe_offset = num_aframes;
		num_aframes += anim._aframe_num;
		out_info._anims.push_back(anim);
	}

	for (int a = 0; a < num_aframes; a++)
	{
		PineSpriteAFrame aframe;

		aframe._fid = is->ReadInt16();
		aframe._x = is->ReadInt16();
		aframe._y = is->ReadInt16();
		aframe._trans = is->ReadInt8();
		aframe._time = is->ReadInt8();
	}

	//index color
	if (out_info._export_quality == SPRITE_EXPORT_QUALITY_INDEX_COLOR)
	{
		//big image
		if (out_info._export_image_type == SPRITE_EXPORT_TYPE_3D_TEXTURE)
		{
			out_info._num_texture = is->ReadInt8();
			for (int t = 0; t < out_info._num_texture; t++)
			{
				PineSpriteTexture tex;
				tex._format = is->ReadInt8();
				tex._num_mask_color = is->ReadInt8();
				tex._num_pal = is->ReadInt8();
				tex._backup_mask_pos = is->GetPos();
				tex._has_mask = true;
				for (int i = 0; i < tex._num_pal; i++)
				{
					for (int j = 0; j < tex._num_mask_color; j++)
					{
						INT32 color = is->ReadInt24();
					}
				}
				PineSpriteTextureData tex_data;

				tex_data._w = is->ReadInt16();
				tex_data._h = is->ReadInt16();
				tex_data._backup_pos = is->GetPos();

				tex_data._ind_size = is->ReadInt24();
				tex_data._alp_size = is->ReadInt24();

				is->Seek(SEEK_SET, is->GetPos() + tex_data._ind_size + tex_data._alp_size);

				tex._textures.push_back(tex_data);

				out_info._textures.push_back(tex);
			}
		}
		//each module is an image
		else
		{
			PineSpriteTexture tex;
			tex._format = is->ReadInt8();
			tex._num_mask_color = is->ReadInt8();
			tex._num_pal = is->ReadInt8();
			tex._backup_mask_pos = is->GetPos();
			tex._has_mask = true;
			for (int i = 0; i < tex._num_pal; i++)
			{
				for (int j = 0; j < tex._num_mask_color; j++)
				{
					INT32 color = is->ReadInt24();
				}
			}

			for (int m = 0; m < out_info._modules.size(); m++)
			{
				PineSpriteTextureData tex_data;
				tex_data._backup_pos = is->GetPos();
				tex_data._ind_size = is->ReadInt24();
				tex_data._alp_size = is->ReadInt24();

				is->Seek(SEEK_SET, is->GetPos() + tex_data._ind_size + tex_data._alp_size);

				tex._textures.push_back(tex_data);
			}

			out_info._textures.push_back(tex);
		}
	}
	//full color
	else
	{
		out_info._num_texture = is->ReadInt8();

		for (int t = 0; t < out_info._num_texture; t++)
		{
			PineSpriteTexture tex;
			tex._format = is->ReadInt8();
			tex._has_mask = false;

			if (tex._format == SPRITE_IMAGE_FORMAT_RAW_8888 ||
				tex._format == SPRITE_IMAGE_FORMAT_RAW_0888 ||
				tex._format == SPRITE_IMAGE_FORMAT_RAW_1888)
			{
				PineSpriteTextureData tex_data;
				tex_data._w = is->ReadInt16();
				tex_data._h = is->ReadInt16();
				tex_data._backup_pos = is->GetPos();
				tex._textures.push_back(tex_data);

				passUnzipRawImage(&out_info, is, tex);
				
			}

			out_info._textures.push_back(tex);
		}
	}

#if defined(_DEBUG)
	DbgSpriteInfo(&out_info);
#endif
}


Buffer PineSprite::LoadRawModuleImage(PineSpriteInfo* info, PineColorType color_type, PineSize& out_size, int module_index, int flip)
{
	Buffer buff;

	if (module_index >= 0 && module_index < info->_modules.size())
	{
		PineSpriteModule module = info->_modules[module_index];

		if (module._type == 0 && module.Image.ImageIndex >= 0 && module.Image.ImageIndex < info->_textures.size())
		{
			PineSpriteTexture tex = info->_textures[module.Image.ImageIndex];

			//this function douesn't not support mask yet, and doesn't support multipal also.
			PineSize tex_size;
			UnzipRAWImage(info, tex, color_type, tex_size);

			out_size.W = module.Image.W;
			out_size.H = module.Image.H;

			int img_size = out_size.W * out_size.H * 4;

			buff.Init(img_size);
			BYTE* buff_data = buff.Data();

			int imgx = module.Image.X;
			int imgy = module.Image.Y;
			int out_offset = 0;

			if (flip == 0)
			{
				for (int y = 0; y < out_size.H; y++)
				{
					int offsety = (imgy + y) * tex_size.W * 4;
					for (int x = 0; x < out_size.W; x++)
					{
						int offset = offsety + (imgx + x) * 4;
						buff_data[out_offset++] = _unzip_buff[offset];
						buff_data[out_offset++] = _unzip_buff[offset + 1];
						buff_data[out_offset++] = _unzip_buff[offset + 2];
						buff_data[out_offset++] = _unzip_buff[offset + 3];
					}
				}
			}
			else if (flip == 2)
			{
				for (int y = out_size.H -1; y >=0 ; y--)
				{
					int offsety = (imgy + y) * tex_size.W * 4;
					for (int x = 0; x < out_size.W; x++)
					{
						int offset = offsety + (imgx + x) * 4;
						buff_data[out_offset++] = _unzip_buff[offset];
						buff_data[out_offset++] = _unzip_buff[offset + 1];
						buff_data[out_offset++] = _unzip_buff[offset + 2];
						buff_data[out_offset++] = _unzip_buff[offset + 3];
					}
				}
			}
			else if (flip == 1)
			{
				for (int y = 0; y < out_size.H; y++)
				{
					int offsety = (imgy + y) * tex_size.W * 4;
					for (int x = out_size.W - 1 ; x >= 0; x--)
					{
						int offset = offsety + (imgx + x) * 4;
						buff_data[out_offset++] = _unzip_buff[offset];
						buff_data[out_offset++] = _unzip_buff[offset + 1];
						buff_data[out_offset++] = _unzip_buff[offset + 2];
						buff_data[out_offset++] = _unzip_buff[offset + 3];
					}
				}
			}
			else
			{
				for (int y = out_size.H -1; y >= 0; y--)
				{
					int offsety = (imgy + y) * tex_size.W * 4;
					for (int x = out_size.W -1; x >=0; x--)
					{
						int offset = offsety + (imgx + x) * 4;
						buff_data[out_offset++] = _unzip_buff[offset];
						buff_data[out_offset++] = _unzip_buff[offset + 1];
						buff_data[out_offset++] = _unzip_buff[offset + 2];
						buff_data[out_offset++] = _unzip_buff[offset + 3];
					}
				}
			}
		}
	}
	return buff;
}

static int debug_sprite_id = 0;

void PineSprite::LoadRawModuleImageToBuffer(PineSpriteInfo* info, PineColorType color_type, Buffer& buff, int buff_w, int buff_h, PineSize& out_size, int module_index,  int flip, int tx, int ty)
{
	if (module_index >= 0 && module_index < info->_modules.size())
	{
		PineSpriteModule module = info->_modules[module_index];

		if (module._type == 0 && module.Image.ImageIndex >= 0 && module.Image.ImageIndex < info->_textures.size())
		{
			PineSpriteTexture tex = info->_textures[module.Image.ImageIndex];

			//this function douesn't not support mask yet, and doesn't support multipal also.
			PineSize tex_size;
			UnzipRAWImage(info, tex, color_type, tex_size);

			out_size.W = module.Image.W;
			out_size.H = module.Image.H;
			
			BYTE* buff_data = buff.Data();

			int imgx = module.Image.X;
			int imgy = module.Image.Y;

			int out_offset = 0;
			int rx = 0, ry = 0;

			//tx = 0; ty = 0;

			if (flip == 0)
			{
				for (int y = 0; y < out_size.H; y++)
				{
					int offsety = (imgy + y) * tex_size.W * 4;
					for (int x = 0; x < out_size.W; x++)
					{
						ry = (ty + y);
						rx = (tx + x);
						if (ry < 0 || rx < 0) {
							printf("erorr here");
						}
						if (ry < 0 || rx < 0 || ry >= buff_h || rx >= buff_w)continue;

						out_offset = (ry * buff_w + rx) * 4;

						int offset = offsety + (imgx + x) * 4;
						buff_data[out_offset] = _unzip_buff[offset];
						buff_data[out_offset + 1] = _unzip_buff[offset + 1];
						buff_data[out_offset + 2] = _unzip_buff[offset + 2];
						buff_data[out_offset + 3] = _unzip_buff[offset + 3];
					}
				}
			}
			else if (flip == 2)
			{
				for (int y = out_size.H - 1; y >= 0; y--)
				{

					int offsety = (imgy + y) * tex_size.W * 4;
					for (int x = 0; x < out_size.W; x++)
					{
						ry = (ty + y);
						rx = (tx + x);

						if (ry >= buff_h || rx >= buff_w)continue;

						out_offset = (ry * buff_w + rx) * 4;

						int offset = offsety + (imgx + x) * 4;
						buff_data[out_offset++] = _unzip_buff[offset];
						buff_data[out_offset++] = _unzip_buff[offset + 1];
						buff_data[out_offset++] = _unzip_buff[offset + 2];
						buff_data[out_offset++] = _unzip_buff[offset + 3];
					}
				}
			}
			else if (flip == 1)
			{
				for (int y = 0; y < out_size.H; y++)
				{
					int offsety = (imgy + y) * tex_size.W * 4;
					for (int x = out_size.W - 1; x >= 0; x--)
					{
						ry = (ty + y);
						rx = (tx + x);

						if (ry >= buff_h || rx >= buff_w)continue;

						out_offset = (ry * buff_w + rx) * 4;

						int offset = offsety + (imgx + x) * 4;
						buff_data[out_offset++] = _unzip_buff[offset];
						buff_data[out_offset++] = _unzip_buff[offset + 1];
						buff_data[out_offset++] = _unzip_buff[offset + 2];
						buff_data[out_offset++] = _unzip_buff[offset + 3];
					}
				}
			}
			else
			{
				for (int y = out_size.H - 1; y >= 0; y--)
				{
					int offsety = (imgy + y) * tex_size.W * 4;
					for (int x = out_size.W - 1; x >= 0; x--)
					{
						ry = (ty + y);
						rx = (tx + x);

						if (ry >= buff_h || rx >= buff_w)continue;

						out_offset = (ry * buff_w + rx) * 4;

						int offset = offsety + (imgx + x) * 4;
						buff_data[out_offset++] = _unzip_buff[offset];
						buff_data[out_offset++] = _unzip_buff[offset + 1];
						buff_data[out_offset++] = _unzip_buff[offset + 2];
						buff_data[out_offset++] = _unzip_buff[offset + 3];
					}
				}
			}
		}
	}
}

#include "../../GameCore/Os_Functions.h"


static int debug_id = 0;
void PineSprite::LoadRawFrameImage(PineSpriteInfo* info, PineColorType color_type, BYTE* out_buff, PineRectangle& bound, int frame_index)
{
	if (frame_index < 0 || frame_index >= info->_frames.size()) {
		bound.W = 0;
		bound.H = 0;
		return;
	}
	PineRectangle rect = info->GetFrameBound(frame_index);
	bound.W = rect.W;
	bound.H = rect.H;
	bound.X = rect.X;
	bound.Y = rect.Y;

	int img_index = -1;

	if (rect.W > 0 && rect.H > 0)
	{
		GraphicUtil::FillTransparent(out_buff, bound.W * bound.H * 4);
		printf("output:w:%f h:%f i:%d\n", rect.W, rect.H, debug_id);
		
		PineSpriteFrame frm = info->_frames[frame_index];

		for (int fm = frm._fm_offset; fm < frm._fm_offset + frm._fm_num; fm++)
		{
			PineSize msize;
			PineSpriteFModule frm_module = info->_fmodules[fm];
			PinePoint real_pos = frm_module.GetRealPos();

			PineSpriteModule module = info->_modules[frm_module._mid];

			PineSize tex_size = {0, 0};

			if (module.Image.ImageIndex != img_index)
			{
				PineSpriteTexture tex = info->_textures[module.Image.ImageIndex];
				
				UnzipRAWImage(info, tex, color_type, tex_size);
				img_index = module.Image.ImageIndex;

				/*char file_name[255];
				sprintf(file_name, "raw_%d", debug_id++);
				FILE* tmp_file = fopen(file_name, "wb");
				if (tmp_file != NULL)
				{
					fwrite(_unzip_buff, 1, tex_size.W * tex_size.H * 4, tmp_file);
					fclose(tmp_file);
				}*/
				
			}

			PineRectangle rect;
			rect.X = (float)module.Image.X;
			rect.Y = (float)module.Image.Y;
			rect.W = (float)module.Image.W;
			rect.H = (float)module.Image.H;

			PineSize size;
			size.W = bound.W;
			size.H = bound.H;

			//GraphicUtil::DrawImage(out_buff, size, _unzip_buff, tex_size, rect, real_pos.X - rect.X, real_pos.Y - rect.Y, 4, info->_fmodules[fm]._flag);
			GraphicUtil::DrawImage(out_buff, size, _unzip_buff, tex_size,rect, real_pos.X, real_pos.Y, 4, info->_fmodules[fm]._flag);
			
			//break;
			//LoadRawModuleImageToBuffer(info, color_type, buff, rect.W, rect.H, msize, info->_fmodules[fm]._mid, info->_fmodules[fm]._flag, real_pos.X - rect.X, real_pos.Y - rect.Y);

			/*char file_name[255];
			sprintf(file_name, "raw_%d", debug_id++);
			FILE* tmp_file = fopen(file_name, "wb");
			if (tmp_file != NULL)
			{
				fwrite(out_buff, 1, bound.W * bound.H * 4, tmp_file);
				fclose(tmp_file);
			}*/
		}
	}
}

void PineSprite::UnzipRAWImage(PineSpriteInfo* info, PineSpriteTexture tex, PineColorType color_type, PineSize& tex_size, BYTE pal_mask)
{
	DataInputStream is;

	if (tex._textures.size() > 0 && is.Open(info->_file_path, 0, -1))
	{
		PineSpriteTextureData tex_data = tex._textures[0];
		tex_size.W = tex_data._w;
		tex_size.H = tex_data._h;

		if (info->_export_quality == SPRITE_EXPORT_QUALITY_INDEX_COLOR)
		{
			INT32** texture_mask = NULL;
			//tex._num_mask_color
			if (tex._has_mask)
			{
				is.Seek(SEEK_SET, tex._backup_mask_pos);
				INT32** texture_mask = new INT32*[tex._num_pal];
				for (int i = 0; i < tex._num_pal; i++)
				{
					texture_mask[i] = new INT32[tex._num_mask_color];

					for (int j = 0; j < tex._num_mask_color; j++)
					{
						UINT32 color = is.ReadInt24();
						texture_mask[i][j] = color;
					}
				}
			}
			

			UINT8* rle_ind_buffer = new UINT8[tex_data._ind_size];
			is.ReadData(rle_ind_buffer, tex_data._ind_size);

			UINT8* rle_alp_buffer = new UINT8[tex_data._alp_size];
			is.ReadData(rle_alp_buffer, tex_data._alp_size);

			is.Close();

			int img_size = tex_data._w * tex_data._h;

			// decode RLE
			UINT8* ind_buffer = new UINT8[img_size];
			UINT8* alp_buffer = new UINT8[img_size];

			DecodeRLE(rle_ind_buffer, ind_buffer, tex_data._ind_size, 0xFF);
			DecodeRLE(rle_alp_buffer, alp_buffer, tex_data._alp_size, 0xFE);


			for (int h = 0; (pal_mask >> h) > 0; h++)
			{
				if (((pal_mask >> h) & 1) == 1)
				{
					int offset = 0;
					for (int j = 0; j < img_size; j++)
					{
						_unzip_buff[offset++] = (texture_mask[h][ind_buffer[j]] >> 16) & 0xff;
						_unzip_buff[offset++] = (texture_mask[h][ind_buffer[j]] >> 8) & 0xff;
						_unzip_buff[offset++] = (texture_mask[h][ind_buffer[j]]) & 0xff;
						_unzip_buff[offset++] = alp_buffer[j];
					}
				}
			}


			delete[] rle_ind_buffer;
			delete[] rle_alp_buffer;
			delete[] ind_buffer;
			delete[] alp_buffer;

			for (int i = 0; i < tex._num_pal; i++)
			{
				texture_mask[i] = new INT32[tex._num_mask_color];
				delete[] texture_mask[i];
			}
			if (tex._has_mask)
			{
				delete[] texture_mask;
			}
		}
		else
		{
			if (tex._format == SPRITE_IMAGE_FORMAT_RAW_8888 ||
				tex._format == SPRITE_IMAGE_FORMAT_RAW_0888 ||
				tex._format == SPRITE_IMAGE_FORMAT_RAW_1888)
			{
				is.Seek(SEEK_SET, tex_data._backup_pos);
				UnzipRAWImage(info, color_type,true, true, &is, tex._format, _unzip_buff, tex_data._w, tex_data._h);
				

			}
		}
		
	}
	is.Close();
}

void PineSprite::UnzipRAWImage(PineSpriteInfo* info, PineColorType color_type, bool use_true_color_texture, bool use_alpha, DataInputStream* is, BYTE format, UINT8* output, INT32 w, INT32 h)
{
	//unzip RGB data
	INT32 i;
	INT32 img_size = 0;

	if (use_true_color_texture)
	{
		if (use_alpha)
		{
			img_size = w*h * 4;
		}
		else
		{
			img_size = w*h * 3;
		}
	}
	else
	{
		img_size = w*h * 2;
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

			if (format == SPRITE_IMAGE_FORMAT_RAW_8888)
			{
				if (info->_export_pixel_type == SPRITE_EXPORT_PIXEL_16BITS)
				{
					d = is->ReadInt8();

					b = d & 0x0F;
					b = b | (b << 4);

					g = d & 0xF0;
					g = g | (g >> 4);

					d = is->ReadInt8();

					r = d & 0x0F;
					r = r | (r << 4);

					a = d & 0xF0;
					a = a | (a >> 4);
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
				if (info->_export_pixel_type == SPRITE_EXPORT_PIXEL_16BITS)
				{
					d = is->ReadInt8();

					b = d & 0x1F;
					b = (b & 0x07) | (b << 3);

					g = (d & 0xE0) >> 5;

					d = is->ReadInt8();

					g = (g & 0x03) | (g << 2) | ((d & 0x07) << 5);

					r = d & 0xF8;
					r = r | ((r >> 3) & 0x07);

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
				if (use_true_color_texture)
				{
					//use 8888
					if (use_alpha)
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
					if (use_alpha)
					{
						//use 4444
						if (format == SPRITE_IMAGE_FORMAT_RAW_8888)
						{
							if (color_type == PineColorType::CTYPE_OPENGL) {
								output[i++] = (b & 0xF0) | ((a >> 4) & 0x0F);
								output[i++] = (r & 0xF0) | ((g >> 4) & 0x0F);
							}
							else if (color_type == PineColorType::CTYPE_DIRECTX) {
								output[i++] = (g & 0xF0) | ((b >> 4) & 0x0F);
								output[i++] = (a & 0xF0) | ((r >> 4) & 0x0F);
							}
						}
						//use 5551
						else if (format == SPRITE_IMAGE_FORMAT_RAW_1888)
						{
							if (color_type == PineColorType::CTYPE_OPENGL) {
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
						if (color_type == PineColorType::CTYPE_OPENGL) {
							output[i++] = ((g << 3) & 0xE0) | ((b >> 3) & 0x1F);
							output[i++] = (r & 0xF8) | ((g >> 5) & 0x07);
						}
						else if (color_type == PineColorType::CTYPE_DIRECTX) {
							output[i++] = ((g << 3) & 0xE0) | ((r >> 3) & 0x1F);
							output[i++] = (b & 0xF8) | ((g >> 5) & 0x07);
						}
					}
				}
			}
		}
		else
		{
			if (format == SPRITE_IMAGE_FORMAT_RAW_8888)
			{
				if (info->_export_pixel_type == SPRITE_EXPORT_PIXEL_16BITS)
				{
					b = val & 0x0F;
					b = b | (b << 4);

					g = val & 0xF0;
					g = g | (g >> 4);

					d = is->ReadInt8();

					r = d & 0x0F;
					r = r | (r << 4);

					a = d & 0xF0;
					a = a | (a >> 4);
				}
				//_EXPORT_PIXEL_32BITS
				else
				{
					b = val & 0xFF;
					g = is->ReadInt8();
					r = is->ReadInt8();
					a = is->ReadInt8();
				}
			}
			else
			{
				if (info->_export_pixel_type == SPRITE_EXPORT_PIXEL_16BITS)
				{
					b = val & 0x1F;
					b = (b & 0x07) | (b << 3);

					g = (val & 0xE0) >> 5;

					d = is->ReadInt8();

					g = (g & 0x03) | (g << 2) | ((d & 0x07) << 5);

					r = d & 0xF8;
					r = r | ((r >> 3) & 0x07);

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
					b = val & 0xFF;
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

			if (use_true_color_texture)
			{
				//use 8888
				if (use_alpha)
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
				if (use_alpha)
				{
					//use 4444
					if (format == SPRITE_IMAGE_FORMAT_RAW_8888)
					{
						if (color_type == PineColorType::CTYPE_OPENGL) {
							output[i++] = (b & 0xF0) | ((a >> 4) & 0x0F);
							output[i++] = (r & 0xF0) | ((g >> 4) & 0x0F);
						}
						else if (color_type == PineColorType::CTYPE_DIRECTX) {
							output[i++] = (g & 0xF0) | ((b >> 4) & 0x0F);
							output[i++] = (a & 0xF0) | ((r >> 4) & 0x0F);
						}
					}
					//use 5551
					else if (format == SPRITE_IMAGE_FORMAT_RAW_1888)
					{
						if (color_type == PineColorType::CTYPE_OPENGL) {
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
				//use 565
				else	
				{
					if (color_type == PineColorType::CTYPE_OPENGL) {
						output[i++] = ((g << 3) & 0xE0) | ((b >> 3) & 0x1F);
						output[i++] = (r & 0xF8) | ((g >> 5) & 0x07);
					}
					else if (color_type == PineColorType::CTYPE_DIRECTX) {
						output[i++] = ((g << 3) & 0xE0) | ((r >> 3) & 0x1F);
						output[i++] = (b & 0xF8) | ((g >> 5) & 0x07);
					}
				}
			}
		}
	}
}

void PineSprite::passUnzipRawImage(PineSpriteInfo* info, DataInputStream* is, PineSpriteTexture tex)
{

		PineSpriteTextureData tex_data = tex._textures[0];


		if (info->_export_quality == SPRITE_EXPORT_QUALITY_INDEX_COLOR)
		{
			int pos = tex._backup_mask_pos;
			//tex._num_mask_color
			if (tex._has_mask)
			{
				pos += (tex._num_pal * tex._num_mask_color * 3);
			}

			pos += tex_data._ind_size + tex_data._alp_size;

			is->Seek(SEEK_SET, pos);	

		}
		else
		{
			if (tex._format == SPRITE_IMAGE_FORMAT_RAW_8888 ||
				tex._format == SPRITE_IMAGE_FORMAT_RAW_0888 ||
				tex._format == SPRITE_IMAGE_FORMAT_RAW_1888)
			{
				is->Seek(SEEK_SET, tex_data._backup_pos);
				passUnzipRawImage(info,true, true, is, tex._format, tex_data._w, tex_data._h);
			}
		}

}

void PineSprite::passUnzipRawImage(PineSpriteInfo* info, bool use_true_color_texture, bool use_alpha, DataInputStream* is, BYTE format, INT32 w, INT32 h)
{
	//unzip RGB data
	INT32 i;
	INT32 img_size = 0;
	int fixel_size = 2;
	if (use_true_color_texture)
	{
		if (use_alpha)
		{
			img_size = w*h * 4;
			fixel_size = 4;
		}
		else
		{
			img_size = w*h * 3;
			fixel_size = 3;
		}
	}
	else
	{
		img_size = w*h * 2;
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

			if (format == SPRITE_IMAGE_FORMAT_RAW_8888)
			{
				if (info->_export_pixel_type == SPRITE_EXPORT_PIXEL_16BITS)
				{
					d = is->ReadInt8();
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
				if (info->_export_pixel_type == SPRITE_EXPORT_PIXEL_16BITS)
				{
					d = is->ReadInt8();
					d = is->ReadInt8();
				}
				else
				{
					b = is->ReadInt8();
					g = is->ReadInt8();
					r = is->ReadInt8();
				}
			}
		}
		else
		{
			if (format == SPRITE_IMAGE_FORMAT_RAW_8888)
			{
				if (info->_export_pixel_type == SPRITE_EXPORT_PIXEL_16BITS)
				{
					d = is->ReadInt8();
				}
				//_EXPORT_PIXEL_32BITS
				else
				{
					g = is->ReadInt8();
					r = is->ReadInt8();
					a = is->ReadInt8();
				}
			}
			else
			{
				if (info->_export_pixel_type == SPRITE_EXPORT_PIXEL_16BITS)
				{
					d = is->ReadInt8();
				}
				else
				{	
					g = is->ReadInt8();
					r = is->ReadInt8();
				}
			}
		}
		i += fixel_size;
	}
}

void PineSprite::DecodeRLE(UINT8* in, UINT8* out, int rle_size, int rle_flag)
{
	int offset = 0;
	int out_offset = 0;

	while (offset < rle_size)
	{
		if (in[offset++] == (BYTE)rle_flag)
		{
			int count = in[offset++];
			int val = in[offset++];

			for (int j = 0; j < count; j++)
			{
				out[out_offset++] = val;
			}
		}
		else
		{
			out[out_offset++] = in[offset - 1];
		}
	}
}
#if defined(_DEBUG)
UINT32 PineSprite::_dbg_flag = PINE_SPRITE_DEBUG_NONE;

void PineSprite::SetDebugFlagOn(UINT32 flag)
{
	_dbg_flag |= flag;
}
void PineSprite::SetDebugFlagOff(UINT32 flag)
{
	_dbg_flag &= ~flag;
}

void PineSprite::DbgSpriteInfo(PineSpriteInfo* info)
{
	if (_dbg_flag & PINE_SPRITE_DEBUG_INFO)
	{
		PDEBUG("Sprite Info\n");
		PDEBUG("file path:%s\n", info->_file_path);
		PDEBUG("num module:%d\n", info->_modules.size());
		PDEBUG("num frame:%d\n", info->_frames.size());
		PDEBUG("num anim:%d\n", info->_anims.size());
		PDEBUG("num texture:%d\n", info->_textures.size());
		PineSpriteTextureData* data;
		for (int t = 0; t < info->_textures.size(); t++)
		{

			PDEBUG("tex:%d\n", t);

			for (int d = 0; d < info->_textures[t]._textures.size(); d++)
			{
				data = &info->_textures[t]._textures[d];
				PDEBUG("texdata: w:%d h:%d\n", data->_w, data->_h);
			}
		}

	}
}
#endif