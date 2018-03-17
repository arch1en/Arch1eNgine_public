////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Mesh Base
//  @author     : Artur Ostrowski
//  @usage      : Base class for all meshes.
//  @version    : 1.0.0
//
////////////////////////////////////////
#include "MeshBase.h"

bool MeshBase::HasTexture() const
{
	return mTexture->IsInitiated();
}

std::shared_ptr<Texture> MeshBase::GetTexture() const
{
	return mTexture;
}