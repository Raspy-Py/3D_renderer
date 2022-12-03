#include "SceneImporter.h"

#include <exception>

SceneImporter::SceneImporter(const std::string& sceneFile)
{
	Assimp::Importer importer;

	pScene = const_cast<aiScene*>(importer.ReadFile(sceneFile,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType));
	
}
