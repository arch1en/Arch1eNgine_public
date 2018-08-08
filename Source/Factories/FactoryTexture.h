#pragma once

class Texture;
struct TextureData;

class FactoryTexture
{
public:

	FactoryTexture() {}
	~FactoryTexture() {}

	const unsigned char* LoadImageFromFile(const char* aFilePath);
	void CreateTexture(TextureData aData);

private:

	std::vector<std::unique_ptr<Texture>> mTextures;

};