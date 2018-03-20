#include "ATriangle.h"
#include "Engine/Engine.h"
#include "Engine/Builders/MeshBuilder.h"
#include "Mesh/MeshTriangle.h"

extern Engine GEngine;

ATriangle::ATriangle()
{
	GEngine.GetMeshBuilder()->NewStaticMesh<MeshTriangle>(mMeshComponent);;
}