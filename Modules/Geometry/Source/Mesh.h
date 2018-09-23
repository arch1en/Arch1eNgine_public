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

#include <memory>
#include <Math/Math.h>

#include "Engine/Rendering/Material.h"

struct Face
{
	Face() = delete;
	Face(unsigned int NumIndices, ...);
	Face(unsigned int NumIndices, unsigned int IndicesArray[]);

	~Face();

	std::vector<unsigned int> Indices;

	const unsigned int GetNumIndices() const;

};

struct PolygonData
{
	PolygonData()
	{}

	unsigned int NumElements;

	std::vector<Vector3<GLfloat>>	Vertices;
	std::vector<Vector3<GLfloat>>	VertexNormals;
	std::vector<Face>				Faces;
	std::vector<Vector2<GLfloat>>	TextureCoordinates;
	std::vector<std::shared_ptr<Material>>	Materials;

	void ReCalculateNumberOfIndexElements();

};

class Mesh
{

public:
	virtual ~Mesh() {}

	PolygonData		mPolygonData;
	
	void AddMaterial(std::shared_ptr<Material> NewMaterial);

protected:

	const char*					MeshTag;

private:

	std::shared_ptr<GLuint> mRefVAO;
	std::shared_ptr<GLuint> mRefVBO;
	std::shared_ptr<GLuint> mRefEBO;
	std::shared_ptr<GLuint> mRefTBO;

};