////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @author     : Artur Ostrowski
//  @usage      : This class is responsible for allocating memory to GPU. Everything that is suppose to be deallocated, has to be deallocated through this class !
//  @version    : 1.0.0
//
////////////////////////////////////////

#include "AllocatorGPU.h"

#include "Debuggers/Debugger.h"

void AllocatorGPU::Initialize()
{
	GLuint NewVAO;
	glGenVertexArrays(1, &NewVAO);
	glBindVertexArray(NewVAO);
	mVAOs.push_back(NewVAO);
	SetActiveVAO(NewVAO);
}

AllocatorGPU::~AllocatorGPU()
{
	if (mVBOs.size() > 0)
	{
		glDeleteBuffers(mVBOs.size(), &mVBOs[0]);
	}
	if (mEBOs.size() > 0)
	{
		glDeleteBuffers(mEBOs.size(), &mEBOs[0]);
	}
	if (mVAOs.size() > 0)
	{
		glDeleteVertexArrays(mVAOs.size(), &mVAOs[0]);
	}
	Log(DebugType::EDT_Notice, "AllocatorGPU::Dtor - All buffers are destroyed.");
}

bool AllocatorGPU::AllocateStaticMesh(Mesh* aMesh)
{
	GLuint NewVBO;
	GLuint NewEBO;
	GLuint NewTBO;

	GLsizeiptr IndicesBufferSize = sizeof(aMesh->mPolygonData.Vertices[0]) * aMesh->mPolygonData.Vertices.size();
	//GLsizeiptr ColorBufferSize = sizeof(aMesh->mPolygonData.Materials[0]->GetDiffuseColor()->r) * aMesh->mPolygonData.Color.size();
	GLsizeiptr ElementBufferSize = sizeof(aMesh->mPolygonData.Faces[0].Indices[0]) * aMesh->mPolygonData.NumElements;
	GLsizeiptr TextureBufferSize = sizeof(aMesh->mPolygonData.TextureCoordinates[0]) * aMesh->mPolygonData.TextureCoordinates.size();

	GLsizeiptr CombinedArrayBufferSize = IndicesBufferSize + /*ColorBufferSize +*/ TextureBufferSize;

	if (IndicesBufferSize == 0)
	{
		Log(DebugType::EDT_Error, "Index Buffer is empty.");
		return false;
	}
	//if (ColorBufferSize == 0)
	//{
	//	Log(DebugType::EDT_Warning, "Color Buffer is empty.");
	//	return false;
	//}
	if (ElementBufferSize == 0)
	{
		Log(DebugType::EDT_Error, "Element Buffer is empty.");
		return false;
	}

	if (TextureBufferSize == 0)
	{
		Log(DebugType::EDT_Notice, "Texture Buffer is empty.");
	}

	glGenBuffers(1, &NewVBO);
	glGenBuffers(1, &NewEBO);

	if (TextureBufferSize > 0)
	{
		glGenBuffers(1, &NewTBO);
	}

	glBindBuffer(GL_ARRAY_BUFFER, NewVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NewEBO);

	GLintptr Offset = 0;

	glBufferData(GL_ARRAY_BUFFER, CombinedArrayBufferSize, 0, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, Offset, IndicesBufferSize, &aMesh->mPolygonData.Vertices[0]);
		Offset += IndicesBufferSize;
		//glBufferSubData(GL_ARRAY_BUFFER, Offset, ColorBufferSize, &aMesh->mPolygonData.Color[0]);
	if (TextureBufferSize > 0)
	{
		//Offset += ColorBufferSize;
		glBufferSubData(GL_ARRAY_BUFFER, Offset, TextureBufferSize, &aMesh->mPolygonData.TextureCoordinates[0]);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)IndicesBufferSize);
	if (TextureBufferSize > 0)
	{
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(IndicesBufferSize/* + ColorBufferSize*/));
	}

	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	if (TextureBufferSize > 0)
	{
		glEnableVertexAttribArray(1);
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ElementBufferSize, &aMesh->mPolygonData.Faces[0].Indices[0], GL_STATIC_DRAW);
	GLintptr ElementBufferOffset = 0;
	for (int i = 0; i < aMesh->mPolygonData.Faces.size(); i++)
	{
		GLintptr FaceBufferSize = sizeof(aMesh->mPolygonData.Faces[i].Indices[0]) * aMesh->mPolygonData.Faces[i].GetNumIndices();
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, ElementBufferOffset, FaceBufferSize, &aMesh->mPolygonData.Faces[i].Indices[0]);
		ElementBufferOffset += FaceBufferSize;
	}

	mVBOs.push_back(NewVBO);
	mEBOs.push_back(NewEBO);
	if (TextureBufferSize > 0)
	{
		mTBOs.push_back(NewTBO);
	}

	return true;
}

void AllocatorGPU::SetActiveVAO(GLuint aValue)
{
	mActiveVAO = aValue;
}

GLuint AllocatorGPU::GetActiveVAO() const
{
	return mActiveVAO;
}