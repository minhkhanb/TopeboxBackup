#if !defined(_H_GLESLIB_1)
#define _H_GLESLIB_1
#include "Graphic.h"
#include "../../Define/Defines.h"

#ifdef MAC_OS
#	include <OpenGLES/ES1/gl.h>
//#	include <GLES/gl.h>
#elif ANDROID_OS
#include <GLES/gl.h> 
#else
#	include <windows.h>
#	include <gl/gl.h>
#	include "./win/glext.h"
#endif
///////////////////////////////////////////
//DEFINE CONST
///////////////////////////////////////////
#define VTX_COUNT_PER_QUAD			4
#define VTX_COUNT_PER_TRI			3
#define VTX_COUNT_PER_LINE			2
#define VTX_COUNT_PER_TRIANGLE		3
#define TRI_COUNT_PER_QUAD			2
#define VTX_COUNT_PER_CIRCLE		19
#define VTX_SIZE					2
#define COLOR_SIZE					4
#define TEXCOOR_SIZE				2
#define MODEL_SIZE					1
#define MODEL_COUNT_PER_QUAD		6

#define NUM_ROTATE_PARAMS			3
#define NUM_SCALE_PARAMS			4
#define NUM_TRANSLATE_PARAMS		2

//flag
#define TRANSP_FLAG					1//(1<<0)
#define SETCLIP_FLAG				2//(1<<1)
#define CLEARCLIP_FLAG				4//(1<<2)
#define DRAWLINE_FLAG				8//(1<<3)
#define ROTATE_FLAG					16//(1<<4)
#define FREE_TRANSFORM_FLAG			32//(1<<5)
#define SCALE_FLAG					64//(1<<6)
#define DRAWCIRCLE_FLAG				128//(1<<7)
#define TRANSLATE_FLAG				256//(1<<8)
#define DRAWTRIANGLE_FLAG			512//(1<<9)
#define ZOOM_FLAG					1024//(1<<10)


class GLESLib1 :public Graphic
{
private:
	GraphicContext  _curr_gcontext;
	
	//render manager
	float			_render_width;
	float			_render_height;
	
	VTXUNIT*		_pVtxBuffer2D;
	UINT8*			_pClrBuffer2D;
	float*			_pTexBuffer2D;
	HTEXTURE*		_pTexID;
	UINT16*			_pIndices;
	UINT16*			_pFlags;
	int				_nNum2DQuads;
	//color
	COLOR			_nColor2D;
#if defined(USE_COLOR_MASK)
	bool			_is_color_flag;
	COLOR			_color_mask;
#endif

	//rotate
#if defined(USE_GL_ROTATE)
	float*			_pRotateParams;
	bool			_is_rotate;
	float			_rotate_angle;
	float			_rotate_x;
	float			_rotate_y;
#endif

	//translate
#if defined(USE_TRANSLATE)
	float *			_pTranslateParams;
	bool			_is_translate;
	float			_translate_x;
	float			_translate_y;
#endif

	//scale
#if defined(USE_GL_SCALE)
	float*			_pScaleParams;
	bool			_is_scale;
	float			_scale_x;
	float			_scale_y;
	float			_scale_center_x;
	float			_scale_center_y;
#endif

	//opacity && anpha
#if defined(ENABLE_OPACITY_SETTING)
	int				_opacity_percent;
#ifdef USE_BLEND_ALPHA
	BLEND_MODE*			_fillter_mode;
	BLEND_MODE		_current_fillter_mode;
#endif
#endif

	//draw line
#if defined(USE_DRAW_LINE)
	UINT16*		_pLineIndices;
#endif
#if defined(USE_DRAW_TRIANGLE)
	UINT16*		_pTriangleIndices;
#endif
#if defined(USE_FREE_TRANSFORM)
	float*		_pModBuffer2D;
	PineTransformParam _transform_param;
	bool _is_transform;
#endif
	//private function
	PineClip _curr_clip;
	PineClip _viewport_clip;
	void Clip(int x, int y, int w, int h);

	PineClip _fullscreen_clip;
	//
	PineTransformParam _world_transform_param;
public:
    int GetCurrentOpacity(){ return _opacity_percent;};
	GLESLib1();
	~GLESLib1();
	////////////////////////////////////////////
	//IMPLEMENT GRAPHIC
	////////////////////////////////////////////
	//init and release function
	virtual bool IsTexturePowOf2();
	virtual void Init();
	virtual void Release();
	virtual void ChangeContext(GraphicContext context);
	virtual void BeginTransaction();
	virtual void Flush();
	virtual void CleanCurrentFlag();

	virtual float GetCurrentScaleX();
	virtual float GetCurrentScaleY();
	virtual float GetCurrentCenterScaleX();
	virtual float GetCurrentCenterScaleY();
	virtual int GetCurrentOpacityPercent();


	//function work with cliping
	virtual void SetClip(int x, int y, int w, int h);
	virtual void SetClip(PineClip clip);
	virtual void ClearClip();
	virtual PineClip GetCurrentClip();

