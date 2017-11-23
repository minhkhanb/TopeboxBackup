//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
//
//*********************************************************

#pragma once
#include "pch.h"
#include "../Common/DirectXPanelBase.h"
#include "../Common/StepTimer.h"
#include "ShaderStructures.h"
namespace Pine
{
    // Hosts a DirectX rendering surface that draws a spinning 3D cube using Direct3D.

    [Windows::Foundation::Metadata::WebHostHidden]
	public ref class D3DPanel sealed : public Pine::DirectXPanelBase
    {
    public:
        D3DPanel();

        void StartRenderLoop();
        void StopRenderLoop();

    private protected:

        virtual void Render() override;
        virtual void CreateDeviceResources() override;
        virtual void CreateSizeDependentResources() override;

		DXGI_ADAPTER_DESC									m_dxgiDesc;
        Microsoft::WRL::ComPtr<IDXGIOutput>                 m_dxgiOutput;

        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>      m_renderTargetView;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      m_depthStencilView;
        Microsoft::WRL::ComPtr<ID3D11VertexShader>          m_vertexShader;
        Microsoft::WRL::ComPtr<ID3D11PixelShader>           m_pixelShader;
        Microsoft::WRL::ComPtr<ID3D11InputLayout>           m_inputLayout;
        Microsoft::WRL::ComPtr<ID3D11Buffer>                m_vertexBuffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer>                m_indexBuffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer>                m_constantBuffer;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		m_depthDisabledStencilState;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>		m_rasterState;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>			m_sampler;
		

		ModelViewProjectionConstantBuffer					m_constantBufferData;

        uint32                                              m_indexCount;

        float	                                            m_degreesPerSecond;

        Windows::Foundation::IAsyncAction^					m_renderLoopWorker;
        // Rendering loop timer.
        DX::StepTimer                                       m_timer;

    private:
        ~D3DPanel();
    };
}