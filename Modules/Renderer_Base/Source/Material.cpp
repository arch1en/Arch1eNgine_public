//#include "stdafx.h"
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

void Material::SetDiffuseColor(const Color& aColor)
{
	mDiffuseColor = aColor;
}

void Material::SetSpecularColor(const Color& aColor)
{
	mSpecularColor = aColor;
}

Color	Material::GetDiffuseColor() const
{
	return mDiffuseColor;
}

Color	Material::GetSpecularColor() const
{
	return mSpecularColor;
}