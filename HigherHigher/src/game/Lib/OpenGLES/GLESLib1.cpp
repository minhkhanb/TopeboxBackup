#include "pch.h"
#include "GLESLib1.h"

#include <math.h>
#include "../DataType/CType.h"
#include "../Math/CMath.h"
#include <stdio.h>

#if defined (USE_CALC_BY_VECTOR_AND_REUSE_RESULT)
#include "../Math/PineVector3.h"
#endif


Orientation GLESLib1::_device_orientation = Orientation::Portrait;
#if	defined(MAC_OS) || defined(ANDROID_OS)
#define 	glClearDepth		glClearDepthf
#define 	glOrtho				glOrthof
#else
#endif
GLESLib1::GLESLib1()
{
	_curr_gcontext.TranslateX = 0.0f;
	_curr_gcontext.TranslateY = 0.0f;
	_curr_gcontext.ScaleX = 1.0f;
	_curr_gcontext.ScaleY = 1.0f;
	_curr_gcontext.ComplexMode = PineGraphicComplexOrientation::ComplexNone;
#if defined(_DEBUG)
	if (!_create_dbg) {
		_create_dbg = true;
		_dbg = new GLESLib1();
		
	}
#endif
}

GLESLib1::~GLESLib1()
{
#if defined(_DEBUG)
	if (_dbg != NULL && this != _dbg && _create_dbg) {
		delete _dbg;
		_dbg = NULL;
		_create_dbg = false;
	}
#endif
}
bool GLESLib1::IsTexturePowOf2() 
{
	return true;
}

void GLESLib1::Init()
{
	_pVtxBuffer2D	= new VTXUNIT[MAX_2D_QUADS * VTX_SIZE * VTX_COUNT_PER_QUAD];
	_pClrBuffer2D	= new UINT8[MAX_2D_QUADS * COLOR_SIZE * VTX_COUNT_PER_QUAD];
	_pTexBuffer2D	= new float[MAX_2D_QUADS * TEXCOOR_SIZE * VTX_COUNT_PER_QUAD];
	_pTexID			= new HTEXTURE[MAX_2D_QUADS];
	_pIndices		= new UINT16[MAX_2D_QUADS * TRI_COUNT_PER_QUAD * VTX_COUNT_PER_TRI];
	_pFlags			= new UINT16[MAX_2D_QUADS];


#if defined(USE_BLEND_ALPHA)
	_fillter_mode = new BLEND_MODE[MAX_2D_QUADS];
	_current_fillter_mode = BLEND_MODE::ALPHA_FILLTER_NORMAL;
#endif

#if defined(USE_FREE_TRANSFORM)
	_pModBuffer2D = new float[MAX_2D_QUADS * MODEL_SIZE * MODEL_COUNT_PER_QUAD];
	_transform_param.TranslateX = 0.0f;
	_transform_param.TranslateY = 0.0f;
	_transform_param.m11 = 0.0f;
	_transform_param.m12 = 0.0f;
	_transform_param.m21 = 0.0f;
	_transform_param.m22 = 0.0f;
	_is_transform = false;
#endif
	//init rotate
#if defined(USE_GL_ROTATE)
	_pRotateParams	= new float[MAX_2D_QUADS * NUM_ROTATE_PARAMS];
	_is_rotate		= false;
	_rotate_angle	= 0.0f;
	_rotate_x		= 0.0f;
	_rotate_y		= 0.0f;
#endif

	//init translate
#if defined(USE_TRANSLATE)
	_pTranslateParams = new float[MAX_2D_QUADS*NUM_TRANSLATE_PARAMS];
	_is_translate	= false;
	_translate_x	= 0.0f;
	_translate_y	= 0.0f;
#endif
	

	//init scale
#if defined(USE_GL_SCALE)
	_pScaleParams = new float[MAX_2D_QUADS * NUM_SCALE_PARAMS];
	_is_scale		= false;
	_scale_x		= 0.0f;
	_scale_y		= 0.0f;
	_scale_center_x = 0.0f;
	_scale_center_y	= 0.0f;
#endif

#if defined(USE_DRAW_LINE)
	_pLineIndices = new UINT16[MAX_2D_QUADS * VTX_COUNT_PER_LINE];
#endif

#if defined(USE_DRAW_TRIANGLE)
	_pTriangleIndices = new UINT16[MAX_2D_QUADS * VTX_COUNT_PER_TRIANGLE];
#endif

	//Generate Indice
	for (int i = 0; i < MAX_2D_QUADS; i++)
	{
		//first triangle
		_pIndices[i * TRI_COUNT_PER_QUAD * VTX_COUNT_PER_TRI] = i*VTX_COUNT_PER_QUAD + 0;
		_pIndices[i * TRI_COUNT_PER_QUAD * VTX_COUNT_PER_TRI + 1] = i*VTX_COUNT_PER_QUAD + 1;
		_pIndices[i * TRI_COUNT_PER_QUAD * VTX_COUNT_PER_TRI + 2] = i*VTX_COUNT_PER_QUAD + 2;

		//second triangle
		_pIndices[i * TRI_COUNT_PER_QUAD * VTX_COUNT_PER_TRI + 3] = i*VTX_COUNT_PER_QUAD + 0;
		_pIndices[i * TRI_COUNT_PER_QUAD * VTX_COUNT_PER_TRI + 4] = i*VTX_COUNT_PER_QUAD + 2;
		_pIndices[i * TRI_COUNT_PER_QUAD * VTX_COUNT_PER_TRI + 5] = i*VTX_COUNT_PER_QUAD + 3;

#if defined(USE_DRAW_LINE)
		_pLineIndices[i * VTX_COUNT_PER_LINE + 0] = i*VTX_COUNT_PER_QUAD + 0;
		_pLineIndices[i * VTX_COUNT_PER_LINE + 1] = i*VTX_COUNT_PER_QUAD + 1;
#endif

#if defined(USE_DRAW_TRIANGLE)
		for (int j = 0; j< VTX_COUNT_PER_TRIANGLE; j++)
		{
			_pTriangleIndices[i* VTX_COUNT_PER_TRIANGLE + j] = i* VTX_COUNT_PER_QUAD + j;
		}
#endif

	}

	_world_transform_param.TranslateX = 0.0f;
	_world_transform_param.TranslateY = 0.0f;
	_world_transform_param.m11 = 1.0f;
	_world_transform_param.m12 = 0.0f;
	_world_transform_param.m21 = 0.0f;
	_world_transform_param.m22 = 1.0f;


#if defined(_DEBUG)
	if (this == _dbg || _dbg == NULL) return;
	_dbg->Init();
#endif
}

void GLESLib1::Release()
{
	if (_pVtxBuffer2D != NULL)
	{
		delete[] _pVtxBuffer2D;
		_pVtxBuffer2D = NULL;
	}

	if (_pClrBuffer2D != NULL)
	{
		delete[] _pClrBuffer2D;
		_pClrBuffer2D = NULL;
	}

	if (_pTexBuffer2D != NULL)
	{
		delete[] _pTexBuffer2D;
		_pTexBuffer2D = NULL;
	}

	if (_pTexID != NULL)
	{
		delete[] _pTexID;
		_pTexID = NULL;
	}

	if (_pIndices != NULL)
	{
		delete[] _pIndices;
		_pIndices = NULL;
	}

	if (_pFlags != NULL)
	{
		delete[] _pFlags;
		_pFlags = NULL;
	}

#if defined(USE_GL_ROTATE)
	if (_pRotateParams != NULL)
	{
		delete[] _pRotateParams;
		_pRotateParams = NULL;
	}
#endif

#if defined(USE_TRANSLATE)
	if (_pTranslateParams != NULL)
	{
		delete[] _pTranslateParams;
		_pTranslateParams = NULL;
	}
#endif

#if defined(USE_GL_SCALE)
	if (_pScaleParams != NULL)
	{
		delete[] _pScaleParams;
		_pScaleParams = NULL;
	}
#endif

#if defined(_DEBUG)
	if (this == _dbg) return;
	_dbg->Release();
#endif
}

///////////////////////////////////////////////////////////
///IMPLEMENT RENDERER
///////////////////////////////////////////////////////////
void GLESLib1::ChangeContext(GraphicContext context){
	_curr_gcontext = context;
	PDEBUG("change render size:%0.2f %0.2f\n", context.DeviceSize.W, context.DeviceSize.H);
#if !OPENGLES_20_SUPPORT
	_render_width = context.DeviceSize.W;
	_render_height = context.DeviceSize.H;
	
//#if defined(MAC_OS)
//	if (_device_orientation == ORIENTATION_PORTRAIT)
//		::glViewport(0, 0, _render_width, _render_height);
//	else
//		::glViewport(0, 0, _render_height, _render_width);
//#else
#	ifdef USE_HALF_SCREEN_WIN
	::glViewport(0, 0, _render_width / 2, _render_height / 2);
#	else
	//::glViewport(0, 0, _render_width, _render_height);
#	endif
//#endif
	::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	::glClearDepth(1);
	::glDepthMask(GL_FALSE);
	::glDisable(GL_DEPTH_TEST);
	::glEnable(GL_ALPHA_TEST);
	::glAlphaFunc(GL_GREATER, 0.0f);

	::glDisable(GL_CULL_FACE);

#ifdef USE_BLEND_ALPHA
	::glEnable(GL_BLEND);
#endif

	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();
	::glOrtho(0, _render_width, _render_height, 0, -1.0f, 1.0f);

#endif

	//
	if (_curr_gcontext.ComplexMode == PineGraphicComplexOrientation::ComplexToLeft)
	{
		::glMatrixMode(GL_PROJECTION);
		::glLoadIdentity();
		::glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		::glOrtho(0, _render_height, _render_width, 0, -1.0f, 1.0f);
		//::glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		//::glOrtho(0, _render_width, _render_height, 0, -1.0f, 1.0f);
	}
	else if (_curr_gcontext.ComplexMode == PineGraphicComplexOrientation::ComplexToRight)
	{
		::glMatrixMode(GL_PROJECTION);
		::glLoadIdentity();

		//::glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);        
		::glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		//::glTranslatef(_render_width, 0.0f, 0.0f);
		//::glOrtho(0, _render_width, _render_height, 0, -1.0f, 1.0f);
		::glOrtho(0, _render_height, _render_width, 0, -1.0f, 1.0f);
	}
	else{
		::glMatrixMode(GL_PROJECTION);
		::glLoadIdentity();
		//::glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		//::glOrtho(0, _render_width, _render_height, 0, -1.0f, 1.0f);
		::glOrtho(0, _render_width, _render_height, 0, -1.0f, 1.0f);
	}

	//viewport clip
	float x = -(_curr_gcontext.TranslateX / _curr_gcontext.ScaleX);
	float y = -(_curr_gcontext.TranslateY / _curr_gcontext.ScaleY);

	float w = _curr_gcontext.DeviceSize.W / _curr_gcontext.ScaleX;
	float h = _curr_gcontext.DeviceSize.H / _curr_gcontext.ScaleY;
	float sizemax = w > h ? w : h;
	w = h = sizemax;
	_viewport_clip.ClipArea.X = x;
	_viewport_clip.ClipArea.Y = y;
	_viewport_clip.ClipArea.W = w;

	_fullscreen_clip.ClipArea.X = x;
	_fullscreen_clip.ClipArea.Y = y;
	_fullscreen_clip.ClipArea.W = w;
	_fullscreen_clip.ClipArea.H = h;
	PDEBUG("Set global context:tx:%0.2f ty:%0.2f sx:%0.2f sy:%0.2f\n", _curr_gcontext.TranslateX, _curr_gcontext.TranslateY, _curr_gcontext.ScaleX, _curr_gcontext.ScaleY);

	//complex mode
	/*if(_complex_mode == PineGraphicComplexOrientation::ComplexNone){
	::glOrtho(0, _render_width, _render_height, 0, -1.0f, 1.0f);
	}else if(_complex_mode == PineGraphicComplexOrientation::ComplexToLeft){
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();
	//::glTranslatef(0.0f, _render_width, 0.0f);
	//::glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
	//::glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	//::glOrtho(0, _render_width, _render_height, 0, -1.0f, 1.0f);
	PDEBUG("complex to left\n");
	::glOrtho(0, _render_height, _render_width, 0, -1.0f, 1.0f);

	}else if(_complex_mode == PineGraphicComplexOrientation::ComplexToRight){
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();
	//::glTranslatef(_render_height, 0.0f, 0.0f);
	//::glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
	//::glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	//::glOrtho(0, _render_width, _render_height, 0, -1.0f, 1.0f);
	PDEBUG("complex to right\n");
	::glOrtho(0, _render_height, _render_width, 0, -1.0f, 1.0f);
	}*/

#if defined(_DEBUG)
	if (this == _dbg || _dbg == NULL) return;
	_dbg->ChangeContext(context);
#endif
}

