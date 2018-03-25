#pragma once

#include <stdafx.h>

#include "Engine/Rendering/Textures/Texture.h"
#include "Math/Color.h"

class Material
{
public:

	void AddTexture(std::shared_ptr<Texture> NewTexture);
	const bool HasTextures() const;
	const std::vector<std::shared_ptr<Texture>>*	GetTextures() const;

	void SetDiffuseColor(Color aColor);
	void SetSpecularColor(Color aColor);

	const Color*	GetDiffuseColor() const;
	const Color*	GetSpecularColor() const;

private:

	Color														mDiffuse;
	Color														mSpecular;
	std::vector<std::shared_ptr<Texture>>						mTextures;

};