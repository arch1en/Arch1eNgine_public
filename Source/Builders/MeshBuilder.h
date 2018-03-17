////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @author     : Artur Ostrowski
//  @usage      : Handles creation of new meshes onto the scene.
//  @version    : 1.0.0
//	@tags		: [Static]
//
////////////////////////////////////////
#pragma once

#include "stdafx.h"

#include <memory>

#include "Engine/Allocators/AllocatorGPU.h"
#include "Engine/Rendering/Renderer.h"
#include "Debuggers/Assertions.h"

class MeshBase;

enum class MeshType : uint8_t
{
	EMT_PrimitiveTriangle,
	EMT_PrimitiveCube,
	EMT_PrimitivePlane
};

class MeshBuilder
{
public:
	MeshBuilder(std::shared_ptr<Renderer> aRenderer, std::shared_ptr<AllocatorGPU> aAllocator);
	virtual ~MeshBuilder();

	template<class T>
	std::shared_ptr<T> NewMesh(std::shared_ptr<MeshComponent> aComponent)
	{
		static_assert(std::is_base_of<MeshBase, T>::value, "Cannot create new mesh that is not deriving from MeshBase.");

		if (mRenderer == nullptr || mAllocatorGPU == nullptr)
		{
			Log(DebugType::EDT_Error, "Mesh creation failed ! Renderer or GPU Allocator is invalid !");
			return nullptr;
		}

		std::shared_ptr<T> Mesh = std::make_shared<T>();

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

	void DestroyMesh(std::shared_ptr<MeshComponent> aMesh);

private:

	std::shared_ptr<Renderer>		mRenderer;
	std::shared_ptr<AllocatorGPU>	mAllocatorGPU;
	std::vector<std::shared_ptr<MeshComponent>> mCreatedMeshes;

};