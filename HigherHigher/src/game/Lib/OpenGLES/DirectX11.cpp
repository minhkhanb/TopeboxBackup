#include "pch.h"
#include "DirectX11.h"
#include "../Math/SMatrix4x4.h"


#include "../Debug/Test.h"

#if defined(_USE_TEST)
//#include <d3d11_1.h>
//#pragma comment(lib, "dxerr.lib")
#endif


GDirectX11::GDirectX11(){
	_curr_device = NULL;
	_curr_context = NULL;
	//_word_matrix = XMMatrixIdentity();
	XMStoreFloat4x4(&_mx_world, XMMatrixIdentity());
	_curr_gcontext.ComplexMode = PineGraphicComplexOrientation::ComplexNone;
}
bool GDirectX11 :: IsTexturePowOf2(){
	return true;
}
void GDirectX11 :: Init()
{
	_pVtxBuffer2D = new VTXUNIT[MAX_2D_QUADS *((VTX_SIZE + COLOR_SIZE + TEXCOOR_SIZE + PARAM_SIZE) * VTX_COUNT_PER_QUAD)];
	_vertexPos = new int[MAX_2D_QUADS];
	_pTexID = new HTEXTURE[MAX_2D_QUADS];
	_pFlags = new UINT16[MAX_2D_QUADS];

#ifdef USE_FREE_TRANSFORM
	_pModBuffer2D = new float[MAX_2D_QUADS * MODEL_SIZE * MODEL_COUNT_PER_QUAD];
#endif
	ZeroMemory(&_vtexdesc, sizeof(_vtexdesc));
	_vtexdesc.Usage = D3D11_USAGE_DYNAMIC;
	_vtexdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	_vtexdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	_vtexdesc.ByteWidth = MAX_2D_QUADS * (VTX_SIZE + COLOR_SIZE + TEXCOOR_SIZE + PARAM_SIZE) * VTX_COUNT_PER_QUAD;
#ifdef USE_BLEND_ALPHA
	_fillter_mode = new BLEND_MODE[MAX_2D_QUADS];
	_current_fillter_mode = BLEND_MODE::ALPHA_FILLTER_NORMAL;
#endif
	for (int i = 0; i < MAX_2D_QUADS; i++)
	{
		_pTexID[i] = NULL;
		_pFlags[i] = 0;
#if defined(USE_BLEND_ALPHA)
		_fillter_mode[i] = BLEND_MODE::ALPHA_FILLTER_NORMAL;
#endif
	}
	_nNum2DQuads = 0;
	//OnChangeDevice();
}

void GDirectX11 :: Release()
{

}

void GDirectX11::ChangeContext(GraphicContext context){
	_device_width = context.DeviceSize.W;
	_device_height = context.DeviceSize.H;
	_screen_clip = PineClip(0, 0, _device_width, _device_height);
	_curr_gcontext = context;
	if (context.ComplexMode == PineGraphicComplexOrientation::ComplexNone){
		XMStoreFloat4x4(&_mx_world, 
			XMMatrixMultiply(
				XMMatrixScaling(context.ScaleX, context.ScaleY, 1.0f),
				XMMatrixTranslation(context.TranslateX, context.TranslateY, 0)
			)
		);
	}
	else if (context.ComplexMode == PineGraphicComplexOrientation::ComplexToLeft){
		PDEBUG("cplex mode to left\n");
#if defined(_WINDOWS_PHONE8)
		XMStoreFloat4x4(&_mx_world,

			XMMatrixMultiply(
			XMMatrixMultiply(
			XMMatrixRotationZ(-DirectX::XM_PIDIV2),
			XMMatrixTranslation(0, context.DeviceSize.H / context.ScaleY, 0)
			),
				XMMatrixMultiply(
				XMMatrixScaling(context.ScaleX, context.ScaleY, 1.0f),
					XMMatrixTranslation(context.TranslateY, context.TranslateX, 0)
					
				)
				
			)
			
		);
#elif defined(_WIN8)
		XMStoreFloat4x4(&_mx_world,
			XMMatrixMultiply(
				XMMatrixMultiply(
					XMMatrixRotationZ(-DirectX::XM_PIDIV2),
					XMMatrixTranslation(context.TranslateY * (1.0f/context.ScaleY), context.DeviceSize.H * (1.0f / context.ScaleY), 0)
				),
				XMMatrixScaling(context.ScaleX, context.ScaleY, 1.0f)
			)
		);
#endif
	}
	else if (context.ComplexMode == PineGraphicComplexOrientation::ComplexToRight){
		PDEBUG("cplex mode to right\n");
#if defined(_WINDOWS_PHONE8)
		XMStoreFloat4x4(&_mx_world,
			XMMatrixMultiply(
				XMMatrixMultiply(
				XMMatrixScaling(context.ScaleX, context.ScaleY, 1.0f),
					XMMatrixTranslation(context.TranslateX, context.TranslateY, 0)
					
				),
				XMMatrixMultiply(
					XMMatrixRotationZ(DirectX::XM_PIDIV2),
					XMMatrixTranslation(context.DeviceSize.W, 0, 0)
				)
			)
		);
		/*XMStoreFloat4x4(&_mx_world,
			XMMatrixMultiply(
				XMMatrixMultiply(
					XMMatrixRotationZ(DirectX::XM_PIDIV2),
					XMMatrixTranslation(context.TranslateX * (1.0f / context.ScaleX), context.DeviceSize.W * (1.0f / context.ScaleX), 0)
				),
				XMMatrixScaling(context.ScaleX, context.ScaleY, 1.0f)
			)
		);*/
#elif defined(_WIN8)
		XMStoreFloat4x4(&_mx_world,
			XMMatrixMultiply(
				XMMatrixMultiply(
					XMMatrixRotationZ(DirectX::XM_PIDIV2),
					XMMatrixTranslation(context.DeviceSize.H + context.TranslateY * (1.0f/context.ScaleY), 0, 0)
				)
				,
				//XMMatrixMultiply(
				//	XMMatrixTranslation(context.TranslateX, context.TranslateY, 0),
					XMMatrixScaling(context.ScaleX, context.ScaleY, 1.0f)
				//)
			)
		);
#endif
	}
}
void GDirectX11::BeginTransaction()
{
	_opacity_percent = 100;
	_curr_vertex = 0;
	_nNum2DQuads = 0;
	_nColor2D = 0xFFFFFFFF;
#if defined(USE_GL_ROTATE)
	_is_rotate = false;
#endif
#if defined(USE_TRANSLATE)
	_is_translate = false;
#endif
#if defined(USE_GL_SCALE)
	_is_scale = false;
	_is_global_scale = false;
#endif
#if defined(USE_COLOR_MASK)
	ClearColorMask();
#endif
	if (_curr_device != NULL && _curr_context != NULL){
		setBlendFunction(BLEND_MODE::ALPHA_FILLTER_NORMAL);
	}
	_is_transform = false;
}
void GDirectX11::Flush()
{
	//////////////////////////////////////////////////
	/////FLUSH2D - DRAW IF _nNUM2DQuads > 0
	//////////////////////////////////////////////////
	//printf("plush canvas\n");
	if (_nNum2DQuads>0){
		if (_curr_context == NULL || _curr_device == NULL){
			return;
		}

		unsigned int stride = (VTX_SIZE + COLOR_SIZE + TEXCOOR_SIZE + PARAM_SIZE)* sizeof(float);
		unsigned int offset = 0;
		D3D11_MAPPED_SUBRESOURCE resource;
		HRESULT hResult = _curr_context->Map(_dxVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

		// This will be S_OK
		if (hResult != S_OK){
			printf("here\n");
			return;
		}

		int len = _curr_vertex * (VTX_SIZE + COLOR_SIZE + TEXCOOR_SIZE + PARAM_SIZE);
		float *ptr = (float*)resource.pData;
		for (int i = 0; i < len; i++){
			ptr[i] = _pVtxBuffer2D[i];
		}

		_curr_context->Unmap(_dxVertexBuffer, 0);

		_curr_context->IASetVertexBuffers(0, 1, &_dxVertexBuffer, &stride, &offset);

		UINT32 current_flag = _pFlags[0];
		HTEXTURE current_texture = _pTexID[0];
		INT32 texture_start_index = 0;
		INT32 total_element = 0;
#ifdef USE_BLEND_ALPHA
		BLEND_MODE current_fillter_mode = _fillter_mode[0];
#else
		setBlendFunction(BLEND_MODE::ALPHA_FILLTER_NORMAL);
#endif
		//////////////////////////////////////////////////
		/////FLUSH2D - LOOP TO DRAW
		//////////////////////////////////////////////////
		for (int i = 0; i <= _nNum2DQuads; i++){
			if (current_texture != _pTexID[i] ||
				current_flag != _pFlags[i]
#ifdef USE_GL_ROTATE				
				|| ((current_flag & ROTATE_FLAG) != 0)
#endif
#ifdef USE_GL_SCALE				
				|| ((current_flag & SCALE_FLAG) != 0)
#endif
#ifdef USE_TRANSLATE
				|| ((current_flag & TRANSLATE_FLAG) != 0)
#endif
				)
			{
				if (current_flag&SETCLIP_FLAG){
					float* vtx_pos = &_pVtxBuffer2D[_vertexPos[texture_start_index] * (VTX_SIZE + COLOR_SIZE + TEXCOOR_SIZE + PARAM_SIZE)];
					int x = (int)*vtx_pos++;
					int y = (int)*vtx_pos++;
					int w = (int)*vtx_pos++;
					int h = (int)*vtx_pos++;
					Clip(x, y, w, h);
				}
				else if (current_flag&CLEARCLIP_FLAG){
					ResetClip();
				}
				else{
					total_element = (i - texture_start_index);
					if (total_element > 0){
						if (current_texture != NULL){
							BindTexture(0, current_texture);
						}
						else{
							//printf("GRAPHIC ENGINE ERROR : NOT HAVE TEXTURE\n");
						}

#ifdef USE_BLEND_ALPHA
						if (current_flag&TRANSP_FLAG){
							setBlendFunction(_fillter_mode[i]);
						}
						else{
							setBlendFunction(BLEND_MODE::ALPHA_FILLTER_NORMAL);
						}
#endif
#ifdef USE_DRAW_LINE
						if (current_flag&DRAWLINE_FLAG){
							_curr_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
							_curr_context->Draw(total_element * VTX_COUNT_PER_LINE, _vertexPos[texture_start_index]);
							//Utils::Loga("here:1 :%d\n", (total_element * VTX_COUNT_PER_LINE));
						}
						else
#endif
#ifdef USE_DRAW_TRIANGLE
						if (current_flag&FILLTRIANGLE_FLAG){
							_curr_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
							_curr_context->Draw(total_element * VTX_COUNT_PER_TRIANGLE, _vertexPos[texture_start_index]);
						}
						else
#endif
						{
							_curr_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
							_curr_context->Draw(total_element * TRI_COUNT_PER_QUAD * VTX_COUNT_PER_TRI, _vertexPos[texture_start_index]);
							//Utils::Loga("here:3 :%d\n", (total_element * TRI_COUNT_PER_QUAD * VTX_COUNT_PER_TRI));
						}
					}
				}

				current_flag = _pFlags[i];
				current_texture = _pTexID[i];
				texture_start_index = i;
#ifdef USE_BLEND_ALPHA
				current_fillter_mode = _fillter_mode[i];
#endif
			}
		}
		//////////////////////////////////////////////////
		/////FLUSH2D - DRAW LAST ONE
		//////////////////////////////////////////////////
		//if not set clip, glDrawElements 
		if (current_flag&SETCLIP_FLAG){
			float* vtx_pos = &_pVtxBuffer2D[_vertexPos[texture_start_index] * (VTX_SIZE + COLOR_SIZE + TEXCOOR_SIZE + PARAM_SIZE)];
			int x = (int)*vtx_pos++;
			int y = (int)*vtx_pos++;
			int w = (int)*vtx_pos++;
			int h = (int)*vtx_pos++;
			Clip(x, y, w, h);
		}
		else if (current_flag&CLEARCLIP_FLAG){
			//PineDevice::clearClip();
		}
		else{
			total_element = _nNum2DQuads - texture_start_index;
			if (total_element <= 0){
				return;
			}
			if (current_texture != NULL){
				BindTexture(0, current_texture);
			}
			else{
				//printf("GRAPHIC ENGINE ERROR : NOT HAVE TEXTURE\n");
			}

			
#ifdef USE_BLEND_ALPHA
			if (current_flag&TRANSP_FLAG){
				setBlendFunction(_current_fillter_mode);
			}
			else{
				setBlendFunction(BLEND_MODE::ALPHA_FILLTER_NORMAL);
			}
#endif
#ifdef USE_DRAW_LINE
			if (current_flag&DRAWLINE_FLAG)
			{
				_curr_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ);
				_curr_context->Draw(total_element * VTX_COUNT_PER_LINE, _vertexPos[texture_start_index]);
				//Utils::Loga("here:4\n");
			}
			else

#endif
#ifdef USE_DRAW_TRIANGLE
			if (current_flag&FILLTRIANGLE_FLAG)
			{
				_curr_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				_curr_context->Draw(total_element * VTX_COUNT_PER_TRIANGLE, _vertexPos[texture_start_index]);
				//Utils::Loga("here:5\n");
			}
			else
#endif
			{
				_curr_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				_curr_context->Draw(total_element * TRI_COUNT_PER_QUAD * VTX_COUNT_PER_TRI, _vertexPos[texture_start_index]);
				//Utils::Loga("here:6\n");
			}
		}
	}
}
void GDirectX11::CleanCurrentFlag()
{
	_pFlags[_nNum2DQuads] = 0;
}
//get property
float GDirectX11::GetCurrentScaleX()
{
	return _scale_x;
}
float GDirectX11::GetCurrentScaleY()
{
	return _scale_y;
}
float GDirectX11::GetCurrentCenterScaleX(){
	return _scale_center_x;
}

