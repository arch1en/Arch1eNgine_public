#include "ATriangle.h"
#include "Engine/Engine.h"
#include "Builders/MeshBuilder.h"
#include "Mesh/MeshTriangle.h"

extern Engine GEngine;

ATriangle::ATriangle()
{
	GEngine.GetMeshBuilder()->NewMesh<MeshTriangle>(mMeshComponent);;
}