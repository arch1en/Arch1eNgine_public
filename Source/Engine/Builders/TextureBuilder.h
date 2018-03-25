#pragma once

#include <stdafx.h>

class Texture;
struct TextureData;

class TextureBuilder
{
public:

	TextureBuilder() {}
	~TextureBuilder() {}

	std::shared_ptr<Texture>  CreateTexture(TextureData aData, std::string aPath);

private:

	std::vector<std::unique_ptr<Texture>> mTextures;

};