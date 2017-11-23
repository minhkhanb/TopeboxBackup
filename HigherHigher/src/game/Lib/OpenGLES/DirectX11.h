#if !defined(_H_DIRECTX11_GRAPHIC)
#define _H_DIRECTX11_GRAPHIC
#include "Graphic.h"
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <memory>
#define VTX_COUNT_PER_QUAD			4
#define VTX_COUNT_PER_TRI			3
#define VTX_COUNT_PER_LINE			2
#define VTX_COUNT_PER_TRIANGLE		3
#define TRI_COUNT_PER_QUAD			2
#define VTX_COUNT_PER_CIRCLE		19
#define VTX_SIZE					3
#define COLOR_SIZE					4
#define TEXCOOR_SIZE				2
#define MODEL_SIZE					1
#define MODEL_COUNT_PER_QUAD		6
#define NUM_ROTATE_PARAMS			3
#define NUM_SCALE_PARAMS			4
#define NUM_TRANSLATE_PARAMS		2
#define PARAM_SIZE 1

#define TRANSP_FLAG					(1<<0)
#define SETCLIP_FLAG				(1<<1)
#define CLEARCLIP_FLAG				(1<<2)
#define DRAWLINE_FLAG				(1<<3)
#define ROTATE_FLAG					(1<<4)
#define FREE_TRANSFORM_FLAG			(1<<5)
#define SCALE_FLAG					(1<<6)
#define DRAWCIRCLE_FLAG				(1<<7)
#define TRANSLATE_FLAG				(1<<8)
#define DRAWTRIANGLE_FLAG			(1<<9)
#define FILLTRIANGLE_FLAG			(1<<10)



#ifdef USE_BLEND_ALPHA
#define BLEND_ALPHA_FILLTER_NORMAL		0
#define BLEND_ALPHA_FILLTER_ADD			1
#define BLEND_ALPHA_FILLTER_CONSTRACT	2
#define BLEND_ALPHA_FILLTER_LIGHT		3
#endif

#define ALIGN_BIT_SIZE	16
#define ALIGN //__declspec(align(ALIGN_BIT_SIZE))
#define ALIGN_16	__declspec(align(16))
#define ALIGN_32	//__declspec(align(32))

