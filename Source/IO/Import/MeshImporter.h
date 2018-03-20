#pragma once

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>

#include "Math/Math.h"
#include "Mesh/Mesh.h"

// TODO : Needs implementation !
namespace MeshImporter
{
	template<class T>
	std::shared_ptr<T> ImportMesh(std::string aPath, int aMeshIndex)
	{
		static_assert(std::is_base_of<Mesh, T>::value, "T must derive from Mesh !");

		const aiScene* Scene = aiImportFile(Path.c_str(), 0);

		aiMesh* ImportedMesh = Scene->mMeshes[aMeshIndex];

		std::shared_ptr<T> NewMesh = std::make_unique<Mesh>();

		// Load Vertices
		for (unsigned int i = 0; i < ImportedMesh->mNumVertices; i++)
		{
			NewMesh->mPolygonData.Vertices.push_back(Vector3<GLfloat>(ImportedMesh->mVertices[i].x, ImportedMesh->mVertices[i].y, ImportedMesh->mVertices[i].z));
		}

		return NewMesh;
	}
};