	//function work with color
	virtual void SetColor(COLOR clr);
    virtual COLOR GetCurrentColor();
#if defined(USE_COLOR_MASK)
	virtual COLOR GetColorMask();
	virtual void SetColorMask(COLOR clr);
	virtual void ClearColorMask();
#endif

	//function work with opacity
#if defined(ENABLE_OPACITY_SETTING)
	virtual void SetOpacity(int percent);
#if defined(USE_BLEND_ALPHA)
	virtual void SetFillterMode(BLEND_MODE fillter_mode);
#endif
#endif

	//function work with rotation
#if defined(USE_GL_ROTATE)
	virtual bool HasRotate();
	virtual float GetCurrentRotateAngle();
	virtual float GetCurrentRotateX();
	virtual float GetCurrentRotateY();
	virtual void SetRotate(float angle, float x, float y);
	virtual void ClearRotate();
#endif

#if defined(USE_TRANSLATE)
	virtual void SetTranslate(float x, float y);
	virtual void ClearTranslate();
#endif

#if defined(USE_GL_SCALE)
	virtual bool HasScale();
	virtual void SetScale(float scale_x, float scale_y, float center_x, float center_y);
	virtual void ClearScale();
	virtual ScaleParam GetCurrentScaleParam();
#endif

	//function work with line
#if defined(USE_DRAW_LINE)
	virtual void DrawLine(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, bool enableAlpha = false);
	virtual void DrawLine(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, COLOR color1, COLOR color2);
#if defined(USE_DRAW_TRIANGLE)
	virtual void DrawTriangle(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, VTXUNIT x3, VTXUNIT y3, bool enableAlpha = false);
#endif
#if defined(USE_DRAW_CIRCLE)
	virtual void DrawCircle(VTXUNIT x, VTXUNIT y, float radius, int density, bool enableAlpha = false);
#endif
	virtual void DrawPolygon(PinePolygon polygon, bool enableAlpha = false);
#endif//USE_DRAW_LINE

	//function work with triangle
#if defined(USE_DRAW_TRIANGLE)
	virtual void FillTriangle(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, VTXUNIT x3, VTXUNIT y3, bool enableAlpha = false);
	virtual void FillTriangle(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, VTXUNIT x3, VTXUNIT y3,HTEXTURE tex_id, float u1, float v1, float u2, float v2, float u3, float v3, bool enableAnpha = true);
	virtual void FillPolygon(PinePolygon polygon, bool enableAlpha = false);
	virtual void FillPolygon(PinePolygon polygon, HTEXTURE texId, float tw, float th, float tx = 0, float ty = 0, bool enableAlpha = false);
#if defined(USE_DRAW_CIRCLE)
	virtual void FillCircle(VTXUNIT x, VTXUNIT y, float radius, int density, bool enableAlpha = false);
#endif
#endif

	//function work with rectangle
#if defined(USE_DRAW_LINE)
	virtual void DrawRect(VTXUNIT x, VTXUNIT y, VTXUNIT w, VTXUNIT h, bool enableAlpha = false);
#endif
	virtual void FillRect(VTXUNIT x, VTXUNIT y, VTXUNIT w, VTXUNIT h, bool enableAlpha = false);
	virtual void FillFullScreen(bool enableAlpha = false);
	//function work with texture
	virtual void Draw2DTexture(VTXUNIT x, VTXUNIT y, int w, int h, HTEXTURE texId, float uv[], int flags, bool enableAlpha);
#if defined(USE_FREE_TRANSFORM)
	virtual void LoadModel(float ox, float oy, float m11, float m12, float m21, float m22);
	virtual void SetTransform(PineTransformParam param);
	virtual void ClearTransform();
#endif

	virtual HTEXTURE createTexture(BYTE* input_data, int texw, int texh, PineColorFormat input_colorformat, int is_altialias = true);
	virtual void updateTexture(HTEXTURE tex, BYTE* input_data, int dw, int dh, int tx, int ty, PineColorFormat color_format);
	virtual void freeTexture(HTEXTURE tex);

	virtual bool IsEngineReady();

	virtual PineColorType ColorType();
	static Orientation 		_device_orientation;

	virtual void setWorldMatrix(PineTransformParam param);


#if defined(_DEBUG)
	static UINT32 _dbg_flag;
	static bool _create_dbg;
	static Graphic* _dbg;

	static std::vector<Graphic::GraphicDebugTexture> _debug_texture;
	static UINT64 _debug_tex_mem_use;
	static UINT64 _debug_tex_mem_use_max;

	virtual void setDebugFlagOn(UINT32 flag);
	virtual void setDebugFlagOff(UINT32 flag);
	virtual void debugInfo();

	static void DebugCacheRectangle(PineRectangle rect);
	static void DebugCacheLine(PinePoint p1, PinePoint p2);
	static void DebugCacheDrawTexBorder(PineRectangle rect);

	
#endif

};

#endif
