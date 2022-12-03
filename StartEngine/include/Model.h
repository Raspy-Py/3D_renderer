#pragma once
#include "Mesh.h"

#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <string>
#include <memory>
#include <vector>

class Model
{
public:
	Model(const std::string& FileName);

	void LoadFromFile(const std::string& FileName);
private:
	std::vector<Mesh> meshes;
};

