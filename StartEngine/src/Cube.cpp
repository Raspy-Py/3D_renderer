#include "Cube.h"
#include "BindableBase.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

Cube::Cube(Graphics* gfx, float x, float y, float z)
	:
	x(x), y(y), z(z)
{
	auto vertexShader = std::make_unique<VertexShader>(gfx, L"PhongLightVS.cso");
	auto vertexShaderByteCode = vertexShader->GetBlob();

	struct Vertex
	{
		Vertex(
			float x, float y, float z, 
			float u, float v, 
			float nx, float ny, float nz)
			:
			position(x, y, z),
			texCoords(u, v),
			normal(nx, ny, nz)
		{}

		XMFLOAT3 position;
		XMFLOAT2 texCoords;
		XMFLOAT3 normal;
	};
	// Importing model

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile("./data/scenes/alien/alien.obj",
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices);

	std::vector<uint16_t> indices;
	std::vector<Vertex> vertices;

	for (int n = 0; n < 1; n++)
	{
		const auto mesh = scene->mMeshes[0];

		float scale = 0.02;

		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			const auto& vertex = mesh->mVertices[i];
			const auto& texCoord = mesh->mTextureCoords[0][i];
			const auto& normal = mesh->mNormals[i];
			vertices.push_back({
					vertex.x * scale,vertex.y * scale,vertex.z * scale,
					texCoord.x,texCoord.y,
					normal.x, normal.y,	normal.z,
				});
		}

		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			const auto& face = mesh->mFaces[i];
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}
	}
/*
	std::vector<Vertex> vertices =
	{	
		// front
		{-1	,1	,-1	,0, 1,0,0,-1},
		{-1	,-1	,-1	,0, 0,0,0,-1},
		{1	,-1	,-1	,1, 0,0,0,-1},
		{1	,1	,-1	,1, 1,0,0,-1},

		// top
		{-1	,1	,1	,0, 1,0,1,0},
		{-1	,1	,-1	,0, 0,0,1,0},
		{1	,1	,-1	,1, 0,0,1,0},
		{1	,1	,1	,1, 1,0,1,0},

		// bottom
		{-1	,-1	,-1	,0, 1,0,-1,0},
		{-1	,-1	,1	,0, 0,0,-1,0},
		{1	,-1	,1	,1, 0,0,-1,0},
		{1	,-1	,-1	,1, 1,0,-1,0},

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
*/
	std::vector<D3D11_INPUT_ELEMENT_DESC> layouts = 
	{
		{"POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20,	D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	Camera* cam = Camera::GetInstance();

	sceneTransform.world = XMMatrixIdentity();
	sceneTransform.viewProj = cam->GetViewMatrix() * cam->GetProjectionMatrix();
	sceneTransform.camPos = cam->GetPositionFloat3();

	AddBind(std::move(vertexShader));
	AddBind(std::make_unique<PixelShader>(gfx, L"PhongLightPS.cso"));
	//AddBind(std::make_unique<Texture>(gfx, L"./data/scenes/alien/alien.dds", 0));
	//AddBind(std::make_unique<Sampler>(gfx, 0));
	AddBind(std::make_unique<Texture>(gfx, L"./data/images/test_surface.dds", 1));
	AddBind(std::make_unique<Sampler>(gfx, 1));
	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));
	AddBind(std::make_unique<InputLayout>(gfx, layouts, vertexShaderByteCode));

	AddBind(std::make_unique<VertexConstantBuffer<SceneTransform>>(gfx, 0, sceneTransform));
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));
}

void Cube::Update(Graphics* gfx, float frameTime)
{
	Camera* cam = Camera::GetInstance();
	sceneTransform.viewProj = cam->GetViewMatrix() * cam->GetProjectionMatrix();
	sceneTransform.camPos = cam->GetPositionFloat3();
	sceneTransform.world = XMMatrixIdentity();

	// search for Vertex Constant Buffer
	for (const auto& b : binds)
	{
		if (const auto p = dynamic_cast<VertexConstantBuffer<SceneTransform>*>(b.get()))
		{
			p->Update(gfx, sceneTransform);
		}
	}
}
