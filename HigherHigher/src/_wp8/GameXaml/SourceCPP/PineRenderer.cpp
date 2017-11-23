#include "pch.h"
#include "PineRenderer.h"
#include "Utils.h"
#include "ServerStream\ServerStreamManager.h"
#include "WP8_Connector.h"
#include "Environment.h"

using namespace concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::Storage;
#if _DEBUG

#else
using namespace Windows::ApplicationModel::Store;
#endif

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

using namespace Windows::Networking::Connectivity;

using namespace PhoneDirect3DXamlAppComponent;

PineRenderer::PineRenderer() :
	m_loadingComplete(false),
	m_indexCount(0)
{
	_this = this;
	_is_init = false;
	_factor = 1.0f;
	WaitingInAppItem = nullptr;
	is_render = false;
	is_update = false;
}

void PineRenderer::CreateDeviceResources()
{
	//Direct3DBase::CreateDeviceResources();
	while (Environment::GetInstace() == NULL);
	auto loadVSTask = DX::ReadDataAsync("SimpleVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync("SimplePixelShader.cso");

	auto createVSTask = loadVSTask.then([this](Platform::Array<byte>^ fileData) {
		DX::ThrowIfFailed(
			m_d3dDevice->CreateVertexShader(
				fileData->Data,
				fileData->Length,
				nullptr,
				&m_vertexShader
				)
			);

		const D3D11_INPUT_ELEMENT_DESC vertexDesc[] = 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "PARAM",    0, DXGI_FORMAT_R32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		DX::ThrowIfFailed(
			m_d3dDevice->CreateInputLayout(
				vertexDesc,
				ARRAYSIZE(vertexDesc),
				fileData->Data,
				fileData->Length,
				&m_inputLayout
				)
			);
	});

	auto createPSTask = loadPSTask.then([this](Platform::Array<byte>^ fileData) {
		DX::ThrowIfFailed(
			m_d3dDevice->CreatePixelShader(
				fileData->Data,
				fileData->Length,
				nullptr,
				&m_pixelShader
				)
			);

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_d3dDevice->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&m_constantBuffer
				)
			);
	});
	auto initTask = (createPSTask && createVSTask).then([this] () {
		while (Environment::GetInstace() == NULL);
		Environment* env = Environment::GetInstace();
		if(m_d3dDevice.Get() != NULL){
			env->SetDevice(m_d3dDevice.Get());
		}

		if(!_is_init && m_d3dContext != nullptr && m_d3dDevice != nullptr){
			env->SetContext(m_d3dContext.Get());
			_is_init = true;
			// create the sampler
			D3D11_SAMPLER_DESC samplerDesc;
			ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
			//samplerDesc.Filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
			//samplerDesc.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
			//samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			//samplerDesc.Filter = D3D11_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.MipLODBias = 0.0f;
			// use 4x on feature level 9.2 and above, otherwise use only 2x
			//samplerDesc.MaxAnisotropy = m_featureLevel > D3D_FEATURE_LEVEL_9_1 ? 4 : 2;
			samplerDesc.MaxAnisotropy = 16;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.BorderColor[0] = 0.0f;
			samplerDesc.BorderColor[1] = 0.0f;
			samplerDesc.BorderColor[2] = 0.0f;
			samplerDesc.BorderColor[3] = 0.0f;
			// allow use of all mip levels
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			DX::ThrowIfFailed(m_d3dDevice->CreateSamplerState(&samplerDesc,&m_sampler));

			// Setup the raster description which will determine how and what polygons will be drawn.
			rasterDesc.AntialiasedLineEnable = true;
			rasterDesc.CullMode = D3D11_CULL_NONE;
			rasterDesc.DepthBias = 0;
			rasterDesc.DepthBiasClamp = 0.0f;
			rasterDesc.DepthClipEnable = true;
			rasterDesc.FillMode = D3D11_FILL_SOLID;
			rasterDesc.FrontCounterClockwise = false;
			rasterDesc.MultisampleEnable = false;// true;
			rasterDesc.ScissorEnable = true;
			rasterDesc.SlopeScaledDepthBias = 0.0f;

			// Create the rasterizer state from the description we just filled out.
			m_d3dDevice->CreateRasterizerState(&rasterDesc, &m_rasterState);

			// Now set the rasterizer state.
			m_d3dContext->RSSetState(m_rasterState);
		}
	});

	initTask.then([this] () {
		m_loadingComplete = true;
		is_render = false;
	});
}

