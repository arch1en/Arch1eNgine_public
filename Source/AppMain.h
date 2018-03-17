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

#include "stdafx.h"
#include "Factories/FactoryMesh.h"
#include "Factories/FactoryActor.h"
#include "Builders/ActorBuilder.h"
#include "Engine/Rendering/Renderer.h"
#include "Engine/Core/ModuleHandler.h"
#include "Actors/ACamera.h"

#include "Pools/TextureObjectPool.h"
#include "Pools/MaterialObjectPool.h"


class AppMain
{
public:

	enum VAO_IDs
	{
		Triangles = 0,
		NumVAOs,
		NumVBOs
	};

	enum Buffer_IDs
	{
		ArrayBuffer = 0,
		NumBuffers
	};

	enum Attrib_IDs
	{
		vPosition = 0
	};

	AppMain();
	~AppMain();

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

private:

	bool mRenderingFailed = false;

	Uint64	mDeltaTimeNow;
	Uint64	mDeltaTimeLast;

	SDL_Window *	Window;
	SDL_GLContext	GLContext;
	Uint32			Start;
	bool			Running;

	Engine			

	SDL_Event		Event;
	ModuleHandler	mModuleHandler;
	std::unique_ptr<ACamera> mMainCamera;

	// Builders

	ActorBuilder	mActorBuilder;

	// ObjectPools
	std::unique_ptr<TextureObjectPool>	mTextureObjectPool;
	std::unique_ptr<MaterialObjectPool> mMaterialObjectPool;

	std::vector<std::shared_ptr<AActor>> mActors;

	// OpenGL vars
	std::shared_ptr<AllocatorGPU>	mAllocatorGPU;
	std::shared_ptr<Renderer>		mRenderer;

	// TEMP
	std::shared_ptr<AActor> Actor;
	// ~TEMP

}; 