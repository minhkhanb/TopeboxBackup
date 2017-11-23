#if !defined(_H_PINE_SPRITE)
#define _H_PINE_SPRITE
#include "../DataType/CType.h"
#include "../DataInputStream/DataInputStream.h"
#include "../OpenGLES/Graphic.h"
#include "../Graphic/PineColorPalette.h"
#include "PineSpriteModule.h"
#include "PineSpriteFModule.h"
#include "PineSpriteFrame.h"
#include "PineSpriteAFrame.h"
#include "PineSpriteAnim.h"

#include "PineSpriteTexture.h"

#include <vector>

//general flag
#define		SPRITE_FLAG_FREE_TRANSFORM				1//(1<<0)
#define		SPRITE_FLAG_MULTI_MODULE_TYPE	 			2//(1<<1)
#define		SPRITE_FLAG_EXPORT_FM_OPACITY				4//(1<<2)
#define		SPRITE_FLAG_CLIP_BUG_FIXED				16//(1<<4)
//quality type
#define		SPRITE_EXPORT_QUALITY_TRUE_COLOR 			1//(1<<0)
#define		SPRITE_EXPORT_QUALITY_INDEX_COLOR			2//(1<<1)
//image data type
#define		SPRITE_EXPORT_TYPE_3D_TEXTURE				1//(1<<0)
#define		SPRITE_EXPORT_TYPE_MODULE_IMAGE			2//(1<<1)
//pixel format type	
#define		SPRITE_EXPORT_PIXEL_32BITS				1//(1<<0)
#define		SPRITE_EXPORT_PIXEL_16BITS				2//(1<<1)

//image format
#define 	SPRITE_IMAGE_FORMAT_RAW_0888	 			4//(1<<2)
#define 	SPRITE_IMAGE_FORMAT_RAW_8888	 			8//(1<<3)
#define 	SPRITE_IMAGE_FORMAT_RAW_1888	 			16//(1<<4)

#define		SPRITE_FLIP_H								1//(1<<0)
#define		SPRITE_FLIP_V								2//(1<<1)
#define		SPRITE_FREE_TRANSFORM						4//(1<<2)

class PineSpriteInfo
{
private:
	char _file_path[255];

	UINT8 _flag;
	//bool _use_true_color_texture;
	//bool _has_alpha;
	BYTE _export_quality;
	BYTE _export_image_type;
	BYTE _export_pixel_type;
	int _image_nums;
	std::vector<std::vector<PineColorPalette>> _img_palette;

	std::vector<PineSpriteModule> _modules;
	
	std::vector<PineSpriteFrame> _frames;
	std::vector<PineSpriteAFrame> _aframes;
	std::vector<PineSpriteAnim> _anims;
	std::vector<PineSpriteTexture> _textures;

	int _num_module;
	int _resoffset_module;

	int _num_frame;
	int _resoffset_frame;

	int _num_anim;
	int _resoffset_anim;

	int _num_texture;
public:
	std::vector<PineSpriteFModule> _fmodules;
	int FindModuleIndex(UINT32 frame, UINT32 fmodule);
	PineRectangle GetFrameBound(UINT32 frame);
	friend class PineSprite;
	
};

class PineSprite
{
private:
	static UINT8*	_unzip_buff;
	static UINT32	_unzip_buff_length;

	static void DecodeRLE(UINT8* in, UINT8* out, int rle_size, int rle_flag);

	static void passUnzipRawImage(PineSpriteInfo* info, DataInputStream* is, PineSpriteTexture tex);
	static void passUnzipRawImage(PineSpriteInfo* info, bool use_true_color_texture, bool use_alpha, DataInputStream* is, BYTE format, INT32 w, INT32 h);
	
public:
	//static PineSprite* Load(ResourceItem res);
	static void BindUnzipBuffer(UINT8* buff, UINT32 len);
	static Buffer LoadRawModuleImage(PineSpriteInfo* info, PineColorType color_type, PineSize& out_size, int module_index, int flip = 0);
	static void LoadRawModuleImageToBuffer(PineSpriteInfo* info, PineColorType color_type, Buffer& buff, int buff_w, int buff_h, PineSize& out_size, int module_index, int flip = 0, int tx = 0, int ty = 0);

	static void LoadRawFrameImage(PineSpriteInfo* info, PineColorType color_type,BYTE* out_buff, PineRectangle& out_bound, int frame_index);
	static void LoadInfo(DataInputStream* is, PineSpriteInfo& out_info);

	//util

	static void UnzipRAWImage(PineSpriteInfo* info, PineSpriteTexture tex, PineColorType color_type,PineSize& tex_size, BYTE pal_mask = 1);
	static void UnzipRAWImage(PineSpriteInfo* info, PineColorType color_type,bool use_true_color_texture, bool use_alpha, DataInputStream* is, BYTE format, UINT8* output, INT32 w, INT32 h);

#if defined(_DEBUG)
#	define PINE_SPRITE_DEBUG_NONE 0
#	define PINE_SPRITE_DEBUG_INFO 1

	static UINT32 _dbg_flag;
	static void SetDebugFlagOn(UINT32 flag);
	static void SetDebugFlagOff(UINT32 flag);

	static void DbgSpriteInfo(PineSpriteInfo* info);
#endif
};


#endif//_H_PINE_SPRITE