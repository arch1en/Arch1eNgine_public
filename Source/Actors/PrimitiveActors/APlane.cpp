#include "APlane.h"
#include "Engine/Engine.h"
#include "Builders/MeshBuilder.h"
#include "Mesh/MeshPlane.h"

extern Engine GEngine;

APlane::APlane()
{
	GEngine.GetMeshBuilder()->NewMesh<MeshPlane>(mMeshComponent);
}