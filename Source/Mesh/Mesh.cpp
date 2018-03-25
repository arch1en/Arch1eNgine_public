////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Mesh Base
//  @author     : Artur Ostrowski
//  @usage      : Base class for all meshes.
//  @version    : 1.0.0
//
////////////////////////////////////////
#include "Mesh.h"
Face::Face(unsigned int NumIndices, ...)
{
	va_list Args;
	va_start(Args, NumIndices);

	for (unsigned int i = 0; i < NumIndices; i++)
	{
		Indices.push_back(va_arg(Args, unsigned int));
	}

	va_end(Args);
}

Face::Face(unsigned int NumIndices, unsigned int IndicesArray[])
{
	for (unsigned int i = 0; i < NumIndices; i++)
	{
		Indices.push_back(IndicesArray[i]);
	}
}
Face::~Face()
{
	Indices.empty();
}

const unsigned int Face::GetNumIndices() const
{
	return (unsigned int)Indices.size();
}

void PolygonData::ReCalculateNumberOfIndexElements()
{
	NumElements = 0;
	for (const auto& i : Faces)
	{
		NumElements += i.GetNumIndices();
	}
}

void Mesh::AddMaterial(std::shared_ptr<Material> NewMaterial)
{
	mPolygonData.Materials.push_back(NewMaterial);
}