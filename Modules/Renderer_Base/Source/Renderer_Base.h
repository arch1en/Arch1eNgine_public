////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Base
//  @author     : Artur Ostrowski
//  @usage      : Base class responsible for rendering tasks.
//  @version    : 1.0.0
//
////////////////////////////////////////
#pragma once

#include <stack>

#include <SDL.h>
#include "ShaderProgram.h"
#include "glm/glm.hpp"
#include "Utilities/RendererUtilities.h"
#include "Mesh.h"

struct DrawData
{
	const int VAOIndex;
	const glm::mat4& ViewMatrix;
};

namespace Renderer
{
	class Base
	{
	public:
		Base();
		~Base();

		virtual bool Initialize() = 0;
		virtual void CreateContext() = 0;
		virtual void AttachToWindow(SDL_Window* Window) = 0;

		/**
		*   Draws meshes that are on RenderingStack.
		*/
		virtual void DrawMeshes(DrawData aDrawData) = 0;

		void AddMeshToDraw(std::shared_ptr<Mesh> InMesh);

	protected:

		unsigned int	DrawingMode;
		int	ParametersInOneVector;

		ShaderProgram	mShaderProgram;

		int mModelUniformLocation = -1;
		int mViewUniformLocation = -1;
		int mProjectionUniformLocation = -1;
		int mColorUniformLocation = -1;

		// Keeps all objects that needs to be rendered on the scene.
		std::vector<std::shared_ptr<Mesh>> Meshes;

	};
}
