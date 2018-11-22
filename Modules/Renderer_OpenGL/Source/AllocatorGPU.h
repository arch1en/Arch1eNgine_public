////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @author     : Artur Ostrowski
//  @usage      : This class is responsible for allocating memory to GPU. Everything that is suppose to be deallocated, has to be deallocated through this class !
//  @version    : 1.0.0
//
////////////////////////////////////////
#ifndef ALLOCATORGPU_H
#define ALLOCATORGPU_H

class Mesh;

#include <vector>

namespace Renderer
{

	struct MeshData
	{
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;
	};

	class AllocatorGPU
	{
	public:
		AllocatorGPU() {}
		~AllocatorGPU();

		void Initialize();

		bool AllocateStaticMesh(Mesh* aMesh);
		//void AllocateTextureData(Mesh* aMesh);

		unsigned int GetActiveVAO() const;

	private:

		unsigned int mActiveVAO;

		std::vector<unsigned int>		mVAOs;	// Vertex Array Object
		std::vector<unsigned int>		mVBOs;	// Vertex Buffer Object
		std::vector<unsigned int>		mTBOs;	// Texture Buffer Object
		std::vector<unsigned int>		mEBOs;	// Element Buffer OBject

		void SetActiveVAO(unsigned int aValue);
	};

}

#endif