float GDirectX11::GetCurrentCenterScaleY(){
	return _scale_center_y;
}

int GDirectX11::GetCurrentOpacityPercent()
{
	return _opacity_percent;
}
//function work with cliping
void GDirectX11::SetClip(int x, int y, int w, int h)
{
	_curr_clip.ClipArea.X = x;
	_curr_clip.ClipArea.Y = y;
	_curr_clip.ClipArea.W = w;
	_curr_clip.ClipArea.H = h;

	if (_nNum2DQuads >= (MAX_2D_QUADS - 1)){
		printf("MAX_2D_QUADS OVER FLOW\n");
		return;
	}
	clipx = x;
	clipy = y;
	clipw = w;
	cliph = h;
	_vertexPos[_nNum2DQuads] = _curr_vertex;
	VTXUNIT*	pVtx = _pVtxBuffer2D + _curr_vertex * (VTX_SIZE + COLOR_SIZE + TEXCOOR_SIZE + PARAM_SIZE);
	_curr_vertex += 1;

	_pTexID[_nNum2DQuads] = NULL;
	_pFlags[_nNum2DQuads] = SETCLIP_FLAG;
	*pVtx++ = clipx;		*pVtx++ = clipy;
	*pVtx++ = clipw + 1;	*pVtx++ = cliph + 1;
	_nNum2DQuads++;
}
void GDirectX11::SetClip(PineClip clip){
	SetClip(clip.ClipArea.X, clip.ClipArea.Y, clip.ClipArea.W, clip.ClipArea.H);
}
void GDirectX11::ClearClip()
{
	if (_nNum2DQuads >= (MAX_2D_QUADS - 1)){
		printf("MAX_2D_QUADS OVER FLOW\n");
		return;
	}
	_pTexID[_nNum2DQuads] = NULL;
	_pFlags[_nNum2DQuads] = CLEARCLIP_FLAG;
	_nNum2DQuads++;
}
PineClip GDirectX11::GetCurrentClip(){
	return _curr_clip;
}
void GDirectX11::Clip(int x, int y, int w, int h)
{
#if defined(_WINDOWS_PHONE8)
	
	/*x *= _curr_gcontext.ScaleX;
	y *= _curr_gcontext.ScaleY;
	w *= _curr_gcontext.ScaleX;
	h *= _curr_gcontext.ScaleY;
	int tmp;
	switch (_curr_gcontext.ComplexMode)
	{
	case PineGraphicComplexOrientation::ComplexNone:
		//y = _curr_gcontext.DeviceSize.H - y - h;
		break;

	case PineGraphicComplexOrientation::ComplexToLeft:
		tmp = y;
		y = _curr_gcontext.DeviceSize.H - x - w;
		x = tmp;
		

		tmp = w;
		w = h;
		h = tmp;

		break;

	case PineGraphicComplexOrientation::ComplexToRight:
		//printf("set clip %d %d %d %d", x, y, w, h);
		//tmp = y;
		//y = _curr_gcontext.DeviceSize.W - x - w;
		//x = _curr_gcontext.DeviceSize.H - tmp - h;

		tmp = x;
		x = _curr_gcontext.DeviceSize.W - y - h;
		y = tmp;

		tmp = w;
		w = h;
		h = tmp;

		break;
	}
	
	x += _curr_gcontext.TranslateX;
	y += _curr_gcontext.TranslateY;
	PineClip clip = _screen_clip.GetSubClip(x, y, w, h);
	D3D11_RECT rect[1];
	rect[0].top = clip.ClipArea.Y;
	rect[0].left = clip.ClipArea.X;
	rect[0].right = clip.ClipArea.X + clip.ClipArea.W;
	rect[0].bottom = clip.ClipArea.Y + clip.ClipArea.H;
	_curr_context->RSSetScissorRects(1, rect);
	*/
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
		break;

	case PineGraphicComplexOrientation::ComplexToRight:
		tmp = cy;
		cy = cx;
		cx = _curr_gcontext.DeviceSize.W - tmp - ch;

		tmp = cw;
		cw = ch;
		ch = tmp;
		break;

	case PineGraphicComplexOrientation::ComplexToLeft:
		tmp = cy;
		cy = _curr_gcontext.DeviceSize.H - cx - cw;
		cx = tmp;

		tmp = cw;
		cw = ch;
		ch = tmp;
		break;
	}
	
	D3D11_RECT rect[1];
	if (_curr_gcontext.ClipFixContext){
		PineClip clip = _screen_clip.GetSubClip(cx, cy, cw, ch);
		rect[0].top = clip.ClipArea.Y;
		rect[0].left = clip.ClipArea.X;
		rect[0].right = clip.ClipArea.X + clip.ClipArea.W;
		rect[0].bottom = clip.ClipArea.Y + clip.ClipArea.H;
	}
	else{
		rect[0].top = cy;
		rect[0].left = cx;
		rect[0].right = cx + cw;
		rect[0].bottom = cy + ch;
	}

	_curr_context->RSSetScissorRects(1, rect);
	
