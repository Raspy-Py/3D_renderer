#include "Renderer.h"
#include "GraphicsExceptionMacroses.h"
#include <vector>

Renderer::Renderer(Graphics* gfx, const wchar_t* vertexShaderPath, const wchar_t* pixelShaderPath)
	:
	gfx(gfx)
{
	pCamera = Camera::GetInstance();
	INFOMAN(gfx);

	ID3D11Device* pDevice = gfx->GetDevice();

	GFX_THROW_INFO(D3DReadFileToBlob(vertexShaderPath, &pVertexShaderByteCode));
	GFX_THROW_INFO(D3DReadFileToBlob(pixelShaderPath, &pPixelShaderByteCode));
	
	// creating shader objects
	GFX_THROW_INFO(pDevice->CreateVertexShader(
		pVertexShaderByteCode->GetBufferPointer(),
		pVertexShaderByteCode->GetBufferSize(),
		nullptr,
		&pVertexShader
	));

	GFX_THROW_INFO(pDevice->CreatePixelShader(
		pPixelShaderByteCode->GetBufferPointer(),
		pPixelShaderByteCode->GetBufferSize(),
		nullptr,
		&pPixelShader
	));
	
	//vertex data
	std::vector<Vertex> vertices = 
	{	// coords					 // color
		{0.5,	-0.5,	0.5,	1.0, 0.0, 1.0, 0.0, 1.0}, //  2---1
		{0.5,	 0.5,	0.5,	1.0, 0.0, 1.0, 0.0, 1.0}, //  |\  |
		{-0.5,	 0.5,	0.5,	1.0, 0.0, 0.0, 1.0, 1.0}, //  |  \|
		{-0.5,	-0.5,	0.5,	1.0, 0.0, 0.0, 1.0, 1.0}, //  3---0

		{0.5,	-0.5,	-0.5,	1.0, 0.0, 1.0, 0.0, 1.0}, //  6---5
		{0.5,	 0.5,	-0.5,	1.0, 0.0, 1.0, 0.0, 1.0}, //  |\  |
		{-0.5,	 0.5,	-0.5,	1.0, 0.0, 0.0, 1.0, 1.0}, //  |  \|
		{-0.5,	-0.5,	-0.5,	1.0, 0.0, 0.0, 1.0, 1.0}  //  7---4
	};

	// indices
	std::vector<UINT> indices =
	{
		0,1,2, // front
		0,2,3,

		4,5,1, // top
		4,1,0,

		7,6,5, // back
		7,5,4,

		3,2,6, // down
		3,6,7,

		1,5,6, // right
		1,6,2,

		4,0,3, // left
		4,3,7
	};

	/*
	*  WVP
	*/
	
	// Camera set up
	float FOVRad = 70.0f / 180.0f * 3.141592;
	pCamera->SetProjectionValues(70.0f, 800.0f / 450.0f, 0.1f, 100.0f);
	pCamera->AdjustPosition(0, 0, -2);

	//filling descriptors for input layout and vertex buffer
	std::vector<D3D11_INPUT_ELEMENT_DESC> layouts =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// creating buffers
	D3D11_BUFFER_DESC vbd = {};
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.CPUAccessFlags = 0u;
	vbd.MiscFlags = 0u;
	vbd.ByteWidth = UINT(sizeof(Vertex) * vertices.size());
	vbd.StructureByteStride = sizeof(Vertex);

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = UINT(indices.size() * sizeof(UINT));
	ibd.StructureByteStride = sizeof(UINT);

	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.CPUAccessFlags = 0u;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(CBPerObject);

	// subresource structure³
	D3D11_SUBRESOURCE_DATA vsd = {};
	vsd.pSysMem = vertices.data();

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();

	// creating vertex buffer
	GFX_THROW_INFO(pDevice->CreateBuffer(&vbd, &vsd, &pVertexBuffer));
	// creating index buffer
	GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));
	// creating constant buffer
	GFX_THROW_INFO(pDevice->CreateBuffer(&cbd, nullptr, &pWVPConstBuffer));
	// creating input layout
	GFX_THROW_INFO(pDevice->CreateInputLayout(
		layouts.data(), (UINT)layouts.size(),
		pVertexShaderByteCode->GetBufferPointer(),
		pVertexShaderByteCode->GetBufferSize(),
		&pInputLayout
	));
}

Renderer::~Renderer()
{
}

void Renderer::DoFrame(float frameTime)
{
	INFOMAN(gfx);
	ID3D11DeviceContext* pDeviceContext = gfx->GetDeviceContext();
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;

	pDeviceContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->IASetInputLayout(pInputLayout.Get());
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pDeviceContext->VSSetConstantBuffers(0, 1, pWVPConstBuffer.GetAddressOf());
	pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	// Clearing screen
	gfx->ClearFrame(1.0, 1.0, 1.0);

	// Cube WVP
	XMMATRIX view = pCamera->GetViewMatrix();
	XMMATRIX projection = pCamera->GetProjectionMatrix();
	XMMATRIX world = XMMatrixIdentity();
	cbPerObject.WVP = world * view * projection;
	pDeviceContext->UpdateSubresource(pWVPConstBuffer.Get(), 0, nullptr, &cbPerObject, 0, 0);

	// Draw
	GFX_THROW_INFO_ONLY(pDeviceContext->DrawIndexed(36, 0, 0));

    //Present the backbuffer to the screen
    gfx->GetSwapChain()->Present(1, 0);
}

DxgiInfoManager& Renderer::GetInfoManager(Graphics* gfx)
{
#if !NDEBUG
	return gfx->infoManager;
#endif
}
