#pragma once

#include "stb_image.h"

class TextureImporter final
{
public:

	TextureImporter();
	virtual ~TextureImporter();

	int LoadTextureImage(std::string aPath, int& aOutWidth, int& aOutHeight, int& aOutNrChannels, unsigned char*& aOutImageData);

private:

	int mImageID = 0;

};