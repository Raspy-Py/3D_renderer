#pragma once
#include "CleanWindows.h"
#include "Graphics.h"
#include "Camera.h"

#include <string>
#include <wrl.h>
#include <DirectXMath.h>
#include <memory>

namespace dx = DirectX;
namespace wrl = Microsoft::WRL;

class Renderer
{
	struct Vertex 
	{
		Vertex(
			float x, float y, float z, float w,
			float r, float g, float b, float a
		)
			:
			position(x,y,z,w), color(r,g,b,a)
		{};
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 color;
	};
	struct CBPerObject 
	{
		XMMATRIX WVP;
	};
public:
	Renderer(Graphics* gfx, const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath);
	~Renderer();

	void DoFrame(float frameTime);
private:
	Graphics* gfx;

	// Camera stuff
	Camera* pCamera;
	CBPerObject cbPerObject;

	// COM objects
	wrl::ComPtr<ID3DBlob> pVertexShaderByteCode;
	wrl::ComPtr<ID3DBlob> pPixelShaderByteCode;

	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;

	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
	wrl::ComPtr<ID3D11Buffer> pWVPConstBuffer;
	static DxgiInfoManager& GetInfoManager(Graphics& gfx);
};

