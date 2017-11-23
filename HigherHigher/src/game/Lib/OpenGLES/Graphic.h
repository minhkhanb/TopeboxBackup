#if !defined(_H_GRAPHIC)
#define _H_GRAPHIC
#include "../DataType/CType.h"
#include "../Math/PinePolygon.h"
#include "../../Define/Defines.h"
#include <vector>
#if defined(_DEBUG)
//#define USE_DRAW_LINE
//#define USE_GL_ROTATE
//#define USE_TRANSLATE
//#define USE_GL_SCALE
//#define USE_DRAW_TRIANGLE
//#define USE_COLOR_MASK
//#define ENABLE_OPACITY_SETTING
//#define USE_BLEND_ALPHA
//#define USE_DRAW_CIRCLE
#endif

//define VTXUNIT int or float
#if defined(USE_FLOAT_VERTEX_POINTER)
typedef float VTXUNIT;
#else
typedef int VTXUNIT;
#endif

//define HTEXTURE for each graphic machine
#if ISGVER_DIRECTX11
typedef ID3D11Texture2D* HTEXTURE;
#elif ISGVER_GLES1 || ISGVER_GLES2
typedef UINT32 HTEXTURE;
#endif
typedef enum PineColorFormat{
	RGBA_4444 = 7,
	RGBA_8881 = 4,
	RGBA_5551 = 5,
	RGB_565 = 6,
	RGBA_1BYTE = 0, //each sample 4 byte
	RGBA_2BYTE = 1, //each sample 8 byte
	RGB_1BYTE = 2,
	RGB_2BYTE = 3
};

typedef enum PineColorType{
	CTYPE_OPENGL = 0,
	CTYPE_DIRECTX = 1
};


#if defined(_DEBUG)

#define DEBUG_GRAPHIC_NONE		0
#define DEBUG_GRAPHIC_DRAWTEX	1

#endif

struct ColorFloatARGB {
	float A;
	float R;
	float G;
	float B;
	ColorFloatARGB() {
		A = R = G = B = 0;
	}
};
struct GraphicContext{
	PineSize DeviceSize;
	PineRectangle ClipArea;
	PineGraphicComplexOrientation ComplexMode;
	float TranslateX;
	float TranslateY;
	float ScaleX;
	float ScaleY;
	bool ClipFixContext;
	PineRectangle Viewport;
};
class Graphic
{
public:
#if defined(_DEBUG)
	struct GraphicDebugTexture 
	{
		HTEXTURE _tex;
		UINT32 _size;
	};
	
	virtual void debugInfo() = 0;
#endif
	virtual bool IsTexturePowOf2() = 0;
	//init and release function
	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void ChangeContext(GraphicContext context) = 0;
	virtual void BeginTransaction() = 0;
	virtual void Flush() = 0;
	virtual void CleanCurrentFlag() = 0;

	//get property
	virtual float GetCurrentScaleX() = 0;
	virtual float GetCurrentScaleY() = 0;
    virtual float GetCurrentCenterScaleX() = 0;
	virtual float GetCurrentCenterScaleY() = 0;
	virtual int GetCurrentOpacityPercent() = 0;

	//function work with cliping
	virtual void SetClip(int x, int y, int w, int h) = 0;
	virtual void SetClip(PineClip clip) = 0;
	virtual void ClearClip() = 0;
	virtual PineClip GetCurrentClip() = 0;

	//function work with color
	virtual void SetColor(COLOR clr) = 0;
    virtual COLOR GetCurrentColor() = 0;
#if defined(USE_COLOR_MASK)
    virtual COLOR GetColorMask()=0;
	virtual void SetColorMask(COLOR clr) = 0;
	virtual void ClearColorMask() = 0;
#endif

	//function work with opacity
#if defined(ENABLE_OPACITY_SETTING)
	virtual void SetOpacity(int percent) = 0;
    virtual int GetCurrentOpacity() = 0;
#if defined(USE_BLEND_MODE)
	virtual void SetFillterMode(BLEND_MODE fillter_mode) = 0;
#endif
#endif

