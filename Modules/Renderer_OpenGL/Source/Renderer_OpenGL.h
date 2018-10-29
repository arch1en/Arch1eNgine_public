////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Renderer
//  @author     : Artur Ostrowski
//  @usage      : Base class responsible for rendering tasks.
//  @version    : 1.0.0
//
////////////////////////////////////////
#ifndef RENDERER_OPENGL_H
#define RENDERER_OPENGL_H

#include <stack>

#include "Renderer_Base.h"

//#include "MeshComponent.h"
#include "ShaderProgram.h"
#include "Utilities/RendererUtilities.h"

namespace Renderer
{
	class MODULE_API OpenGL : public Renderer::Base
	{
	public:
		OpenGL();
		~OpenGL();

		virtual bool Initialize() override;
		// Renderer must be attached to the window, before creating context
		virtual void CreateContext() override;
		virtual void AttachToWindow(SDL_Window* Window);
		void AttachContextToWindow(SDL_Window* Window);
		/**
		*   Draws meshes that are on RenderingStack.
		*/
		virtual void DrawMeshes(const DrawData aDrawData) override;

	private:

		std::unique_ptr<SDL_GLContext> Context;

		SDL_Window* mApplicationWindow;

	};
}


#endif