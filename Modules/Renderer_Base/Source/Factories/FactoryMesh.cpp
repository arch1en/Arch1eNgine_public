////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @author     : Artur Ostrowski
//  @usage      : Handles creation of new meshes onto the scene.
//  @version    : 1.0.0
//
////////////////////////////////////////
#include "stdafx.h"
#include "FactoryMesh.h"
#include "Mesh/Mesh.h"
#include "Mesh/MeshTriangle.h"
#include "Mesh/MeshCube.h"

FactoryMesh::FactoryMesh()
	: mRenderer{ nullptr }
{

}

void FactoryMesh::SetRenderer(std::shared_ptr<Renderer> aRenderer)
{
	mRenderer = aRenderer;
}

void FactoryMesh::SetAllocatorGPU(std::shared_ptr<AllocatorGPU> aAllocator)
{
	mAllocatorGPU = aAllocator;
}

FactoryMesh* FactoryMesh::GetInstance()
{
	static FactoryMesh Instance;
	return &Instance;
}

std::shared_ptr<Mesh> FactoryMesh::NewMesh(std::shared_ptr<MeshComponent> aComponent, std::string InMeshType)
{
	if (mRenderer == nullptr || mAllocatorGPU == nullptr)
	{
		Log(DebugType::EDT_Error, "Mesh creation failed ! Renderer or GPU Allocator is invalid !");
		return nullptr;
	}

	std::shared_ptr<Mesh> Mesh(nullptr);

	if (InMeshType.compare("triangle") == 0)
	{
		Mesh = std::make_shared<MeshTriangle>();
	}
	else if (InMeshType.compare("cube") == 0)
	{
		Mesh = std::make_shared<MeshCube>();
	}

	bool MeshAllocationResult = mAllocatorGPU->AllocateStaticMesh(Mesh.get());
	if (MeshAllocationResult == false)
	{
		Log(DebugType::EDT_Error, "Mesh creation failed !");
		return nullptr;
	}
	aComponent->SetMesh(Mesh);
	mRenderer->AddMeshToDraw(aComponent);

	// Allocate memory for mesh
	// Create Mesh
	// Return Mesh
	mCreatedMeshes.push_back(aComponent);

	return Mesh;
}

void FactoryMesh::DestroyMesh(std::shared_ptr<MeshComponent> aMesh)
{
	if (mCreatedMeshes.size() == 0) return;

	for (auto& iter = mCreatedMeshes.begin(); iter != mCreatedMeshes.end(); iter++)
	{
		// TODO : Test if this works.
		if (*iter == aMesh)
		{
			iter->reset();
			mCreatedMeshes.erase(iter);
		}
	}
}