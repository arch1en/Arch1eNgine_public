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
	void ImportMeshData(std::shared_ptr<T>& aMesh, std::string aPath, int aMeshIndex)
	{
		static_assert(std::is_base_of<Mesh, T>::value, "T must derive from Mesh !");

		const aiScene* Scene = aiImportFile(aPath.c_str(), 0);
		if (!Scene)
		{
			Log(DebugType::EDT_Warning, "File importing failed on path %s", aPath.c_str());
			return;
		}
		aiMesh* ImportedMesh = Scene->mMeshes[aMeshIndex];

		// Load Vertices
		for (unsigned int i = 0; i < ImportedMesh->mNumVertices; i++)
		{
			aMesh->mPolygonData.Vertices.push_back(Vector3<GLfloat>(ImportedMesh->mVertices[i].x, ImportedMesh->mVertices[i].y, ImportedMesh->mVertices[i].z));
		}
		// Load Faces
		if (ImportedMesh->HasFaces())
		{
			for (unsigned int i = 0; i < ImportedMesh->mNumFaces; i++)
			{
				aMesh->mPolygonData.Faces.push_back(Face(ImportedMesh->mFaces[i].mNumIndices, ImportedMesh->mFaces[i].mIndices));
			}
			aMesh->mPolygonData.ReCalculateNumberOfIndexElements();
		}
	}
};