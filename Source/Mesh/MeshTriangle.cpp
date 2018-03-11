////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Mesh Triangle
//  @author     : Artur Ostrowski
//  @usage      : Hardcoded triangle mesh for testing purposes.
//  @version    : 1.0.0
//
////////////////////////////////////////
#include "MeshTriangle.h"

MeshTriangle::MeshTriangle()
{
	mPolygonData.Vertices =  {	
		Vector3<GLfloat>(-1.0f, -1.0f, +0.0f),
		Vector3<GLfloat>(+0.0f, +1.0f, +0.0f),
		Vector3<GLfloat>(+1.0f, -1.0f, +0.0f)
	};

	mPolygonData.Color = {
		Vector3<GLfloat>(1.0f, 0.0f, 0.0f),
		Vector3<GLfloat>(0.0f, 1.0f, 0.0f),
		Vector3<GLfloat>(0.0f, 0.0f, 1.0f)
	};

	mPolygonData.Elements = { 0, 1, 2 };

	mPolygonData.TextureCoordinates = {
		Vector2<GLfloat>(0.0f, 0.0f),
		Vector2<GLfloat>(0.5f, 1.0f),
		Vector2<GLfloat>(1.0f, 0.0f)
	};

	MeshTag = "Primitive2DTriangle";
}