#elif defined(_WIN8)
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
		break;

	case PineGraphicComplexOrientation::ComplexToRight:
		tmp = cy;
		cy = cx;
		cx = tmp;

		tmp = cw;
		cw = ch;
		ch = tmp;
		break;

	case PineGraphicComplexOrientation::ComplexToLeft:
		tmp = cy;
		cy = _curr_gcontext.DeviceSize.H - cx - cw;
		cx = tmp;

		tmp = cw;
		cw = ch;
		ch = tmp;
		break;
	}
	D3D11_RECT rect[1];
	if (_curr_gcontext.ClipFixContext){
		PineClip clip = _screen_clip.GetSubClip(cx, cy, cw, ch);
		rect[0].top = clip.ClipArea.Y;
		rect[0].left = clip.ClipArea.X;
		rect[0].right = clip.ClipArea.X + clip.ClipArea.W;
		rect[0].bottom = clip.ClipArea.Y + clip.ClipArea.H;
	}
	else{
		rect[0].top = cx;
		rect[0].left = cy;
		rect[0].right = cx + cw;
		rect[0].bottom = cy + ch;
	}
	
	_curr_context->RSSetScissorRects(1, rect);
#endif
}
void GDirectX11::ResetClip(){
	if (_curr_context == NULL || _device_width == 0 || _device_height == 0) return;
	D3D11_RECT rect[1];
	rect[0].top = _screen_clip.ClipArea.Y;
	rect[0].left = _screen_clip.ClipArea.X;
	rect[0].right = _screen_clip.ClipArea.X + _screen_clip.ClipArea.W;
	rect[0].bottom = _screen_clip.ClipArea.Y + _screen_clip.ClipArea.H;
	_curr_context->RSSetScissorRects(1, rect);
}

void GDirectX11::BindTexture(int slot, HTEXTURE tex){
	if (_curr_context == NULL || _curr_device == NULL) return;
	ID3D11ShaderResourceView *texv = NULL;
	if (tex == NULL)return;
	try{
		HRESULT rs = (_curr_device->CreateShaderResourceView(tex, NULL, &texv));
		if (rs != S_OK)
		{
			return;
		}
		_curr_context->PSSetShaderResources(slot, 1, &texv);
		texv->Release();
	}
	catch (...){
	
	}
}

//function work with color
void GDirectX11::SetColor(COLOR clr)
{
	_nColor2D = clr;
}
#if defined(USE_COLOR_MASK)
COLOR GDirectX11::GetColorMask(){
	return _nColorMask;
}
void GDirectX11::SetColorMask(COLOR clr)
{
	_is_color_mask = true;
	_nColorMask = clr;
}
void GDirectX11::ClearColorMask()
{
	_is_color_mask = false;
}
#endif
void GDirectX11::SetGlobalContext(float tx, float ty, float sx, float sy){
	
}
//function work with opacity
#if defined(ENABLE_OPACITY_SETTING)
void GDirectX11::SetOpacity(int percent)
{
	if (percent < 0) percent = 0;
	if (percent > 100) percent = 100;
	_opacity_percent = percent;
}
int GDirectX11::GetCurrentOpacity()
{
	return _opacity_percent;
}
#if defined(USE_BLEND_MODE)
void GDirectX11::SetFillterMode(BLEND_MODE fillter_mode)
{
#if defined(USE_BLEND_ALPHA)
	_current_fillter_mode = fillter_mode;
	//_current_fillter_mode = BLEND_MODE::ALPHA_FILLTER_NORMAL;
#endif
}
#endif
#endif


