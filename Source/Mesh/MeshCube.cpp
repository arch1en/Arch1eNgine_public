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
		Vector3<GLfloat>(1.0f, -1.0f,  1.0f),
		Vector3<GLfloat>(1.0f, 1.0f,  1.0f),
		Vector3<GLfloat>(-1.0f,  1.0f,  1.0f),
		Vector3<GLfloat>(-1.0f,  -1.0f,  1.0f),
		// back
		Vector3<GLfloat>(1.0f, -1.0f, -1.0f),
		Vector3<GLfloat>(1.0f, 1.0f, -1.0f),
		Vector3<GLfloat>(-1.0f,  1.0f, -1.0f),
		Vector3<GLfloat>(-1.0f, -1.0f, -1.0f)
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
	    //6---------------------/5
	  //  .  				  // |
	//2---------------------1	 |
	//	  . 				|    |
	//	  .                 |	 |
	//	  .					|	 |
	//	  .					|	 |
	//	  .					|	 |
	//	  .					|	 |
	//	  7..............	|	/ 4
	//						| //
	//3--------------------/0

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
		Vector2<GLfloat>(1.f, 0.f),
		Vector2<GLfloat>(1.f, 1.f),
		Vector2<GLfloat>(0.f, 1.f),
		Vector2<GLfloat>(0.f, 0.f),

		Vector2<GLfloat>(1.f, 0.f),
		Vector2<GLfloat>(1.f, 1.f),
		Vector2<GLfloat>(0.f, 1.f),
		Vector2<GLfloat>(0.f, 0.f),
		// back
		//Vector2<GLfloat>(1.f, 1.f),
		//Vector2<GLfloat>(1.f, 0.f),
		//Vector2<GLfloat>(0.f, 0.f),
		//Vector2<GLfloat>(0.f, 1.f),
	};

	TextureData Data(
		ETextureWrappingMode::Repeat,
		ETextureWrappingMode::Repeat,
		ETextureFilteringMode::Nearest,
		ETextureFilteringMode::Linear);

	mTexture = std::make_shared<Texture>();
	//mTexture->Initiate(Paths::GetInstance().GetPathAssets() + "Textures\\dirt.png", Data);

	MeshTag = "PrimitiveCube";

}