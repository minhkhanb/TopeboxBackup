#pragma once
#define ALIGN_BIT_SIZE	16
#define ALIGN __declspec(align(ALIGN_BIT_SIZE))
#define ALIGN_16	__declspec(align(16))
#define ALIGN_32	__declspec(align(32))
namespace Pine
{
	// Constant buffer used to send MVP matrices to the vertex shader.
	ALIGN_16 struct ModelViewProjectionConstantBuffer
	{
		ALIGN_16 DirectX::XMFLOAT4X4 model;
		ALIGN_16 DirectX::XMFLOAT4X4 view;
		ALIGN_16 DirectX::XMFLOAT4X4 projection;
	};
	/*__declspec(align(16))*/ 
	ALIGN_16 struct ModelViewProjectionMatrix{
		ALIGN_16 DirectX::XMMATRIX model;
		ALIGN_16 DirectX::XMMATRIX view;
		ALIGN_16 DirectX::XMMATRIX projection;
	};
	// Used to send per-vertex data to the vertex shader.
	ALIGN_16 struct VertexPositionColor
	{
		ALIGN_16 DirectX::XMFLOAT3 pos;
		ALIGN_16 DirectX::XMFLOAT4 color;
		ALIGN_16 DirectX::XMFLOAT2 tex_coord;
		float map_tex;
	};
}