//function work with line
#if defined(USE_DRAW_LINE)
void GDirectX11::DrawLine(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, bool enableAlpha)
{
	if (_nNum2DQuads >= (MAX_2D_QUADS - 1)){
		printf("MAX_2D_QUADS OVER FLOW\n");
		return;
	}

	_vertexPos[_nNum2DQuads] = _curr_vertex;
	VTXUNIT*	pVtx = _pVtxBuffer2D + _curr_vertex * (VTX_SIZE + COLOR_SIZE + TEXCOOR_SIZE + PARAM_SIZE);
	_curr_vertex += 2;
	XMVECTOR x0y0 = XMVectorSet(x1, y1, 0.0f, 0.0f);
	XMVECTOR x1y1 = XMVectorSet(x2, y2, 0.0f, 0.0f);

	UINT8	a = (UINT8)((_nColor2D >> 24) & 0xFF);
	UINT8	r = (_nColor2D >> 16) & 0xFF;
	UINT8	g = (_nColor2D >> 8) & 0xFF;
	UINT8	b = _nColor2D & 0xFF;

#ifdef ENABLE_OPACITY_SETTING
	if (enableAlpha){
		a = a* ((float)_opacity_percent / (float)100);
	}
	else{
		a = 255 * ((float)_opacity_percent / (float)100);
	}
#ifdef USE_BLEND_ALPHA
	_fillter_mode[_nNum2DQuads] = _current_fillter_mode;
#endif
#endif
	//a = 0xFF;

	_pTexID[_nNum2DQuads] = NULL;
	_pFlags[_nNum2DQuads] = DRAWLINE_FLAG;
	if (enableAlpha
#ifdef ENABLE_OPACITY_SETTING
		|| _opacity_percent < 100
#endif
		)
	{
		_pFlags[_nNum2DQuads] |= TRANSP_FLAG;
	}
#ifdef USE_TRANSLATE
	if (_is_translate){
		const XMMATRIX mx_translate = XMLoadFloat4x4(&_mx_translate);
		x0y0 = XMVector3Transform(x0y0, mx_translate);
		x1y1 = XMVector3Transform(x1y1, mx_translate);
	}
#endif
#ifdef USE_GL_ROTATE
	if (_is_rotate){
		const XMVECTOR vector_rotate = XMLoadFloat4(&_vector_rotate);
		const XMMATRIX mx_rotate_fre_translate = XMLoadFloat4x4(&_mx_rotate_fre_translate);
		const XMMATRIX mx_rotate_post_translate = XMLoadFloat4x4(&_mx_rotate_post_translate);
		x0y0 = XMVector3Transform(x0y0, mx_rotate_fre_translate);
		x1y1 = XMVector3Transform(x1y1, mx_rotate_fre_translate);

		x0y0 = XMVector3Rotate(x0y0, vector_rotate);
		x1y1 = XMVector3Rotate(x1y1, vector_rotate);

		x0y0 = XMVector3Transform(x0y0, mx_rotate_post_translate);
		x1y1 = XMVector3Transform(x1y1, mx_rotate_post_translate);
	}
#endif


	*pVtx++ = XMVectorGetX(x0y0);		*pVtx++ = XMVectorGetY(x0y0);		*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = a;
	*pVtx++ = 0.0f;	*pVtx++ = 0.0f;
	*pVtx++ = 0.0f;

	*pVtx++ = XMVectorGetX(x1y1);		*pVtx++ = XMVectorGetY(x1y1);		*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = a;
	*pVtx++ = 0.0f;	*pVtx++ = 0.0f;
	*pVtx++ = 0.0f;
	_nNum2DQuads++;
}
void GDirectX11::DrawLine(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, COLOR color1, COLOR color2)
{
	if (_nNum2DQuads >= (MAX_2D_QUADS - 1)){
		printf("MAX_2D_QUADS OVER FLOW\n");
		return;
	}
	_vertexPos[_nNum2DQuads] = _curr_vertex;
	VTXUNIT*	pVtx = _pVtxBuffer2D + _curr_vertex * (VTX_SIZE + COLOR_SIZE + TEXCOOR_SIZE + PARAM_SIZE);
	_curr_vertex += 2;
	XMVECTOR x0y0 = XMVectorSet(x1, y1, 0.0f, 0.0f);
	XMVECTOR x1y1 = XMVectorSet(x2, y2, 0.0f, 0.0f);

	UINT8	a1 = (UINT8)((color1 >> 24) & 0xFF);
	UINT8	r1 = (color1 >> 16) & 0xFF;
	UINT8	g1 = (color1 >> 8) & 0xFF;
	UINT8	b1 = color1 & 0xFF;

	UINT8	a2 = (UINT8)((color2 >> 24) & 0xFF);
	UINT8	r2 = (color2 >> 16) & 0xFF;
	UINT8	g2 = (color2 >> 8) & 0xFF;
	UINT8	b2 = color2 & 0xFF;

	_pTexID[_nNum2DQuads] = NULL;
	_pFlags[_nNum2DQuads] = DRAWLINE_FLAG;
	//if (enableAlpha){
	//	_pFlags[_nNum2DQuads] |= TRANSP_FLAG;
#ifdef USE_BLEND_ALPHA
		_fillter_mode[_nNum2DQuads] = _current_fillter_mode;
#endif
	//}
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
#ifdef USE_TRANSLATE
	if (_is_translate){
		const XMMATRIX mx_translate = XMLoadFloat4x4(&_mx_translate);
		x0y0 = XMVector3Transform(x0y0, mx_translate);
		x1y1 = XMVector3Transform(x1y1, mx_translate);
	}
#endif
#ifdef USE_GL_ROTATE
	if (_is_rotate){
		const XMVECTOR vector_rotate = XMLoadFloat4(&_vector_rotate);
		const XMMATRIX mx_rotate_fre_translate = XMLoadFloat4x4(&_mx_rotate_fre_translate);
		const XMMATRIX mx_rotate_post_translate = XMLoadFloat4x4(&_mx_rotate_post_translate);
		x0y0 = XMVector3Transform(x0y0, mx_rotate_fre_translate);
		x1y1 = XMVector3Transform(x1y1, mx_rotate_fre_translate);

		x0y0 = XMVector3Rotate(x0y0, vector_rotate);
		x1y1 = XMVector3Rotate(x1y1, vector_rotate);

		x0y0 = XMVector3Transform(x0y0, mx_rotate_post_translate);
		x1y1 = XMVector3Transform(x1y1, mx_rotate_post_translate);
	}
#endif

	*pVtx++ = XMVectorGetX(x0y0);		*pVtx++ = XMVectorGetY(x0y0);		*pVtx++ = 0.0f;
	*pVtx++ = r1;		*pVtx++ = g1;	*pVtx++ = b1;	*pVtx++ = a1;
	*pVtx++ = 0.0f;	*pVtx++ = 0.0f;
	*pVtx++ = 0.0f;

	*pVtx++ = XMVectorGetX(x1y1);		*pVtx++ = XMVectorGetY(x1y1);		*pVtx++ = 0.0f;
	*pVtx++ = r2;		*pVtx++ = g2;	*pVtx++ = b2;	*pVtx++ = a2;
	*pVtx++ = 0.0f;	*pVtx++ = 0.0f;
	*pVtx++ = 0.0f;
	_nNum2DQuads++;
}
#if defined(USE_DRAW_TRIANGLE)
void GDirectX11::DrawTriangle(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, VTXUNIT x3, VTXUNIT y3, bool enableAlpha)
{
	DrawLine(x1, y1, x2, y2, enableAlpha);
	DrawLine(x2, y2, x3, y3, enableAlpha);
	DrawLine(x3, y3, x1, y1, enableAlpha);
}
#if defined(USE_DRAW_CIRCLE)
void GDirectX11::DrawCircle(VTXUNIT x, VTXUNIT y, float radius, int density, bool enableAlpha){
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
void GDirectX11::DrawPolygon(PinePolygon polygon, bool enableAlpha){
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

//function work with triangle
#if defined(USE_DRAW_TRIANGLE)
void GDirectX11::FillTriangle(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, VTXUNIT x3, VTXUNIT y3, bool enableAlpha)
{
	if (_nNum2DQuads >= (MAX_2D_QUADS - 1)){
		return; //return is better
	}
	_vertexPos[_nNum2DQuads] = _curr_vertex;
	VTXUNIT*	pVtx = _pVtxBuffer2D + _curr_vertex * (VTX_SIZE + COLOR_SIZE + TEXCOOR_SIZE + PARAM_SIZE);
	_curr_vertex += 3;

	XMVECTOR x0y0 = XMVectorSet(x1, y1, 0.0f, 0.0f);
	XMVECTOR x1y1 = XMVectorSet(x2, y2, 0.0f, 0.0f);
	XMVECTOR x2y2 = XMVectorSet(x3, y3, 0.0f, 0.0f);

	UINT8	a = (UINT8)((_nColor2D >> 24) & 0xFF);
	UINT8	r = (_nColor2D >> 16) & 0xFF;
	UINT8	g = (_nColor2D >> 8) & 0xFF;
	UINT8	b = _nColor2D & 0xFF;

	_pTexID[_nNum2DQuads] = NULL;
	_pFlags[_nNum2DQuads] = FILLTRIANGLE_FLAG;
	if (enableAlpha)
	{
		_pFlags[_nNum2DQuads] |= TRANSP_FLAG;
	}
	if (enableAlpha){
		_pFlags[_nNum2DQuads] |= TRANSP_FLAG;
#ifdef USE_BLEND_ALPHA
		_fillter_mode[_nNum2DQuads] = _current_fillter_mode;
#endif
	}
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
#ifdef USE_TRANSLATE
	if (_is_translate){
		const XMMATRIX mx_translate = XMLoadFloat4x4(&_mx_translate);
		x0y0 = XMVector3Transform(x0y0, mx_translate);
		x1y1 = XMVector3Transform(x1y1, mx_translate);
		x2y2 = XMVector3Transform(x1y1, mx_translate);
	}
#endif
#ifdef USE_GL_ROTATE
	if (_is_rotate){
		const XMVECTOR vector_rotate = XMLoadFloat4(&_vector_rotate);
		const XMMATRIX mx_rotate_fre_translate = XMLoadFloat4x4(&_mx_rotate_fre_translate);
		const XMMATRIX mx_rotate_post_translate = XMLoadFloat4x4(&_mx_rotate_post_translate);
		x0y0 = XMVector3Transform(x0y0, mx_rotate_fre_translate);
		x1y1 = XMVector3Transform(x1y1, mx_rotate_fre_translate);
		x2y2 = XMVector3Transform(x1y1, mx_rotate_fre_translate);

		x0y0 = XMVector3Rotate(x0y0, vector_rotate);
		x1y1 = XMVector3Rotate(x1y1, vector_rotate);
		x2y2 = XMVector3Rotate(x1y1, vector_rotate);

		x0y0 = XMVector3Transform(x0y0, mx_rotate_post_translate);
		x1y1 = XMVector3Transform(x1y1, mx_rotate_post_translate);
		x2y2 = XMVector3Transform(x1y1, mx_rotate_post_translate);
	}
#endif
	*pVtx++ = XMVectorGetX(x0y0);		*pVtx++ = XMVectorGetY(x0y0);		*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = a;
	*pVtx++ = 0.0f;	*pVtx++ = 0.0f;
	*pVtx++ = 0.0f;

	*pVtx++ = XMVectorGetX(x1y1);		*pVtx++ = XMVectorGetY(x1y1);		*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = a;
	*pVtx++ = 0.0f;	*pVtx++ = 0.0f;
	*pVtx++ = 0.0f;

	*pVtx++ = XMVectorGetX(x2y2);		*pVtx++ = XMVectorGetY(x2y2);		*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = a;
	*pVtx++ = 0.0f;	*pVtx++ = 0.0f;
	*pVtx++ = 0.0f;
	_nNum2DQuads++;
}
#if defined(USE_DRAW_CIRCLE)
void GDirectX11::FillCircle(VTXUNIT x, VTXUNIT y, float radius, int density, bool enableAlpha){
	if (density < 1)return;
	VTXUNIT xx1 = x + (VTXUNIT)(0 * radius);
	VTXUNIT yy1 = y + (VTXUNIT)(1 * radius);
	for (int angle = 0; angle <= 360; angle += (360 / ((4 * density) - 1)))
	{
		VTXUNIT xx = x + (VTXUNIT)(sin((double)angle*3.1416f / 180.0f) * radius);
		VTXUNIT yy = y + (VTXUNIT)(cos((double)angle*3.1416f / 180.0f) * radius);

		FillTriangle(xx1, yy1, xx, yy,x,y, enableAlpha);
		xx1 = xx;
		yy1 = yy;
	}
	FillTriangle(xx1, yy1, x + (VTXUNIT)(0 * radius), y + (VTXUNIT)(1 * radius), x, y, enableAlpha);
}
#endif
void GDirectX11::FillTriangle(VTXUNIT x1, VTXUNIT y1, VTXUNIT x2, VTXUNIT y2, VTXUNIT x3, VTXUNIT y3, HTEXTURE tex_id, float u1, float v1, float u2, float v2, float u3, float v3, bool enableAnpha) {

}
void GDirectX11::FillPolygon(PinePolygon polygon, bool enableAlpha) {

}
void GDirectX11::FillPolygon(PinePolygon polygon, HTEXTURE texId, float tw, float th, float tx, float ty, bool enableAlpha) {

}
#endif

//function work with rectangle
#if defined(USE_DRAW_LINE)
void GDirectX11::DrawRect(VTXUNIT x, VTXUNIT y, VTXUNIT w, VTXUNIT h, bool enableAlpha)
{
	DrawLine(x, y, x, y + h, enableAlpha);
	DrawLine(x, y + h, x + w, y + h, enableAlpha);
	DrawLine(x + w, y + h, x + w, y, enableAlpha);
	DrawLine(x + w, y, x, y, enableAlpha);
}
#endif
void GDirectX11::FillRect(VTXUNIT x, VTXUNIT y, VTXUNIT w, VTXUNIT h, bool enableAlpha)
{
	if (_nNum2DQuads >= (MAX_2D_QUADS - 1)){
		printf("MAX_2D_QUADS OVER FLOW\n");
		return;
	}

	_vertexPos[_nNum2DQuads] = _curr_vertex;
	VTXUNIT*	pVtx = _pVtxBuffer2D + _curr_vertex * (VTX_SIZE + COLOR_SIZE + TEXCOOR_SIZE + PARAM_SIZE);
	_curr_vertex += 6;
	XMVECTOR x0y0 = XMVectorSet(x, y, 0.0f, 0.0f);
	XMVECTOR x0y1 = XMVectorSet(x, y + h, 0.0f, 0.0f);
	XMVECTOR x1y1 = XMVectorSet(x + w, y + h, 0.0f, 0.0f);
	XMVECTOR x1y0 = XMVectorSet(x + w, y, 0.0f, 0.0f);


	UINT8	a = (UINT8)((_nColor2D >> 24) & 0xFF);
	UINT8	r = (_nColor2D >> 16) & 0xFF;
	UINT8	g = (_nColor2D >> 8) & 0xFF;
	UINT8	b = _nColor2D & 0xFF;

	_pTexID[_nNum2DQuads] = NULL;
	_pFlags[_nNum2DQuads] = 0;
	if (enableAlpha){
		_pFlags[_nNum2DQuads] = TRANSP_FLAG;
#ifdef USE_BLEND_ALPHA
		_fillter_mode[_nNum2DQuads] = _current_fillter_mode;
#endif
	}

#if defined(USE_FREE_TRANSFORM)
	if (_is_transform){
		ALIGN_16 XMMATRIX mxmodel = XMMATRIX(
			_transform_param.m11, _transform_param.m12, 0.0f, 0.0f,
			_transform_param.m21, _transform_param.m22, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			_transform_param.TranslateX, _transform_param.TranslateY, 0.0f, 1.0f
			);
		x0y0 = XMVector3Transform(x0y0, (const XMMATRIX)mxmodel);
		x0y1 = XMVector3Transform(x0y1, (const XMMATRIX)mxmodel);
		x1y1 = XMVector3Transform(x1y1, (const XMMATRIX)mxmodel);
		x1y0 = XMVector3Transform(x1y0, (const XMMATRIX)mxmodel);
	}
#endif

#ifdef USE_TRANSLATE
	if (_is_translate){
		const XMMATRIX mx_translate = XMLoadFloat4x4(&_mx_translate);
		x0y0 = XMVector3Transform(x0y0, mx_translate);
		x0y1 = XMVector3Transform(x0y1, mx_translate);
		x1y1 = XMVector3Transform(x1y1, mx_translate);
		x1y0 = XMVector3Transform(x1y0, mx_translate);
	}
#endif

#ifdef USE_GL_ROTATE
	if (_is_rotate){
		const XMVECTOR vector_rotate = XMLoadFloat4(&_vector_rotate);
		const XMMATRIX mx_rotate_fre_translate = XMLoadFloat4x4(&_mx_rotate_fre_translate);
		const XMMATRIX mx_rotate_post_translate = XMLoadFloat4x4(&_mx_rotate_post_translate);
		x0y0 = XMVector3Transform(x0y0, mx_rotate_fre_translate);
		x0y1 = XMVector3Transform(x0y1, mx_rotate_fre_translate);
		x1y1 = XMVector3Transform(x1y1, mx_rotate_fre_translate);
		x1y0 = XMVector3Transform(x1y0, mx_rotate_fre_translate);

		x0y0 = XMVector3Rotate(x0y0, vector_rotate);
		x0y1 = XMVector3Rotate(x0y1, vector_rotate);
		x1y1 = XMVector3Rotate(x1y1, vector_rotate);
		x1y0 = XMVector3Rotate(x1y0, vector_rotate);

		x0y0 = XMVector3Transform(x0y0, mx_rotate_post_translate);
		x0y1 = XMVector3Transform(x0y1, mx_rotate_post_translate);
		x1y1 = XMVector3Transform(x1y1, mx_rotate_post_translate);
		x1y0 = XMVector3Transform(x1y0, mx_rotate_post_translate);
	}
#endif





#ifdef USE_GL_SCALE
	if (_is_scale){
		const XMMATRIX mx_scale_fre_translate = XMLoadFloat4x4(&_mx_scale_fre_translate);
		const XMMATRIX mx_scale = XMLoadFloat4x4(&_mx_scale);
		const XMMATRIX mx_scale_post_translate = XMLoadFloat4x4(&_mx_scale_post_translate);
		x0y0 = XMVector3Transform(x0y0, mx_scale_fre_translate);
		x0y1 = XMVector3Transform(x0y1, mx_scale_fre_translate);
		x1y1 = XMVector3Transform(x1y1, mx_scale_fre_translate);
		x1y0 = XMVector3Transform(x1y0, mx_scale_fre_translate);

		x0y0 = XMVector3Transform(x0y0, mx_scale);
		x0y1 = XMVector3Transform(x0y1, mx_scale);
		x1y1 = XMVector3Transform(x1y1, mx_scale);
		x1y0 = XMVector3Transform(x1y0, mx_scale);

		x0y0 = XMVector3Transform(x0y0, mx_scale_fre_translate);
		x0y1 = XMVector3Transform(x0y1, mx_scale_fre_translate);
		x1y1 = XMVector3Transform(x1y1, mx_scale_fre_translate);
		x1y0 = XMVector3Transform(x1y0, mx_scale_fre_translate);
	}
	/*if (_is_global_scale){
		x0y0 = XMVector3Transform(x0y0, _mx_global_scale_fre_translate);
		x0y1 = XMVector3Transform(x0y1, _mx_global_scale_fre_translate);
		x1y1 = XMVector3Transform(x1y1, _mx_global_scale_fre_translate);
		x1y0 = XMVector3Transform(x1y0, _mx_global_scale_fre_translate);

		x0y0 = XMVector3Transform(x0y0, _mx_global_scale);
		x0y1 = XMVector3Transform(x0y1, _mx_global_scale);
		x1y1 = XMVector3Transform(x1y1, _mx_global_scale);
		x1y0 = XMVector3Transform(x1y0, _mx_global_scale);

		x0y0 = XMVector3Transform(x0y0, _mx_global_scale_post_translate);
		x0y1 = XMVector3Transform(x0y1, _mx_global_scale_post_translate);
		x1y1 = XMVector3Transform(x1y1, _mx_global_scale_post_translate);
		x1y0 = XMVector3Transform(x1y0, _mx_global_scale_post_translate);
	}*/
#endif

	*pVtx++ = XMVectorGetX(x0y0);		*pVtx++ = XMVectorGetY(x0y0);		*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;		*pVtx++ = b;		*pVtx++ = a;
	*pVtx++ = 0.0f;	*pVtx++ = 0.0f;
	*pVtx++ = 0.0f;

	*pVtx++ = XMVectorGetX(x1y0);	*pVtx++ = XMVectorGetY(x1y0);		*pVtx++ = 0.0f;;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = a;
	*pVtx++ = 0.0f;	*pVtx++ = 0.0f;
	*pVtx++ = 0.0f;

	*pVtx++ = XMVectorGetX(x0y1);		*pVtx++ = XMVectorGetY(x0y1);		*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = a;
	*pVtx++ = 0.0f;	*pVtx++ = 0.0f;
	*pVtx++ = 0.0f;

	*pVtx++ = XMVectorGetX(x0y1);		*pVtx++ = XMVectorGetY(x0y1);		*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = a;
	*pVtx++ = 0.0f;	*pVtx++ = 0.0f;
	*pVtx++ = 0.0f;

	*pVtx++ = XMVectorGetX(x1y0);	*pVtx++ = XMVectorGetY(x1y0);			*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = a;
	*pVtx++ = 0.0f;	*pVtx++ = 0.0f;
	*pVtx++ = 0.0f;

	*pVtx++ = XMVectorGetX(x1y1);	*pVtx++ = XMVectorGetY(x1y1);			*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = a;
	*pVtx++ = 0.0f;	*pVtx++ = 0.0f;
	*pVtx++ = 0.0f;

	_nNum2DQuads++;
}
void GDirectX11::FillFullScreen(bool enableAlpha){
	if (_nNum2DQuads >= (MAX_2D_QUADS - 1)){
		printf("MAX_2D_QUADS OVER FLOW\n");
		return;
	}
	
	_vertexPos[_nNum2DQuads] = _curr_vertex;
	VTXUNIT*	pVtx = _pVtxBuffer2D + _curr_vertex * (VTX_SIZE + COLOR_SIZE + TEXCOOR_SIZE + PARAM_SIZE);
	_curr_vertex += 6;
	float x = -(_curr_gcontext.TranslateX / _curr_gcontext.ScaleX);
	float y = -(_curr_gcontext.TranslateY / _curr_gcontext.ScaleY);
	float w = _device_width / _curr_gcontext.ScaleX;// (_device_width + 2 * _curr_gcontext.TranslateX) / _curr_gcontext.ScaleX;
	float h = _device_height / _curr_gcontext.ScaleY;// (_device_height + 2 * _curr_gcontext.TranslateY) / _curr_gcontext.ScaleY;
	//x -= 1;
	//y -= 1;
	//w += 2;
	//h += 2;
	if (_curr_gcontext.ComplexMode != PineGraphicComplexOrientation::ComplexNone){

		float tmp = h;
		h = w; 
		w = tmp;
	}
	XMVECTOR x0y0 = XMVectorSet(x, y, 0.0f, 0.0f);
	XMVECTOR x0y1 = XMVectorSet(x, y + h, 0.0f, 0.0f);
	XMVECTOR x1y1 = XMVectorSet(x + w, y + h, 0.0f, 0.0f);
	XMVECTOR x1y0 = XMVectorSet(x + w, y, 0.0f, 0.0f);


	UINT8	a = (UINT8)((_nColor2D >> 24) & 0xFF);
	UINT8	r = (_nColor2D >> 16) & 0xFF;
	UINT8	g = (_nColor2D >> 8) & 0xFF;
	UINT8	b = _nColor2D & 0xFF;

	_pTexID[_nNum2DQuads] = NULL;
	_pFlags[_nNum2DQuads] = 0;
	if (enableAlpha){
		_pFlags[_nNum2DQuads] = TRANSP_FLAG;
#ifdef USE_BLEND_ALPHA
		_fillter_mode[_nNum2DQuads] = _current_fillter_mode;
#endif
	}

	*pVtx++ = XMVectorGetX(x0y0);		*pVtx++ = XMVectorGetY(x0y0);		*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;		*pVtx++ = b;		*pVtx++ = a;
	*pVtx++ = 0.0f;	*pVtx++ = 0.0f;
	*pVtx++ = 0.0f;

	*pVtx++ = XMVectorGetX(x1y0);	*pVtx++ = XMVectorGetY(x1y0);		*pVtx++ = 0.0f;;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = a;
	*pVtx++ = 0.0f;	*pVtx++ = 0.0f;
	*pVtx++ = 0.0f;

	*pVtx++ = XMVectorGetX(x0y1);		*pVtx++ = XMVectorGetY(x0y1);		*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = a;
	*pVtx++ = 0.0f;	*pVtx++ = 0.0f;
	*pVtx++ = 0.0f;

	*pVtx++ = XMVectorGetX(x0y1);		*pVtx++ = XMVectorGetY(x0y1);		*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = a;
	*pVtx++ = 0.0f;	*pVtx++ = 0.0f;
	*pVtx++ = 0.0f;

	*pVtx++ = XMVectorGetX(x1y0);	*pVtx++ = XMVectorGetY(x1y0);			*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = a;
	*pVtx++ = 0.0f;	*pVtx++ = 0.0f;
	*pVtx++ = 0.0f;

	*pVtx++ = XMVectorGetX(x1y1);	*pVtx++ = XMVectorGetY(x1y1);			*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = a;
	*pVtx++ = 0.0f;	*pVtx++ = 0.0f;
	*pVtx++ = 0.0f;

	_nNum2DQuads++;
}
//function work with texture
void GDirectX11::Draw2DTexture(VTXUNIT x, VTXUNIT y, int w, int h, HTEXTURE texId, float uv[], int flags, bool enableAlpha)
{
	if (_nNum2DQuads >= (MAX_2D_QUADS - 1)){
		printf("MAX_2D_QUADS OVER FLOW\n");
		return;
	}
	_vertexPos[_nNum2DQuads] = _curr_vertex;
	VTXUNIT*	pVtx = _pVtxBuffer2D + _curr_vertex * (VTX_SIZE + COLOR_SIZE + TEXCOOR_SIZE + PARAM_SIZE);
	_curr_vertex += 6;
	ALIGN_16 XMVECTOR x0y0 = XMVectorSet(x, y, 0.0f, 0.0f);
	ALIGN_16 XMVECTOR x0y1 = XMVectorSet(x, y + h, 0.0f, 0.0f);
	ALIGN_16 XMVECTOR x1y1 = XMVectorSet(x + w, y + h, 0.0f, 0.0f);
	ALIGN_16 XMVECTOR x1y0 = XMVectorSet(x + w, y, 0.0f, 0.0f);
	if (texId == 0){
		printf("TEXTURE NULL\n");
	}
	_pTexID[_nNum2DQuads] =  texId;

	if (enableAlpha){
		_pFlags[_nNum2DQuads] |= TRANSP_FLAG;
#ifdef USE_BLEND_ALPHA
		_fillter_mode[_nNum2DQuads] = _current_fillter_mode;
#endif
	}

#if defined(USE_FREE_TRANSFORM)
	if ((_pFlags[_nNum2DQuads] & FREE_TRANSFORM_FLAG) != 0){
		float*	pModel = _pModBuffer2D + (MODEL_SIZE * MODEL_COUNT_PER_QUAD * _nNum2DQuads);
		ALIGN_16 XMMATRIX mxmodel = XMMATRIX(
			*(pModel + 2), *(pModel + 3), 0.0f, 0.0f,
			*(pModel + 4), *(pModel + 5), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			(*(pModel + 0)), *(pModel + 1), 0.0f, 1.0f
			);
		x0y0 = XMVector3Transform(x0y0, (const XMMATRIX)mxmodel);
		x0y1 = XMVector3Transform(x0y1, (const XMMATRIX)mxmodel);
		x1y1 = XMVector3Transform(x1y1, (const XMMATRIX)mxmodel);
		x1y0 = XMVector3Transform(x1y0, (const XMMATRIX)mxmodel);
		//Utils::Loga("mul model matrix: %0.2f %0.2f %0.2f %0.2f %0.2f %0.2f\n", *(pModel),*(pModel + 1),*(pModel + 2),*(pModel + 3),*(pModel + 4),*(pModel + 5));
	}
	else if(_is_transform){
		ALIGN_16 XMMATRIX mxmodel = XMMATRIX(
			_transform_param.m11,		_transform_param.m12, 0.0f, 0.0f,
			_transform_param.m21,		_transform_param.m22, 0.0f, 0.0f,
			0.0f,						0.0f,			1.0f,			0.0f,
			_transform_param.TranslateX, _transform_param.TranslateY,	0.0f, 1.0f
			);
		x0y0 = XMVector3Transform(x0y0, (const XMMATRIX)mxmodel);
		x0y1 = XMVector3Transform(x0y1, (const XMMATRIX)mxmodel);
		x1y1 = XMVector3Transform(x1y1, (const XMMATRIX)mxmodel);
		x1y0 = XMVector3Transform(x1y0, (const XMMATRIX)mxmodel);
	}
#endif
	
	
	
#ifdef USE_TRANSLATE
	if (_is_translate){
		const XMMATRIX mx_translate = XMLoadFloat4x4(&_mx_translate);
		x0y0 = XMVector3Transform((const XMVECTOR)x0y0, mx_translate);
		x0y1 = XMVector3Transform((const XMVECTOR)x0y1, mx_translate);
		x1y1 = XMVector3Transform((const XMVECTOR)x1y1, mx_translate);
		x1y0 = XMVector3Transform((const XMVECTOR)x1y0, mx_translate);
	}
#endif
#ifdef USE_GL_SCALE
	if (_is_scale){
		const XMMATRIX mx_scale_fre_translate = XMLoadFloat4x4(&_mx_scale_fre_translate);
		const XMMATRIX mx_scale = XMLoadFloat4x4(&_mx_scale);
		const XMMATRIX mx_scale_post_translate = XMLoadFloat4x4(&_mx_scale_post_translate);
		x0y0 = XMVector3Transform((const XMVECTOR)x0y0, mx_scale_fre_translate);
		x0y1 = XMVector3Transform((const XMVECTOR)x0y1, mx_scale_fre_translate);
		x1y1 = XMVector3Transform((const XMVECTOR)x1y1, mx_scale_fre_translate);
		x1y0 = XMVector3Transform((const XMVECTOR)x1y0, mx_scale_fre_translate);

		x0y0 = XMVector3Transform((const XMVECTOR)x0y0, mx_scale);
		x0y1 = XMVector3Transform((const XMVECTOR)x0y1, mx_scale);
		x1y1 = XMVector3Transform((const XMVECTOR)x1y1, mx_scale);
		x1y0 = XMVector3Transform((const XMVECTOR)x1y0, mx_scale);

		x0y0 = XMVector3Transform((const XMVECTOR)x0y0, mx_scale_post_translate);
		x0y1 = XMVector3Transform((const XMVECTOR)x0y1, mx_scale_post_translate);
		x1y1 = XMVector3Transform((const XMVECTOR)x1y1, mx_scale_post_translate);
		x1y0 = XMVector3Transform((const XMVECTOR)x1y0, mx_scale_post_translate);
	}
#endif
#ifdef USE_GL_ROTATE
	if (_is_rotate){
		const XMMATRIX mx_rotate_fre_translate = XMLoadFloat4x4(&_mx_rotate_fre_translate);
		const XMMATRIX mx_rotate_post_translate = XMLoadFloat4x4(&_mx_rotate_post_translate);
		const XMVECTOR vector_rotate = XMLoadFloat4(&_vector_rotate);
		x0y0 = XMVector3Transform((const XMVECTOR)x0y0, mx_rotate_fre_translate);
		x0y1 = XMVector3Transform((const XMVECTOR)x0y1, mx_rotate_fre_translate);
		x1y1 = XMVector3Transform((const XMVECTOR)x1y1, mx_rotate_fre_translate);
		x1y0 = XMVector3Transform((const XMVECTOR)x1y0, mx_rotate_fre_translate);

		x0y0 = XMVector3Rotate((const XMVECTOR)x0y0, vector_rotate);
		x0y1 = XMVector3Rotate((const XMVECTOR)x0y1, vector_rotate);
		x1y1 = XMVector3Rotate((const XMVECTOR)x1y1, vector_rotate);
		x1y0 = XMVector3Rotate((const XMVECTOR)x1y0, vector_rotate);

		x0y0 = XMVector3Transform((const XMVECTOR)x0y0, (const XMMATRIX)mx_rotate_post_translate);
		x0y1 = XMVector3Transform((const XMVECTOR)x0y1, (const XMMATRIX)mx_rotate_post_translate);
		x1y1 = XMVector3Transform((const XMVECTOR)x1y1, (const XMMATRIX)mx_rotate_post_translate);
		x1y0 = XMVector3Transform((const XMVECTOR)x1y0, (const XMMATRIX)mx_rotate_post_translate);
	}
#endif



	/*
#ifdef USE_GL_ROTATE
	if (_is_rotate){
		const XMMATRIX mx_rotate_fre_translate = XMLoadFloat4x4(&_mx_rotate_fre_translate);
		const XMMATRIX mx_rotate_post_translate = XMLoadFloat4x4(&_mx_rotate_post_translate);
		const XMVECTOR vector_rotate = XMLoadFloat4(&_vector_rotate);
		x0y0 = XMVector3Transform((const XMVECTOR)x0y0, mx_rotate_fre_translate);
		x0y1 = XMVector3Transform((const XMVECTOR)x0y1, mx_rotate_fre_translate);
		x1y1 = XMVector3Transform((const XMVECTOR)x1y1, mx_rotate_fre_translate);
		x1y0 = XMVector3Transform((const XMVECTOR)x1y0, mx_rotate_fre_translate);

		x0y0 = XMVector3Rotate((const XMVECTOR)x0y0, vector_rotate);
		x0y1 = XMVector3Rotate((const XMVECTOR)x0y1, vector_rotate);
		x1y1 = XMVector3Rotate((const XMVECTOR)x1y1, vector_rotate);
		x1y0 = XMVector3Rotate((const XMVECTOR)x1y0, vector_rotate);

		x0y0 = XMVector3Transform((const XMVECTOR)x0y0, (const XMMATRIX)mx_rotate_post_translate);
		x0y1 = XMVector3Transform((const XMVECTOR)x0y1, (const XMMATRIX)mx_rotate_post_translate);
		x1y1 = XMVector3Transform((const XMVECTOR)x1y1, (const XMMATRIX)mx_rotate_post_translate);
		x1y0 = XMVector3Transform((const XMVECTOR)x1y0, (const XMMATRIX)mx_rotate_post_translate);
	}
#endif


#ifdef USE_GL_SCALE
	if (_is_scale){
		const XMMATRIX mx_scale_fre_translate = XMLoadFloat4x4(&_mx_scale_fre_translate);
		const XMMATRIX mx_scale = XMLoadFloat4x4(&_mx_scale);
		const XMMATRIX mx_scale_post_translate = XMLoadFloat4x4(&_mx_scale_post_translate);
		x0y0 = XMVector3Transform((const XMVECTOR)x0y0, mx_scale_fre_translate);
		x0y1 = XMVector3Transform((const XMVECTOR)x0y1, mx_scale_fre_translate);
		x1y1 = XMVector3Transform((const XMVECTOR)x1y1, mx_scale_fre_translate);
		x1y0 = XMVector3Transform((const XMVECTOR)x1y0, mx_scale_fre_translate);

		x0y0 = XMVector3Transform((const XMVECTOR)x0y0, mx_scale);
		x0y1 = XMVector3Transform((const XMVECTOR)x0y1, mx_scale);
		x1y1 = XMVector3Transform((const XMVECTOR)x1y1, mx_scale);
		x1y0 = XMVector3Transform((const XMVECTOR)x1y0, mx_scale);

		x0y0 = XMVector3Transform((const XMVECTOR)x0y0, mx_scale_post_translate);
		x0y1 = XMVector3Transform((const XMVECTOR)x0y1, mx_scale_post_translate);
		x1y1 = XMVector3Transform((const XMVECTOR)x1y1, mx_scale_post_translate);
		x1y0 = XMVector3Transform((const XMVECTOR)x1y0, mx_scale_post_translate);
	}
#endif
	*/
#ifdef ENABLE_OPACITY_SETTING
	float alpha_val = 255.0f *(float)_opacity_percent / 100.0f;
#else
	float alpha_val = 2550f;
#endif
	float r = 255.0f, g = 255.0f, b = 255.0f;
#ifdef USE_COLOR_MASK
	if (_is_color_mask){
		r = (float)((_nColorMask & 0x00FF0000) >> 16);
		g = (float)((_nColorMask & 0x0000FF00) >> 8);
		b = (float)((_nColorMask & 0x000000FF));
	}
#endif

	*pVtx++ = XMVectorGetX(x0y0);		*pVtx++ = XMVectorGetY(x0y0);		*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = alpha_val;
	*pVtx++ = uv[0];	*pVtx++ = uv[1]; //u0 v0
	*pVtx++ = 1.0f;

	*pVtx++ = XMVectorGetX(x1y0);	*pVtx++ = XMVectorGetY(x1y0);		*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = alpha_val;
	*pVtx++ = uv[2];	*pVtx++ = uv[1]; //u1 v0
	*pVtx++ = 1.0f;

	*pVtx++ = XMVectorGetX(x0y1);		*pVtx++ = XMVectorGetY(x0y1);		*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = alpha_val;
	*pVtx++ = uv[0];	*pVtx++ = uv[3]; //u0 v1
	*pVtx++ = 1.0f;

	*pVtx++ = XMVectorGetX(x0y1);		*pVtx++ = XMVectorGetY(x0y1);		*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = alpha_val;
	*pVtx++ = uv[0];	*pVtx++ = uv[3]; //u0 v1
	*pVtx++ = 1.0f;

	*pVtx++ = XMVectorGetX(x1y0);	*pVtx++ = XMVectorGetY(x1y0);		*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = alpha_val;
	*pVtx++ = uv[2];	*pVtx++ = uv[1]; //u1 v0
	*pVtx++ = 1.0f;

	*pVtx++ = XMVectorGetX(x1y1);	*pVtx++ = XMVectorGetY(x1y1);			*pVtx++ = 0.0f;
	*pVtx++ = r;		*pVtx++ = g;	*pVtx++ = b;	*pVtx++ = alpha_val;
	*pVtx++ = uv[2];	*pVtx++ = uv[3];  //u1 v1
	*pVtx++ = 1.0f;
	_nNum2DQuads++;
}
void GDirectX11::LoadModel(float ox, float oy, float m11, float m12, float m21, float m22)
{
	if (_nNum2DQuads >= (MAX_2D_QUADS - 1)){
		printf("MAX_2D_QUADS OVER FLOW\n");
		return;
	}
	_pFlags[_nNum2DQuads] |= FREE_TRANSFORM_FLAG;
	float*	pModel = _pModBuffer2D + (MODEL_SIZE * MODEL_COUNT_PER_QUAD * _nNum2DQuads);
	if (_is_transform){
		SMatrix4x4 m, m_model, m_result;
		m.Init(_transform_param);
		PineTransformParam model_param;
		model_param.TranslateX = ox;
		model_param.TranslateY = oy;
		model_param.m11 = m11;
		model_param.m12 = m12;
		model_param.m21 = m21;
		model_param.m22 = m22;
		m_model.Init(model_param);
		SMatrix4x4::Mul(m_result, m_model, m);
		PineTransformParam result_param = m_result.GetParam();
		*pModel++ = result_param.TranslateX;
		*pModel++ = result_param.TranslateY;
		*pModel++ = result_param.m11;
		*pModel++ = result_param.m12;
		*pModel++ = result_param.m21;
		*pModel++ = result_param.m22;
	}
	else{
		*pModel++ = ox;
		*pModel++ = oy;
		*pModel++ = m11;
		*pModel++ = m12;
		*pModel++ = m21;
		*pModel++ = m22;
	}
}
void GDirectX11::SetTransform(PineTransformParam param){
	_transform_param = param;
	_is_transform = true;
}
void GDirectX11::ClearTransform(){
	_transform_param.TranslateX = 0.0f;
	_transform_param.TranslateY = 0.0f;
	_transform_param.m11 = 0.0f;
	_transform_param.m12 = 0.0f;
	_transform_param.m21 = 0.0f;
	_transform_param.m22 = 0.0f;
	_is_transform = false;
}

//function work with rotation
#if defined(USE_GL_ROTATE)
void GDirectX11::SetRotate(float angle, float x, float y){
	_is_rotate = true;
	float radian = angle * XM_PI / 180.0f;
	XMStoreFloat4(&_vector_rotate, XMQuaternionRotationAxis(XMVectorSet(0.0f, 0.0f, 0.1f, 1.0f), radian));
	XMStoreFloat4x4(&_mx_rotate_fre_translate, XMMatrixTranslation(-x, -y, 0));
	XMStoreFloat4x4(&_mx_rotate_post_translate, XMMatrixTranslation(x, y, 0));
}
void GDirectX11::ClearRotate(){
	_is_rotate = false;
}
#endif

#if defined(USE_TRANSLATE)
void GDirectX11::SetTranslate(float x, float y)
{
	_is_translate = true;
	XMStoreFloat4x4(&_mx_translate, XMMatrixTranslation(-x, y, 0));
}
void GDirectX11::ClearTranslate()
{
	_is_translate = false;
}
#endif

#if defined(USE_GL_SCALE)
bool GDirectX11::HasScale(){
	return _is_scale;
}
void GDirectX11::SetScale(float scale_x, float scale_y, float center_x, float center_y)
{
	_is_scale = true;
	XMStoreFloat4x4(&_mx_scale, XMMatrixScaling(scale_x, scale_y, 1.0f));
	XMStoreFloat4x4(&_mx_scale_fre_translate, XMMatrixTranslation(-center_x, -center_y, 0));
	XMStoreFloat4x4(&_mx_scale_post_translate, XMMatrixTranslation(center_x, center_y, 0));
	_scale_x = scale_x;
	_scale_y = scale_y;
	_scale_center_x = center_x;
	_scale_center_y = center_y;
}

void GDirectX11::ClearScale()
{
	_is_scale = false;
	_scale_x = 1.0f;
	_scale_y = 1.0f;
	_scale_center_x = 0;
	_scale_center_y = 0;
}
ScaleParam GDirectX11::GetCurrentScaleParam(){
	return ScaleParam(_scale_x, _scale_y, _scale_center_x, _scale_center_y);
}
#endif

/*public static Texture2D FromImage(BitmapImage image, GraphicsDevice device)
{
	WriteableBitmap bitmap = new WriteableBitmap(image);

	return FromImageData(bitmap.Pixels, bitmap.PixelWidth,
		bitmap.PixelHeight, device);
}

public static Texture2D FromImageData(int[] data, int width, int height, GraphicsDevice device)
{
	Texture2D texture = Texture2D.New(device, width, height,
		PixelFormat.B8G8R8A8.UNorm);

	texture.SetData<int>(data);

	return texture;
}*/

HTEXTURE GDirectX11::createTexture(BYTE* input_data, int texw, int texh, PineColorFormat input_colorformat, int is_altialias)
{

	if (_curr_device != NULL){
		DXGI_FORMAT color_format = DXGI_FORMAT_R8G8B8A8_UNORM;
		int color_size = 2;
		if (input_colorformat == PineColorFormat::RGBA_1BYTE)
		{
			color_format = DXGI_FORMAT_R8G8B8A8_UNORM;
			color_size = 4;
		}
		else if (input_colorformat == PineColorFormat::RGBA_4444){
			color_format = DXGI_FORMAT_B4G4R4A4_UNORM;
			color_size = 2;
		}

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = texw;
		desc.Height = texh;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = color_format;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		desc.Usage =  D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		

		D3D11_SUBRESOURCE_DATA resdata;
		resdata.pSysMem = input_data;
		resdata.SysMemPitch = texw* color_size;
		resdata.SysMemSlicePitch = texw* texh * color_size;

		HTEXTURE texture2D = NULL;
		HRESULT rs = _curr_device->CreateTexture2D(&desc, &resdata, &texture2D);

		if (rs != S_OK){
			TEST_LOG_RAW("create texture fail with code: %x clrsize:%d texw:%d texh:%d", rs, color_size, texw, texh);
			return NULL;
		}
		else{
			TEST_LOG_RAW("create texture sucess with clrsize:%d texw:%d texh:%d", color_size, texw, texh);
			return texture2D;
		}
	}
	return NULL;
}
void GDirectX11::freeTexture(HTEXTURE tex)
{
	if (tex != NULL){
		while (tex->Release() != 0);
		tex = NULL;
	}
}
bool GDirectX11::IsEngineReady(){
	return (_curr_device != NULL && _curr_context != NULL);

}

PineColorType GDirectX11::ColorType(){
	return PineColorType::CTYPE_DIRECTX;
}
void GDirectX11::ChangeDevice(ID3D11Device1* device)
{
	_curr_device = device;
	if (_curr_device != NULL){
		D3D11_SUBRESOURCE_DATA resourceData;
		ZeroMemory(&resourceData, sizeof(resourceData));
		resourceData.pSysMem = _pVtxBuffer2D;
		HRESULT result = _curr_device->CreateBuffer(&_vtexdesc, &resourceData, &_dxVertexBuffer);
		if (result != S_OK)
		{
			_curr_device = NULL;
			printf("create buffer fail\n");
		}

		if (_blendState != NULL) while (_blendState->Release() != 0);
		if (_blendState_add != NULL) while (_blendState_add->Release() != 0);
		if (_blendState_constract != NULL) while (_blendState_constract->Release() != 0);
		if (_blendState_light != NULL) while (_blendState_light->Release() != 0);

		//if(_blendState != NULL) delete _blendState;
		//if(_blendState_add != NULL) delete _blendState_add;
		//if(_blendState_constract != NULL) delete _blendState_constract;
		//if(_blendState_light != NULL) delete _blendState_light;

		///BLEND NORMAL
		D3D11_BLEND_DESC blend_desc;
		blend_desc.AlphaToCoverageEnable = false;
		blend_desc.IndependentBlendEnable = true;
		for (int i = 0; i < 8; i++){
			// Create an alpha enabled blend state description.
			blend_desc.RenderTarget[i].BlendEnable = true;

			blend_desc.RenderTarget[i].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
			blend_desc.RenderTarget[i].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
			blend_desc.RenderTarget[i].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

			blend_desc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
			blend_desc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
			blend_desc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			blend_desc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
		}
		_curr_device->CreateBlendState(&blend_desc, &_blendState);

		///BLEND ADD
		D3D11_BLEND_DESC blend_desc_add;
		blend_desc_add.AlphaToCoverageEnable = false;
		blend_desc_add.IndependentBlendEnable = true;
		for (int i = 0; i < 8; i++){
			// Create an alpha enabled blend state description.
			blend_desc_add.RenderTarget[i].BlendEnable = true;

			blend_desc_add.RenderTarget[i].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
			blend_desc_add.RenderTarget[i].DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;
			blend_desc_add.RenderTarget[i].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

			blend_desc_add.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
			blend_desc_add.RenderTarget[i].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
			blend_desc_add.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			blend_desc_add.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
		}
		_curr_device->CreateBlendState(&blend_desc_add, &_blendState_add);
		///BLEND CONSTRACT
		D3D11_BLEND_DESC blend_desc_constract;
		blend_desc_constract.AlphaToCoverageEnable = false;
		blend_desc_constract.IndependentBlendEnable = true;
		for (int i = 0; i < 8; i++){
			// Create an alpha enabled blend state description.
			blend_desc_constract.RenderTarget[i].BlendEnable = true;

			blend_desc_constract.RenderTarget[i].SrcBlend = D3D11_BLEND::D3D11_BLEND_DEST_COLOR;
			blend_desc_constract.RenderTarget[i].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
			blend_desc_constract.RenderTarget[i].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

			blend_desc_constract.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
			blend_desc_constract.RenderTarget[i].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
			blend_desc_constract.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			blend_desc_constract.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
		}
		_curr_device->CreateBlendState(&blend_desc_constract, &_blendState_constract);

		///BLEND LIGHT
		D3D11_BLEND_DESC blend_desc_light;
		blend_desc_light.AlphaToCoverageEnable = false;
		blend_desc_light.IndependentBlendEnable = true;
		for (int i = 0; i < 8; i++){
			// Create an alpha enabled blend state description.
			blend_desc_light.RenderTarget[i].BlendEnable = true;

			blend_desc_light.RenderTarget[i].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
			blend_desc_light.RenderTarget[i].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
			blend_desc_light.RenderTarget[i].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;

			blend_desc_light.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
			blend_desc_light.RenderTarget[i].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_DEST_ALPHA;
			blend_desc_light.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			blend_desc_light.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
		}
		_curr_device->CreateBlendState(&blend_desc_light, &_blendState_light);
	}
}
void GDirectX11::ChangeContext(ID3D11DeviceContext1* context)
{
	_curr_context = context;
}

void GDirectX11::setBlendFunction(BLEND_MODE mode)
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	UINT sampleMask = 0xffffffff;
	switch (mode){
	case BLEND_MODE::ALPHA_FILLTER_NORMAL:
		_curr_context->OMSetBlendState(_blendState, blendFactor, sampleMask);
		break;
	case BLEND_MODE::ALPHA_FILLTER_ADD:
		_curr_context->OMSetBlendState(_blendState_add, blendFactor, sampleMask);
		break;
#if defined(USE_BLEND_ALPHA)
	case BLEND_MODE::ALPHA_FILLTER_CONSTRACT:
		_curr_context->OMSetBlendState(_blendState_constract, blendFactor, sampleMask);
		break;
	case BLEND_MODE::ALPHA_FILLTER_LIGHT:
		_curr_context->OMSetBlendState(_blendState_light, blendFactor, sampleMask);
		break;
#endif
	default:
		_curr_context->OMSetBlendState(_blendState, blendFactor, sampleMask);
		break;
	}
}
void GDirectX11::setWorldMatrix(PineTransformParam param){

}
void GDirectX11::updateTexture(HTEXTURE tex, BYTE* input_data, int dw, int dh, int tx, int ty, PineColorFormat color_format)
{

}

#if defined(_DEBUG)

Graphic* GDirectX11::_dbg = NULL;
bool GDirectX11::_create_dbg = false;
UINT32 GDirectX11::_dbg_flag = DEBUG_GRAPHIC_NONE;
std::vector<Graphic::GraphicDebugTexture> GDirectX11::_debug_texture;
UINT64 GDirectX11::_debug_tex_mem_use = 0;
UINT64 GDirectX11::_debug_tex_mem_use_max = 0;

void GDirectX11::setDebugFlagOn(UINT32 flag)
{
	_dbg_flag |= flag;
}
void GDirectX11::setDebugFlagOff(UINT32 flag)
{
	_dbg_flag &= ~flag;
}
void GDirectX11::debugInfo()
{
	PDEBUG("Graphic memory use for texture: %lld max:%lld\n", _debug_tex_mem_use, _debug_tex_mem_use_max);
	PDEBUG("Graphic current texture: %d\n", _debug_texture.size());
}

void GDirectX11::DebugCacheRectangle(PineRectangle rect)
{
	if (_dbg != NULL)
	{
		_dbg->SetColor(0x01FF0000);
		_dbg->SetOpacity(100);
		_dbg->DrawRect(rect.X, rect.Y, rect.W, rect.H, true);
	}
}
void GDirectX11::DebugCacheLine(PinePoint p1, PinePoint p2)
{
	if (_dbg != NULL)
	{
		_dbg->DrawLine(p1.X, p1.Y, p2.X, p2.Y, true);
	}
}
void GDirectX11::DebugCacheDrawTexBorder(PineRectangle rect)
{
	if (_dbg != NULL && (_dbg_flag & DEBUG_GRAPHIC_DRAWTEX))
	{
		_dbg->SetColor(0x01aa0000);
		_dbg->SetOpacity(100);
		_dbg->DrawRect(rect.X, rect.Y, rect.W, rect.H, true);
	}
}
#endif