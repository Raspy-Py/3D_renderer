#include "Model.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


Model::Model(Graphics* gfx, const std::string& modelFile, const std::wstring& vertexShaderFile, const std::wstring& pixelShaderFile)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelFile,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices
	);

	rootTransform = XMMatrixIdentity();

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		meshes.push_back(ParseMesh(gfx, scene->mMeshes[i]));
	}
	rootNode = ParseNode(scene->mRootNode);
}

void Model::Draw(Graphics* gfx)
{
	Draw(gfx, rootNode.get(), rootTransform);
}

void Model::Draw(Graphics* gfx, Node* node, XMMATRIX transform)
{
	transform *= node->transform;

	for (auto* mesh : node->meshes)
	{
		mesh->Draw(gfx, transform);
	}

	for (auto& child : node->children)
	{
		Draw(gfx, child.get(), transform);
	}
}

std::unique_ptr<Node> Model::ParseNode(aiNode* srcNode)
{
	return std::unique_ptr<Node>();
}

std::vector<std::unique_ptr<Mesh>> Model::ParseMesh(Graphics* gfx, aiMesh* mesh)
{
	return std::vector<std::unique_ptr<Mesh>>();
}

Mesh::Mesh(Graphics* gfx, std::vector<std::unique_ptr<Bindable>> bindables)
{
	for (auto& bindable : bindables)
	{
		if (auto pIndexBuffer = dynamic_cast<IndexBuffer*>(bindable.get()))
		{
			AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, pIndexBuffer));
			bindable.release();
		}
		else
		{
			AddBind(std::move(bindable));
		}
	}
}

void Mesh::Draw(Graphics* gfx, XMMATRIX transform)
{
	for (auto& bind : binds)
	{
		if (auto pTransform = dynamic_cast<VertexConstantBuffer<XMMATRIX>*>(bind.get()))
		{
			pTransform->Update(gfx, transform);
		}
		bind->Bind(gfx);
	}

	Drawable::Draw(gfx);
}
