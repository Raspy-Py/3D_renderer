#pragma once
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <string>
#include <memory>

class SceneImporter
{
public:
	SceneImporter(const std::string& sceneFile);

private:
	aiScene* pScene;
};

