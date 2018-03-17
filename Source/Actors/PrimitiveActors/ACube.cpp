#include "ACube.h"
#include "Engine/Engine.h"
#include "Builders/MeshBuilder.h"
#include "Mesh/MeshCube.h"

extern Engine GEngine;

ACube::ACube()
{

	GEngine.GetMeshBuilder()->NewMesh<MeshCube>(mMeshComponent);
	//FactoryMesh* Factory = FactoryMesh::GetInstance();
	//Factory->NewMesh(mMeshComponent, "cube");
}