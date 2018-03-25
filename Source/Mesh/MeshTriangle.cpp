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
#include "Engine/Engine.h"
#include "Engine/Builders/MaterialBuilder.h"

extern Engine GEngine;

MeshTriangle::MeshTriangle()
{
	mPolygonData.Vertices =  {	
		Vector3<GLfloat>(-1.0f, -1.0f, +0.0f),
		Vector3<GLfloat>(+0.0f, +1.0f, +0.0f),
		Vector3<GLfloat>(+1.0f, -1.0f, +0.0f)
	};

	mPolygonData.Faces = {
		Face(3, 0, 1, 2)
	};

	mPolygonData.ReCalculateNumberOfIndexElements();

	mPolygonData.TextureCoordinates = {
		Vector2<GLfloat>(0.0f, 0.0f),
		Vector2<GLfloat>(0.5f, 1.0f),
		Vector2<GLfloat>(1.0f, 0.0f)
	};

	mPolygonData.Materials.push_back(GEngine.GetMaterialBuilder()->NewDefaultMaterial());

	MeshTag = "Primitive2DTriangle";
}
