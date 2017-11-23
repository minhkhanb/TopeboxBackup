#pragma once
#include <DirectXMath.h>
#include <d3d11_1.h>
#include "Direct3DBase.h"
#include "../../../game/Lib/DataType/CType.h"
////#include "WindowsPhoneCore\PineCanvas.h"
////#include "WindowsPhoneCore\PTextures.h"
////#include "WindowsPhoneCore\PineSprite.h"
////#include "WindowsPhoneCore\CGameCore.h"

struct ModelViewProjectionConstantBuffer
{
	DirectX::XMFLOAT4X4 model;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
};

struct VertexPositionColor
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 tex_coord;
	float map_tex;
};

delegate void RenderRequestBaseService(UINT32 construct_id, UINT32 data_len);
ref class PineRenderer sealed : public Direct3DBase
{
internal:
	//virtual DirectX::XMMATRIX GetOrientTransform3D() override;
	//virtual DirectX::XMMATRIX GetOrientTransform2D() override;
public:
	event RenderRequestBaseService^ OnRequestBaseService;
	PineRenderer();
	//int GetTransformNum(){return _transform_counter;};
	// Direct3DBase methods.
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;
	
	// Method for updating time-dependent objects.
	void Update(float timeTotal, float timeDelta);
	void DetectOrientation(int ori);
public:
	property static PineRenderer^ _this;
	static PineRenderer^ getInstance()
	{
		return _this;
	}
	void NotifyTouchBegin(int touch_id, int touch_x, int touch_y, int touch_tap);
	void NotifyTouchMove(int touch_id, int touch_x, int touch_y, int touch_tap);
	void NotifyTouchEnd(int touch_id, int touch_x, int touch_y, int touch_tap);
	void NotifyPause();
	void NotifyResume();
	void purchateitem(Platform::String^ item_id);
	void CallBaseService(UINT32 construct_id, byte* data, UINT32 data_len);
	void NotifyBackKey();
	bool CaptureScreen();
private:
	Platform::String^ WaitingInAppItem;
	bool m_loadingComplete;
	DirectX::XMMATRIX _modelMatrix;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;

	/*Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState_add;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState_constract;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState_light;*/

	uint32 m_indexCount;
	ModelViewProjectionConstantBuffer m_constantBufferData;
	D3D11_RASTERIZER_DESC rasterDesc;
	ID3D11RasterizerState* m_rasterState;
	D3D11_VIEWPORT viewport;
	float _factor;
	bool _is_init;
	bool is_render;
	bool is_update;

	
};
