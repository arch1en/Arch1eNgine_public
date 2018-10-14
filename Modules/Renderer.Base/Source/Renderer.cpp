////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Renderer
//  @author     : Artur Ostrowski
//  @usage      : Base class responsible for rendering tasks.
//  @version    : 1.0.0
//
////////////////////////////////////////
#include "stdafx.h"
#include "Renderer.h"
#include "Mesh/Mesh.h"
#include "IO/Paths.h"

struct Color;

// TEMP
#include "Engine/Rendering/Material.h"
// ~TEMP

Renderer::Renderer(RendererType aRendererType)
{
	switch (aRendererType)
	{

	}

	mRendererType = aRendererType;
}

void Renderer::Initialize()
{
	DrawingMode = GL_TRIANGLES;

	mShaderProgram.Init();

	mShaderProgram.LoadShader((Paths::GetInstance().GetPathAssets() + "\\Shaders\\hello_glsl.vert").c_str(), GL_VERTEX_SHADER);
	mShaderProgram.LoadShader((Paths::GetInstance().GetPathAssets() + "\\Shaders\\hello_glsl.frag").c_str(), GL_FRAGMENT_SHADER);

	mShaderProgram.LinkProgram();
	mShaderProgram.CheckProgramStatus();

	mModelUniformLocation = mShaderProgram.GetUniformLocation("model");
	mViewUniformLocation = mShaderProgram.GetUniformLocation("view");
	mProjectionUniformLocation = mShaderProgram.GetUniformLocation("projection");
	mColorUniformLocation = mShaderProgram.GetUniformLocation("color");

	// TEMP TEXTURE SETUP
	glUniform1i(mShaderProgram.GetUniformLocation("aTexture"), 0);
	// ~TEMP TEXTURE SETUP

	mShaderProgram.Bind();
}

void Renderer::AttachContextToWindow(SDL_Window* Window)
{

}

Renderer::~Renderer()
{

}

void Renderer::AddMeshToDraw(std::shared_ptr<Mesh> InMesh)
{
	Meshes.push_back(InMesh);
}