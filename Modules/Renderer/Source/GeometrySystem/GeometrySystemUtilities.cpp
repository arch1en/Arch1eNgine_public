#include "GeometrySystemUtilities.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/mesh.h>

#include "Debug/LogSystem.h"

bool GeometrySystemUtilities::OpenMesh(const char* Path, Mesh& MeshData)
{
	Assimp::Importer Importer;

	const aiScene* Scene = Importer.ReadFile(Path, 0);
	if (!Scene)
	{
		Log(LogType::Warning, "Error importing mesh. %s", Importer.GetErrorString());
		return false;
	}

	if(Scene->HasMeshes())
	{ 
		MeshData.VerticesPerFace = Scene->mMeshes[0]->mFaces[0].mNumIndices;
		MeshData.Vertices.resize(Scene->mMeshes[0]->mNumVertices);
		MeshData.Indices.resize(Scene->mMeshes[0]->mNumFaces * Scene->mMeshes[0]->mFaces[0].mNumIndices);

		uint16_t IndexID = 0;

		for (unsigned int i = 0; i < Scene->mMeshes[0]->mNumVertices; i++)
		{
			MeshData.Vertices[i].Position.x = (*Scene->mMeshes)->mVertices[i].x;
			MeshData.Vertices[i].Position.y = (*Scene->mMeshes)->mVertices[i].y;
			MeshData.Vertices[i].Position.z = (*Scene->mMeshes)->mVertices[i].z;

			MeshData.Vertices[i].TexCoord.x = (*Scene->mMeshes)->mTextureCoords[0][i].x;
			MeshData.Vertices[i].TexCoord.y = (*Scene->mMeshes)->mTextureCoords[0][i].y;
		}

		for (unsigned int i = 0; i < Scene->mMeshes[0]->mNumFaces; i++)
		{
			for (unsigned int j = 0; j < Scene->mMeshes[0]->mFaces[i].mNumIndices; j++)
			{
				MeshData.Indices[IndexID] = (*Scene->mMeshes)->mFaces[i].mIndices[j]; IndexID++;
			}
		}
	}

	return true;
}