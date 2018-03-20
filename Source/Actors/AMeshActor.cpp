#include "AMeshActor.h"

AMeshActor::AMeshActor()
{
	mMeshComponent = std::make_shared<MeshComponent>(this);
}

std::shared_ptr<MeshComponent> AMeshActor::GetMeshComponent()
{
	return mMeshComponent;
}