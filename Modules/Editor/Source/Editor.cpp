#include "Editor.h"

bool Editor::Initiate()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	Window = SDL_CreateWindow("SDL_OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)WINDOW_WIDTH, (int)WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!Window)
		g_sdldie("Unable to create SDL window");



	// [Deprecated] Now GLAD takes over GLEW responsibilities.
	// Initialize GLEW
	//glewExperimental = GL_TRUE;
	//GLenum glewError = glewInit();
	//if (glewError != GLEW_OK)
	//{
	//	g_sdldie("Error initializing GLEW! %s\n");
	//	printf("%s\n", glewGetErrorString(glewError));
	//}



	// This makes our buffer swap synchronized with monitor vertical refresh (VSync).


	InitializeActors();

	Running = true;

	return true;
}

bool Editor::InitiateRenderer()
{
	// Create Context
	GLContext = SDL_GL_CreateContext(Window);
	if (!GLContext)
		g_sdldie("Unable to create OpenGL Context");

	// Initialize GLEW and Vertex data
	if (!InitGL())											// InitGL
		g_sdldie("Unable to initialize OpenGL");

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
}