////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Application Main
//  @author     : Artur Ostrowski
//  @usage      : Root of the project.
//  @version    : 1.0.0
//
////////////////////////////////////////
////////////////////////////////////////////////////////////////
///
///		Main application of OpenGL program.
///		Here's what needs to be done:
///			1. Initialize SDL
///			2. Create Window.
///			3. Create OpenGL Context and bind it with the window.
///			4. Initialize GLEW (dont forget to glewExperimental = true; if you want to use modern OpenGL stuff.
///			5. Generate and Bind VAO and VBO and pass data into it.
///			6. Enable VSync.
///			7. Clear color to black and swap buffers.
///
////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "Engine.h"
#include "Allocators/AllocatorGPU.h"
#include "IO/Paths.h"
#include "stb_image.h"

#include "Builders/ActorBuilder.h"
#include "Builders/MeshBuilder.h"
#include "Builders/MaterialBuilder.h"
#include "Builders/TextureBuilder.h"

#include "Rendering/Renderer.h"
// TEST
#include "IO/ConfigLoader.h"
#include "Actors/PrimitiveActors/ACube.h"
#include "IO/Import/MeshImporter.h"
// ~TEST
Engine::Engine()
	: Window(nullptr)
	, GLContext()
	, Start(0)
	, Running(false)
	, Event()
	, mAllocatorGPU{ std::make_shared<AllocatorGPU>() }
	, mRenderer{ std::make_shared<Renderer>() }
	, mMainCamera(nullptr)
	, mActorBuilder{std::make_shared<ActorBuilder>()}
{
	mMeshBuilder = std::make_shared<MeshBuilder>(mRenderer, mAllocatorGPU);
	Init();
	Loop();
}

Engine::~Engine()
{
	Destroy();
}

bool Engine::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	Window = SDL_CreateWindow("SDL_OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)WINDOW_WIDTH, (int)WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if(!Window)
		g_sdldie("Unable to create SDL window");

	// Create Context
	GLContext = SDL_GL_CreateContext(Window);
	if (!GLContext)
		g_sdldie("Unable to create OpenGL Context");

	// [Deprecated] Now GLAD takes over GLEW responsibilities.
	// Initialize GLEW
	//glewExperimental = GL_TRUE;
	//GLenum glewError = glewInit();
	//if (glewError != GLEW_OK)
	//{
	//	g_sdldie("Error initializing GLEW! %s\n");
	//	printf("%s\n", glewGetErrorString(glewError));
	//}
		
	// Initialize GLEW and Vertex data
	if (!InitGL())											// InitGL
		g_sdldie("Unable to initialize OpenGL");
	
	// This makes our buffer swap synchronized with monitor vertical refresh (VSync).
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		printf("Warning: Unable to set VSync. SDL Error ! : %s", SDL_GetError());
	}

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SDL_GL_SwapWindow(Window);

	// Smooth the color
	//glShadeModel(GL_SMOOTH);

	// 2D rendering
	//glMatrixMode(GL_MODELVIEW);

	// Reset viewport position to the origin.
	//glLoadIdentity();

	//gluPerspective(90.0, WINDOW_WIDTH/WINDOW_HEIGHT, 1.0, 500.0);

	// Z-Buffer options.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glDepthRange(0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	// Set the area to view in frustum.
	glViewport(0, 0, (GLsizei)WINDOW_WIDTH, (GLsizei)WINDOW_HEIGHT);

	InitializeActors();

	Running = true;

	return true;
}

bool Engine::InitGL()
{

	if (!gladLoadGL())
	{
		g_sdldie("Unable to initialize glew");
		return false;
	}

	mAllocatorGPU->Initialize();
	mRenderer->Initialize();
	////////////////////////////////////////////////////////////////
	///
	///		The correct order of initializing buffers is:
	///		1. Vertex Array Object (VAO)
	///		2. Vertex Buffer Object (VBO)
	///
	///		After that we can use functions like glVertexAttribPointer.
	///		Without both these buffers binded to context, we would have UB.
	///
	////////////////////////////////////////////////////////////////

	ErrorHandle("VertexAttribPointer : ");

	return true;
}

