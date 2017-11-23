#if !_PINE_IMAGE_H
#define _PINE_IMAGE_H 1
#include "Sprite/CSprite.h"
#include "OpenGLES/Graphic.h"


class PineImage
{
//#ifndef _WINDOWS_PHONE8
	int _width;
	int _height;
	int TEXTURE_CELL_W;
	int TEXTURE_CELL_H;
	HTEXTURE** _texnames;

	int _cols_texture_num;
	int _rows_texutre_num;
	int _pixel_type;
	int _pixel_format;
	static float* 	_uv;
	bool _isCache;
	BYTE* _buffer;

	static UINT8*	_unzip_buff;
	static UINT32	_unzip_buff_length;

    int CreateImageFromPNG_URL(Graphic* g, ResourceItem resitem,bool isCacheBuffer,bool optimized, bool isInThread);
public:
	static void BindUnzipBuffer(UINT8* buff, UINT32 len)
	{
		_unzip_buff = buff;
		_unzip_buff_length = len;
	};
	int GetWidth(){return _width;};
	int GetHeight(){return _height;};
	const HTEXTURE** GetTextureNames(){return (const HTEXTURE**)_texnames;};
	PineSize GetTextureCellSize() { PineSize size; size.W = (float)TEXTURE_CELL_W; size.H = (float)TEXTURE_CELL_H; return size; };
	void SetTextureCellSize(PineSize size) { TEXTURE_CELL_W = size.W; TEXTURE_CELL_H = size.H; };
	
	PineImage();
	~PineImage();
	void SetTextureMaxSize(int width,int height);
	int CreateImageWithMixImage(Graphic* g, const char* mask_file, const char* source_file);
	int CreateImageMix2(Graphic* g, PineImage* image1, PineImage* image2, int type = 0);
    int CreateImageWithRGBA(Graphic* g, UINT8* rgba, int width, int height, int pixel_type = _EXPORT_PIXEL_32BITS, int pixel_format = _IMAGE_FORMAT_RAW_8888, bool optimized = false, bool isInThread = false);
	HTEXTURE CreateSubImageWithRGBA(Graphic* g, UINT8* rgba, int x, int y, int width, int height, int scan_width, int scan_height, int pixel_type = _EXPORT_PIXEL_32BITS, int pixel_format = _IMAGE_FORMAT_RAW_8888, bool optimized = false, bool isInThread = false);
	//int CreateImageFromScreen(int x, int y, int width, int height,bool isCache);
	void DrawImageWithRect(Graphic* g,float posX,float posY,float x, float y,int width,int height,int transform=0);
	void DrawImageWithRect(Graphic* g, float posX, float posY, int transform);
	void DrawImageWithRect(Graphic* g, float posX, float posY);
    
    //-----------------------------------------------------------------
    // CreateImageFromPNGinBundle: create GLtexture by file name in bundle folder 
    // CreateImageFromPNGinDocument: create GLtexture by file name in document folder
    // @param : file_name - fileName (only file name - not dir, not extence) ex : temp_png ; NOT : temp.png
    //-----------------------------------------------------------------
    int CreateImageFromPNGinBundle(Graphic* g, const char *file_name, bool IsCache = false, bool optimized = false);
    int CreateImageFromPNGinBundleInSubThread(Graphic* g, const char *file_name, bool IsCache = false, bool optimized = false);
	int CreateImageFromPNGinDocument(Graphic* g, const char*file_name, bool IsCache = false, bool optimized = false);
    int CreateImageFromPNGinDocumentInSubThread(Graphic* g, const char*file_name, bool IsCache = false, bool optimized = false);
	BYTE* GetBufferImage();
	void FreeBufferImage();
	void FreeTexture(Graphic* g);
	static void CannyEdgeDetect(BYTE* buffer,int width, int height);
	static void findEdge(BYTE* m_destinationBmp,int** edgeDir,int** gradient, int rowShift, int colShift, int row, int col, int dir, int lowerThreshold,int width,int height);
	static void suppressNonMax(BYTE* m_destinationBmp,int** edgeDir,int** gradient,int rowShift, int colShift, int row, int col, int dir, int lowerThreshold,int width,int height);

    // svenkas
    bool IsFree();
    
    enum enumImageLoadingState
    {
        IMAGE_LOADING_STATE_NONE,
        IMAGE_LOADING_STATE_BEGIN,
        IMAGE_LOADING_STATE_COMPLETE,
    };
    
    enumImageLoadingState _loadingState;
//#endif
};
#endif