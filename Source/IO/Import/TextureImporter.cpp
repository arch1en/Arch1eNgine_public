#include "stdafx.h"
#include "TextureImporter.h"

#include "Helpers.h"

#include <stb_image.h>
#include <algorithm>

TextureImporter::TextureImporter()
{
}

TextureImporter::~TextureImporter()
{
}

int TextureImporter::LoadTextureImage(std::string aPath, int& aOutWidth, int& aOutHeight, int& aOutNrChannels, unsigned char*& aOutImageData)
{
	std::ifstream file(aPath);

	if (!file.good())
	{
		Log(DebugType::EDT_Error, "Sampler : cannot open file");
		file.close();
		return 1;
	}

	file.close();

	aOutImageData = stbi_load(aPath.c_str(), &aOutWidth, &aOutHeight, &aOutNrChannels, 0);

	if (!aOutImageData)
	{
		Log(DebugType::EDT_Error, "Cannot load image under path : %s", aPath.c_str());
		file.close();
		return 3;
	}


	return 0;
}