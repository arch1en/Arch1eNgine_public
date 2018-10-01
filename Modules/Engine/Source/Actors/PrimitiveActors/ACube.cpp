//#include "stdafx.h"
#include "ACube.h"
#include "Engine.h"
#include "Builders/MeshBuilder.h"
#include "Mesh/MeshCube.h"

extern Engine GEngine;

ACube::ACube()
{
	GEngine.GetMeshBuilder()->NewStaticMesh<MeshCube>(mMeshComponent);
}