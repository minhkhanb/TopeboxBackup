#ifndef _ANIMATION_H
#define _ANIMATION_H
#include "../DataType/CType.h"
#include "../OpenGLES/Graphic.h"
#include "../Math/PinePolygon.h"
#if defined(ANDROID_OS)
#	include <stdlib.h>
#endif

#include "../Math/CMath.h"
#include "../DataInputStream/DataInputStream.h"

//general flag
#define		_FLAG_FREE_TRANSFORM				1//(1<<0)
#define		_FLAG_MULTI_MODULE_TYPE	 			2//(1<<1)
#define		_FLAG_EXPORT_FM_OPACITY				4//(1<<2)
#define		_FLAG_CLIP_BUG_FIXED				16//(1<<4)
//quality type
#define		_EXPORT_QUALITY_TRUE_COLOR 			1//(1<<0)
#define		_EXPORT_QUALITY_INDEX_COLOR			2//(1<<1)
//image data type
#define		_EXPORT_TYPE_3D_TEXTURE				1//(1<<0)
#define		_EXPORT_TYPE_MODULE_IMAGE			2//(1<<1)
//pixel format type	
#define		_EXPORT_PIXEL_32BITS				1//(1<<0)
#define		_EXPORT_PIXEL_16BITS				2//(1<<1)

//image format
#define 	_IMAGE_FORMAT_RAW_0888	 			4//(1<<2)
#define 	_IMAGE_FORMAT_RAW_8888	 			8//(1<<3)
#define 	_IMAGE_FORMAT_RAW_1888	 			16//(1<<4)

#define		_FLIP_H								1//(1<<0)
#define		_FLIP_V								2//(1<<1)
#define		_FREE_TRANSFORM						4//(1<<2)

#define		_LEFT								1//(1<<0)
#define		_RIGHT								2//(1<<1)
#define		_HCENTER							4//(1<<2)
#define		_TOP								8//(1<<3)
#define		_BOTTOM								16//(1<<4)
#define		_VCENTER							32//(1<<5)

#define		_BACK			1//(1<<0)
#define		_FRONT			2//(1<<1)
//#define		_FLAG_EXPORT_FM_ANGLE				(1<<0)
//#define		_FLAG_EXPORT_FM_OPACITY				(1<<1)
//#define		_FLAG_EXPORT_FM_SCALE				(1<<2)

#define		_PAL_LUMINANCE						1

#define	_MODULE_TYPE_IMAGE			0
#define	_MODULE_TYPE_LINE			1
#define	_MODULE_TYPE_RECT			2
#define	_MODULE_TYPE_FILL_RECT		3
#define	_MODULE_TYPE_ROUND_RECT		4
#define	_MODULE_TYPE_FILL_ROUND_RECT	5
#define	_MODULE_TYPE_ELLIPSE		6
#define	_MODULE_TYPE_FILL_ELLIPSE	7
#define _NUM_PAL_MAX (100)

//hao.ly add TEXT color MASK
#define TOTAL_TEXT_MASK_SUPPORT		(10)

class CGame;
class CSprite
{
protected:

	static UINT8*	_unzip_buff;
	static UINT32	_unzip_buff_length;
	float _texture_scale;
	int _texture_texcoord_delta;
	bool _has_texture;
public:
    float _current_page_height;
	static UINT8* _rle_ind_buffer;
	static UINT8* _rle_alp_buffer;
	static UINT8* _ind_buffer;
	static UINT8* _alp_buffer;
	bool			_fixed_clip_bug;
	int				_num_of_module_replay;
	//int				_module_replay_data[_NUM_OF_REPLAY_MODULE_MAX*3];//module replay:module, sprite replace, module will replace;
	INT16*			_module_replay_data;
	void			ClearReplace();
	void			ReplaceModule(int module,int sprite,int module_replace);
	void			ClearReplace(int module);
	bool			_pal_loaded[_NUM_PAL_MAX];
	BYTE			_flag;
	BYTE			_export_quality;
	BYTE			_export_image_type;
	BYTE			_export_pixel_type;
	
	INT32			_num_texture_images;
	BYTE* 			_image_format;	
	int*			_backup_pos;
	int				_backup_pallete;
	bool			_backup_antialias;
	
	INT32*	 		_texture_image_w;
	INT32*	 		_texture_image_h;
	HTEXTURE** 		_texture_image;
	BYTE**			_texture_image_data;
	INT32**			_texture_mask;
	
	INT32 			_num_modules;
	BYTE* 			_module_img_id;
	INT16* 			_module_x;
	INT16* 			_module_y;
	INT16* 			_module_w;
	INT16* 			_module_h;
	BYTE* 			_module_type;
	UINT32* 		_module_color;
	BYTE* 			_module_thickness;
	INT16* 			_module_ext_param1;
	
