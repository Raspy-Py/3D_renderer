#include "Cube.h"
#include "BindableBase.h"

Cube::Cube(Graphics* gfx, float x, float y, float z)
	:
	x(x), y(y), z(z)
{
	auto vertexShader = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
	auto vertexShaderByteCode = vertexShader->GetBlob();
	AddBind(std::move(vertexShader));
	AddBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

	struct Vertex
	{
		Vertex(float x, float y, float z, float r, float g, float b)
			:
			position(x, y, z),
			color(r, g, b)
		{}

		XMFLOAT3 position;
		XMFLOAT3 color;
	};
	//vertex data
	std::vector<Vertex> vertices =
	{	// coords				// color
		{0.5,	-0.5,	0.5,	0.0, 1.0, 0.0}, //  2---1
		{0.5,	 0.5,	0.5,	0.0, 1.0, 0.0}, //  |\  |
		{-0.5,	 0.5,	0.5,	0.0, 0.0, 1.0}, //  |  \|
		{-0.5,	-0.5,	0.5,	0.0, 0.0, 1.0}, //  3---0

		{0.5,	-0.5,	-0.5,	0.0, 1.0, 0.0}, //  6---5
		{0.5,	 0.5,	-0.5,	0.0, 1.0, 0.0}, //  |\  |
		{-0.5,	 0.5,	-0.5,	0.0, 0.0, 1.0}, //  |  \|
		{-0.5,	-0.5,	-0.5,	0.0, 0.0, 1.0}  //  7---4
	};

	// indices
	std::vector<uint16_t> indices =
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

	std::vector<D3D11_INPUT_ELEMENT_DESC> layouts = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));
	AddBind(std::make_unique<InputLayout>(gfx, layouts, vertexShaderByteCode));
	AddBind(std::make_unique<TransformBuffer>(gfx, this));
}

void Cube::Update(float frameTime)
{
	//TODO: add some movement
}

XMMATRIX Cube::GetTransformXM() const
{
	return XMMatrixTranslation(x,y,z);
}