void PineRenderer::CreateWindowSizeDependentResources()
{
	while (Environment::GetInstace() == NULL);
	Direct3DBase::CreateWindowSizeDependentResources();
	ResolutionScale scale = DisplayProperties::ResolutionScale;
	
	_factor = 1.0f;
	switch (scale)
	{
	case Windows::Graphics::Display::ResolutionScale::Invalid:
		break;
	case Windows::Graphics::Display::ResolutionScale::Scale100Percent:
		_factor = 1.0f;
		break;
	case Windows::Graphics::Display::ResolutionScale::Scale140Percent:
		_factor = 1.4f;
		break;
	case Windows::Graphics::Display::ResolutionScale::Scale150Percent:
		_factor = 1.5f;
		break;
	case Windows::Graphics::Display::ResolutionScale::Scale160Percent:
		_factor = 1.6f;
		break;
	case Windows::Graphics::Display::ResolutionScale::Scale180Percent:
		_factor = 1.8f;
		break;
	default:

		break;
	}

	XMMATRIX identityMatrix = XMMatrixIdentity();

	XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMStoreFloat4x4(&m_constantBufferData.view, (XMMatrixLookAtLH(eye, at, up)));


	_modelMatrix = identityMatrix;

	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(_modelMatrix));
	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		XMMatrixTranspose(
		XMMatrixOrthographicOffCenterLH(0.0f, m_windowBounds.Width *_factor, m_windowBounds.Height *_factor, 0.0f, -0.1f, 0.1f)
		));

	float screen_width = m_windowBounds.Width * _factor;
	float screen_height = m_windowBounds.Height *_factor;
	float tmp = screen_width;

	Windows::Graphics::Display::DisplayOrientations ori = DisplayProperties::NativeOrientation;
	switch (ori)
	{
	case DisplayOrientations::Landscape:
	case DisplayOrientations::LandscapeFlipped:
		screen_width = screen_width > screen_height ? screen_width : screen_height;
		screen_height = tmp == screen_width ? screen_height : tmp;
		break;

	case DisplayOrientations::Portrait:
	case DisplayOrientations::PortraitFlipped:
		screen_width = screen_width < screen_height ? screen_width : screen_height;
		screen_height = tmp == screen_width ? screen_height : tmp;
		break;

	default:
		break;
	}

	Environment::GetInstace()->SetDeviceScreen(ceil(screen_width), ceil(screen_height));
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.Width = m_windowBounds.Width * _factor;
	viewport.Height = m_windowBounds.Height * _factor;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	m_d3dContext->RSSetViewports(1, &viewport);

	m_d3dContext->OMSetRenderTargets(1,m_renderTargetView.GetAddressOf(),m_depthStencilView.Get());
}
void PineRenderer::DetectOrientation(int ori){
	Orientation orientation;
	switch (ori)
	{
	case 1:
		orientation = Orientation::Landscape_90;
		break;
	case 2:
		orientation = Orientation::Landscape_270;
		break;

	case 3:
	case 4:
		orientation = Orientation::Portrait;
		break;

	default:
		break;
	}
	if (Environment::GetInstace() != NULL){
		Environment::GetInstace()->SetDeviceOrientaion(orientation);
	}
}
void PineRenderer::Update(float timeTotal, float timeDelta)
{
	if (Environment::GetInstace() != NULL && m_loadingComplete && !is_render && !is_update){
		is_update = true;
		Environment::GetInstace()->Update();
		is_update = false;
	}
}