void GLESLib1::BeginTransaction()
{
	_nNum2DQuads = 0;
#if defined(USE_COLOR_MASK)
	_is_color_flag = false;
	_color_mask = 0xFFFFFFFF;
#endif

#if defined(USE_GL_ROTATE)
	_is_rotate = false;
#endif

#if defined(USE_TRANSLATE)
	_is_translate = false;
#endif

#if defined(USE_GL_SCALE)
	_is_scale = false;
#endif

#if defined(USE_FREE_TRANSFORM)
	ClearTransform();
#endif

#if defined(_DEBUG)
	if (this == _dbg) return;
	_dbg->BeginTransaction();
#endif
}
#define BLEND_FUNC_SCREEN ::glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE) //screen
//#define BLEND_FUNC_SCREEN ::glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA); //multiply
//#define BLEND_FUNC_SCREEN ::glBlendFunc(GL_ONE, GL_ONE); //linear dodge
//#define BLEND_FUNC_SCREEN ::glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);//more dark
//#define BLEND_FUNC_SCREEN ::glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);//more dark
void GLESLib1::Flush()
{
	if (_curr_gcontext.ComplexMode == PineGraphicComplexOrientation::ComplexToLeft)
	{
		::glMatrixMode(GL_PROJECTION);						
		::glLoadIdentity();
		::glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		::glOrtho(0, _render_height, _render_width, 0, -1.0f, 1.0f);
		//::glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		//::glOrtho(0, _render_width, _render_height, 0, -1.0f, 1.0f);
	}
	else if (_curr_gcontext.ComplexMode == PineGraphicComplexOrientation::ComplexToRight)
	{
		::glMatrixMode(GL_PROJECTION);						
		::glLoadIdentity();
		
		//::glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);        
		::glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		//::glTranslatef(_render_width, 0.0f, 0.0f);
		//::glOrtho(0, _render_width, _render_height, 0, -1.0f, 1.0f);
		::glOrtho(0, _render_height, _render_width, 0, -1.0f, 1.0f);
	} else{
		::glMatrixMode(GL_PROJECTION);						
		::glLoadIdentity();
		//::glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		//::glOrtho(0, _render_width, _render_height, 0, -1.0f, 1.0f);
		::glOrtho(0, _render_width, _render_height, 0, -1.0f, 1.0f);
	}
	GLfloat world_matrix[] =
	{
		_world_transform_param.m11, _world_transform_param.m12, 0.0f, 0.0f,
		_world_transform_param.m21, _world_transform_param.m22, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		_world_transform_param.TranslateX, _world_transform_param.TranslateY, 0.0f, 1.0f
	};
	::glMultMatrixf(world_matrix);

#if USE_RESIZE_WINDOWS
	::glMatrixMode(GL_PROJECTION);						
	::glLoadIdentity();
	::glOrtho(0, _render_width, _render_height, 0, -1.0f, 1.0f);
	{

		GLfloat model[] =
		{
			CGame::_this->_windows_screen_scale, 0.0f, 0.0f, 0.0f,
			0.0f, CGame::_this->_windows_screen_scale, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		::glMultMatrixf(model);
	}
	::glViewport(0, 0, _render_width, _render_height - 2 * GetPaintY());
#endif

	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
	::glTranslatef(_curr_gcontext.TranslateX, _curr_gcontext.TranslateY, 0.0f);
	::glScalef(_curr_gcontext.ScaleX, _curr_gcontext.ScaleY, 1.0f);
	if (_nNum2DQuads>0)
	{
		//Render current 2D primitives		
		::glEnableClientState(GL_VERTEX_ARRAY);
		::glEnableClientState(GL_COLOR_ARRAY);
		::glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		::glEnable(GL_TEXTURE_2D);

#ifdef USE_FLOAT_VERTEX_POINTER
		::glVertexPointer(2, GL_FLOAT, 0, _pVtxBuffer2D);
#else
		::glVertexPointer(2, GL_SHORT, 0, _pVtxBuffer2D);
#endif
		::glColorPointer(4, GL_UNSIGNED_BYTE, 0, _pClrBuffer2D);
		::glTexCoordPointer(2, GL_FLOAT, 0, _pTexBuffer2D);

		UINT32 current_flag = _pFlags[0];
		UINT32 current_texture = _pTexID[0];

#ifdef USE_BLEND_ALPHA
		BLEND_MODE current_fillter_mode = _fillter_mode[0];
#endif
		INT32 texture_start_index = 0;
		INT32 total_element = 0;

#if defined(_DEBUG)
		if (this != _dbg)
		{
			::glClear(GL_COLOR_BUFFER_BIT);
			::glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
		}
#else
		::glClear(GL_COLOR_BUFFER_BIT);
		::glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
#endif
		for (int i = 0; i <= _nNum2DQuads; i++)
		{
			if (current_texture != _pTexID[i] 
				|| current_flag != _pFlags[i]
#if defined(USE_GL_ROTATE)				
				|| current_flag & ROTATE_FLAG
#endif

#if defined(USE_GL_SCALE)			
				|| current_flag & SCALE_FLAG
#endif

#if defined(USE_TRANSLATE)
				|| current_flag & TRANSLATE_FLAG
#endif

#if defined(USE_FREE_TRANSFORM)
				|| current_flag & FREE_TRANSFORM_FLAG
#endif
				)
			{
				if (current_flag&SETCLIP_FLAG)
				{
					::glEnable(GL_SCISSOR_TEST);
					int x = _pVtxBuffer2D[VTX_SIZE * VTX_COUNT_PER_QUAD * texture_start_index + 0];
					int y = _pVtxBuffer2D[VTX_SIZE * VTX_COUNT_PER_QUAD * texture_start_index + 1];
					int w = _pVtxBuffer2D[VTX_SIZE * VTX_COUNT_PER_QUAD * texture_start_index + 2];
					int h = _pVtxBuffer2D[VTX_SIZE * VTX_COUNT_PER_QUAD * texture_start_index + 3];

					/*float * transform;// = new float[16];
					float * matrix;// = new float[16];

					transform = _zoom_transform_list[texture_start_index];

					matrix = GetMatrix(x, y, w, h);

					matrix = MulMatrix(transform, matrix);

					printf("\n%f ", matrix[0]); printf("%f ", matrix[1]); printf("%f ", matrix[2]); printf("%f ", matrix[3]);
					printf("\n%f ", matrix[4]); printf("%f ", matrix[5]); printf("%f ", matrix[6]); printf("%f ", matrix[7]);
					printf("\n%f ", matrix[8]); printf("%f ", matrix[9]); printf("%f ", matrix[10]); printf("%f ", matrix[11]);
					printf("\n%f ", matrix[12]); printf("%f ", matrix[13]); printf("%f ", matrix[14]); printf("%f ", matrix[15]);*/

					Clip(x, y, w, h);
				}
				else if (current_flag&CLEARCLIP_FLAG)
				{
					::glDisable(GL_SCISSOR_TEST);
				}
				else
				{
					if (current_texture>0)
					{
						glEnable(GL_TEXTURE_2D);
						::glBindTexture(GL_TEXTURE_2D, current_texture);
					}
					else
					{
						glDisable(GL_TEXTURE_2D);
					}
					if (current_flag&TRANSP_FLAG)
					{
						::glEnable(GL_BLEND);
#if defined(USE_BLEND_ALPHA)	
						switch (current_fillter_mode)
						{
						case BLEND_MODE::ALPHA_FILLTER_NORMAL:
							::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							break;
						case BLEND_MODE::ALPHA_FILLTER_ADD:
							::glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
							break;
						case BLEND_MODE::ALPHA_FILLTER_CONSTRACT:
							::glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
							break;
						case BLEND_MODE::ALPHA_FILLTER_LIGHT:
							::glBlendFunc(GL_SRC_ALPHA, GL_ONE);
							break;
						case BLEND_MODE::SCREEN:
							BLEND_FUNC_SCREEN;
							//::glBlendFunc(GL_ONE_MINUS_CONSTANT_COLOR, GL_ONE_MINUS_SRC_ALPHA);
							break;
						case BLEND_MODE::ALPHA_FILLTER_LOW_BRIGHTNESS:
							::glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
							break;
						case BLEND_MODE::ALPHA_FILTER_ONE:
							::glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
							break;
						default:
							::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							break;
						}
#else
						::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
					}
					else
					{
						::glDisable(GL_BLEND);
					}

					if (current_flag & ROTATE_FLAG || current_flag & SCALE_FLAG || current_flag & FREE_TRANSFORM_FLAG)
					{
						// ::glPushMatrix();
						// ::glLoadIdentity();
						// ::glTranslatef(_global_translate_x, _global_translate_y, 0.0f);
						// ::glScalef(_global_scale_x, _global_scale_y, 1.0f);
						::glPushMatrix();
						::glMatrixMode(GL_MODELVIEW);
						::glLoadIdentity();
						//if(_complex_mode == PineGraphicComplexOrientation::ComplexNone){
						::glTranslatef(_curr_gcontext.TranslateX, _curr_gcontext.TranslateY, 0.0f);
						::glScalef(_curr_gcontext.ScaleX, _curr_gcontext.ScaleY, 1.0f);
						//}else if(_complex_mode == PineGraphicComplexOrientation::ComplexToLeft){
							
						//}else if(_complex_mode == PineGraphicComplexOrientation::ComplexToRight){
						//}
					}

#if defined(USE_GL_ROTATE)
					if (current_flag & ROTATE_FLAG)
					{
						::glTranslatef((_pRotateParams[texture_start_index*NUM_ROTATE_PARAMS + 1]), (_pRotateParams[texture_start_index*NUM_ROTATE_PARAMS + 2]), 0);
						::glRotatef(_pRotateParams[texture_start_index*NUM_ROTATE_PARAMS], 0.0f, 0.0f, 1.0f);
						::glTranslatef(-(_pRotateParams[texture_start_index*NUM_ROTATE_PARAMS + 1]), -(_pRotateParams[texture_start_index*NUM_ROTATE_PARAMS + 2]), 0);
					}
#endif

#if defined(USE_TRANSLATE)
					if (current_flag & TRANSLATE_FLAG)
					{
						::glTranslatef(_pTranslateParams[texture_start_index*NUM_TRANSLATE_PARAMS], _pTranslateParams[texture_start_index*NUM_TRANSLATE_PARAMS + 1], 0);
					}
#endif

#if defined(USE_FREE_TRANSFORM)
#if defined(USE_GL_SCALE)
					if ((current_flag & SCALE_FLAG))
					{
						::glTranslatef((_pScaleParams[texture_start_index*NUM_SCALE_PARAMS + 2]), (_pScaleParams[texture_start_index*NUM_SCALE_PARAMS + 3]), 0);
						::glScalef(_pScaleParams[texture_start_index*NUM_SCALE_PARAMS], _pScaleParams[texture_start_index*NUM_SCALE_PARAMS + 1], 1.0f);
						::glTranslatef(-(_pScaleParams[texture_start_index*NUM_SCALE_PARAMS + 2]), -(_pScaleParams[texture_start_index*NUM_SCALE_PARAMS + 3]), 0);
					}
#endif

#if defined(USE_GL_ZOOM)
					if (current_flag & ZOOM_FLAG)
					{
						::glMultMatrixf(_zoom_transform_list[texture_start_index]);
					}
#endif
					if (current_flag & FREE_TRANSFORM_FLAG)
					{
						float*	pModel = _pModBuffer2D + (MODEL_SIZE * MODEL_COUNT_PER_QUAD * texture_start_index);
						GLfloat model[] =
						{
							*(pModel + 2), *(pModel + 3), 0.0f, 0.0f,
							*(pModel + 4), *(pModel + 5), 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							*(pModel + 0), *(pModel + 1), 0.0f, 1.0f
						};
						::glMultMatrixf(model);
					}

#endif
					total_element = (i - texture_start_index);

#if defined(USE_DRAW_LINE)
					if (current_flag&DRAWLINE_FLAG)
					{
						::glDrawElements(GL_LINES, total_element * VTX_COUNT_PER_LINE, GL_UNSIGNED_SHORT, _pLineIndices + (texture_start_index * VTX_COUNT_PER_LINE));
					}
					else
#endif

#if defined(USE_DRAW_TRIANGLE)
					if (current_flag&DRAWTRIANGLE_FLAG)
					{
						::glDrawElements(GL_TRIANGLES, total_element * VTX_COUNT_PER_TRIANGLE, GL_UNSIGNED_SHORT, _pTriangleIndices + (texture_start_index * VTX_COUNT_PER_TRIANGLE));
					}
					else
#endif
					{
						::glDrawElements(GL_TRIANGLES, total_element * TRI_COUNT_PER_QUAD * VTX_COUNT_PER_TRI, GL_UNSIGNED_SHORT, _pIndices + (texture_start_index * TRI_COUNT_PER_QUAD * VTX_COUNT_PER_TRI));
					}

					if (current_flag & ROTATE_FLAG || current_flag & SCALE_FLAG || current_flag & FREE_TRANSFORM_FLAG)
					{
						::glPopMatrix();
					}
				}

				current_flag = _pFlags[i];
				
 				
				current_texture = _pTexID[i];
				texture_start_index = i;

#if defined(USE_BLEND_ALPHA)
				current_fillter_mode = _fillter_mode[i];
				if (current_flag&TRANSP_FLAG)
				{
					::glEnable(GL_BLEND);
					switch (current_fillter_mode)
					{
					case BLEND_MODE::ALPHA_FILLTER_NORMAL:
						::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						break;
					case BLEND_MODE::ALPHA_FILLTER_ADD:
						::glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
						break;
					case BLEND_MODE::ALPHA_FILLTER_CONSTRACT:
						::glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
						break;
					case BLEND_MODE::ALPHA_FILLTER_LIGHT:
						::glBlendFunc(GL_SRC_ALPHA, GL_ONE);
						break;
					case BLEND_MODE::SCREEN:
						//::glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
						BLEND_FUNC_SCREEN;
						break;
					case BLEND_MODE::ALPHA_FILLTER_LOW_BRIGHTNESS:
						::glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
						break;
					case BLEND_MODE::ALPHA_FILTER_ONE:
						::glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
						break;
					default:
						::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						break;
					}
				}
#endif
			}
			_pFlags[i] = 0;
		}


#if defined(USE_BLEND_ALPHA)
		if (current_flag&TRANSP_FLAG)
		{
			::glEnable(GL_BLEND);
			switch (current_fillter_mode)
			{
			case BLEND_MODE::ALPHA_FILLTER_NORMAL:
				::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;
			case BLEND_MODE::ALPHA_FILLTER_ADD:
				::glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
				break;
			case BLEND_MODE::ALPHA_FILLTER_CONSTRACT:
				::glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
				break;
			case BLEND_MODE::ALPHA_FILLTER_LIGHT:
				::glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				break;
			case BLEND_MODE::SCREEN:
				//::glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
				BLEND_FUNC_SCREEN;
				break;
			case BLEND_MODE::ALPHA_FILLTER_LOW_BRIGHTNESS:
				::glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
				break;
			case BLEND_MODE::ALPHA_FILTER_ONE:
				::glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
				break;
			default:
				::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;
			}
		}
#endif


		//if not set clip, glDrawElements 
		if (current_flag&SETCLIP_FLAG)
		{
			::glEnable(GL_SCISSOR_TEST);
			int x = _pVtxBuffer2D[VTX_SIZE * VTX_COUNT_PER_QUAD * texture_start_index + 0];
			int y = _pVtxBuffer2D[VTX_SIZE * VTX_COUNT_PER_QUAD * texture_start_index + 1];
			int w = _pVtxBuffer2D[VTX_SIZE * VTX_COUNT_PER_QUAD * texture_start_index + 2];
			int h = _pVtxBuffer2D[VTX_SIZE * VTX_COUNT_PER_QUAD * texture_start_index + 3];

			Clip(x, y, w, h);
		}
		else if (current_flag&CLEARCLIP_FLAG)
		{
			::glDisable(GL_SCISSOR_TEST);
		}
		else
		{
			if (current_texture>0)
			{
				glEnable(GL_TEXTURE_2D);
				::glBindTexture(GL_TEXTURE_2D, current_texture);
			}
			else
			{
				glDisable(GL_TEXTURE_2D);
			}
			if (current_flag&TRANSP_FLAG)
			{
				::glEnable(GL_BLEND);
#if defined(USE_BLEND_ALPHA)
				switch (current_fillter_mode)
				{
				case BLEND_MODE::ALPHA_FILLTER_NORMAL:
					::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					break;
				case BLEND_MODE::ALPHA_FILLTER_ADD:
					::glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
					break;
				case BLEND_MODE::ALPHA_FILLTER_CONSTRACT:
					::glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
					break;
				case BLEND_MODE::ALPHA_FILLTER_LIGHT:
					::glBlendFunc(GL_SRC_ALPHA, GL_ONE);
					break;
				case BLEND_MODE::SCREEN:
					//::glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
					BLEND_FUNC_SCREEN;
					break;
				case BLEND_MODE::ALPHA_FILLTER_LOW_BRIGHTNESS:
					::glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
					break;
				case BLEND_MODE::ALPHA_FILTER_ONE:
					::glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
					break;
				default:
					::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					break;
				}
#else
				::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
			}
			else
			{
				::glDisable(GL_BLEND);
			}
			if (current_flag & ROTATE_FLAG || current_flag & SCALE_FLAG || current_flag & FREE_TRANSFORM_FLAG)
			{
				::glPushMatrix();
				::glMatrixMode(GL_MODELVIEW);
				::glLoadIdentity();
				::glTranslatef(_curr_gcontext.TranslateX, _curr_gcontext.TranslateY, 0.0f);
				::glScalef(_curr_gcontext.ScaleX, _curr_gcontext.TranslateY, 1.0f);
				//::glLoadIdentity();
			}	
#if defined(USE_GL_ROTATE)
			if (current_flag & ROTATE_FLAG)
			{
				::glTranslatef(_pRotateParams[texture_start_index*NUM_ROTATE_PARAMS + 1], _pRotateParams[texture_start_index*NUM_ROTATE_PARAMS + 2], 0);
				::glRotatef(_pRotateParams[texture_start_index*NUM_ROTATE_PARAMS], 0.0f, 0.0f, 1.0f);
				::glTranslatef(-(_pRotateParams[texture_start_index*NUM_ROTATE_PARAMS + 1]), -(_pRotateParams[texture_start_index*NUM_ROTATE_PARAMS + 2]), 0);
			}
#endif
#if defined(USE_TRANSLATE)
			if (current_flag & TRANSLATE_FLAG)
			{
				::glTranslatef(_pTranslateParams[texture_start_index*NUM_TRANSLATE_PARAMS], _pTranslateParams[texture_start_index*NUM_TRANSLATE_PARAMS + 1], 0);
			}
#endif

#if defined(USE_GL_SCALE)
			if (current_flag & SCALE_FLAG)
			{
				::glTranslatef((_pScaleParams[texture_start_index*NUM_SCALE_PARAMS + 2]), (_pRotateParams[texture_start_index*NUM_SCALE_PARAMS + 3]), 0);
				::glScalef(_pScaleParams[texture_start_index*NUM_SCALE_PARAMS], _pScaleParams[texture_start_index*NUM_SCALE_PARAMS + 1], 1.0f);
				::glTranslatef(-(_pScaleParams[texture_start_index*NUM_SCALE_PARAMS + 2]), -(_pRotateParams[texture_start_index*NUM_SCALE_PARAMS + 3]), 0);
			}
#endif

#if defined(USE_GL_ZOOM)
			if (current_flag & ZOOM_FLAG)
			{
				::glMultMatrixf(_zoom_transform_list[texture_start_index]);
			}
#endif

#if defined(USE_FREE_TRANSFORM)
			if (current_flag & FREE_TRANSFORM_FLAG)
			{
				float*	pModel = _pModBuffer2D + (MODEL_SIZE * MODEL_COUNT_PER_QUAD * texture_start_index);
				GLfloat model[] =
				{
					*(pModel + 2), *(pModel + 3), 0.0f, 0.0f,
					*(pModel + 4), *(pModel + 5), 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					*(pModel + 0), *(pModel + 1), 0.0f, 1.0f
				};

				::glMultMatrixf(model);
			}
#endif
			total_element = _nNum2DQuads - texture_start_index;
#if defined(USE_DRAW_LINE)
			if (current_flag&DRAWLINE_FLAG)
			{
				::glDrawElements(GL_LINES, total_element * VTX_COUNT_PER_LINE, GL_UNSIGNED_SHORT, _pLineIndices + (texture_start_index * VTX_COUNT_PER_LINE));
			}
			else

#endif
#if defined(USE_DRAW_TRIANGLE)
			if (current_flag&DRAWTRIANGLE_FLAG)
			{
				::glDrawElements(GL_TRIANGLES, total_element * VTX_COUNT_PER_TRIANGLE, GL_UNSIGNED_SHORT, _pTriangleIndices + (texture_start_index * VTX_COUNT_PER_TRIANGLE));
			}
			else
#endif
			{
				::glDrawElements(GL_TRIANGLES, total_element * TRI_COUNT_PER_QUAD * VTX_COUNT_PER_TRI, GL_UNSIGNED_SHORT, _pIndices + (texture_start_index * TRI_COUNT_PER_QUAD * VTX_COUNT_PER_TRI));
			}
			if (current_flag & ROTATE_FLAG || current_flag & SCALE_FLAG || current_flag & FREE_TRANSFORM_FLAG)
			{
				::glPopMatrix();
			}
		}
		::glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		::glDisableClientState(GL_COLOR_ARRAY);
		::glDisableClientState(GL_VERTEX_ARRAY);
	}
	/*if (_isCacheScreenState == 1)
	{
		BeginReadScreen();
	}*/
#if defined(_DEBUG)
	if (this == _dbg || _dbg == NULL) return;
	_dbg->Flush();
#endif
}

void GLESLib1::CleanCurrentFlag()
{
	_pFlags[_nNum2DQuads] = 0;
#if defined(USE_BLEND_ALPHA)
	_fillter_mode[_nNum2DQuads] = BLEND_MODE::ALPHA_FILLTER_NORMAL;
#endif
}
float GLESLib1::GetCurrentCenterScaleX()
{
#if defined(USE_GL_SCALE)
	return _scale_center_x;
#else
	return 0.0f;
#endif
}
float GLESLib1::GetCurrentCenterScaleY()
{
#if defined(USE_GL_SCALE)
	return _scale_center_y;
#else
	return 0.0f;
#endif
}
float GLESLib1::GetCurrentScaleX()
{
#if defined(USE_GL_SCALE)
	return _scale_x;
#else
	return 0.0f;
#endif
}
float GLESLib1::GetCurrentScaleY()
{
#if defined(USE_GL_SCALE)
	return _scale_y;
#else
	return 0.0f;
#endif
}
int GLESLib1::GetCurrentOpacityPercent()
{
#if defined(ENABLE_OPACITY_SETTING)
	return _opacity_percent;
#else
	return 100;
#endif
}

void GLESLib1::Clip(int x, int y, int w, int h)
{
	float cx = (float)x * _curr_gcontext.ScaleX;
	float cy = (float)y *_curr_gcontext.ScaleY;
	float cw = (float)w *_curr_gcontext.ScaleX;
	float ch = (float)h *_curr_gcontext.ScaleY;

	cx += _curr_gcontext.TranslateX;
	cy += _curr_gcontext.TranslateY;
	float tmp;
	switch (_curr_gcontext.ComplexMode)
	{
	case PineGraphicComplexOrientation::ComplexNone:
		cy = _curr_gcontext.DeviceSize.H - cy - ch;
		//x += _curr_gcontext.TranslateX;
		//y += _curr_gcontext.TranslateY;
		break;

	case PineGraphicComplexOrientation::ComplexToLeft:
		tmp = cy;
		cy = cx;
		cx = tmp;
			
		tmp = cw;
		cw = ch;
		ch = tmp;
		break;

	case PineGraphicComplexOrientation::ComplexToRight:
		tmp = cy;
		cy = _curr_gcontext.DeviceSize.H - cx - cw;
		cx = _curr_gcontext.DeviceSize.W - tmp -ch;

		tmp = cw;
		cw = ch;
		ch = tmp;

		//x -= _curr_gcontext.TranslateY;
		//y -= _curr_gcontext.TranslateX;
		break;
	}
	
#ifdef USE_HALF_SCREEN_WIN
	::glScissor(cx / 2, cy / 2, cw / 2, ch / 2);
#else
	::glScissor(cx, cy, cw, ch);
#endif
}

void GLESLib1::SetClip(int x, int y, int w, int h)
{
	_curr_clip.ClipArea.X = x;
	_curr_clip.ClipArea.Y = y;
	_curr_clip.ClipArea.W = w;
	_curr_clip.ClipArea.H = h;

	if (_nNum2DQuads >= (MAX_2D_QUADS - 1))
	{
		return;
	}
#ifdef USE_GL_ZOOM
	if (_is_zoom)
	{
		GLfloat matrix[16];
		GetMatrix(x, y, w, h);
		for (int i = 0; i < 16; i++)
		{
			matrix[i] = _maxtrix_result[i];
		}
		MulMatrix(matrix, _zoom_transform);


		float xx = x;
		float yy = y;
		float ww = w;
		float hh = h;

		xx = _maxtrix_result[0];
		yy = _maxtrix_result[1];
		ww *= _zoom_transform[0];
		hh *= _zoom_transform[5];

		x = (xx + 0.5f);
		y = (yy + 0.5f);
		w = (ww + 0.5f);
		h = (hh + 0.5f);
	}
#endif
#if USE_RESIZE_WINDOWS
	x = x*CGame::_this->_windows_screen_scale;
	y = y*CGame::_this->_windows_screen_scale + 2 * GetPaintY();
	w = w*CGame::_this->_windows_screen_scale;
	h = h*CGame::_this->_windows_screen_scale + 3 * GetPaintY();

#endif

	//clipx = x;
	//clipy = y;
	//clipw = w;
	//cliph = h;

	VTXUNIT*	pVtx = _pVtxBuffer2D + (VTX_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);

	_pTexID[_nNum2DQuads] = 0;
	_pFlags[_nNum2DQuads] = SETCLIP_FLAG;

	*pVtx++ = x;		*pVtx++ = y;
	*pVtx++ = w + 1;	*pVtx++ = h + 1;
	//_pAlpha[_nNum2DQuads] = 255;
	_nNum2DQuads++;
}
void GLESLib1::SetClip(PineClip clip){
	SetClip(clip.ClipArea.X, clip.ClipArea.Y, clip.ClipArea.W, clip.ClipArea.H);
}
void GLESLib1::ClearClip()
{
	if (_nNum2DQuads >= (MAX_2D_QUADS - 1))
	{
		return;
	}

	_pTexID[_nNum2DQuads] = 0;
	_pFlags[_nNum2DQuads] = CLEARCLIP_FLAG;
	//_pAlpha[_nNum2DQuads] = 255;
	_nNum2DQuads++;
}
PineClip GLESLib1::GetCurrentClip(){
	return _curr_clip;
}
COLOR GLESLib1::GetCurrentColor()
{
    return _nColor2D;
}
void GLESLib1::SetColor(COLOR clr)
{
	_nColor2D = clr;
}
#if defined(USE_COLOR_MASK)
COLOR GLESLib1::GetColorMask()
{
	return _color_mask;
}
void GLESLib1::SetColorMask(COLOR clr)
{
	_is_color_flag = true;
	_color_mask = clr;
}
void GLESLib1::ClearColorMask()
{
	_is_color_flag = false;
	_color_mask = 0xFFFFFFFF;
}
#endif

//////////////////////////////////////////
//OPACITY
//////////////////////////////////////////
#if defined(ENABLE_OPACITY_SETTING)
void GLESLib1::SetOpacity(int percent)
{
	if (percent < 0)
		percent = 0;
	if (percent > 100)
		percent = 100;
	_opacity_percent = percent;
}
#if defined(USE_BLEND_ALPHA)
void GLESLib1::SetFillterMode(BLEND_MODE fillter_mode)
{
	_current_fillter_mode = fillter_mode;
}
#endif
#endif

///////////////////////////////////////////
//ROTATE
///////////////////////////////////////////
#if defined(USE_GL_ROTATE)
bool GLESLib1::HasRotate()
{
	return _is_rotate;
}

float GLESLib1::GetCurrentRotateAngle()
{
	return _rotate_angle;
}

float GLESLib1::GetCurrentRotateX()
{
	return _rotate_x;
}
float GLESLib1::GetCurrentRotateY()
{
	return _rotate_y;
}
void GLESLib1::SetRotate(float angle, float x, float y)
{
	_is_rotate = true;
#if OPENGLES_20_SUPPORT
	_rotate_angle = angle*M_PI / 180;
#else
	_rotate_angle = angle;
#endif
	_rotate_x = x;
	_rotate_y = y;
}
void GLESLib1::ClearRotate()
{
	_is_rotate = false;
}
#endif

///////////////////////////////////////////
//TRANSLATE
///////////////////////////////////////////
#if defined(USE_TRANSLATE)
void GLESLib1::SetTranslate(float x, float y)
{
	_is_translate = true;
	_translate_x = x;
	_translate_y = y;
}
void GLESLib1::ClearTranslate()
{
	_is_translate = false;
}
#endif

///////////////////////////////////////////
//SCALE
///////////////////////////////////////////
#if defined(USE_GL_SCALE)
bool GLESLib1::HasScale()
{
	return _is_scale;
}
void GLESLib1::SetScale(float scale_x, float scale_y, float center_x, float center_y)
{
	_is_scale = true;
	_scale_x = scale_x;
	_scale_y = scale_y;
	_scale_center_x = center_x;
	_scale_center_y = center_y;
}
void GLESLib1::ClearScale()
{
	_is_scale = false;
	_scale_x = 1.0f;
	_scale_y = 1.0f;
}
ScaleParam GLESLib1::GetCurrentScaleParam(){
	return ScaleParam(_scale_x, _scale_y, _scale_center_x, _scale_center_y);
}
#endif


///////////////////////////////////////////
//DRAW LINE
///////////////////////////////////////////
#if defined(USE_DRAW_LINE)
void GLESLib1::DrawLine(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, bool enableAlpha)
{
	VTXUNIT*	pVtx = _pVtxBuffer2D + (VTX_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);
	UINT8*	pClr = _pClrBuffer2D + (COLOR_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);
	float*  pTex = _pTexBuffer2D + (TEXCOOR_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);


	UINT8	a = (UINT8)((_nColor2D >> 24) & 0xFF);
	UINT8	r = (_nColor2D >> 16) & 0xFF;
	UINT8	g = (_nColor2D >> 8) & 0xFF;
	UINT8	b = _nColor2D & 0xFF;

#if defined(USE_COLOR_MASK)
	if (_is_color_flag)
	{
		r = (_color_mask >> 16) & 0xFF;
		g = (_color_mask >> 8) & 0xFF;
		b = (_color_mask)& 0xFF;
	}
#endif

#if defined(ENABLE_OPACITY_SETTING)
	if (enableAlpha)
	{
		a = a*_opacity_percent / 100.0f;
	}
	else
	{
		a = 255.0f * _opacity_percent / 100.0f;
	}
#endif
	_pTexID[_nNum2DQuads] = 0;
	_pFlags[_nNum2DQuads] = DRAWLINE_FLAG;
	if (enableAlpha
#if defined(ENABLE_OPACITY_SETTING)
		|| _opacity_percent < 100
#endif
		)
	{
		_pFlags[_nNum2DQuads] |= TRANSP_FLAG;
#ifdef USE_BLEND_ALPHA
		_fillter_mode[_nNum2DQuads] = _current_fillter_mode;
#endif
	}
#if defined(USE_GL_ROTATE)
	if (_is_rotate)
	{
		_pFlags[_nNum2DQuads] |= ROTATE_FLAG;
		_pRotateParams[_nNum2DQuads*NUM_ROTATE_PARAMS] = _rotate_angle;
		_pRotateParams[_nNum2DQuads*NUM_ROTATE_PARAMS + 1] = _rotate_x;
		_pRotateParams[_nNum2DQuads*NUM_ROTATE_PARAMS + 2] = _rotate_y;
	}
#endif
#if defined(USE_TRANSLATE)
	if (_is_translate)
	{
		_pFlags[_nNum2DQuads] |= TRANSLATE_FLAG;
		_pTranslateParams[_nNum2DQuads*NUM_TRANSLATE_PARAMS] = _translate_x;
		_pTranslateParams[_nNum2DQuads*NUM_TRANSLATE_PARAMS + 1] = _translate_y;
	}
#endif

#if defined(USE_FREE_TRANSFORM)
	if (_is_transform){
		_pFlags[_nNum2DQuads] |= FREE_TRANSFORM_FLAG;
		float*	pModel = _pModBuffer2D + (MODEL_SIZE * MODEL_COUNT_PER_QUAD * _nNum2DQuads);
		*pModel++ = _transform_param.TranslateX;
		*pModel++ = _transform_param.TranslateY;
		*pModel++ = _transform_param.m11;
		*pModel++ = _transform_param.m12;
		*pModel++ = _transform_param.m21;
		*pModel++ = _transform_param.m22;
	}
#endif

#ifdef USE_GL_MULTI_BUFFER
	_pframe[_nNum2DQuads] = _curr_frame;
#endif
	*pVtx++ = x1;		*pVtx++ = y1;
	*pClr++ = r;		*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = 0.0f;	*pTex++ = 0.0f;

	*pVtx++ = x2;		*pVtx++ = y2;
	*pClr++ = r;		*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = 0.0f;	*pTex++ = 0.0f;
	//_pAlpha[_nNum2DQuads] = a;
	_nNum2DQuads++;
}
void GLESLib1::DrawLine(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, COLOR color1, COLOR color2)
{
	VTXUNIT*	pVtx = _pVtxBuffer2D + (VTX_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);
	UINT8*	pClr = _pClrBuffer2D + (COLOR_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);
	float*  pTex = _pTexBuffer2D + (TEXCOOR_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);


	UINT8	a1 = (UINT8)((color1 >> 24) & 0xFF);
	UINT8	r1 = (color1 >> 16) & 0xFF;
	UINT8	g1 = (color1 >> 8) & 0xFF;
	UINT8	b1 = color1 & 0xFF;

	UINT8	a2 = (UINT8)((color2 >> 24) & 0xFF);
	UINT8	r2 = (color2 >> 16) & 0xFF;
	UINT8	g2 = (color2 >> 8) & 0xFF;
	UINT8	b2 = color2 & 0xFF;

	_pTexID[_nNum2DQuads] = 0;
	_pFlags[_nNum2DQuads] = DRAWLINE_FLAG;
	//if (enableAlpha)
	{
		_pFlags[_nNum2DQuads] |= TRANSP_FLAG;
#if defined(USE_BLEND_ALPHA)
		_fillter_mode[_nNum2DQuads] = _current_fillter_mode;
#endif
	}
#if defined(USE_GL_ROTATE)
	if (_is_rotate)
	{
		_pFlags[_nNum2DQuads] |= ROTATE_FLAG;
		_pRotateParams[_nNum2DQuads*NUM_ROTATE_PARAMS] = _rotate_angle;
		_pRotateParams[_nNum2DQuads*NUM_ROTATE_PARAMS + 1] = _rotate_x;
		_pRotateParams[_nNum2DQuads*NUM_ROTATE_PARAMS + 2] = _rotate_y;
	}
#endif
#if defined(USE_TRANSLATE)
	if (_is_translate)
	{
		_pFlags[_nNum2DQuads] |= TRANSLATE_FLAG;
		_pTranslateParams[_nNum2DQuads*NUM_TRANSLATE_PARAMS] = _translate_x;
		_pTranslateParams[_nNum2DQuads*NUM_TRANSLATE_PARAMS + 1] = _translate_y;
	}
#endif
#if defined(USE_FREE_TRANSFORM)
	if (_is_transform){
		if ((_pFlags[_nNum2DQuads] & FREE_TRANSFORM_FLAG) == 0){
			_pFlags[_nNum2DQuads] |= FREE_TRANSFORM_FLAG;
			float*	pModel = _pModBuffer2D + (MODEL_SIZE * MODEL_COUNT_PER_QUAD * _nNum2DQuads);
			*pModel++ = _transform_param.TranslateX;
			*pModel++ = _transform_param.TranslateY;
			*pModel++ = _transform_param.m11;
			*pModel++ = _transform_param.m12;
			*pModel++ = _transform_param.m21;
			*pModel++ = _transform_param.m22;
		}
	}
#endif
	*pVtx++ = x1;		*pVtx++ = y1;
	*pClr++ = r1;		*pClr++ = g1;	*pClr++ = b1;	*pClr++ = a1;
	*pTex++ = 0.0f;	*pTex++ = 0.0f;

	*pVtx++ = x2;		*pVtx++ = y2;
	*pClr++ = r2;		*pClr++ = g2;	*pClr++ = b2;	*pClr++ = a2;
	*pTex++ = 0.0f;	*pTex++ = 0.0f;
	//_pAlpha[_nNum2DQuads] = 255;
	_nNum2DQuads++;
}
#if defined(USE_DRAW_TRIANGLE)
void GLESLib1::DrawTriangle(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, VTXUNIT x3, VTXUNIT y3, bool enableAlpha)
{
	DrawLine(x1, y1, x2, y2, enableAlpha);
	DrawLine(x2, y2, x3, y3, enableAlpha);
	DrawLine(x3, y3, x1, y1, enableAlpha);
}
#if defined(USE_DRAW_CIRCLE)
void GLESLib1::DrawCircle(VTXUNIT x, VTXUNIT y, float radius, int density, bool enableAlpha){
	if (density < 1)return;
	VTXUNIT xx1 = x + (VTXUNIT)(0 * radius);
	VTXUNIT yy1 = y + (VTXUNIT)(1 * radius);
	float delta = (360 / ((4 * density) - 1));
	for (int angle = 0; angle <= 360; angle += delta)
	{
		VTXUNIT xx = x + (VTXUNIT)(sin((double)angle*3.1416f / 180.0f) * radius);
		VTXUNIT yy = y + (VTXUNIT)(cos((double)angle*3.1416f / 180.0f) * radius);

		DrawLine(xx1, yy1, xx, yy, enableAlpha);
		xx1 = xx;
		yy1 = yy;
	}
	DrawLine(xx1, yy1, x + (VTXUNIT)(0 * radius), y + (VTXUNIT)(1 * radius), enableAlpha);
}
#endif
#endif
void GLESLib1::DrawPolygon(PinePolygon polygon, bool enableAlpha){
	int vert_num = polygon.getVertexNum();
	if (vert_num > 0){
		PinePoint curr_point = polygon.getVertex(0);
		for (int v = 1; v < vert_num; v++){
			PinePoint point = polygon.getVertex(v);
			DrawLine(curr_point.X, curr_point.Y, point.X, point.Y, enableAlpha);
			curr_point = point;
		}
	}
}
#endif//USE_DRAW_LINE


///////////////////////////////////////////
//DRAW TRIANGLE
///////////////////////////////////////////
#if defined(USE_DRAW_TRIANGLE)
void GLESLib1::FillTriangle(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, VTXUNIT x3, VTXUNIT y3, bool enableAlpha)
{
	VTXUNIT*	pVtx = _pVtxBuffer2D + (VTX_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);
	UINT8*	pClr = _pClrBuffer2D + (COLOR_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);
	float*  pTex = _pTexBuffer2D + (TEXCOOR_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);


	UINT8	a = (UINT8)((_nColor2D >> 24) & 0xFF);
	UINT8	r = (_nColor2D >> 16) & 0xFF;
	UINT8	g = (_nColor2D >> 8) & 0xFF;
	UINT8	b = _nColor2D & 0xFF;
#if defined(ENABLE_OPACITY_SETTING)
	float opacity = (float)_opacity_percent / 100.0f;
	a *= opacity;
#endif
#if defined(USE_COLOR_MASK)
	if (_is_color_flag)
	{
		r = (_color_mask >> 16) & 0xFF;
		g = (_color_mask >> 8) & 0xFF;
		b = (_color_mask)& 0xFF;
	}
#endif

	_pTexID[_nNum2DQuads] = 0;
	_pFlags[_nNum2DQuads] = DRAWTRIANGLE_FLAG;
	if (enableAlpha)
	{
		_pFlags[_nNum2DQuads] |= TRANSP_FLAG;
	}
	
	#ifdef USE_GL_ROTATE
	if (_is_rotate)
	{
	_pFlags[_nNum2DQuads] |= ROTATE_FLAG;
	_pRotateParams[_nNum2DQuads*NUM_ROTATE_PARAMS] = _rotate_angle;
	_pRotateParams[_nNum2DQuads*NUM_ROTATE_PARAMS + 1] = _rotate_x;
	_pRotateParams[_nNum2DQuads*NUM_ROTATE_PARAMS + 2] = _rotate_y;
	}
	#endif

	#if USE_TRANSLATE
	if(_is_translate)
	{
	_pFlags[_nNum2DQuads] |= TRANSLATE_FLAG;
	_pTranslateParams[_nNum2DQuads*NUM_TRANSLATE_PARAMS] = _translate_x;
	_pTranslateParams[_nNum2DQuads*NUM_TRANSLATE_PARAMS+1] = _translate_y;
	}
	#endif
#if defined(USE_FREE_TRANSFORM)
	if (_is_transform){
		if ((_pFlags[_nNum2DQuads] & FREE_TRANSFORM_FLAG) == 0){
			_pFlags[_nNum2DQuads] |= FREE_TRANSFORM_FLAG;
			float*	pModel = _pModBuffer2D + (MODEL_SIZE * MODEL_COUNT_PER_QUAD * _nNum2DQuads);
			*pModel++ = _transform_param.TranslateX;
			*pModel++ = _transform_param.TranslateY;
			*pModel++ = _transform_param.m11;
			*pModel++ = _transform_param.m12;
			*pModel++ = _transform_param.m21;
			*pModel++ = _transform_param.m22;
		}
	}
#endif
	*pVtx++ = x1;		*pVtx++ = y1;
	*pClr++ = r;		*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = 0.0f;	*pTex++ = 0.0f;

	*pVtx++ = x2;		*pVtx++ = y2;
	*pClr++ = r;		*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = 0.0f;	*pTex++ = 0.0f;

	*pVtx++ = x3;		*pVtx++ = y3;
	*pClr++ = r;		*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = 0.0f;	*pTex++ = 0.0f;
	//_pAlpha[_nNum2DQuads] = a;
	_nNum2DQuads++;
}
void GLESLib1::FillTriangle(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, VTXUNIT x3, VTXUNIT y3, HTEXTURE tex_id, float u1, float v1, float u2, float v2, float u3, float v3, bool enableAlpha) {
	VTXUNIT*	pVtx = _pVtxBuffer2D + (VTX_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);
	UINT8*	pClr = _pClrBuffer2D + (COLOR_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);
	float*  pTex = _pTexBuffer2D + (TEXCOOR_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);


	UINT8	a = (UINT8)((_nColor2D >> 24) & 0xFF);
	UINT8	r = (_nColor2D >> 16) & 0xFF;
	UINT8	g = (_nColor2D >> 8) & 0xFF;
	UINT8	b = _nColor2D & 0xFF;
#if defined(ENABLE_OPACITY_SETTING)
	float opacity = (float)_opacity_percent / 100.0f;
	a *= opacity;
#endif
#if defined(USE_COLOR_MASK)
	if (_is_color_flag)
	{
		r = (_color_mask >> 16) & 0xFF;
		g = (_color_mask >> 8) & 0xFF;
		b = (_color_mask)& 0xFF;
	}
#endif

	_pTexID[_nNum2DQuads] = tex_id;
	_pFlags[_nNum2DQuads] = DRAWTRIANGLE_FLAG;
	if (enableAlpha)
	{
		_pFlags[_nNum2DQuads] |= TRANSP_FLAG;
	}

#ifdef USE_GL_ROTATE
	if (_is_rotate)
	{
		_pFlags[_nNum2DQuads] |= ROTATE_FLAG;
		_pRotateParams[_nNum2DQuads*NUM_ROTATE_PARAMS] = _rotate_angle;
		_pRotateParams[_nNum2DQuads*NUM_ROTATE_PARAMS + 1] = _rotate_x;
		_pRotateParams[_nNum2DQuads*NUM_ROTATE_PARAMS + 2] = _rotate_y;
	}
#endif

#if USE_TRANSLATE
	if (_is_translate)
	{
		_pFlags[_nNum2DQuads] |= TRANSLATE_FLAG;
		_pTranslateParams[_nNum2DQuads*NUM_TRANSLATE_PARAMS] = _translate_x;
		_pTranslateParams[_nNum2DQuads*NUM_TRANSLATE_PARAMS + 1] = _translate_y;
	}
#endif
#if defined(USE_FREE_TRANSFORM)
	if (_is_transform) {
		if ((_pFlags[_nNum2DQuads] & FREE_TRANSFORM_FLAG) == 0) {
			_pFlags[_nNum2DQuads] |= FREE_TRANSFORM_FLAG;
			float*	pModel = _pModBuffer2D + (MODEL_SIZE * MODEL_COUNT_PER_QUAD * _nNum2DQuads);
			*pModel++ = _transform_param.TranslateX;
			*pModel++ = _transform_param.TranslateY;
			*pModel++ = _transform_param.m11;
			*pModel++ = _transform_param.m12;
			*pModel++ = _transform_param.m21;
			*pModel++ = _transform_param.m22;
		}
	}
#endif
	*pVtx++ = x1;		*pVtx++ = y1;
	*pClr++ = r;		*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = u1;	*pTex++ = v1;

	*pVtx++ = x2;		*pVtx++ = y2;
	*pClr++ = r;		*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = u2;	*pTex++ = v2;

	*pVtx++ = x3;		*pVtx++ = y3;
	*pClr++ = r;		*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = u3;	*pTex++ = v3;
	//_pAlpha[_nNum2DQuads] = a;
	_nNum2DQuads++;
}
void GLESLib1::FillPolygon(PinePolygon polygon, bool enableAlpha) {
	int vert_num = polygon.getVertexNum();
	if (vert_num > 3) {
		PinePoint point_o = polygon.getVertex(0);
		PinePoint curr_point = polygon.getVertex(1);
		for (int v = 2; v < vert_num; v++) {
			PinePoint point = polygon.getVertex(v);
			FillTriangle(point_o.X, point_o.Y, curr_point.X, curr_point.Y, point.X, point.Y, enableAlpha);
			curr_point = point;
		}
	}
}
void GLESLib1::FillPolygon(PinePolygon polygon, HTEXTURE texId, float tw, float th, float tx, float ty, bool enableAlpha ) {
	int vert_num = polygon.getVertexNum();
	if (vert_num > 3) {
		PinePoint point_o = polygon.getVertex(0);
		float u0 = (point_o.X - tx)/ tw;
		float v0 = (point_o.Y - ty)/ th;
		PinePoint curr_point = polygon.getVertex(1);
		float u1 = (curr_point.X - tx)/ tw;
		float v1 = (curr_point.Y - ty)/ th;
		for (int v = 2; v < vert_num; v++) {
			PinePoint point = polygon.getVertex(v);
			float u2 = (point.X - tx)/ tw;
			float v2 = (point.Y - ty)/ th;
			FillTriangle(point_o.X, point_o.Y, curr_point.X, curr_point.Y, point.X, point.Y, texId,u0, v0, u1,v1, u2,v2, enableAlpha);
			curr_point = point;
			u1 = u2;
			v1 = v2;
		}
	}
}
#if defined(USE_DRAW_CIRCLE)
void GLESLib1::FillCircle(VTXUNIT x, VTXUNIT y, float radius, int density, bool enableAlpha){
	if (density < 1)return;
	VTXUNIT xx1 = x + (VTXUNIT)(0 * radius);
	VTXUNIT yy1 = y + (VTXUNIT)(1 * radius);
	for (int angle = 0; angle <= 360; angle += (360 / ((4 * density) - 1)))
	{
		VTXUNIT xx = x + (VTXUNIT)(sin((double)angle*3.1416f / 180.0f) * radius);
		VTXUNIT yy = y + (VTXUNIT)(cos((double)angle*3.1416f / 180.0f) * radius);

		FillTriangle(xx1, yy1, xx, yy, x, y, enableAlpha);
		xx1 = xx;
		yy1 = yy;
	}
	FillTriangle(xx1, yy1, x + (VTXUNIT)(0 * radius), y + (VTXUNIT)(1 * radius), x, y, enableAlpha);
}
#endif
#endif//USE_DRAW_TRIANGLE

#if defined(USE_DRAW_LINE)
void GLESLib1::DrawRect(VTXUNIT x, VTXUNIT y, VTXUNIT w, VTXUNIT h, bool enableAlpha)
{
	DrawLine(x, y, x, y + h);
	DrawLine(x, y + h, x + w, y + h);
	DrawLine(x + w, y + h, x + w, y);
	DrawLine(x + w, y, x, y);
}
#endif
void GLESLib1::FillRect(VTXUNIT x, VTXUNIT y, VTXUNIT w, VTXUNIT h, bool enableAlpha)
{
	VTXUNIT*	pVtx = _pVtxBuffer2D + (VTX_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);
	UINT8*	pClr = _pClrBuffer2D + (COLOR_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);
	float*  pTex = _pTexBuffer2D + (TEXCOOR_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);

	UINT8	a = (UINT8)((_nColor2D >> 24) & 0xFF);
	UINT8	r = (_nColor2D >> 16) & 0xFF;
	UINT8	g = (_nColor2D >> 8) & 0xFF;
	UINT8	b = _nColor2D & 0xFF;

#if defined(ENABLE_OPACITY_SETTING)
	float opacity = (float)_opacity_percent/100.0f;
	a *= opacity;
#endif
#if defined(USE_COLOR_MASK)
	if (_is_color_flag)
	{
		r = (_color_mask >> 16) & 0xFF;
		g = (_color_mask >> 8) & 0xFF;
		b = (_color_mask)& 0xFF;
	}
#endif

	_pTexID[_nNum2DQuads] = 0;
	_pFlags[_nNum2DQuads] = 0;

	if (enableAlpha)
	{
		_pFlags[_nNum2DQuads] = TRANSP_FLAG;
#if defined(USE_BLEND_ALPHA)
		_fillter_mode[_nNum2DQuads] = _current_fillter_mode;
#endif
	}
	else
	{
		a = 0xff;
	}

#if defined(USE_GL_ROTATE)
	if (_is_rotate)
	{
		_pFlags[_nNum2DQuads] |= ROTATE_FLAG;
		_pRotateParams[_nNum2DQuads*NUM_ROTATE_PARAMS] = _rotate_angle;
		_pRotateParams[_nNum2DQuads*NUM_ROTATE_PARAMS + 1] = _rotate_x;
		_pRotateParams[_nNum2DQuads*NUM_ROTATE_PARAMS + 2] = _rotate_y;
	}
#endif
#if defined(USE_TRANSLATE)
	if (_is_translate)
	{
		_pFlags[_nNum2DQuads] |= TRANSLATE_FLAG;
		_pTranslateParams[_nNum2DQuads*NUM_TRANSLATE_PARAMS] = _translate_x;
		_pTranslateParams[_nNum2DQuads*NUM_TRANSLATE_PARAMS + 1] = _translate_y;
	}
#endif
#if defined(USE_GL_SCALE)
	if (_is_scale)
	{
		_pFlags[_nNum2DQuads] |= SCALE_FLAG;
		_pScaleParams[_nNum2DQuads*NUM_SCALE_PARAMS] = _scale_x;
		_pScaleParams[_nNum2DQuads*NUM_SCALE_PARAMS + 1] = _scale_y;
		_pScaleParams[_nNum2DQuads*NUM_SCALE_PARAMS + 2] = _scale_center_x;
		_pScaleParams[_nNum2DQuads*NUM_SCALE_PARAMS + 3] = _scale_center_y;
	}
#endif
#if defined(USE_FREE_TRANSFORM)
	if (_is_transform){
		if ((_pFlags[_nNum2DQuads] & FREE_TRANSFORM_FLAG) == 0){
			_pFlags[_nNum2DQuads] |= FREE_TRANSFORM_FLAG;
			float*	pModel = _pModBuffer2D + (MODEL_SIZE * MODEL_COUNT_PER_QUAD * _nNum2DQuads);
			*pModel++ = _transform_param.TranslateX;
			*pModel++ = _transform_param.TranslateY;
			*pModel++ = _transform_param.m11;
			*pModel++ = _transform_param.m12;
			*pModel++ = _transform_param.m21;
			*pModel++ = _transform_param.m22;
		}
}
#endif
#if defined(USE_GL_ZOOM)
	if (_is_zoom)
	{
		_pFlags[_nNum2DQuads] |= ZOOM_FLAG;
		for (int i = 0; i < 16; i++)
		{
			_zoom_transform_list[_nNum2DQuads][i] = _zoom_transform[i];
		}
	}
#endif
	//if (_curr_gcontext.Viewport.IsRectOutSide(x, y, w, h))return;
	*pVtx++ = x;		*pVtx++ = y;
	*pClr++ = r;		*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = 0.0f;	*pTex++ = 0.0f;

	*pVtx++ = (x + w);	*pVtx++ = y;
	*pClr++ = r;		*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = 0.0f;	*pTex++ = 0.0f;

	*pVtx++ = (x + w);	*pVtx++ = (y + h);
	*pClr++ = r;		*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = 0.0f;	*pTex++ = 0.0f;

	*pVtx++ = x;		*pVtx++ = (y + h);
	*pClr++ = r;		*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = 0.0f;	*pTex++ = 0.0f;
	//_pAlpha[_nNum2DQuads] = a;
	_nNum2DQuads++;
}
void GLESLib1::FillFullScreen(bool enableAlpha){

	float x = -(_curr_gcontext.TranslateX / _curr_gcontext.ScaleX);
	float y = -(_curr_gcontext.TranslateY / _curr_gcontext.ScaleY);
	 
	float w = _curr_gcontext.DeviceSize.W / _curr_gcontext.ScaleX;
	float h = _curr_gcontext.DeviceSize.H/ _curr_gcontext.ScaleY;
	float sizemax = w > h ? w : h;
	w = h = sizemax;

	VTXUNIT*	pVtx = _pVtxBuffer2D + (VTX_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);
	UINT8*	pClr = _pClrBuffer2D + (COLOR_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);
	float*  pTex = _pTexBuffer2D + (TEXCOOR_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);

	UINT8	a = (UINT8)((_nColor2D >> 24) & 0xFF);
	UINT8	r = (_nColor2D >> 16) & 0xFF;
	UINT8	g = (_nColor2D >> 8) & 0xFF;
	UINT8	b = _nColor2D & 0xFF;

#if defined(ENABLE_OPACITY_SETTING)
	float opacity = (float)_opacity_percent / 100.0f;
	a *= opacity;
#endif

#if defined(USE_COLOR_MASK)
	if (_is_color_flag)
	{
		r = (_color_mask >> 16) & 0xFF;
		g = (_color_mask >> 8) & 0xFF;
		b = (_color_mask)& 0xFF;
	}
#endif

	_pTexID[_nNum2DQuads] = 0;
	_pFlags[_nNum2DQuads] = 0;

	if (enableAlpha)
	{
		_pFlags[_nNum2DQuads] = TRANSP_FLAG;

#if defined(USE_BLEND_ALPHA)
		_fillter_mode[_nNum2DQuads] = _current_fillter_mode;
#endif

	}
	else
	{
		a = 0xff;
	}

	*pVtx++ = x;		*pVtx++ = y;
	*pClr++ = r;		*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = 0.0f;	*pTex++ = 0.0f;

	*pVtx++ = (x + w);	*pVtx++ = y;
	*pClr++ = r;		*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = 0.0f;	*pTex++ = 0.0f;

	*pVtx++ = (x + w);	*pVtx++ = (y + h);
	*pClr++ = r;		*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = 0.0f;	*pTex++ = 0.0f;

	*pVtx++ = x;		*pVtx++ = (y + h);
	*pClr++ = r;		*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = 0.0f;	*pTex++ = 0.0f;
	//_pAlpha[_nNum2DQuads] = a;
	_nNum2DQuads++;
	
}

void GLESLib1::Draw2DTexture(VTXUNIT x, VTXUNIT y, int w, int h, HTEXTURE texId, float uv[], int flags, bool enableAlpha)
{
	PineMatrix4 trans;
#if defined (USE_CALC_BY_VECTOR_AND_REUSE_RESULT)
	PineVector3 x0y0, x0y1, x1y1, x1y0;
	x0y0.SetValue(x, y, 0.0f);
	x0y1.SetValue(x, y + h, 0.0f);
	x1y1.SetValue(x + w, y + h, 0.0f);
	x1y0.SetValue(x + w, y, 0.0f);

	if (_pFlags[_nNum2DQuads] & FREE_TRANSFORM_FLAG)
	{
		_pFlags[_nNum2DQuads] &= ~FREE_TRANSFORM_FLAG;
		float*	pModel = _pModBuffer2D + (MODEL_SIZE * MODEL_COUNT_PER_QUAD * _nNum2DQuads);

		PineTransformParam tp;
		tp.TranslateX = *pModel++;
		tp.TranslateY = *pModel++;
		tp.m11 = *pModel++;
		tp.m12 = *pModel++;
		tp.m21 = *pModel++;
		tp.m22 = *pModel;

		trans = CMath::Matrix4FromParam(tp);

		x0y0 = CMath::Vector3Transform(x0y0, trans);
		x0y1 = CMath::Vector3Transform(x0y1, trans);
		x1y1 = CMath::Vector3Transform(x1y1, trans);
		x1y0 = CMath::Vector3Transform(x1y0, trans);
	}
	else if (_is_transform)
	{
		trans = CMath::Matrix4FromParam(_transform_param);

		x0y0 = CMath::Vector3Transform(x0y0, trans);
		x0y1 = CMath::Vector3Transform(x0y1, trans);
		x1y1 = CMath::Vector3Transform(x1y1, trans);
		x1y0 = CMath::Vector3Transform(x1y0, trans);
	}

	if (_is_scale)
	{
		PineVector3 scaleVec, center;
		center.SetValue(_scale_center_x, _scale_center_y, 0.0f);
		scaleVec.SetValue(_scale_x, _scale_y, 1.0f);

		x0y0 = CMath::Vector3Scale(x0y0, scaleVec, center);
		x0y1 = CMath::Vector3Scale(x0y1, scaleVec, center);
		x1y1 = CMath::Vector3Scale(x1y1, scaleVec, center);
		x1y0 = CMath::Vector3Scale(x1y0, scaleVec, center);
	}

	if (_is_rotate)
	{
		PineVector3 center;
		center.SetValue(_rotate_x, _rotate_y, 0.0f);

		x0y0 = CMath::Vector3RotateZ(x0y0, _rotate_angle, center);
		x0y1 = CMath::Vector3RotateZ(x0y1, _rotate_angle, center);
		x1y1 = CMath::Vector3RotateZ(x1y1, _rotate_angle, center);
		x1y0 = CMath::Vector3RotateZ(x1y0, _rotate_angle, center);
	}
	PineRectangle rect = CMath::RectFromVectors(x0y0, x0y1, x1y1, x1y0);
#else
	PineMatrix4 mat = CMath::Matrix4FromRectangle(x, y, w, h);
	if (_is_rotate)
	{
		trans = CMath::Matrix4RotateZ2D(_rotate_angle, _rotate_x, _rotate_y);
	}

	if (_pFlags[_nNum2DQuads] & FREE_TRANSFORM_FLAG)
	{
		float*	pModel = _pModBuffer2D + (MODEL_SIZE * MODEL_COUNT_PER_QUAD * _nNum2DQuads);

		PineTransformParam tp;
		tp.TranslateX = *pModel++;
		tp.TranslateY = *pModel++;
		tp.m11 = *pModel++;
		tp.m12 = *pModel++;
		tp.m21 = *pModel++;
		tp.m22 = *pModel;

		trans = CMath::Matrix4Multiply( CMath::Matrix4FromParam(tp), trans);
	}
	else if(_is_transform)
	{
		float minx, miny, maxx, maxy;
		trans = CMath::Matrix4Multiply( CMath::Matrix4FromParam(_transform_param), trans);
	}

	if (_is_scale)
	{
		trans = CMath::Matrix4Multiply(trans, CMath::Matrix4Scale2D(_scale_x, _scale_y, _scale_center_x, _scale_center_y));
	}

	mat = CMath::Matrix4Multiply(mat, trans );

	PineRectangle rect = CMath::BoundFromMatrix4(mat);
#endif
#if defined(_DEBUG)
	DebugCacheDrawTexBorder(rect);
#endif

	if (_fullscreen_clip.ClipArea.IsRectOutSide(rect))
	{
//#if defined(_DEBUG)
		return;
//#endif
	}
	
	VTXUNIT*	pVtx = _pVtxBuffer2D + (VTX_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);
	UINT8*	pClr = _pClrBuffer2D + (COLOR_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);
	float*	pTex = _pTexBuffer2D + (TEXCOOR_SIZE * VTX_COUNT_PER_QUAD * _nNum2DQuads);

	UINT8	a = 0xFF;
	UINT8	r = 0xFF;
	UINT8	g = 0xFF;
	UINT8	b = 0xFF;

#if defined(USE_COLOR_MASK)
	if (_is_color_flag)
	{
		r = (_color_mask >> 16) & 0xFF;
		g = (_color_mask >> 8) & 0xFF;
		b = (_color_mask)& 0xFF;
	}
#endif

	_pTexID[_nNum2DQuads] = texId;

	if (enableAlpha)
	{
		_pFlags[_nNum2DQuads] |= TRANSP_FLAG;

#if defined(USE_BLEND_ALPHA)
		_fillter_mode[_nNum2DQuads] = _current_fillter_mode;
#endif

	}

#if defined(USE_GL_ROTATE) && !defined (USE_CALC_BY_VECTOR_AND_REUSE_RESULT)
	if (_is_rotate)
	{
		_pFlags[_nNum2DQuads] |= ROTATE_FLAG;
		_pRotateParams[_nNum2DQuads*NUM_ROTATE_PARAMS] = _rotate_angle;
		_pRotateParams[_nNum2DQuads*NUM_ROTATE_PARAMS + 1] = _rotate_x;
		_pRotateParams[_nNum2DQuads*NUM_ROTATE_PARAMS + 2] = _rotate_y;
	}
#endif

#if defined(USE_TRANSLATE) && !defined (USE_CALC_BY_VECTOR_AND_REUSE_RESULT)
	if (_is_translate)
	{
		_pFlags[_nNum2DQuads] |= TRANSLATE_FLAG;
		_pTranslateParams[_nNum2DQuads*NUM_TRANSLATE_PARAMS] = _translate_x;
		_pTranslateParams[_nNum2DQuads*NUM_TRANSLATE_PARAMS + 1] = _translate_y;
	}
#endif

#if defined(USE_GL_SCALE) && !defined (USE_CALC_BY_VECTOR_AND_REUSE_RESULT)
	if (_is_scale)
	{
		_pFlags[_nNum2DQuads] |= SCALE_FLAG;
		_pScaleParams[_nNum2DQuads*NUM_SCALE_PARAMS] = _scale_x;
		_pScaleParams[_nNum2DQuads*NUM_SCALE_PARAMS + 1] = _scale_y;
		_pScaleParams[_nNum2DQuads*NUM_SCALE_PARAMS + 2] = _scale_center_x;
		_pScaleParams[_nNum2DQuads*NUM_SCALE_PARAMS + 3] = _scale_center_y;
	}
#endif

#if defined(USE_FREE_TRANSFORM) && !defined (USE_CALC_BY_VECTOR_AND_REUSE_RESULT)
	if (_is_transform){
		if ((_pFlags[_nNum2DQuads] & FREE_TRANSFORM_FLAG) == 0){
			_pFlags[_nNum2DQuads] |= FREE_TRANSFORM_FLAG;
			float*	pModel = _pModBuffer2D + (MODEL_SIZE * MODEL_COUNT_PER_QUAD * _nNum2DQuads);
			*pModel++ = _transform_param.TranslateX;
			*pModel++ = _transform_param.TranslateY;
			*pModel++ = _transform_param.m11;
			*pModel++ = _transform_param.m12;
			*pModel++ = _transform_param.m21;
			*pModel++ = _transform_param.m22;
		}
	}
#endif

#if defined(USE_GL_ZOOM)
	if (_is_zoom)
	{
		_pFlags[_nNum2DQuads] |= ZOOM_FLAG;
		for (int i = 0; i < 16; i++)
		{
			_zoom_transform_list[_nNum2DQuads][i] = _zoom_transform[i];
		}
	}
#endif

#if defined(ENABLE_OPACITY_SETTING)
	a = 0xFF *(_opacity_percent / 100.0f);
#endif
	
	//uv  (u0 v0)  (u1 v1)
#if !defined(USE_CALC_BY_VECTOR_AND_REUSE_RESULT)
	*pVtx++ = x;		*pVtx++ = y;
	*pClr++ = r;	*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = uv[0];	*pTex++ = uv[1]; //u0 v0

	*pVtx++ = x + w;	*pVtx++ = y;
	*pClr++ = r;	*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = uv[2];	*pTex++ = uv[1]; //u1 v0

	*pVtx++ = x + w;	*pVtx++ = y + h;
	*pClr++ = r;	*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = uv[2];	*pTex++ = uv[3];  //u1 v1

	*pVtx++ = x;		*pVtx++ = y + h;
	*pClr++ = r;	*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = uv[0];	*pTex++ = uv[3]; //u0 v1
#else
	*pVtx++ = x0y0.x;		*pVtx++ = x0y0.y;
	*pClr++ = r;	*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = uv[0];	*pTex++ = uv[1]; //u0 v0

	*pVtx++ = x1y0.x;	*pVtx++ = x1y0.y;
	*pClr++ = r;	*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = uv[2];	*pTex++ = uv[1]; //u1 v0

	*pVtx++ = x1y1.x;	*pVtx++ = x1y1.y;
	*pClr++ = r;	*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = uv[2];	*pTex++ = uv[3];  //u1 v1

	*pVtx++ = x0y1.x;		*pVtx++ = x0y1.y;
	*pClr++ = r;	*pClr++ = g;	*pClr++ = b;	*pClr++ = a;
	*pTex++ = uv[0];	*pTex++ = uv[3]; //u0 v1
#endif
	//_pAlpha[_nNum2DQuads] = alpha_val;
	_nNum2DQuads++;
}

#if defined(USE_FREE_TRANSFORM)
void GLESLib1::LoadModel(float ox, float oy, float m11, float m12, float m21, float m22)
{
	if (_nNum2DQuads >= (MAX_2D_QUADS - 1))
	{
		return;
	}
	_pFlags[_nNum2DQuads] |= FREE_TRANSFORM_FLAG;
	float*	pModel = _pModBuffer2D + (MODEL_SIZE * MODEL_COUNT_PER_QUAD * _nNum2DQuads);
	if (_is_transform){
		PineMatrix4 mat = CMath::Matrix4FromParam(_transform_param);

		PineTransformParam model_param;
		model_param.TranslateX = ox;
		model_param.TranslateY = oy;
		model_param.m11 = m11;
		model_param.m12 = m12;
		model_param.m21 = m21;
		model_param.m22 = m22;
		
		PineMatrix4 mat_model = CMath::Matrix4FromParam(model_param);

		mat = CMath::Matrix4Multiply(mat_model, mat);

		PineTransformParam result_param = CMath::Matrix4ToParam(mat);
		*pModel++ = result_param.TranslateX;
		*pModel++ = result_param.TranslateY;
		*pModel++ = result_param.m11;
		*pModel++ = result_param.m12;
		*pModel++ = result_param.m21;
		*pModel++ = result_param.m22;
	}
	else
	{
		*pModel++ = ox;
		*pModel++ = oy;
		*pModel++ = m11;
		*pModel++ = m12;
		*pModel++ = m21;
		*pModel++ = m22;
	}
}

void GLESLib1::SetTransform(PineTransformParam param){
	_transform_param = param;
	_is_transform = true;
}

void GLESLib1::ClearTransform(){
	_transform_param.TranslateX = 0.0f;
	_transform_param.TranslateY = 0.0f;
	_transform_param.m11 = 0.0f;
	_transform_param.m12 = 0.0f;
	_transform_param.m21 = 0.0f;
	_transform_param.m22 = 0.0f;
	_is_transform = false;
}
#endif

HTEXTURE GLESLib1::createTexture(BYTE* input_data, int texw, int texh, PineColorFormat input_colorformat, int is_altialias)
{
	HTEXTURE texname;
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texname);
	glBindTexture(GL_TEXTURE_2D, texname);
//is_altialias = 1;
	if (is_altialias){
        if(is_altialias == 1)
        {
            ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_NEAREST
            ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST
			
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        }
        else if(is_altialias == 2)
        {
            ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//GL_NEAREST
            ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);//GL_NEAREST
            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
            ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
	}
	else{
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_NEAREST
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_NEAREST
        ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	GLint data_format = GL_RGBA;
	GLint pixel_type = GL_UNSIGNED_BYTE;
	int color_size = 4;
		
	if (input_colorformat == PineColorFormat::RGBA_1BYTE){
		data_format = GL_RGBA;
		pixel_type = GL_UNSIGNED_BYTE;
	}
	else if (input_colorformat == PineColorFormat::RGB_1BYTE){
		data_format = GL_RGB;
		pixel_type = GL_UNSIGNED_BYTE;
		color_size = 3;
	}
	else if (input_colorformat == PineColorFormat::RGBA_4444){
		data_format = GL_RGBA;
		pixel_type = GL_UNSIGNED_SHORT_4_4_4_4;
		color_size = 2;
	}
	else if (input_colorformat == PineColorFormat::RGBA_5551){
		data_format = GL_RGBA;
		pixel_type = GL_UNSIGNED_SHORT_5_5_5_1;
		color_size = 2;
	}
	else if (input_colorformat == PineColorFormat::RGB_565){
		data_format = GL_RGB;
		pixel_type = GL_UNSIGNED_SHORT_5_6_5;
		color_size = 2;
	}
    
	glTexImage2D(GL_TEXTURE_2D, 0, data_format, texw, texh, 0, data_format, pixel_type, input_data);

#if defined(_DEBUG)
	if (texname != NULL) 
	{
		//add to debug
		UINT32 size = texw * texh * color_size;
		Graphic::GraphicDebugTexture tex = { texname, size };
		_debug_texture.push_back(tex);
		_debug_tex_mem_use += size;
		if (_debug_tex_mem_use > _debug_tex_mem_use_max) _debug_tex_mem_use_max = _debug_tex_mem_use;
	}
#endif

	return texname;
}

void GLESLib1::updateTexture(HTEXTURE tex, BYTE* input_data, int dw, int dh, int tx, int ty, PineColorFormat color_format)
{
	GLint data_format = GL_RGBA;
	GLint pixel_type = GL_UNSIGNED_BYTE;

	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_NEAREST
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_NEAREST
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	if (color_format == PineColorFormat::RGBA_1BYTE) {
		data_format = GL_RGBA;
		pixel_type = GL_UNSIGNED_BYTE;
	}
	else if (color_format == PineColorFormat::RGB_1BYTE) {
		data_format = GL_RGB;
		pixel_type = GL_UNSIGNED_BYTE;
	}
	else if (color_format == PineColorFormat::RGBA_4444) {
		data_format = GL_RGBA;
		pixel_type = GL_UNSIGNED_SHORT_4_4_4_4;
	}
	else if (color_format == PineColorFormat::RGBA_5551) {
		data_format = GL_RGBA;
		pixel_type = GL_UNSIGNED_SHORT_5_5_5_1;
	}
	else if (color_format == PineColorFormat::RGB_565) {
		data_format = GL_RGB;
		pixel_type = GL_UNSIGNED_SHORT_5_6_5;
	}
	//for (int i = 0; i < dw * dh * 4; i++) input_data[i] = 0;

	glBindTexture(GL_TEXTURE_2D, tex);
	GLenum error = glGetError();
	glTexSubImage2D(GL_TEXTURE_2D, 0, tx, ty, dw, dh, data_format, pixel_type, input_data);

	 error = glGetError();
}

void GLESLib1::freeTexture(HTEXTURE tex)
{
#if defined(_DEBUG)
	for (int i = 0; i < _debug_texture.size(); i++)
	{
		if (_debug_texture.at(i)._tex == tex)
		{
			_debug_tex_mem_use -= _debug_texture.at(i)._size;
			_debug_texture.erase(_debug_texture.begin() + i);
			break;
		}
	}
#endif
	::glDeleteTextures(1, &tex);
}

bool GLESLib1::IsEngineReady()
{
	return true;
}
PineColorType GLESLib1::ColorType()
{
	return PineColorType::CTYPE_OPENGL;
}

void GLESLib1::setWorldMatrix(PineTransformParam param)
{
	_world_transform_param = param;
}





#if defined(_DEBUG)

Graphic* GLESLib1::_dbg = NULL;
bool GLESLib1::_create_dbg = false;
UINT32 GLESLib1::_dbg_flag = DEBUG_GRAPHIC_NONE;
std::vector<Graphic::GraphicDebugTexture> GLESLib1::_debug_texture;
UINT64 GLESLib1::_debug_tex_mem_use = 0;
UINT64 GLESLib1::_debug_tex_mem_use_max = 0;

void GLESLib1::setDebugFlagOn(UINT32 flag)
{
	_dbg_flag |= flag;
}
void GLESLib1::setDebugFlagOff(UINT32 flag)
{
	_dbg_flag &= ~flag;
}
void GLESLib1::debugInfo()
{
	PDEBUG("Graphic memory use for texture: %lld max:%lld\n", _debug_tex_mem_use, _debug_tex_mem_use_max);
	PDEBUG("Graphic current texture: %d\n", _debug_texture.size());
}

void GLESLib1::DebugCacheRectangle(PineRectangle rect)
{
	if (_dbg != NULL)
	{
		_dbg->SetColor(0x01FF0000);
		_dbg->SetOpacity(100);
		_dbg->DrawRect(rect.X, rect.Y, rect.W, rect.H, true);
	}
}
void GLESLib1::DebugCacheLine(PinePoint p1, PinePoint p2)
{
	if (_dbg != NULL)
	{
		_dbg->DrawLine(p1.X, p1.Y, p2.X, p2.Y, true);
	}
}
void GLESLib1::DebugCacheDrawTexBorder(PineRectangle rect)
{
	if (_dbg != NULL && (_dbg_flag & DEBUG_GRAPHIC_DRAWTEX))
	{
		_dbg->SetColor(0x01aa0000);
		_dbg->SetOpacity(100);
		_dbg->DrawRect(rect.X, rect.Y, rect.W, rect.H, true);
	}
}
#endif
