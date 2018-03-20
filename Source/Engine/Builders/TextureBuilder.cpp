#include "TextureBuilder.h"	

#include "Engine/Rendering/Textures/Texture.h"
#include "IO/Import/TextureImporter.h"

#include <memory>

std::unique_ptr<Texture>  TextureBuilder::CreateTexture(TextureData aData, std::string aPath)
{
	std::unique_ptr<Texture> NewTexture = std::make_unique<Texture>();

	GLenum Target = GL_TEXTURE_2D;

	glGenTextures(1, &NewTexture->mTextureID);
	glBindTexture(Target, NewTexture->mTextureID);

	int NrChannels;
	unsigned char* Data = nullptr;

	TextureImporter TexImp;

	auto Width = 0;
	auto Height = 0;

	if (TexImp.LoadTextureImage(aPath, Width, Height, NrChannels, Data) == 0)
	{
		glTexParameteri(Target, GL_TEXTURE_WRAP_S, aData.Wrap_S);
		glTexParameteri(Target, GL_TEXTURE_WRAP_T, aData.Wrap_T);
		glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, aData.Filter_Min);
		glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, aData.Filter_Mag);

		glTexImage2D			// Allocate memory on the GPU
		(
			Target,		// Texture target
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
			Target,		// Texture target
			0,					// Level of Detail
			0,					// X Offset
			0,					// Y Offset
			Width,				// Width of an image
			Height,				// Height of an image
			GL_RGB,				// Format of the pixels in the array that will be loaded
			GL_UNSIGNED_BYTE,	// Datatype of the coordinates in the array that will be loaded
			Data				// Data (image array)
		);

		glGenerateMipmap(Target);
	}
	else
	{
		Log(DebugType::EDT_Warning, "");
		return false;
	}

	NewTexture->Width = Width;
	NewTexture->Height = Height;
	NewTexture->mData = aData;

	glBindTexture(Target, 0);
	Data = nullptr;

	return NewTexture;
}