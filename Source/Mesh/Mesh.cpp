////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Mesh Base
//  @author     : Artur Ostrowski
//  @usage      : Base class for all meshes.
//  @version    : 1.0.0
//
////////////////////////////////////////
#include "Mesh.h"

bool Mesh::HasTexture() const
{
	return true;// mTexture->IsInitiated();
}

std::shared_ptr<Texture> Mesh::GetTexture() const
{
	return mTexture;
}