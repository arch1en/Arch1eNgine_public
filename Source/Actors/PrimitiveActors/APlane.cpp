#include "APlane.h"
#include "Engine/Engine.h"
#include "Engine/Builders/MeshBuilder.h"
#include "Mesh/MeshPlane.h"

extern Engine GEngine;

APlane::APlane()
{
	GEngine.GetMeshBuilder()->NewStaticMesh<MeshPlane>(mMeshComponent);
}

APlane::~APlane()
{}