#pragma once

#include "..\Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include "../../game/GameCore/CGame.h"
#include "../../game/Lib/OpenGLES/DirectX11.h"
#include "../Common/DirectXPanelBase.h"

namespace Pine
{
	// This sample renderer instantiates a basic rendering pipeline.
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class Renderer sealed : public DirectXPanelBase
	{
	public:

		property static Renderer^ _this;
		static Renderer^ shareInstance()
		{
			return _this;
		}
		Renderer();
		void StartRenderLoop();
		void StopRenderLoop();
		void StartProcessingInput();
		void StopProcessingInput();
		
		void ReleaseDeviceDependentResources();
		//void Update(DX::StepTimer const& timer);
		void OnKeyUp(Platform::Object ^sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs ^e);
		void OnKeyDown(Platform::Object ^sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs ^e);
		
	private:
		~Renderer();
	private protected:
		virtual void Render() override;
		virtual void CreateDeviceResources() override;
		virtual void CreateSizeDependentResources() override;
		
		virtual void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e) override;
		////virtual void CreateDeviceDependentResources() override;
		////virtual void CreateWindowSizeDependentResources() override;
		

		Microsoft::WRL::ComPtr<IDXGIOutput>                 m_dxgiOutput;
		Windows::Foundation::IAsyncAction^					m_renderLoopWorker;
		// Rendering loop timer.
		DX::StepTimer                                       m_timer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>      m_renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      m_depthStencilView;
		// Cached pointer to device resources.
		////std::shared_ptr<DX::DeviceResources> m_deviceResources;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		m_depthDisabledStencilState;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>		m_rasterState;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>			m_sampler;

		// Direct3D resources for cube geometry.
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_constantBuffer;



		// System resources for cube geometry.
		ModelViewProjectionConstantBuffer	m_constantBufferData;
		ModelViewProjectionMatrix			m_matrix;
		uint32	m_indexCount;

		// Variables used with the rendering loop.
		bool	m_loadingComplete;
		float	m_degreesPerSecond;
		bool	m_tracking;
		bool	m_rendering;
		bool	m_updating;

		float _factor;


		Windows::UI::Core::CoreIndependentInputSource^						m_coreInput;
		Windows::Foundation::IAsyncAction^									m_inputLoopWorker;
		
		void OnPointerPressed(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerMoved(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnPointerReleased(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e);
		void OnAcceleratorKeyActivated(Windows::UI::Core::CoreDispatcher^ dis, Windows::UI::Core::AcceleratorKeyEventArgs^ fuck_microsoft);
	
#if defined(_USE_TEST)
	private:
		ID3D11Debug * m_pDebug;
		DXGI_ADAPTER_DESC									m_dxgiDesc;
		bool m_hasDxgiDesc;
		bool m_support_color16;
	public:
		void printDebugDxgi();
#endif
	};
}

