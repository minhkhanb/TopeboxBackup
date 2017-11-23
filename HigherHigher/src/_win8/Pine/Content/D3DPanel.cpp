//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
//
//*********************************************************

#pragma once
#include "pch.h"
#include "D3DPanel.h"
#include "../Common/DirectXHelper.h"

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <math.h>
#include <ppltasks.h>
#include <windows.ui.xaml.media.dxinterop.h>
#include "../Src/Environment.h"

using namespace Microsoft::WRL;
using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::System::Threading;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input::Inking;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Interop;
using namespace Concurrency;
using namespace DirectX;
using namespace D2D1;
using namespace Pine;
using namespace DX;

D3DPanel::D3DPanel() :
    m_degreesPerSecond(10),
    m_indexCount(0)
{
    critical_section::scoped_lock lock(m_criticalSection);
    CreateDeviceIndependentResources();
    CreateDeviceResources();
    CreateSizeDependentResources();
}

D3DPanel::~D3DPanel()
{
    m_renderLoopWorker->Cancel();
}

void D3DPanel::StartRenderLoop()
{
    // If the animation render loop is already running then do not start another thread.
    if (m_renderLoopWorker != nullptr && m_renderLoopWorker->Status == AsyncStatus::Started)
    {
        return;
    }

    // Create a task that will be run on a background thread.
    auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction ^ action)
    {
        // Calculate the updated frame and render once per vertical blanking interval.
        while (action->Status == AsyncStatus::Started)
        {
            m_timer.Tick([&]()
            {
                critical_section::scoped_lock lock(m_criticalSection);
                Render();
            });

            // Halt the thread until the next vblank is reached.
            // This ensures the app isn't updating and rendering faster than the display can refresh,
            // which would unnecessarily spend extra CPU and GPU resources.  This helps improve battery life.
            m_dxgiOutput->WaitForVBlank();
        }
    });
    
    // Run task on a dedicated high priority background thread.
    m_renderLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
}

void D3DPanel::StopRenderLoop()
{
    // Cancel the asynchronous task and let the render thread exit.
    m_renderLoopWorker->Cancel();
}

void D3DPanel::Render()
{
    if (!m_loadingComplete || m_timer.GetFrameCount() == 0)
    {
        return;
    }

    // Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
    //static const XMVECTORF32 eye = { 0.0f, 0.0f, 1.5f, 0.0f };
    //static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
    //static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	//XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	//XMVECTOR at = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    // Convert degrees to radians, then convert seconds to rotation angle
    float radiansPerSecond = XMConvertToRadians(m_degreesPerSecond);
    double totalRotation = m_timer.GetTotalSeconds() * radiansPerSecond;
    float animRadians = (float) fmod(totalRotation, XM_2PI);

    // Prepare to pass the view matrix, and updated model matrix, to the shader
    //XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
    XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(animRadians)));

    // Set render targets to the screen.
    ID3D11RenderTargetView *const targets[1] = { m_renderTargetView.Get() };
    m_d3dContext->OMSetRenderTargets(1, targets, m_depthStencilView.Get());
	////m_d3dContext->OMSetDepthStencilState(m_depthDisabledStencilState.Get(), 1);
	////m_d3dContext->RSSetState(m_rasterState.Get());
    // Clear the back buffer and depth stencil view.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), DirectX::Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // Prepare the constant buffer to send it to the Graphics device.
    m_d3dContext->UpdateSubresource(
        m_constantBuffer.Get(),
        0,
        NULL,
        &m_constantBufferData,
        0,
        0
        );

    // Each vertex is one instance of the VertexPositionColor struct.
    UINT stride = sizeof(VertexPositionColor);
    UINT offset = 0;
    m_d3dContext->IASetVertexBuffers(
        0,
        1,
        m_vertexBuffer.GetAddressOf(),
        &stride,
        &offset
        );

    m_d3dContext->IASetIndexBuffer(
        m_indexBuffer.Get(),
        DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
        0
        );

    m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_d3dContext->IASetInputLayout(m_inputLayout.Get());

    // Attach our vertex shader.
    m_d3dContext->VSSetShader(
        m_vertexShader.Get(),
        nullptr,
        0
        );

    // Send the constant buffer to the Graphics device.
    m_d3dContext->VSSetConstantBuffers(
        0,
        1,
        m_constantBuffer.GetAddressOf()
        );

    // Attach our pixel shader.
    m_d3dContext->PSSetShader(
        m_pixelShader.Get(),
        nullptr,
        0
        );
	m_d3dContext->PSSetSamplers(0, 1, m_sampler.GetAddressOf());
    // Draw the objects.
    m_d3dContext->DrawIndexed(
        m_indexCount,
        0,
        0
        );

    Present();
}

