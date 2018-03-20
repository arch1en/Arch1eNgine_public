#pragma once

#include <stdafx.h>

#include <IL/il.h>

class TextureImporter final
{
public:

	TextureImporter();
	virtual ~TextureImporter();

	const unsigned char* LoadImageFromFile(const char* aFilePath);
	int LoadTextureImage(std::string aPath, int& aOutWidth, int& aOutHeight, int& aOutNrChannels, unsigned char*& aOutImageData);

private:

	ILuint mImageID = 0;

};