	INT32			_num_frames;
	INT32*			_frame_offset;
	
	INT32			_num_fmodules;
	INT16*			_fmodule_id;
	INT16*			_fmodule_x;
	INT16*			_fmodule_y;
	BYTE*			_fmodule_transf;
/*#ifdef USE_FM_ROTATION
	INT16*			_fmodule_angle;
#endif*/
#ifdef USE_FM_OPACITY
	BYTE*			_fmodule_opacity;
#endif

/*#ifdef USE_FM_SCALE
	INT16*			_fmodule_scale;
#endif*/
	
	float*			_fmodule_ox;
	float*			_fmodule_oy;
	float*			_fmodule_m11;
	float*			_fmodule_m12;
	float*			_fmodule_m21;
	float*			_fmodule_m22;
	
	INT32			_num_anims;
	INT16*			_anim_offset;
	
	INT32			_num_aframes;
	INT16*			_aframe_id;
	INT16*			_aframe_x;
	INT16*			_aframe_y;
	BYTE*			_aframe_transf;
	BYTE*			_aframe_time;
	
	bool			_has_alpha;
	
	INT32   _word_spacing;
	INT32	_char_spacing;
	INT32	_line_spacing;
	INT32	_fontHeight;
	INT32	_fontSpaceWidth;

	float 	_uv[4];
	UNICHAR _uni_buffer[10240];
	char	_ansi_buffer[10240];

	bool			_use_true_color_texture;
	bool			_use_palette_with_optimize_texture;
	
	INT32			_current_anim;
	INT32			_min_aframes;
	INT32			_max_aframes;
	INT32			_current_aframes;
	INT32			_current_aframes_time;
	bool			_is_current_anim_stop;//run or stop
	
	INT8			_current_pal;
	INT8			_num_pal;

	int _image_nums;
	int* _palette_image_num;
	UINT8*** _palette_table;
	bool _palette_table_loaded;
protected:
	INT32 GetFrameFirstModule(INT32 frame_id);
	INT32 GetFrameLastModule(INT32 frame_id);
public:
	static void BindUnzipBuffer(UINT8* buff, UINT32 len)
	{
		_unzip_buff = buff;
		_unzip_buff_length = len;
	};

	void PreplaceTexture(BYTE* argb,int width, int height,int textid,int palette=0);
	CSprite();
	~CSprite();
	int GetAnimNum(){return _num_anims;};
	//void Load(BYTE* bin, INT32 offset, bool is_create_texture = true);
    void Load(Graphic* g, DataInputStream* is, bool is_create_texture = true, int linear_texture = true, INT8 palette_mask = 1, bool load_trueColor_texture = true,int withPalette=0, bool isInThread = false);
	void LoadFromRGBA(Graphic* g, DataInputStream* is, BYTE* rgba, int width, int height, bool is_create_texture = true, bool linear_texture = true, INT8 palette_mask = 1, bool load_trueColor_texture = true, int withPalette = 0);
	void Unload(Graphic* g);
	
	//CSprite* CloneSprite();	//warning: do not clone image texture and image data
	//void ColorBlendingAllTexture(BYTE** tex_data, int color);
	void ClearAllTexture();
	void CacheTextureData(int tex_id);

