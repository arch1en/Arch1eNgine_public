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

#include "Renderer.h"

#include "MeshComponent.h"
#include "ShaderProgram.h"
#include "Utilities/RendererUtilities.h"

namespace Renderer
{
	class OpenGL : public Renderer
	{
	public:
		OpenGL(RendererType aRendererType);
		~OpenGL();

		virtual bool Initialize() override;
		// Renderer must be attached to the window, before creating context
		virtual void CreateContext() override;
		virtual void AttachToWindow(SDL_Window* Window);

		/**
		*   Draws meshes that are on RenderingStack.
		*/
		virtual void DrawMeshes(const DrawData aDrawData) override;

	private:

		SDL_GLContext * Context;

		GLenum	DrawingMode;
		GLsizei	ParametersInOneVector;

		ShaderProgram	mShaderProgram;

		GLint mModelUniformLocation = -1;
		GLint mViewUniformLocation = -1;
		GLint mProjectionUniformLocation = -1;
		GLint mColorUniformLocation = -1;

		SDL_Window* mApplicationWindow;

	};
}


#endif