#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <vector>

class Mesh
{
public:
	Mesh() = default;
	~Mesh() = default;
private:
	std::vector<uint32_t> indices;
	std::vector<aiVector3D> vertices;
};