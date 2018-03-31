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

#include "Engine/Engine.h"

#include "Engine/Builders/TextureBuilder.h"
#include "Engine/Builders/MaterialBuilder.h"

extern Engine GEngine;

MeshCube::MeshCube()
{
	mPolygonData.Vertices = {	   
		// front
		Vector3<GLfloat>(1.0f, -1.0f,  -1.0f),
		Vector3<GLfloat>(1.0f, 1.0f,  -1.0f),
		Vector3<GLfloat>(-1.0f,  1.0f,  -1.0f),
		Vector3<GLfloat>(-1.0f,  -1.0f,  -1.0f),
		// back
		Vector3<GLfloat>(1.0f, -1.0f, 1.0f),
		Vector3<GLfloat>(1.0f, 1.0f, 1.0f),
		Vector3<GLfloat>(-1.0f,  1.0f, 1.0f),
		Vector3<GLfloat>(-1.0f, -1.0f, 1.0f)
	};

	//mPolygonData.Color = {	    
	//	// front colors
	//	Vector3<GLfloat>(1.0f, 0.0f, 0.0f),
	//	Vector3<GLfloat>(0.0f, 1.0f, 0.0f),
	//	Vector3<GLfloat>(0.0f, 0.0f, 1.0f),
	//	Vector3<GLfloat>(1.0f, 1.0f, 1.0f),
	//	// back colors
	//	Vector3<GLfloat>(1.0f, 0.0f, 0.0f),
	//	Vector3<GLfloat>(0.0f, 1.0f, 0.0f),
	//	Vector3<GLfloat>(0.0f, 0.0f, 1.0f),
	//	Vector3<GLfloat>(1.0f, 1.0f, 1.0f)
	//};

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

	mPolygonData.Faces = {			
		// front
		Face(3, 0, 1, 2),
		Face(3, 0, 2, 3),
		// top
		Face(3, 1, 5, 6),
		Face(3, 1, 6, 2),
		// back
		Face(3, 7, 4, 5),
		Face(3, 7, 5, 6),
		// bottom
		Face(3, 4, 0, 3),
		Face(3, 4, 3, 7),
		// left
		Face(3, 3, 2, 6),
		Face(3, 3, 6, 7),
		// right
		Face(3, 4, 5, 1),
		Face(3, 4, 1, 0),

		};

	mPolygonData.ReCalculateNumberOfIndexElements();

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

	mPolygonData.Materials.push_back(GEngine.GetMaterialBuilder()->NewMaterial()); //std::make_shared<Texture>();
	mPolygonData.Materials[0]->AddTexture(GEngine.GetTextureBuilder()->CreateTexture(Data, Paths::GetInstance().GetPathAssets() + "Textures\\dirt.png"));
	mPolygonData.Materials[0]->SetDiffuseColor(Colors::White);
	//mTexture->Initiate(Paths::GetInstance().GetPathAssets() + "Textures\\dirt.png", Data);

	MeshTag = "PrimitiveCube";

}