	void SetCurrentPalette(INT8 pal);
	void DrawModule(Graphic* g, INT32 module_id, float x, float y, INT32 transform = 0, INT32 opacity = 100, INT8 palette = -1, int withpalette = 0);
	void DrawModule(Graphic* g, INT32 module_id, float x, float y, float scale_x, float scale_y, INT32 transform = 0, INT32 opacity = 100, INT8 palette = -1, int withpalette = 0);
	void DrawFrame(Graphic* g, INT32 frame_id, float x, float y, INT32 transform = 0, INT8 palette = 0, int withpalette = 0, bool hasOverlay = false, int overlay_from = 0, CSprite* sprite_add = NULL, int overlay_add = 0);
	void DrawFrame(Graphic* g, INT32 frame_id, float x, float y, float scale_x, float scale_y, INT32 transform = 0, INT8 palette = 0, int withpalette = 0, bool hasOverlay = false, int overlay_from = 0, CSprite* sprite_add = NULL, int overlay_add = 0);
	void DrawCurrentAFrame(Graphic* g, INT32 anim_id, float x, float y, INT32 transform, INT8 palette = 0, bool hasOverlay = false, int overlay_from = 0, CSprite* sprite_add = NULL, int overlay_add = 0);
	void DrawCurrentAFrame(Graphic* g, INT32 anim_id, float x, float y);
	void DrawAFrame(Graphic*g, INT32 aframe, float x, float y, INT32 transform = 0);
	int GetModuleOfFrame(int frame_id);
	void NextAFrame(bool is_loop = false);
	void SetAnim(INT32 anim, INT32 aframe, INT32 cur_aframe_time, bool is_loop = false);
	INT32 GetFirstAFrameID(INT32 anim);
	INT32 GetFirstAFrameTime(INT32 anim);
	INT32 GetTotalAnimTime(INT32 anim);
	bool IsAnimStop(INT32 anim_id);
	bool IsInside(int x, int y, INT32 aframe);// check if the point is inside current frame	
	void DrawAnsiPageFormat(Graphic* g, float x, float y, int w, INT32 anchor, const char* text, ...);
	void CalcPage(INT32 data[], UNICHAR* text, INT32 length, INT32 w);
	void DrawPage(Graphic* g, UNICHAR* text, INT32 data[], float x, float y, INT32 anchor);
	void DrawPage(Graphic* g, UNICHAR* text, INT32 data[], INT32 lenght, float x, float y, INT32 anchor); //Nghia.dothanh add
	void DrawPage(Graphic* g, UNICHAR* text, INT32 length, float x, float y, int w, INT32 anchor);
    void DrawPageNText(Graphic* g, UNICHAR* text, INT32 length, int render_len,float x, float y, int w, INT32 anchor);
    void DrawPageNTextRange(Graphic* g, UNICHAR* text, INT32 length, int iRenderBegin, int iRenderEnd,float x, float y, int w, INT32 anchor);
	void DrawAnsiText(Graphic* g, float x, float y, INT32 anchor, const char* text);
	void DrawAnsiTextFormat(Graphic* g, float x, float y, INT32 anchor, const char* text, ...);
	void DrawText(Graphic* g, UNICHAR* text, INT32 length, float x, float y, INT32 anchor);
	void DrawText(Graphic* g, UNICHAR* text, INT32 offset, INT32 length, float x, float y, INT32 anchor);
	void DrawText(Graphic* g, UNICHAR* text, INT32 offset, INT32 length, INT32 textwidth, float x, float y, INT32 anchor);
	INT32 GetAnsiTextWidth(const char* text, INT32 offset, INT32 length);
	INT32 GetTextWidth(UNICHAR* text, INT32 offset, INT32 length);
	INT32 GetCharWidth(UNICHAR c);
	INT32 GetFontHeight();
	
	INT32 GetFrameWidth(INT32 frame_id);
	INT32 GetFrameHeight(INT32 frame_id);
	int GetCurrentAFrameWidth(INT32);
	int GetCurrentAFrameHeight(INT32);
	void SetLineSpacing(INT32 space);
	void SetCharSpacing(INT32 space);
	void SetWordSpacing(INT32 space);
	UNICHAR* StringToUnichar(const char* buffer);
	//nghia.do thanh 
	const char* UnicharToString(UNICHAR* text, int len);
	
	
	int* GetAFCollisionCircle(int aframe, int x, int y, int r);
	int* GetAFBoundaryCircle(int aframe);
	void FreePalette(int pal);
	bool PaletteLoaded(int pal);
	//GinHa.do added
	int GetFModuleNum(int frame_id);
	int GetFModuleX(int frame_id, int fmodule_id);
	int GetFModuleY(int frame_id, int fmodule_id);
	PinePoint GetFModulePos(int frame_id, int fmodule_id);
	PineRectangle GetFmoduleBound(int frame_id, int fmodule_id);
	int GetFModuleWidth(int frame_id, int fmodule_id);
	int GetFModuleHeight(int frame_id, int fmodule_id);
	int GetFModuleCenterX(int frame_id,int fmoudle_id){return GetFModuleX(frame_id,fmoudle_id)+GetFModuleWidth(frame_id,fmoudle_id)/2;};
	int GetFModuleCenterY(int frame_id,int fmoudle_id){return GetFModuleY(frame_id,fmoudle_id)+GetFModuleHeight(frame_id,fmoudle_id)/2;}; 
	PineRectangle GetFrameBound(int frame_id);
	PinePolygon GenPolygonFromFrame(int frame_id);
#if defined(USE_FM_OPACITY)
	int GetFModuleOpacity(int frame_id, int fmodule_id);
#endif
	int GetFrameIDOfAFrame(int aframe_id);
	PinePoint GetAFramePos(int aframe_id);
#ifdef ANDROID_OS
	//void FreeTexture();
	//void ReloadTexture(ResourceInputStream* is);
#endif

	void CreateSprite(BYTE* argb, INT32 w, INT32 h);

