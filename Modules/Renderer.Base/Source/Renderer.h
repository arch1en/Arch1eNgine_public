////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Renderer
//  @author     : Artur Ostrowski
//  @usage      : Base class responsible for rendering tasks.
//  @version    : 1.0.0
//
////////////////////////////////////////
#pragma once

#include <stack>

//#include "Components/MeshComponent.h"
#include "ShaderProgram.h"
#include "glm/glm.hpp"
#include "Utilities/RendererUtilities.h"
#include "Mesh.h"

struct DrawData
{
	const int VAOIndex;
	const glm::mat4& ViewMatrix;
};

class Renderer
{
public:
	Renderer(RendererType aRendererType);
	~Renderer();

	virtual bool Initialize() = 0;
	virtual void CreateContext() = 0;
	virtual void AttachToWindow(SDL_Window* Window) = 0;

	/**
	*   Draws meshes that are on RenderingStack.
	*/
	virtual void DrawMeshes(DrawData aDrawData) = 0;

	void AddMeshToDraw(std::shared_ptr<Mesh> InMesh);

private:

	GLenum	DrawingMode;
	GLsizei	ParametersInOneVector;
	
	ShaderProgram	mShaderProgram;

	GLint mModelUniformLocation = -1;
	GLint mViewUniformLocation = -1;
	GLint mProjectionUniformLocation = -1;
	GLint mColorUniformLocation = -1;

	// Keeps all objects that needs to be rendered on the scene.
	std::vector<std::shared_ptr<Mesh>> Meshes;

};