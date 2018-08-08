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

#include "IL/il.h"

enum ETextureType
{
	Ambient,
	Specular,

};

enum ETextureWrappingMode
{
	Repeat = GL_REPEAT,
	MirroredRepeat = GL_MIRRORED_REPEAT,
	ClampToEdge = GL_CLAMP_TO_EDGE,
	ClampToBorder = GL_CLAMP_TO_BORDER
};

enum ETextureFilteringMode
{
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR
};

enum ETextureMipmappingMode
{
	NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
	NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
	LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
	LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
};

enum ETextureType
{
	Albedo,
	Diffuse,
	Specular,
	Normal
};

struct TextureData
{
	std::vector<GLfloat> Coordinates;
	std::vector<GLfloat> BorderColor;

	GLenum Wrap_S;
	GLenum Wrap_T;
	GLenum Filter_Min;
	GLenum Filter_Mag;

	TextureData() {}
	TextureData(
		ETextureWrappingMode aWrap_S,
		ETextureWrappingMode aWrap_T,
		ETextureFilteringMode aFilter_Min,
		ETextureFilteringMode aFilter_Mag);

	TextureData(const TextureData &aData);
};

class Texture
{
	friend class TextureBuilder;

public:

	Texture();
	//void Initiate(std::string aPath, TextureData aTextureData);
	const TextureData* GetData() const;

	GLuint GetTextureID() const;
	//bool IsInitiated() const;

private:

	GLuint			mTextureID;
	TextureData		mData;

	int Width = 0;
	int Height = 0;

};