	//void ChangeImageData();


protected:
    HTEXTURE CreateTextureImage(Graphic*g, INT32 tex_id, UINT8* argb, INT32 w, INT32 h, int linear_texture = true, INT8 pal = 0,int withPalette = 0,int texture_name = -1, bool isInThread = false);
	
	INT32 UnzipRAWImage(PineColorType color_type, BYTE* data, INT32 offset, BYTE format, UINT8* output, INT32 w, INT32 h);
	void UnzipRAWImage(PineColorType color_type, DataInputStream* is, BYTE format, UINT8* output, INT32 w, INT32 h);
	void DecodeRLE(UINT8* in, UINT8* out, int rle_size, int rle_flag);

#ifdef USE_COLOR_MASK
	//hao.ly add TEXT color mask
	int _maskIndex[TOTAL_TEXT_MASK_SUPPORT];
	COLOR _maskColor[TOTAL_TEXT_MASK_SUPPORT];
	int _colorMaskIndex;
#endif	
public:	

	void CalculateCurrentAFrame(INT32 anim_id);

	void DrawAnsiNTextFormat(Graphic* g, float x, float y, int length, INT32 anchor, const char* text, ...);
    void DrawAnsiNTextRangeFormat(Graphic* g, float x, float y, int iRenderBegin, int iRenderEnd, INT32 anchor, const char* text, ...);

    void DrawAnsiNText(Graphic* g, float x, float y, int length, INT32 anchor, const char* text);
    void DrawAnsiNTextRange(Graphic* g, float x, float y, int iRenderBegin, int iRenderEnd, INT32 anchor, const char* text);
	void DrawNText(Graphic* g, UNICHAR* text, INT32 offset, INT32 length, INT32 textwidth, INT32 x, INT32 y, INT32 anchor, int lenght_render);
    void DrawNTextRange(Graphic* g, UNICHAR* text, INT32 offset, INT32 length, INT32 textwidth, INT32 x, INT32 y, INT32 anchor, int iRenderBegin, int iRenderEnd);
	void DrawNText(Graphic* g, UNICHAR* text, INT32 offset, INT32 length, INT32 x, INT32 y, INT32 anchor, int lenght_render);
    void DrawNTextRange(Graphic* g, UNICHAR* text, INT32 offset, INT32 length, INT32 x, INT32 y, INT32 anchor,int iRenderBegin, int iRenderEnd);
    
	void DrawNText(Graphic* g, UNICHAR* text, INT32 length, INT32 x, INT32 y, int lenght_render, INT32 anchor);
    void DrawNTextRange(Graphic* g, UNICHAR* text, INT32 length, INT32 x, INT32 y, int iRenderBegin, int iRenderEnd, INT32 anchor);


	int GetWidthAnsiTextFormat(const char* text, ...);
	int GetWidthAnsiText(const char* text);
	int GetWidthText(UNICHAR* text, INT32 offset, INT32 length);


	static void Embossing(int* pixels,int width,int height);
	static int GetGray(int color);
	static int GetCorrectChannel(int value);
	static int GetCorrectColor(int r, int g, int b);
	static void NegateImage(int* pixels,int width,int height);
	static void ConvertColorToARGB(int* color, BYTE* argb,int width,int height);
	static void ConvertARGBToColor(BYTE* argb,int* color,int width, int height);
	int _is_noClipping;
	void SetNoClipping()
	{
		_is_noClipping = true;
	};
	void ClearNoClipping()
	{
		_is_noClipping = false;
	};

	//hao.ly add
	void DrawAnsiTextPageFormat(Graphic* g, float x, float y, int length, INT32 anchor, const char* text);
	void DrawAnsiTextPageFormatCustomColor(Graphic* g, float x, float y, int width, INT32 anchor, const char* format, ...);
    void DrawAnsiNTextPageFormatCustomColor(Graphic* g, float x, float y,int length, int width, INT32 anchor, const char* format, ...);
    void DrawAnsiNTextPageFormatCustomColorRange(Graphic* g, float x, float y,int iRenderBegin, int iRenderEnd, int width, INT32 anchor, const char* format, ...);
	//nhut.hoang add
	float DrawAnsiTextPageFormatScroll(Graphic* g, float x, float y, int length, INT32 anchor, const char* text);

#ifdef USE_COLOR_MASK
	//hao.ly add TEXT color mask
	void ResetTextColorMask();
#endif
	
	void unloadTexture(Graphic* g);
    void reloadTexture(Graphic* g, DataInputStream* is, bool isInThread = false);

	//duy.nguyen scale Image;
	PineSize scaleBuffer(BYTE* argb, PineColorFormat colorformat, int src_width, int src_height);
protected:
	void SetTextureScale(Graphic* g, float scale);
public:
	friend CGame;
};

#endif