	//function work with line
#if defined(USE_DRAW_LINE)
	virtual void DrawLine(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, bool enableAlpha = false) = 0;
	virtual void DrawLine(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, COLOR color1, COLOR color2) = 0;
#if defined(USE_DRAW_TRIANGLE)
	virtual void DrawTriangle(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, VTXUNIT x3, VTXUNIT y3, bool enableAlpha = false) = 0;
#endif
#if defined(USE_DRAW_CIRCLE)
	virtual void DrawCircle(VTXUNIT x, VTXUNIT y, float radius, int density, bool enableAlpha = false) = 0;
#endif
	virtual void DrawPolygon(PinePolygon polygon, bool enableAlpha = false) = 0;
#endif//USE_DRAW_LINE

	//function work with triangle
#if defined(USE_DRAW_TRIANGLE)
	virtual void FillTriangle(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, VTXUNIT x3, VTXUNIT y3, bool enableAlpha = false) = 0;
	virtual void FillTriangle(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, VTXUNIT x3, VTXUNIT y3, HTEXTURE tex_id, float u1, float v1, float u2, float v2, float u3, float v3, bool enableAnpha = true) = 0;
	virtual void FillPolygon(PinePolygon polygon, bool enableAlpha = false) = 0;
	virtual void FillPolygon(PinePolygon polygon, HTEXTURE texId, float tw, float th, float tx = 0, float ty = 0, bool enableAlpha = false) = 0;
#if defined(USE_DRAW_CIRCLE)
	virtual void FillCircle(VTXUNIT x, VTXUNIT y, float radius, int density, bool enableAlpha = false) = 0;
#endif
#endif

	//function work with rectangle
#if defined(USE_DRAW_LINE)
	virtual void DrawRect(VTXUNIT x, VTXUNIT y, VTXUNIT w, VTXUNIT h, bool enableAlpha = false) = 0;
#endif
	virtual void FillRect(VTXUNIT x, VTXUNIT y, VTXUNIT w, VTXUNIT h, bool enableAlpha = false) = 0;
	virtual void FillFullScreen(bool enableAlpha = false) = 0;
	//function work with texture
	virtual void Draw2DTexture(VTXUNIT x, VTXUNIT y, int w, int h, HTEXTURE texId, float uv[], int flags, bool enableAlpha) = 0;
	virtual void LoadModel(float ox, float oy, float m11, float m12, float m21, float m22) = 0;
	virtual void SetTransform(PineTransformParam param) = 0;
	virtual void ClearTransform() = 0;
	
	//function work with rotation
#if defined(USE_GL_ROTATE)
	virtual bool HasRotate() = 0;
	virtual float GetCurrentRotateAngle() = 0;
	virtual float GetCurrentRotateX() = 0;
	virtual float GetCurrentRotateY() = 0;
	virtual void SetRotate(float angle, float x, float y) = 0;
	virtual void ClearRotate() = 0;
#endif

#if defined(USE_TRANSLATE)
	virtual void SetTranslate(float x, float y) = 0;
	virtual void ClearTranslate() = 0;
#endif

#if defined(USE_GL_SCALE)
    virtual bool HasScale()=0;
	virtual void SetScale(float scale_x, float scale_y, float center_x, float center_y) = 0;
	virtual void ClearScale() = 0;
	virtual ScaleParam GetCurrentScaleParam() = 0;
#endif

	virtual HTEXTURE createTexture(BYTE* input_data, int texw, int texh, PineColorFormat input_colorformat, int is_altialias = true) = 0;
	virtual void updateTexture(HTEXTURE tex, BYTE* input_data, int dw, int dh, int tx, int ty, PineColorFormat color_format) = 0;
	virtual void freeTexture(HTEXTURE tex) = 0;

	virtual bool IsEngineReady() = 0;

	virtual PineColorType ColorType() = 0;

	////virtual void SetGlobalContext(float tx, float ty, float sx, float sy) = 0;
	////virtual void SetComplexMode(PineGraphicComplexOrientation mode) = 0;

	virtual void setWorldMatrix(PineTransformParam param) = 0;


#if defined(_DEBUG)
	virtual void setDebugFlagOn(UINT32 flag) = 0;
	virtual void setDebugFlagOff(UINT32 flag) = 0;
#else 
#endif
};
#endif //_H_GRAPHIC
