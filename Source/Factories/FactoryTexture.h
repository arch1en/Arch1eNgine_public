#pragma once

#include "stdafx.h"


namespace an
{
	class CTexture;
	class STextureData;
};

class FactoryTexture
{
public:

	FactoryTexture() {}
	~FactoryTexture() {}

	const unsigned char* LoadImageFromFile(const char* aFilePath);
	void CreateTexture(an::STextureData aData);

private:

	std::vector<std::unique_ptr<an::CTexture>> mTextures;

};