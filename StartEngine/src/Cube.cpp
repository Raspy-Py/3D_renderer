#include "Cube.h"
#include "BindableBase.h"

Cube::Cube(Graphics* gfx, float x, float y, float z)
	:
	x(x), y(y), z(z)
{
	auto vertexShader = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
	auto vertexShaderByteCode = vertexShader->GetBlob();

	struct Vertex
	{
		Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
			:
			position(x, y, z),
			texCoords(u, v),
			normal(nx, ny, nz)
		{}

		XMFLOAT3 position;
		XMFLOAT2 texCoords;
		XMFLOAT3 normal;
	};
	//vertex data
	std::vector<Vertex> vertices =
	{	
		// front
		{-1	,1	,-1	,0, 1,0,0,-1},
		{-1	,-1	,-1	,0, 0,0,0,-1},
		{1	,-1	,-1	,1, 0,0,0,-1},
		{1	,1	,-1	,1, 1,0,0,-1},

		// top
		{-1	,1	,1	,0, 1,1,0,0},
		{-1	,1	,-1	,0, 0,1,0,0},
		{1	,1	,-1	,1, 0,1,0,0},
		{1	,1	,1	,1, 1,1,0,0},

		// bottom
		{-1	,-1	,-1	,0, 1,-1,0,0},
		{-1	,-1	,1	,0, 0,-1,0,0},
		{1	,-1	,1	,1, 0,-1,0,0},
		{1	,-1	,-1	,1, 1,-1,0,0},

		// left
		{-1	,1	,1	,0, 1,-1,0,0},
		{-1	,-1	,1	,0, 0,-1,0,0},
		{-1	,-1	,-1	,1, 0,-1,0,0},
		{-1	,1	,-1	,1, 1,-1,0,0},

		// right
		{1	,1	,-1	,0, 1,1,0,0},
		{1	,-1	,-1	,0, 0,1,0,0},
		{1	,-1	,1	,1, 0,1,0,0},
		{1	,1	,1	,1, 1,1,0,0},

		// back
		{-1	,-1	,1	,0, 1,0,0,1},
		{-1	,1	,1	,0, 0,0,0,1},
		{1	,1	,1	,1, 0,0,0,1},
		{1	,-1	,1	,1, 1,0,0,1}
	};

	// indices
	std::vector<uint16_t> indices =
	{
		// front
		0,1,2,
		0,2,3,

		// top
		4,5,6,
		4,6,7,

		// bottom
		8,9,10,
		8,10,11,

		// left
		12,13,14,
		12,14,15,

		// right
		16,17,18,
		16,18,19,

		// back
		20,21,22,
		20,22,23
	};

	std::vector<D3D11_INPUT_ELEMENT_DESC> layouts = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	Camera* cam = Camera::GetInstance();

	wvp.world = GetTransformXM();
	wvp.viewProj = cam->GetViewMatrix() * cam->GetProjectionMatrix();

	AddBind(std::move(vertexShader));
	AddBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));
	AddBind(std::make_unique<Texture>(gfx, L"./data/images/fence_1024.dds"));
	AddBind(std::make_unique<Sampler>(gfx));
	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));
	AddBind(std::make_unique<InputLayout>(gfx, layouts, vertexShaderByteCode));

	AddBind(std::make_unique<VertexConstantBuffer<WVP>>(gfx, wvp));
	//AddBind(std::make_unique<TransformBuffer>(gfx, this));

	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));
}

void Cube::Update(Graphics* gfx, float frameTime)
{
	Camera* cam = Camera::GetInstance();
	wvp.viewProj = cam->GetViewMatrix() * cam->GetProjectionMatrix();
	for (const auto& b : binds)
	{
		if (const auto p = dynamic_cast<VertexConstantBuffer<WVP>*>(b.get()))
		{
			p->Update(gfx, wvp);
		}
	}
}

XMMATRIX Cube::GetTransformXM() const
{
	return XMMatrixTranslation(x,y,z);
}
