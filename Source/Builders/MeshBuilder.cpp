////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @author     : Artur Ostrowski
//  @usage      : Handles creation of new meshes onto the scene.
//  @version    : 1.0.0
//
////////////////////////////////////////
#include "MeshBuilder.h"

MeshBuilder::MeshBuilder(std::shared_ptr<Renderer> aRenderer, std::shared_ptr<AllocatorGPU> aAllocator)
	: mRenderer{ aRenderer }
	, mAllocatorGPU {aAllocator}
{}

MeshBuilder::~MeshBuilder()
{
	for (int i = mCreatedMeshes.size(); i < 0; i--)
	{
		mCreatedMeshes[i].reset();
		mCreatedMeshes[i] = nullptr;
	}

	mCreatedMeshes.clear();
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