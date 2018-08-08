#include "stdafx.h"
#include "TextureImporter.h"

#include "Helpers.h"

#include <SDL_image.h>
#include <algorithm>

TextureImporter::TextureImporter()
{
	ilGenImages(1, &mImageID);
	ilBindImage(mImageID);
}

TextureImporter::~TextureImporter()
{
	ilBindImage(0);
	ilDeleteImage(mImageID);
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
	ilInit();

	if (!ilLoadImage(aPath.c_str()))
	{
		Log(DebugType::EDT_Error, "Cannot load image under path : %s", aPath.c_str());
		file.close();
		return 3;
	}

	aOutWidth = ilGetInteger(IL_IMAGE_WIDTH);
	aOutHeight = ilGetInteger(IL_IMAGE_HEIGHT);

	aOutImageData = new unsigned char[aOutWidth * aOutHeight * 3];

	ilCopyPixels(0, 0, 0, aOutWidth, aOutHeight, 1, IL_RGB, IL_UNSIGNED_BYTE, aOutImageData);

	return 0;
}

const unsigned char* TextureImporter::LoadImageFromFile(const char* aFilePath)
{
	int InitFlags = 0;
	std::string Extension;
	Extension = GetFileExtension(aFilePath);

	if (Extension.size() <= 0)
	{
		Log(DebugType::EDT_Error, "Image loading failed, file extension not found.");
		return nullptr;
	}

	// Convert extension to lowercase, so comparing will be possible.
	std::transform(Extension.begin(), Extension.end(), Extension.begin(), ::tolower);

	if (Extension.compare("jpg") == 0)
	{
		InitFlags |= IMG_INIT_JPG;
	}
	if (Extension.compare("png") == 0)
	{
		InitFlags |= IMG_INIT_PNG;
	}
	if (Extension.compare("tif") == 0)
	{
		InitFlags |= IMG_INIT_TIF;
	}

	// All filetypes recognised by SDL (case doesnt matter ?)
	//"BMP"
	//	"CUR"
	//	"GIF"
	//	"ICO"
	//	"JPG"
	//	"LBM"
	//	"PCX"
	//	"PNG"
	//	"PNM"
	//	"TGA"
	//	"TIF"
	//	"XCF"
	//	"XPM"
	//	"XV"

	if (InitFlags == 0)
	{
		Log(DebugType::EDT_Error, "Image loading failed, file extension not recognised.");
		return nullptr;
	}

	// TODO : Loading images using SDL is super heavy, we need to find a way to load images all by once.
	int InitResult = IMG_Init(InitFlags);

	if ((InitResult & InitFlags) != InitFlags)
	{
		Log(DebugType::EDT_Error, "Failed to initialize \"%s\" image extension modules", Extension);
		Log(DebugType::EDT_Notice, "%s", IMG_GetError());
		return nullptr;
	}

	SDL_Surface* NewImageSurface = IMG_Load(aFilePath);

	// Unload resources after loading images.
	IMG_Quit();

	return nullptr;
}