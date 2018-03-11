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
#include <Rendering/Textures/Texture.h>
#include <Math/Math.h>

struct PolygonData
{
	std::vector<Vector3<GLfloat>>	Vertices;
	std::vector<Vector3<GLfloat>>	Color;
	std::vector<GLuint>		Elements;
	std::vector<Vector2<GLfloat>>	TextureCoordinates;

	PolygonData()
	{}

};

class MeshBase
{

public:
	virtual ~MeshBase() {}

	PolygonData		mPolygonData;
	
	std::shared_ptr<an::CTexture> GetTexture() const;
	bool HasTexture() const;

protected:

	const char*			MeshTag;

	std::shared_ptr<an::CTexture>	mTexture;

private:

	std::shared_ptr<GLuint> mRefVAO;
	std::shared_ptr<GLuint> mRefVBO;
	std::shared_ptr<GLuint> mRefEBO;
	std::shared_ptr<GLuint> mRefTBO;

};