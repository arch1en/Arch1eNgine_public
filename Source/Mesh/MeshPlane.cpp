////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @author     : Artur Ostrowski
//  @usage      : Hardcoded plane mesh for testing purposes.
//
////////////////////////////////////////
#include "MeshPlane.h"
#include "IO/Paths.h"
#include "Math/Math.h"

MeshPlane::MeshPlane()
{
	mPolygonData.Vertices = {
		// front
		Vector3<GLfloat>(1.0f, -1.0f,  1.0f),
		Vector3<GLfloat>(1.0f, 1.0f,  1.0f),
		Vector3<GLfloat>(-1.0f,  1.0f,  1.0f),
		Vector3<GLfloat>(-1.0f,  -1.0f,  1.0f),
	};

	mPolygonData.Color = {
		// front colors
		Vector3<GLfloat>(1.0f, 0.0f, 0.0f),
		Vector3<GLfloat>(0.0f, 1.0f, 0.0f),
		Vector3<GLfloat>(0.0f, 0.0f, 1.0f),
		Vector3<GLfloat>(1.0f, 1.0f, 1.0f),

	};

	//2---------------------1
	//	    				| 
	//	                    |
	//	  					|
	//	  					|
	//	  					|
	//	   					|
	//						|
	//						| 
	//3--------------------/0

	mPolygonData.Elements = {
		// front
		0, 1, 2,
		2, 3, 0,
	};

	mPolygonData.TextureCoordinates =
	{
		// front
		Vector2<GLfloat>(1.f, 1.f),
		Vector2<GLfloat>(1.f, 0.f),
		Vector2<GLfloat>(0.f, 0.f),
		Vector2<GLfloat>(0.f, 1.f),

	};

	TextureData Data(
		ETextureWrappingMode::MirroredRepeat,
		ETextureWrappingMode::MirroredRepeat,
		ETextureFilteringMode::Nearest,
		ETextureFilteringMode::Linear);

	mTexture = std::make_shared<Texture>();
	mTexture->Initiate(Paths::GetInstance().GetPathAssets() + "Textures\\cheshire.jpg", Data);

	MeshTag = "PrimitivePlane";
}