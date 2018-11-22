#pragma once

#include "Texture.h"
#include "Color.h"

#include <vector>
#include <memory>

class MODULE_API Material
{
public:

	void AddTexture(std::shared_ptr<Renderer::Texture> NewTexture);
	const bool HasTextures() const;
	const std::vector<std::shared_ptr<Renderer::Texture>>*	GetTextures() const;

	void SetDiffuseColor (const Color& aColor);
	void SetSpecularColor(const Color& aColor);

	Color	GetDiffuseColor() const;
	Color	GetSpecularColor() const;

private:

	Color														mDiffuseColor;
	Color														mSpecularColor;
	std::vector<std::shared_ptr<Renderer::Texture>>						mTextures;

};