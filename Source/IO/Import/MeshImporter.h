#pragma once

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>

#include "Math/Math.h"
#include "Mesh/MeshBase.h"

// TODO : Needs implementation !
class MeshImporter
{
public:

	//template<class T>
	std::unique_ptr<MeshBase> ImportMesh(std::string Path, int MeshIndex)
	{
		//static_assert(std::is_base_of<MeshBase, T>::value, "T must derive from MeshBase !");

		const aiScene* Scene = aiImportFile(Path.c_str(), 0);

		aiMesh* ImportedMesh = Scene->mMeshes[MeshIndex];

		//std::unique_ptr<T> NewMesh = std::make_unique<T>();
		std::unique_ptr<MeshBase> NewMesh = std::make_unique<MeshBase>();

		// Load Vertices
		for (unsigned int i = 0; i < ImportedMesh->mNumVertices; i++)
		{
			NewMesh->mPolygonData.Vertices.push_back(Vector3<GLfloat>(ImportedMesh->mVertices[i].x, ImportedMesh->mVertices[i].y, ImportedMesh->mVertices[i].z));
		}
		//(*ImportedMesh)->Ver

		//NewMesh->mPolygonData

	}
};