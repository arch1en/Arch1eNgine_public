//#include "stdafx.h"
#include "MeshComponent.h"
#include "Factories\FactoryMesh.h"

void MeshComponent::SetMesh(std::shared_ptr<Mesh> aMesh)
{
	mMesh = aMesh;
}

void MeshComponent::Update(float aDeltaTime)
{
	ActorComponent::Update(aDeltaTime);
}

std::shared_ptr<Mesh> MeshComponent::GetMesh() const
{
	return mMesh;
}