////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Application Main
//  @author     : Artur Ostrowski
//  @usage      : Root of the project.
//  @version    : 1.0.0
//
////////////////////////////////////////
#pragma once

#include "Core/ModuleHandler.h"
#include "Actors/ACamera.h"
#include "Actors/AMeshActor.h"

class ActorBuilder;
class MeshBuilder;
class MaterialBuilder;
class TextureBuilder;

class AllocatorGPU;
class Renderer;

class Engine final
{
public:

	const enum VAO_IDs
	{
		Triangles = 0,
		NumVAOs,
		NumVBOs
	};

	const enum Buffer_IDs
	{
		ArrayBuffer = 0,
		NumBuffers
	};

	const enum Attrib_IDs
	{
		vPosition = 0
	};

	Engine();
	virtual ~Engine();

	const std::shared_ptr<ActorBuilder>		GetActorBuilder() const;
	const std::shared_ptr<MeshBuilder>		GetMeshBuilder() const;
	const std::shared_ptr<MaterialBuilder>	GetMaterialBuilder() const;
	const std::shared_ptr<TextureBuilder>	GetTextureBuilder() const;

private:

	bool Init();
	bool InitGL();
	void InitializeActors();

	/**
	*   Responsible for loading core modules that all other systems are dependent on.
	*/
	void LoadCoreModules();

	bool Loop();
	void Update(double aDeltaTime);
	void Render();
	void Events();
	void ErrorHandle(const char* msg);
	void Destroy();

	bool mRenderingFailed = false;

	Uint64	mDeltaTimeNow;
	Uint64	mDeltaTimeLast;

	SDL_Window *	Window;
	SDL_GLContext	GLContext;
	Uint32			Start;
	bool			Running;
	SDL_Event		Event;
	ModuleHandler	mModuleHandler;
	std::shared_ptr<ACamera> mMainCamera;

	// OpenGL vars

	std::shared_ptr<AllocatorGPU>	mAllocatorGPU;
	std::shared_ptr<Renderer>		mRenderer;

	// Builders
	std::shared_ptr<ActorBuilder>		mActorBuilder;
	std::shared_ptr<MeshBuilder>		mMeshBuilder;
	std::shared_ptr<MaterialBuilder>	mMaterialBuilder;
	std::shared_ptr<TextureBuilder>		mTextureBuilder;

	std::vector<std::shared_ptr<AActor>> mActors;

	// TEMP
	std::shared_ptr<AMeshActor> Actor;
	// ~TEMP

}; 