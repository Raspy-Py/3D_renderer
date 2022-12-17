#pragma once
#include "Node.h"
#include "Mesh.h"
#include "Drawable.h"

#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <string>
#include <memory>
#include <vector>

class Model : public Drawable
{
public:
	Model(const std::string& FileName);

	void Draw(Graphics* gfx);
	void LoadFromFile(const std::string& FileName);
private:
	std::vector<Mesh> meshes;
	std::unique_ptr<Node> rootNode;
};

