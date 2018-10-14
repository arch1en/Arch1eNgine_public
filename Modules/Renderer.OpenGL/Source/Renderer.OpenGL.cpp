////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Renderer
//  @author     : Artur Ostrowski
//  @usage      : Base class responsible for rendering tasks.
//  @version    : 1.0.0
//
////////////////////////////////////////
#include "Renderer.h"
#include "Mesh.h"
#include "IO/Paths.h"

struct Color;

// TEMP
#include "Material.h"
// ~TEMP

Renderer::OpenGL::Renderer::OpenGL()
	: Context{nullptr}
	, mApplicationWindow{nullptr}
{

}

void Renderer::OpenGL::Initialize()
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

void Renderer::OpenGL::CreateContext()
{
	Context = SDL_GL_CreateContext()
}

void Renderer::OpenGL::AttachContextToWindow(SDL_Window* Window)
{
	StaticAssert(Window, "Window must not be null !");
	mApplicationWindow = Window;
}

Renderer::OpenGL::~Renderer::OpenGL()
{

}

void Renderer::OpenGL::AddMeshToDraw(std::shared_ptr<MeshComponent> InMesh)
{
	Meshes.push_back(InMesh);
}

void Renderer::OpenGL::DrawMeshes(const DrawData aDrawData)
{
	glBindVertexArray(aDrawData.VAOIndex);

	for (auto& iter : Meshes)
	{
		assert(iter != nullptr);
		//glDrawArrays(DrawingMode, 0, 3);
		glm::mat4 ModelMatrix;
		ModelMatrix = glm::translate(ModelMatrix, iter->GetOwner()->GetWorldPosition());
		glUniformMatrix4fv(mModelUniformLocation, 1, GL_FALSE, &ModelMatrix[0][0]);

		glUniformMatrix4fv(mViewUniformLocation, 1, GL_FALSE, &aDrawData.ViewMatrix[0][0]);

		glm::mat4 ProjectionMatrix;
		ProjectionMatrix = glm::perspective(glm::radians(45.f), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(mProjectionUniformLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);

		const Color DiffuseColor = iter()->mPolygonData.Materials[0]->GetDiffuseColor();

		glUniform4f(mColorUniformLocation, DiffuseColor.R, DiffuseColor.G, DiffuseColor.B, DiffuseColor.A);

		if (iter()->mPolygonData.Materials[0]->HasTextures())
		{
			glBindTexture(GL_TEXTURE_2D, iter()->mPolygonData.Materials[0]->GetTextures()->at(0)->GetTextureID());
		}

		glDrawElements(DrawingMode, iter()->mPolygonData.NumElements, GL_UNSIGNED_INT, 0);// &iter->mPolygonData.Elements[0]);
																								   // TODO [High] : Handle VAO Binding !

		if (iter()->mPolygonData.Materials[0]->HasTextures())
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	glBindVertexArray(0);

}