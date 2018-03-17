#pragma once

#include "stdafx.h"

class Texture;
struct TextureData;

class TextureBuilder
{
public:

	TextureBuilder() {}
	~TextureBuilder() {}

	const unsigned char* LoadImageFromFile(const char* aFilePath);
	std::unique_ptr<Texture>  CreateTexture(TextureData aData);

private:

	std::vector<std::unique_ptr<Texture>> mTextures;

};