void D3DPanel::CreateDeviceResources()
{
    DirectXPanelBase::CreateDeviceResources();

    // Retrieve DXGIOutput representing the main adapter output.
    ComPtr<IDXGIFactory1> dxgiFactory;
    ThrowIfFailed(
        CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory))
        );

    ComPtr<IDXGIAdapter> dxgiAdapter;
    ThrowIfFailed(
        dxgiFactory->EnumAdapters(0, &dxgiAdapter)
        );

    ThrowIfFailed(
        dxgiAdapter->EnumOutputs(0, &m_dxgiOutput)
        );

	HRESULT rs = dxgiAdapter->GetDesc(&m_dxgiDesc);
	if (rs == S_OK){
		TEST_LOG_RAW("dxgi videomem:%d sysmem:%d sharemem:%d", m_dxgiDesc.DedicatedVideoMemory, m_dxgiDesc.DedicatedSystemMemory, m_dxgiDesc.SharedSystemMemory);
	}

    // Asynchronously load vertex shader and create input layout.
    auto loadVSTask = DX::ReadDataAsync(L"Pine\\VertexShader.cso");
    auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
        ThrowIfFailed(
            m_d3dDevice->CreateVertexShader(
            &fileData[0],
            fileData.size(),
            nullptr,
            &m_vertexShader
            )
            );

        static const D3D11_INPUT_ELEMENT_DESC vertexDesc [] =
        {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "PARAM", 0, DXGI_FORMAT_R32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        ThrowIfFailed(
            m_d3dDevice->CreateInputLayout(
            vertexDesc,
            ARRAYSIZE(vertexDesc),
            &fileData[0],
            fileData.size(),
            &m_inputLayout
            )
            );
    });

    // Asynchronously load vertex shader and create constant buffer.
    auto loadPSTask = DX::ReadDataAsync(L"Pine\\PixelShader.cso");
    auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
        ThrowIfFailed(
            m_d3dDevice->CreatePixelShader(
            &fileData[0],
            fileData.size(),
            nullptr,
            &m_pixelShader
            )
            );

        CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
        ThrowIfFailed(
            m_d3dDevice->CreateBuffer(
            &constantBufferDesc,
            nullptr,
            &m_constantBuffer
            )
            );
    });

    // Once both shaders are loaded, create the mesh.
    auto createCubeTask = (createPSTask && createVSTask).then([this]() {

        // Load mesh vertices. Each vertex has a position and a color.
		float size = 1000.5f;
		static const VertexPositionColor cubeVertices[] =
		{
			{ XMFLOAT3(-size, -size, -size), XMFLOAT4(0.0f, 0.0f, 0.0f, 255.0f), XMFLOAT2(0.0f, 0.0f), 0.0f },
			{ XMFLOAT3(-size, -size, size), XMFLOAT4(0.0f, 0.0f, 255.0f, 255.0f), XMFLOAT2(0.0f, 0.0f), 0.0f },
			{ XMFLOAT3(-size, size, -size), XMFLOAT4(0.0f, 255.0f, 0.0f, 255.0f), XMFLOAT2(0.0f, 0.0f), 0.0f },
			{ XMFLOAT3(-size, size, size), XMFLOAT4(0.0f, 255.0f, 255.0f, 255.0f), XMFLOAT2(0.0f, 0.0f), 0.0f },
			{ XMFLOAT3(size, -size, -size), XMFLOAT4(255.0f, 0.0f, 0.0f, 255.0f), XMFLOAT2(0.0f, 0.0f), 0.0f },
			{ XMFLOAT3(size, -size, size), XMFLOAT4(255.0f, 0.0f, 255.0f, 255.0f), XMFLOAT2(0.0f, 0.0f), 0.0f },
			{ XMFLOAT3(size, size, -size), XMFLOAT4(255.0f, 255.0f, 0.0f, 255.0f), XMFLOAT2(0.0f, 0.0f), 0.0f },
			{ XMFLOAT3(size, size, size), XMFLOAT4(255.0f, 255.0f, 255.0f, 255.0f), XMFLOAT2(0.0f, 0.0f), 0.0f },
		};

        D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
        vertexBufferData.pSysMem = cubeVertices;
        vertexBufferData.SysMemPitch = 0;
        vertexBufferData.SysMemSlicePitch = 0;
        CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
        ThrowIfFailed(
            m_d3dDevice->CreateBuffer(
            &vertexBufferDesc,
            &vertexBufferData,
            &m_vertexBuffer
            )
            );

        // Load mesh indices. Each triple of indices represents
        // a triangle to be rendered on the screen.
        // For example, 0,2,1 means that the vertices with indexes
        // 0, 2 and 1 from the vertex buffer compose the 
        // first triangle of this mesh.
        static const unsigned short cubeIndices [] =
        {
            0, 2, 1, // -x
            1, 2, 3,

            4, 5, 6, // +x
            5, 7, 6,

            0, 1, 5, // -y
            0, 5, 4,

            2, 6, 7, // +y
            2, 7, 3,

            0, 4, 6, // -z
            0, 6, 2,

            1, 3, 7, // +z
            1, 7, 5,
        };

        m_indexCount = ARRAYSIZE(cubeIndices);

        D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
        indexBufferData.pSysMem = cubeIndices;
        indexBufferData.SysMemPitch = 0;
        indexBufferData.SysMemSlicePitch = 0;
        CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
        ThrowIfFailed(
            m_d3dDevice->CreateBuffer(
            &indexBufferDesc,
            &indexBufferData,
            &m_indexBuffer
            )
            );
    });

    // Once the cube is loaded, the object is ready to be rendered.
    createCubeTask.then([this]() {
        m_loadingComplete = true;
    });
}

