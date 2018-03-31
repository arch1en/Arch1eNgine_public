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
#include "Engine/Engine.h"

class Mesh;

extern Engine GEngine;

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
	std::shared_ptr<T> NewStaticMesh(std::shared_ptr<MeshComponent> aComponent, std::string aPath, int aMeshIndex)
	{
		static_assert(std::is_base_of<Mesh, T>::value, "Cannot create new mesh that is not deriving from Mesh.");

		if (mRenderer == nullptr || mAllocatorGPU == nullptr)
		{
			Log(DebugType::EDT_Error, "Mesh creation failed ! Renderer or GPU Allocator is invalid !");
			return nullptr;
		}

		std::shared_ptr<T> NewMesh = std::make_shared<T>();

		MeshImporter::ImportMeshData<T>(NewMesh, aPath, aMeshIndex);

		PrepareMeshToDraw<T>(NewMesh, aComponent);

		return NewMesh;
	}

	template<class T>
	std::shared_ptr<T> NewStaticMesh(std::shared_ptr<MeshComponent> aComponent)
	{
		static_assert(std::is_base_of<Mesh, T>::value, "Cannot create new mesh that is not deriving from Mesh.");

		if (mRenderer == nullptr || mAllocatorGPU == nullptr)
		{
			Log(DebugType::EDT_Error, "Mesh creation failed ! Renderer or GPU Allocator is invalid !");
			return nullptr;
		}

		std::shared_ptr<T> NewMesh = std::make_shared<T>();

		PrepareMeshToDraw<T>(NewMesh, aComponent);

		return NewMesh;
	}

	//template<class T>
	//std::shared_ptr<T> NewStaticMesh(PolygonData aData, std::shared_ptr<MeshComponent> aComponent)
	//{
	//	static_assert(std::is_base_of<Mesh, T>::value, "Cannot create new mesh that is not deriving from Mesh.");

	//	if (mRenderer == nullptr || mAllocatorGPU == nullptr)
	//	{
	//		Log(DebugType::EDT_Error, "Mesh creation failed ! Renderer or GPU Allocator is invalid !");
	//		return nullptr;
	//	}

	//	std::shared_ptr<T> NewMesh = std::make_shared<T>();
	//	NewMesh->mPolygonData = aData;

	//	PrepareMeshToDraw<T>(NewMesh, aComponent);

	//	return NewMesh;
	//}

	void DestroyMesh(std::shared_ptr<MeshComponent> aMesh);

private:

	template<class T>
	void PrepareMeshToDraw(std::shared_ptr<T> aMesh, std::shared_ptr<MeshComponent> aComponent)
	{
		bool MeshAllocationResult = mAllocatorGPU->AllocateStaticMesh(aMesh.get());
		if (MeshAllocationResult == false)
		{
			Log(DebugType::EDT_Error, "Mesh creation failed !");
			return;
		}
		aComponent->SetMesh(aMesh);
		mRenderer->AddMeshToDraw(aComponent);

		// Allocate memory for mesh
		// Create Mesh
		// Return Mesh
		mCreatedMeshes.push_back(aComponent);
	}

	std::shared_ptr<Renderer>		mRenderer;
	std::shared_ptr<AllocatorGPU>	mAllocatorGPU;
	std::vector<std::shared_ptr<MeshComponent>> mCreatedMeshes;

};