void PineRenderer::Render()
{
	if (is_render)return;
	is_render = true;
	if (Environment::GetInstace() != NULL && Environment::GetInstace()->_need_capture_screen){
		CaptureScreen();
	}

	m_d3dContext->OMSetRenderTargets(
		1,
		m_renderTargetView.GetAddressOf(),
		m_depthStencilView.Get()
		);

	const float midnightBlue[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_d3dContext->ClearRenderTargetView(
		m_renderTargetView.Get(),
		midnightBlue
		);
	m_d3dContext->OMSetDepthStencilState(m_depthDisabledStencilState.Get(), 1);
	m_d3dContext->ClearDepthStencilView(
		m_depthStencilView.Get(),
		D3D11_CLEAR_DEPTH,
		1.0f,
		0
		);
	//float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//UINT sampleMask   = 0xffffffff;
	//m_d3dContext->OMSetBlendState(m_blendState.Get(),blendFactor, sampleMask);
	// Only draw the cube once it is loaded (loading is asynchronous).
	if (!m_loadingComplete && !is_update)
	{
		is_render = false;
		return;
	}

	m_d3dContext->UpdateSubresource(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0
		);
	m_d3dContext->RSSetState(m_rasterState);
	//UINT stride = sizeof(VertexPositionColor);
	//UINT offset = 0;

	m_d3dContext->IASetInputLayout(m_inputLayout.Get());
	m_d3dContext->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
		);

	m_d3dContext->VSSetConstantBuffers(
		0,
		1,
		m_constantBuffer.GetAddressOf()
		);

	m_d3dContext->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
		);
	m_d3dContext->PSSetSamplers(0,1,m_sampler.GetAddressOf());
	if (Environment::GetInstace() != NULL){
		XMMATRIX model = Environment::GetInstace()->GetWorldMatrix();
		const XMMATRIX mxmodel = (const XMMATRIX)Environment::GetInstace()->GetWorldMatrix();
		XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose((const XMMATRIX)model));
		Environment::GetInstace()->Render();
	}
	m_d3dContext->Flush();
	is_render = false;

	//if (Environment::GetInstace() != NULL && Environment::GetInstace()->_need_capture_screen){
	//	CaptureScreen();
	//}
	//m_d3dContext->ClearState();
	//m_d3dContext->PSSetShaderResources(0, 1, NULL);
}
bool PineRenderer::CaptureScreen(){
	char* filename = Utils::getAbsoluteURL(TMP_FILE_SCREEN_CAPTURE, Utils::DATA_PATH);
	FILE* stream = NULL;
	stream = fopen(filename, "wb");
	bool rs = false;
	if (stream != NULL){
		ComPtr<ID3D11Resource> renderTargetViewResource;
		m_renderTargetView->GetResource(&renderTargetViewResource);

		ComPtr<ID3D11Texture2D> backBuffer;
		DX::ThrowIfFailed(
			renderTargetViewResource.As(&backBuffer)
			);
		
		ID3D11Texture2D* capture_tex = backBuffer.Get();// Environment::GetInstace()->_capture_texture;
		// Cache the rendertarget dimensions in our helper class for convenient use.
		D3D11_TEXTURE2D_DESC backBufferDesc;
		capture_tex->GetDesc(&backBufferDesc);

		ID3D11Texture2D* pNewTexture = NULL;
		D3D11_TEXTURE2D_DESC description;
		capture_tex->GetDesc(&description);

		description.BindFlags = 0;

		description.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		description.Usage = D3D11_USAGE_STAGING;
		HRESULT hr = m_d3dDevice->CreateTexture2D(&description, NULL, &pNewTexture);

		ID3D11DeviceContext* ctx = NULL;
		m_d3dDevice->GetImmediateContext(&ctx);

		ctx->CopyResource(pNewTexture, capture_tex);

		D3D11_MAPPED_SUBRESOURCE resource;
		UINT subresource = D3D11CalcSubresource(0, 0, 0);
		ctx->Map(pNewTexture, subresource, D3D11_MAP_READ_WRITE, 0, &resource);

		// COPY from texture to bitmap buffer
		uint8_t* sptr = reinterpret_cast<uint8_t*>(resource.pData);
		uint8_t* dptr = new uint8_t[backBufferDesc.Width*backBufferDesc.Height * 4];

		for (size_t h = 0; h < backBufferDesc.Height; ++h)
		{
			size_t msize = std::min<size_t>(backBufferDesc.Width * 4, resource.RowPitch);
			memcpy_s(dptr, backBufferDesc.Width * 4, sptr, msize);
			sptr += resource.RowPitch;
			dptr += backBufferDesc.Width * 4;
		}

		dptr -= backBufferDesc.Width*backBufferDesc.Height * 4;

		// SWAP BGR to RGB bitmap and rotate
		uint32_t *dPtr = reinterpret_cast<uint32_t*>(dptr);
		for (size_t count = 0; count < backBufferDesc.Width*backBufferDesc.Height * 4; count += 4){
			uint32_t t = *(dPtr);
			uint8_t b = (t & 0x00ff0000) >> 16;
			uint8_t r = (t & 0x000000ff);
			uint8_t g = (t & 0x0000ff00)>>8;
			uint8_t a = (t & 0xFF000000)>>24;
			*(dPtr++) = ((r<<24) | (g<<16) | (b<<8) | a);
		}

	
		int width = backBufferDesc.Width;
		int height = backBufferDesc.Height;
	
		fwrite((const void*)&width, sizeof(BYTE), 4, stream);
		fwrite((const void*)&height, sizeof(BYTE), 4, stream);
		fwrite((const void*)dptr, sizeof(BYTE), width*height * 4, stream);
		fclose(stream);
		delete[] dptr;
		rs = true;
	}

	delete[] filename;
	return rs;
}
SVHANDLE* _renderer_h_capture_screen = NULL;
/*void PineRenderer::AsynCaptureScreen(SVHANDLE* handle){
	if (_renderer_h_capture_screen == NULL){
		if (handle->isFree()){
			handle->Handle = ServerStreamManager::Request();
			if (handle->Handle > -1){
				_renderer_h_capture_screen = handle;
				Platform::String^ fname = Utils::ConvertCharToString(TMP_FILE_SCREEN_CAPTURE);
				create_task(ApplicationData::Current->LocalFolder->GetFileAsync(fname)).then([](task<Windows::Storage::StorageFile^> task)
				{
					try{
						Windows::Storage::StorageFile^ file = task.get();
						if (file != nullptr){
							create_task(file->DeleteAsync()).then([](){
								//when delete finished
								if (PineRenderer::getInstance()->CaptureScreen()){
									_renderer_h_capture_screen->Status = SVRESULT::SV_DATA_SUCCESS;
								}else{
									_renderer_h_capture_screen->Status = SVRESULT::SV_DATA_ERROR_UNKNOW;
								}
								_renderer_h_capture_screen = NULL;
							});
						}
						else{
							//when file doesn't exists
							if (PineRenderer::getInstance()->CaptureScreen()){
								_renderer_h_capture_screen->Status = SVRESULT::SV_DATA_SUCCESS;
							}
							else{
								_renderer_h_capture_screen->Status = SVRESULT::SV_DATA_ERROR_UNKNOW;
							}
							_renderer_h_capture_screen = NULL;
						}
					}
					catch (...){
						//when deleting file get error
						_renderer_h_capture_screen->Status = SVRESULT::SV_DATA_ERROR_UNKNOW;
						_renderer_h_capture_screen = NULL;
					}
				});
			}
		}
	}
	else if (handle != _renderer_h_capture_screen){
		handle->Status = SVRESULT::SV_DATA_ERROR_UNKNOW;
	}
}*/
void PineRenderer::NotifyTouchBegin(int touch_id, int touch_x, int touch_y, int touch_tap){
	touch_x *= _factor;
	touch_y *= _factor;
	if (Environment::GetInstace() != NULL){
		Environment::GetInstace()->TouchBegin(touch_id, touch_x, touch_y, touch_tap);
	}
}
void PineRenderer::NotifyTouchMove(int touch_id, int touch_x, int touch_y, int touch_tap){
	touch_x *= _factor;
	touch_y *= _factor;
	if (Environment::GetInstace() != NULL){
		Environment::GetInstace()->TouchDrag(touch_id, touch_x, touch_y, touch_tap);
	}
}
void PineRenderer::NotifyTouchEnd(int touch_id, int touch_x, int touch_y, int touch_tap){
	//printf("touch begin: %d %d \n", touch_x, touch_y);
	touch_x *= _factor;
	touch_y *= _factor;
	if (Environment::GetInstace() != NULL){
		Environment::GetInstace()->TouchEnd(touch_id, touch_x, touch_y, touch_tap);
	}
}

void PineRenderer::NotifyBackKey(){
	WP8_OnBackPress();
}
void PineRenderer::CallBaseService(UINT32 construct_id, byte* data, UINT32 data_len){
	OnRequestBaseService(construct_id, data_len);
}
void PineRenderer::NotifyPause(){
	//printf("Notify Pause\n");
#if !defined(DEBUG_REMOVE_GAME)
	if (Environment::GetInstace() != NULL && !Environment::GetInstace()->_is_pause){
		Environment::GetInstace()->_game.Pause();
		Environment::GetInstace()->_is_pause = true;
	}
#endif
}
void PineRenderer::NotifyResume(){
	//printf("Notify Resume\n");
#if !defined(DEBUG_REMOVE_GAME)
	if(Environment::GetInstace()!= NULL && Environment::GetInstace()->_is_pause){
		Environment::GetInstace()->_game.Resume();
		Environment::GetInstace()->_is_pause = false;
	}
#endif
}