void Engine::InitializeActors()
{
	mMainCamera = mActorBuilder->NewActor<ACamera>();

	// TEST
	std::map<std::string, std::string> test;
	ConfigLoader Loader;

	Loader.LoadConfigData("InputProperties", "Input.Scene", test);
	// ~TEST

	//Actor = GetActorBuilder()->NewActor<ACube>();
	Actor = GetActorBuilder()->NewActor<AMeshActor>();

	std::shared_ptr<Mesh> NewMesh = GetMeshBuilder()->NewStaticMesh<Mesh>(Actor->GetMeshComponent(), Paths::GetInstance().GetPathAssets() + "Models\\TriangulatedSuzanne.obj", 0);
	NewMesh->AddMaterial(GetMaterialBuilder()->NewDefaultMaterial());
}

bool Engine::Loop() 
{
	while (Running)
	{
		Start = SDL_GetTicks();

		/**
		*   Continuous update.
		*/
		mModuleHandler.UpdateModule<InputModule>();

 		while (SDL_PollEvent(&Event))
		{
			/**
			*   Intermittent update.
			*/
			if (Event.type >= SDL_KEYDOWN && Event.type <= SDL_CLIPBOARDUPDATE)
			{
				mModuleHandler.UpdateModule<InputModule>(Event);
			}
			if(Event.type == SDL_QUIT)
			{
				Running = false;
				break;
			}
		}

		mDeltaTimeLast = mDeltaTimeNow;
		mDeltaTimeNow = SDL_GetPerformanceCounter();

		double DeltaTime = (double)((mDeltaTimeNow - mDeltaTimeLast) * 1000 / SDL_GetPerformanceCounter());

		Update(DeltaTime);
		if (mRenderingFailed == false)
		{
			Render();
		}
		SDL_GL_SwapWindow(Window);
	}
	return true;
}

void Engine::Update(double aDeltaTime)
{
	for (const auto& Iter : mActors)
	{	
		Iter->Update((float)aDeltaTime);
	}
}

void Engine::Render()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Last binded vertex array will be drawn by OpenGL, thus we will bind VAO
	glm::mat4 View{ glm::mat4() };

	// Calculate model, view and projection matrices.
	int ErrorCode = mMainCamera->GetCameraComponent()->GetViewMatrix(View);

	if (ErrorCode != 0)
	{
		Log(DebugType::EDT_Error, "Attached actor is invalid.");
	}

	mRenderer->DrawMeshes(mAllocatorGPU->GetActiveVAO(), View);

	glFlush();
}

void Engine::Events()
{

}

void Engine::ErrorHandle(const char* msg)
{
	printf(msg);
	switch (glGetError())
	{
		case GL_NO_ERROR:
			printf("GL_NO_ERROR");
		break;
		case GL_INVALID_ENUM:
			printf("GL_INVALID_ENUM");
		break;
		case GL_INVALID_VALUE:
			printf("GL_INVALID_VALUE");
		break;
		case GL_INVALID_OPERATION:
			printf("GL_INVALID_OPERATION");
		break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			printf("GL_INVALID_FRAMEBUFFER_OPERATION");
		break;
		case GL_OUT_OF_MEMORY:
			printf("GL_OUT_OF_MEMORY");
		break;
		case GL_STACK_UNDERFLOW:
			printf("GL_STACK_UNDERFLOW");
		break;
		case GL_STACK_OVERFLOW:
			printf("GL_STACK_OVERFLOW");
		break;
		default:
			break;
	}
	printf("\n");
}

void Engine::Destroy()
{
	SDL_GL_DeleteContext(GLContext);
	SDL_DestroyWindow(Window);
	SDL_Quit();

}

const std::shared_ptr<ActorBuilder>	Engine::GetActorBuilder() const
{
	return mActorBuilder;
}

const std::shared_ptr<MeshBuilder>	Engine::GetMeshBuilder() const
{
	return mMeshBuilder;
}

const std::shared_ptr<MaterialBuilder>	Engine::GetMaterialBuilder() const
{
	return mMaterialBuilder;
}

const std::shared_ptr<TextureBuilder>	Engine::GetTextureBuilder() const
{
	return mTextureBuilder;
}