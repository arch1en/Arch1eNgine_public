#pragma once

#include <stdafx.h>

class Texture;
struct TextureData;

class TextureBuilder
{
public:

	TextureBuilder() {}
	~TextureBuilder() {}

	std::unique_ptr<Texture>  CreateTexture(TextureData aData, std::string aPath);

private:

	std::vector<std::unique_ptr<Texture>> mTextures;

};