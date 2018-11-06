////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Renderer
//  @author     : Artur Ostrowski
//  @usage      : Base class responsible for rendering tasks.
//  @version    : 1.0.0
//
////////////////////////////////////////
#include "Renderer_OpenGL.h"
#include "Mesh.h"
#include "Paths.h"

struct Color;

// TEMP
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Material.h"
#include "Utilities/RendererUtilities.h"
#include "Assertions.inl"
// ~TEMP

namespace Renderer
{

	OpenGL::OpenGL()
	{

	}


	bool OpenGL::Initialize()
	{
		DrawingMode = GL_TRIANGLES;

		mShaderProgram.Init();

		mShaderProgram.LoadShader((Paths::GetInstance().GetPathAssets() + "\\Shaders\\hello_glsl.vert").c_str(), ShaderType::Vertex);
		mShaderProgram.LoadShader((Paths::GetInstance().GetPathAssets() + "\\Shaders\\hello_glsl.frag").c_str(), ShaderType::Fragment);

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

		return true;
	}

	void OpenGL::CreateContext()
	{
		Assert(mApplicationWindow, "Application window is invalid.");
		Context = std::make_unique<SDL_GLContext>(SDL_GL_CreateContext(mApplicationWindow));
	}

	void OpenGL::AttachContextToWindow(SDL_Window* Window)
	{
		Assert(Window, "Window must not be null !");
		mApplicationWindow = Window;
	}

	OpenGL::~OpenGL()
	{

	}

	void OpenGL::DrawMeshes(const DrawData aDrawData)
	{
		glBindVertexArray(aDrawData.VAOIndex);

		for (auto& iter : Meshes)
		{
			assert(iter != nullptr);

			Matrix<4, 4> ModelMatrix;
			//glm::mat4 ModelMatrix;
			ModelMatrix = Translate(ModelMatrix, iter->GetVertexOffset());//glm::translate(ModelMatrix, iter->GetVertexOffset());
			glUniformMatrix4fv(mModelUniformLocation, 1, GL_FALSE, &ModelMatrix[0][0]);

			glUniformMatrix4fv(mViewUniformLocation, 1, GL_FALSE, &aDrawData.ViewMatrix[0][0]);

			glm::mat4 ProjectionMatrix;

			int WindowWidth = 0;
			int WindowHeight = 0;
			SDL_GetWindowSize(mApplicationWindow, &WindowWidth, &WindowHeight);

			ProjectionMatrix = glm::perspective(glm::radians(45.f), (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);
			glUniformMatrix4fv(mProjectionUniformLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);

			const Color DiffuseColor = iter->mPolygonData.Materials[0]->GetDiffuseColor();

			glUniform4f(mColorUniformLocation, DiffuseColor.R, DiffuseColor.G, DiffuseColor.B, DiffuseColor.A);

			if (iter->mPolygonData.Materials[0]->HasTextures())
			{
				glBindTexture(GL_TEXTURE_2D, iter->mPolygonData.Materials[0]->GetTextures()->at(0)->GetTextureID());
			}

			glDrawElements(DrawingMode, iter->mPolygonData.NumElements, GL_UNSIGNED_INT, 0);// &iter->mPolygonData.Elements[0]);
																									   // TODO [High] : Handle VAO Binding !

			if (iter->mPolygonData.Materials[0]->HasTextures())
			{
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		glBindVertexArray(0);

	}

}