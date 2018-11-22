#include "Texture.h"

#include <stb_image.h>

namespace Renderer
{

	TextureData::TextureData()
	{

	}

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

	Texture::Texture() {}

	//void Texture::Initiate(std::string aPath, TextureData aTextureData)
	//{
	//	mData = aTextureData;
	//	mInitiated = GenerateTexture(aPath);
	//}

	const  TextureData*  Texture::GetData() const
	{
		return &mData;
	}

	//bool Texture::IsInitiated() const
	//{
	//	return mInitiated;
	//}

	GLuint Texture::GetTextureID() const
	{
		return mTextureID;
	}

}
