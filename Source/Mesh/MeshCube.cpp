////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Mesh Cube
//  @author     : Artur Ostrowski
//  @usage      : Hardcoded cube mesh for testing purposes.
//  @version    : 1.0.0
//
////////////////////////////////////////
#include "MeshCube.h"
#include "IO/Paths.h"
#include "Math/Math.h"

MeshCube::MeshCube()
{
	mPolygonData.Vertices = {	   
		// front
		Vector3<GLfloat>(-1.0f, -1.0f,  1.0f),
		Vector3<GLfloat>(1.0f, -1.0f,  1.0f),
		Vector3<GLfloat>(1.0f,  1.0f,  1.0f),
		Vector3<GLfloat>(-1.0f,  1.0f,  1.0f),
		// back
		Vector3<GLfloat>(-1.0f, -1.0f, -1.0f),
		Vector3<GLfloat>(1.0f, -1.0f, -1.0f),
		Vector3<GLfloat>(1.0f,  1.0f, -1.0f),
		Vector3<GLfloat>(-1.0f,  1.0f, -1.0f)
	};

	mPolygonData.Color = {	    
		// front colors
		Vector3<GLfloat>(1.0f, 0.0f, 0.0f),
		Vector3<GLfloat>(0.0f, 1.0f, 0.0f),
		Vector3<GLfloat>(0.0f, 0.0f, 1.0f),
		Vector3<GLfloat>(1.0f, 1.0f, 1.0f),
		// back colors
		Vector3<GLfloat>(1.0f, 0.0f, 0.0f),
		Vector3<GLfloat>(0.0f, 1.0f, 0.0f),
		Vector3<GLfloat>(0.0f, 0.0f, 1.0f),
		Vector3<GLfloat>(1.0f, 1.0f, 1.0f)
	};

	mPolygonData.Elements = {			
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 0, 3,
		3, 7, 4,
		// left
		4, 5, 1,
		1, 0, 4,
		// right
		3, 2, 6,
		6, 7, 3,

		};

	mPolygonData.TextureCoordinates = 
	{
		// front
		Vector2<GLfloat>(1.f, 1.f),
		Vector2<GLfloat>(1.f, 0.f),
		Vector2<GLfloat>(0.f, 0.f),
		Vector2<GLfloat>(0.f, 1.f),

	};

	an::STextureData Data(
		an::ETextureWrappingMode::MirroredRepeat,
		an::ETextureWrappingMode::MirroredRepeat,
		an::ETextureFilteringMode::Nearest,
		an::ETextureFilteringMode::Linear);

	mTexture = std::make_shared<an::CTexture>();
	mTexture->Initiate(an::Paths::GetInstance().GetPathAssets() + "Textures\\cheshire.jpg", Data);

	MeshTag = "PrimitiveCube";

}