#include "Material.h"

void Material::AddTexture(std::shared_ptr<Texture> NewTexture)
{
	mTextures.push_back(NewTexture);
}

const bool Material::HasTextures() const
{
	return mTextures.size() > 0;
}

const std::vector<std::shared_ptr<Texture>>*	Material::GetTextures() const
{
	return &mTextures;
}

void Material::SetDiffuseColor(Color aColor)
{
	mDiffuse = aColor;
}

void Material::SetSpecularColor(Color aColor)
{
	mSpecular = aColor;
}

const Color*	Material::GetDiffuseColor() const
{
	return &mDiffuse;
}

const Color*	Material::GetSpecularColor() const
{
	return &mSpecular;
}