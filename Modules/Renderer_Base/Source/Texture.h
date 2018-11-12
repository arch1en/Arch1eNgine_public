////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Texture
//  @author     : Artur Ostrowski
//  @usage      : Keeps data of a texture.
//  @version    : 1.0.0
//
////////////////////////////////////////
#pragma once

#include <vector>
#include "stb_image.h"

enum class ETextureWrappingMode
{
	Repeat,
	MirroredRepeat,
	ClampToEdge,
	ClampToBorder
};

enum class ETextureFilteringMode
{
	Nearest,
	Linear
};

enum class ETextureMipmappingMode
{
	NearestMipmapNearest,
	NearestMipmapLinear,
	LinearMipmapNearest,
	LinearMipmapLinear
};

enum ETextureType
{
	Ambient,
	Albedo,
	Diffuse,
	Specular,
	Normal
};

struct MODULE_API TextureData
{
	std::vector<float> Coordinates;
	std::vector<float> BorderColor;

	ETextureWrappingMode Wrap_S;
	ETextureWrappingMode Wrap_T;
	ETextureFilteringMode Filter_Min;
	ETextureFilteringMode Filter_Mag;

	TextureData() {}
	TextureData(
		ETextureWrappingMode aWrap_S,
		ETextureWrappingMode aWrap_T,
		ETextureFilteringMode aFilter_Min,
		ETextureFilteringMode aFilter_Mag);

	TextureData(const TextureData &aData);
};

class MODULE_API Texture
{
	friend class TextureBuilder;

public:

	Texture();
	//void Initiate(std::string aPath, TextureData aTextureData);
	const TextureData* GetData() const;

	unsigned int GetTextureID() const;
	//bool IsInitiated() const;

private:

	unsigned int	mTextureID;
	TextureData		mData;

	int Width = 0;
	int Height = 0;

};

