#pragma once

#include <string>

class MODULE_API TextureImporter final
{
public:

	TextureImporter();
	virtual ~TextureImporter();

	int LoadTextureImage(std::string aPath, int& aOutWidth, int& aOutHeight, int& aOutNrChannels, unsigned char*& aOutImageData);

private:

	int mImageID = 0;

};