using namespace DirectX;
using namespace Windows::Foundation;
ALIGN_16 class GDirectX11:public Graphic{

public:
	GDirectX11();
/////////////////////////////////////////////
/////GRAPHIC FUNCTIONs
/////////////////////////////////////////////
	virtual bool IsTexturePowOf2();
	//init and release function
	virtual void Init();
	virtual void Release();
	virtual void ChangeContext(GraphicContext context);
	virtual void BeginTransaction();
	virtual void Flush();
	virtual void CleanCurrentFlag();

	//get property
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
#if defined(USE_COLOR_MASK)
	virtual COLOR GetColorMask();
	virtual void SetColorMask(COLOR clr);
	virtual void ClearColorMask();
#endif

	//function work with opacity
#if defined(ENABLE_OPACITY_SETTING)
	virtual void SetOpacity(int percent);
	virtual int GetCurrentOpacity();
#if defined(USE_BLEND_MODE)
	virtual void SetFillterMode(BLEND_MODE fillter_mode);
#endif
#endif

	//function work with line
#if defined(USE_DRAW_LINE)
	virtual void DrawLine(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, bool enableAlpha = false);
	virtual void DrawLine(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, COLOR color1, COLOR color2);
#if defined(USE_DRAW_TRIANGLE)
	virtual void DrawTriangle(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, VTXUNIT x3, VTXUNIT y3, bool enableAlpha = false);
#if defined(USE_DRAW_CIRCLE)
	virtual void DrawCircle(VTXUNIT x, VTXUNIT y, float radius, int density, bool enableAlpha = false);
#endif
	virtual void DrawPolygon(PinePolygon polygon, bool enableAlpha = false);
#endif

#endif//USE_DRAW_LINE

	//function work with triangle
#if defined(USE_DRAW_TRIANGLE)
	virtual void FillTriangle(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, VTXUNIT x3, VTXUNIT y3, bool enableAlpha = false);
	virtual void FillTriangle(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, VTXUNIT x3, VTXUNIT y3, HTEXTURE tex_id, float u1, float v1, float u2, float v2, float u3, float v3, bool enableAnpha = true);
	virtual void FillPolygon(PinePolygon polygon, bool enableAlpha = false);
	virtual void FillPolygon(PinePolygon polygon, HTEXTURE texId, float tw, float th, float tx = 0, float ty = 0, bool enableAlpha = false);
#if defined(USE_DRAW_CIRCLE)
	virtual void FillCircle(VTXUNIT x, VTXUNIT y, float radius,int density, bool enableAlpha = false);
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
	virtual void LoadModel(float ox, float oy, float m11, float m12, float m21, float m22);
	virtual void SetTransform(PineTransformParam param);
	virtual void ClearTransform();

	//function work with rotation
#if defined(USE_GL_ROTATE)
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

	virtual HTEXTURE createTexture(BYTE* input_data, int texw, int texh, PineColorFormat input_colorformat, int is_altialias = true);
	virtual void freeTexture(HTEXTURE tex);

	virtual  bool IsEngineReady();
	virtual PineColorType ColorType();
	virtual void SetGlobalContext(float tx, float ty, float sx, float sy);
	virtual void updateTexture(HTEXTURE tex, BYTE* input_data, int dw, int dh, int tx, int ty, PineColorFormat color_format);
	////virtual void SetComplexMode(PineGraphicComplexOrientation mode);
///end graphic functions

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

/////////////////////////////////////////////
/////VARIABLE FOR ROTATION
/////////////////////////////////////////////
#if defined(USE_GL_ROTATE)
	bool		_is_rotate;
	//XMVECTOR	_vector_rotate;
	XMFLOAT4	_vector_rotate;
	XMFLOAT4X4	_mx_rotate_fre_translate;
	XMFLOAT4X4	_mx_rotate_post_translate;
#endif///end define variable for controlling rotate



/////////////////////////////////////////////
/////VARIABLE FOR TRANSLATION
/////////////////////////////////////////////
#ifdef USE_TRANSLATE
	bool	_is_translate;
	XMFLOAT4X4 _mx_translate;
#endif///end define variable for translation



/////////////////////////////////////////////
/////VARIABLE FOR SCALATION
/////////////////////////////////////////////

#ifdef USE_GL_SCALE
	bool		_is_scale;
	XMFLOAT4X4	_mx_scale;
	XMFLOAT4X4	_mx_scale_fre_translate;
	XMFLOAT4X4	_mx_scale_post_translate;

	bool				_is_global_scale;
	XMFLOAT4X4	_mx_global_scale;
	XMFLOAT4X4	_mx_global_scale_fre_translate;
	XMFLOAT4X4	_mx_global_scale_post_translate;
	
public:
	float		_scale_x;
	float		_scale_y;
	float		_scale_center_x;
	float		_scale_center_y;

	float		_global_scale_x;
	float		_global_scale_y;
	float		_global_scale_center_x;
	float		_global_scale_center_y;
private:
#endif ///end define variable for scalation

/////////////////////////////////////////////
/////VARIABLE FOR TRANSFORM
/////////////////////////////////////////////
#if defined(USE_FREE_TRANSFORM)
	bool		_is_transform;
	PineTransformParam _transform_param;
#endif

/////////////////////////////////////////////
/////VARIABLE FOR COLOR MASK
/////////////////////////////////////////////
#ifdef USE_COLOR_MASK
	UINT32 _nColorMask;
	bool _is_color_mask;
#endif///end define variable for color mask


/////////////////////////////////////////////
/////VARIABLE FOR MANAGEMENT
/////////////////////////////////////////////
	UINT32			_nColor2D;
	int				_nNum2DQuads;
	VTXUNIT*		_pVtxBuffer2D;

	ID3D11Buffer*	_dxVertexBuffer;
	int				_curr_vertex;
	int*			_vertexPos;
	D3D11_BUFFER_DESC   _vtexdesc;
	HTEXTURE*		_pTexID;
	UINT16*			_pFlags;

#ifdef USE_FREE_TRANSFORM
	float*			_pModBuffer2D;
#endif

#ifdef ENABLE_OPACITY_SETTING
	int				_opacity_percent;
#	ifdef USE_BLEND_ALPHA
	BLEND_MODE*			_fillter_mode;
	BLEND_MODE		_current_fillter_mode;
#	endif	
#endif
	
private:
	int				_render_width;
	int				_render_height;
	float			_device_width;
	float			_device_height;
	XMMATRIX		_word_matrix;
	XMFLOAT4X4		_mx_world;
	//Blend state
	ID3D11BlendState* _blendState;
	ID3D11BlendState* _blendState_add;
	ID3D11BlendState* _blendState_constract;
	ID3D11BlendState* _blendState_light;
	void setBlendFunction(BLEND_MODE mode);
public:
	PineClip _screen_clip;
	float clipx;
	float clipy;
	float clipw;
	float cliph;
	ID3D11Device1*			_curr_device;
	ID3D11DeviceContext1*	_curr_context;
	GraphicContext _curr_gcontext;
	void ChangeDevice(ID3D11Device1* device);
	void ChangeContext(ID3D11DeviceContext1* context);
	const XMMATRIX GetWorldMatrix(){ return XMLoadFloat4x4(&_mx_world); };
private:
	PineClip _curr_clip;
	void Clip(int x, int y, int w, int h);
	void ResetClip();
	void BindTexture(int slot, HTEXTURE tex);
	virtual void setWorldMatrix(PineTransformParam param);
};
#endif //_H_DIRECTX11_GRAPHIC