void D3DPanel::CreateSizeDependentResources()
{
    m_renderTargetView = nullptr;
    m_depthStencilView = nullptr;

    DirectXPanelBase::CreateSizeDependentResources();

    // Create a render target view of the swap chain back buffer.
    ComPtr<ID3D11Texture2D> backBuffer;
    ThrowIfFailed(
        m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))
        );

    // Create render target view.
    ThrowIfFailed(
        m_d3dDevice->CreateRenderTargetView(
        backBuffer.Get(),
        nullptr,
        &m_renderTargetView)
        );

    // Create and set viewport.
    D3D11_VIEWPORT viewport = CD3D11_VIEWPORT(
        0.0f,
        0.0f,
        m_renderTargetWidth,
        m_renderTargetHeight
        );

    m_d3dContext->RSSetViewports(1, &viewport);
	

    // Create depth/stencil buffer descriptor.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(
        DXGI_FORMAT_D24_UNORM_S8_UINT,
        static_cast<UINT>(m_renderTargetWidth),
        static_cast<UINT>(m_renderTargetHeight),
        1,
        1,
        D3D11_BIND_DEPTH_STENCIL
        );

    // Allocate a 2-D surface as the depth/stencil buffer.
    ComPtr<ID3D11Texture2D> depthStencil;
    ThrowIfFailed(
        m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencil)
        );

    // Create depth/stencil view based on depth/stencil buffer.
    const CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = CD3D11_DEPTH_STENCIL_VIEW_DESC(D3D11_DSV_DIMENSION_TEXTURE2D);
    ThrowIfFailed(
        m_d3dDevice->CreateDepthStencilView(
        depthStencil.Get(),
        &depthStencilViewDesc,
        &m_depthStencilView
        )
        );


	//disable depth stencil view
	//create disable stencil view 
	/*D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Create the state using the device.
	DX::ThrowIfFailed(
		m_d3dDevice->CreateDepthStencilState(&depthDisabledStencilDesc, m_depthDisabledStencilState.GetAddressOf())
		);

	//create rasterize state
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = true;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = true;
	rasterDesc.ScissorEnable = true;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	DX::ThrowIfFailed(
		m_d3dDevice->CreateRasterizerState(&rasterDesc, &m_rasterState)
		);*/
	// create the sampler
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
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
	DX::ThrowIfFailed(m_d3dDevice->CreateSamplerState(&samplerDesc, &m_sampler));


	XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));

    /*float aspectRatio = m_renderTargetWidth / m_renderTargetHeight;

    float fovAngleY = 70.0f * XM_PI / 180.0f;
    if (aspectRatio < 1.0f)
    {
        fovAngleY /= aspectRatio;
    }

    XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
        fovAngleY,
        aspectRatio,
        0.01f,
        100.0f
        );

    XMFLOAT4X4 orientation =
        XMFLOAT4X4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );

    XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

    XMStoreFloat4x4(
        &m_constantBufferData.projection,
        XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
        );
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixIdentity()));
	
	return;*/
	//try to use ortho
	
	Environment::GetInstance()->SetDeviceScreen(ceil(m_renderTargetWidth), ceil(m_renderTargetHeight));
	//XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	//XMVECTOR at = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//XMMATRIX mxview = XMMatrixMultiply(XMMatrixIdentity(), XMMatrixLookAtLH(eye, at, up));
	//XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(mxview));
	//XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
	// This sample makes use of a right-handed coordinate system using row-major matrices.
	//XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

	//XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);
	XMMATRIX mxmodel = XMMatrixIdentity();
	////_modelMatrix *= XMMatrixRotationZ(DirectX::XM_PI / 2);
	////_modelMatrix *= XMMatrixTranslation(outputSize.Width, 0.0f, 0.0f);
	//m_matrix.model *= XMMatrixTranspose(XMMatrixTranslation(-100, 0.0f, 0.0f));
	//XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(m_matrix.model));

	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(mxmodel));
	//m_matrix.projection = XMMatrixOrthographicOffCenterLH(0.0f, 5000, 5000, 0.0f, -0.1f, 0.1f);
	//m_matrix.projection = XMMatrixOrthographicOffCenterLH(0.0f, outputSize.W, outputSize.H, 0.0f, -0.1f, 0.1f);
	XMMATRIX mxprojection = XMMatrixOrthographicOffCenterLH(0, m_renderTargetWidth, m_renderTargetHeight, 0, -0.1f, 0.1f);
	XMStoreFloat4x4(&m_constantBufferData.projection, XMMatrixTranspose(mxprojection));
	
}