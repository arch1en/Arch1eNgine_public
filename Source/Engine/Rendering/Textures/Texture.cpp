#include "Texture.h"

#include <stb_image.h>

TextureData::TextureData
	(
	ETextureWrappingMode aWrap_S,
	ETextureWrappingMode aWrap_T,
	ETextureFilteringMode aFilter_Min,
	ETextureFilteringMode aFilter_Mag
	)
{
	Wrap_S = aWrap_S;
	Wrap_T = aWrap_T;
	Filter_Min = aFilter_Min;
	Filter_Mag = aFilter_Mag;
}

TextureData::TextureData(const TextureData &aData)
{
	Wrap_S = aData.Wrap_S;
	Wrap_T = aData.Wrap_T;
	Filter_Min = aData.Filter_Min;
	Filter_Mag = aData.Filter_Mag;
	Coordinates = aData.Coordinates;
	BorderColor = aData.BorderColor;
}

Texture::Texture(){}

void Texture::Initiate(std::string aPath, TextureData aTextureData)
{
	mData = aTextureData;
	mInitiated = GenerateTexture(aPath);
}

bool Texture::GenerateTexture(std::string aPath)
{
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	ILuint ImageID = 0;

	ilGenImages(1, &ImageID);
	ilBindImage(ImageID);

	int NrChannels;
	unsigned char* Data = nullptr;

	if (LoadTextureImage(ImageID, aPath, Width, Height, NrChannels, Data) == 0)
	{
		GLenum Target = GL_TEXTURE_2D;

		glTexParameteri(Target, GL_TEXTURE_WRAP_S, mData.Wrap_S);
		glTexParameteri(Target, GL_TEXTURE_WRAP_T, mData.Wrap_T);
		glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, mData.Filter_Min);
		glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, mData.Filter_Mag);

		glTexImage2D			// Allocate memory on the GPU
		(
			GL_TEXTURE_2D,		// Texture target
			0,					// Level of Detail
			GL_RGB,				// Internal pixel format (format in which pixels will be stored in GPU)
			Width,				// Width of an image
			Height,				// Height of an image
			0,					// Wether it has a border
			GL_RGB,				// Format of the pixels in the array that will be loaded
			GL_UNSIGNED_BYTE,	// Datatype of the coordinates in the array that will be loaded
			nullptr				// Data (image array)
		);

		glTexSubImage2D			// Load data onto the GPU. Function starts to load image from coord (0,0). Remember this.
		(
			GL_TEXTURE_2D,		// Texture target
			0,					// Level of Detail
			0,					// X Offset
			0,					// Y Offset
			Width,				// Width of an image
			Height,				// Height of an image
			GL_RGB,				// Format of the pixels in the array that will be loaded
			GL_UNSIGNED_BYTE,	// Datatype of the coordinates in the array that will be loaded
			Data				// Data (image array)
		);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		Log(DebugType::EDT_Warning, "");
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	Data = nullptr;
	ilBindImage(0);
	ilDeleteImage(ImageID);

	return true;
}

int Texture::LoadTextureImage(ILuint ImageID, std::string path, int& aOutWidth, int& aOutHeight, int& aOutNrChannels, unsigned char*& aOutImageData)
{
	std::ifstream file(path);

	if (!file.good())
	{
		SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Sampler : cannot open file");
		file.close();
		return 1;
	}

	file.close();
	ilInit();

	if (!ilLoadImage(path.c_str()))
	{
		Log(DebugType::EDT_Error, "Cannot load image under path : %s", path.c_str());
		return 3;
	}

	aOutWidth = ilGetInteger(IL_IMAGE_WIDTH);
	aOutHeight = ilGetInteger(IL_IMAGE_HEIGHT);

	aOutImageData = new unsigned char[aOutWidth * aOutHeight * 3];

	ilCopyPixels(0, 0, 0, aOutWidth, aOutHeight, 1, IL_RGB, IL_UNSIGNED_BYTE, aOutImageData);

	return 0;
}

const  TextureData*  Texture::GetData() const
{
	return &mData;
}

bool Texture::IsInitiated() const
{
	return mInitiated;
}

GLuint Texture::GetTextureID() const
{
	return mTextureID;
}
