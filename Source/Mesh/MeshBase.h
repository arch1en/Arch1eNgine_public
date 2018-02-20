////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Mesh Base
//  @author     : Artur Ostrowski
//  @usage      : Base class for all meshes.
//  @version    : 1.0.0
//
////////////////////////////////////////
#pragma once

#include "../stdafx.h"
#include <memory>
#include <Rendering\Textures\Texture.h>

struct PolygonData
{
	std::vector<GLfloat>	Indices;
	std::vector<GLfloat>	Color;
	std::vector<GLuint>	Elements;
	std::vector<GLfloat>	TextureCoordinates;

	PolygonData()
	{}

};

class MeshBase
{

public:
	virtual ~MeshBase() {}

	PolygonData		mPolygonData;
	an::CTexture	mTexture;

protected:

	const char*			MeshTag;

private:

	std::shared_ptr<GLuint> mRefVAO;
	std::shared_ptr<GLuint> mRefVBO;
	std::shared_ptr<GLuint> mRefEBO;
	std::shared_ptr<GLuint> mRefTBO;

};