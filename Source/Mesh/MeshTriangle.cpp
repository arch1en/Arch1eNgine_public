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
	mPolygonData.Indices =  {	
		-1.0f, -1.0f, +0.0f,
		+0.0f, +1.0f, +0.0f,
		+1.0f, -1.0f, +0.0f, };

	mPolygonData.Color = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f, };

	mPolygonData.Elements = { 0, 1, 2 };

	mPolygonData.TextureCoordinates = {
		0.0f, 0.0f,
		0.5f, 1.0f,
		1.0f, 0.0f,
	};

	MeshTag = "Primitive2DTriangle";
}
