#include "AMeshActor.h"

AMeshActor::AMeshActor()
{
	mMeshComponent = std::make_shared<MeshComponent>(this);
}