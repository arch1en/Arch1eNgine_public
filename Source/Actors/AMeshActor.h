////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @author     : Artur Ostrowski
//  @usage      : An actor that already has implemented a mesh component.
//
////////////////////////////////////////
#pragma once

#include "AActor.h"
#include "Components/MeshComponent.h"

class AMeshActor : public AActor
{
public:

	AMeshActor();

protected:

	std::shared_ptr<MeshComponent> mMeshComponent;
};