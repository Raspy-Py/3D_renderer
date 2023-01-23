#pragma once
#include "Drawable.h"
#include "BindableBase.h"

#include <assimp/scene.h>           
#include <assimp/postprocess.h>     

#include <string>
#include <memory>
#include <vector>

class Mesh : public Drawable
{
public:
	Mesh(Graphics* gfx, std::vector<std::unique_ptr<Bindable>> bindables);
	~Mesh() override = default;

	void Draw(Graphics* gfx, XMMATRIX transform);
};

class Node
{
public:
	Node(Node* parent, aiNode* nodeData);

	XMMATRIX transform;
	std::vector<std::unique_ptr<Node>> children;
	std::vector<Mesh*> meshes;
};

class Model
{
	struct CameraData
	{
		XMMATRIX viewProj;
		XMFLOAT3 camPos;
	};
public:
	Model(Graphics* gfx, 
		const std::string& modelFile,
		const std::wstring& vertexShader, 
		const std::wstring& pixelShader);
	~Model() = default;
	void Draw(Graphics* gfx);

private:
	void Draw(Graphics* gfx, Node* node, XMMATRIX transform);
	std::unique_ptr<Node> ParseNode(aiNode* srcNode);
	std::unique_ptr<Mesh> ParseMesh(Graphics* gfx, aiMesh* scene);

private:

	XMMATRIX rootTransform;
	CameraData cameraData;
	std::unique_ptr<Node> rootNode;
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::unique_ptr<VertexConstantBuffer<XMMATRIX>> modelTransformCB;
	std::unique_ptr<VertexConstantBuffer<CameraData>> cameraDataCB;
};

