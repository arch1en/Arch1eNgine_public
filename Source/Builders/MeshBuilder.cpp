////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @author     : Artur Ostrowski
//  @usage      : Handles creation of new meshes onto the scene.
//  @version    : 1.0.0
//
////////////////////////////////////////
#include "MeshBuilder.h"
#include "Mesh/MeshBase.h"
#include "Mesh/MeshTriangle.h"
#include "Mesh/MeshCube.h"

MeshBuilder::MeshBuilder()
	: mRenderer{ nullptr }
{

}

void MeshBuilder::Initialize()
{
	// Dummy. Fill up if needed.
}

void MeshBuilder::SetRenderer(std::shared_ptr<Renderer> aRenderer)
{
	mRenderer = aRenderer;
}

void MeshBuilder::SetAllocatorGPU(std::shared_ptr<AllocatorGPU> aAllocator)
{
	mAllocatorGPU = aAllocator;
}

MeshBuilder* MeshBuilder::GetInstance()
{
	static MeshBuilder Instance;
	return &Instance;
}

void MeshBuilder::DestroyMesh(std::shared_ptr<MeshComponent> aMesh)
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