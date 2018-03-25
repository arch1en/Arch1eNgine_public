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
#include "Mesh/Mesh.h"
#include "IO/Paths.h"

// TEMP
#include "Engine/Rendering/Material.h"
// ~TEMP

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

Renderer::~Renderer()
{

}

void Renderer::AddMeshToDraw(std::shared_ptr<MeshComponent> InMesh)
{
	Meshes.push_back(InMesh);
}

void Renderer::DrawMeshes(const GLsizei aVAOIndex, const glm::mat4& aViewMatrix)
{
	glBindVertexArray(aVAOIndex);

	for (auto& iter : Meshes)
	{
		assert(iter != nullptr);
		//glDrawArrays(DrawingMode, 0, 3);
		glm::mat4 ModelMatrix;
		ModelMatrix = glm::translate(ModelMatrix, iter->GetOwner()->GetWorldPosition());
		glUniformMatrix4fv(mModelUniformLocation, 1, GL_FALSE, &ModelMatrix[0][0]);

		glUniformMatrix4fv(mViewUniformLocation, 1, GL_FALSE, &aViewMatrix[0][0]);

		glm::mat4 ProjectionMatrix;
		ProjectionMatrix = glm::perspective(glm::radians(45.f), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(mProjectionUniformLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);

		const Color* DiffuseColor = iter->GetMesh()->mPolygonData.Materials[0]->GetDiffuseColor();

		glUniform4f(mColorUniformLocation, DiffuseColor->r, DiffuseColor->g, DiffuseColor->b, DiffuseColor->a);

		if (iter->GetMesh()->mPolygonData.Materials[0]->HasTextures())
		{
			glBindTexture(GL_TEXTURE_2D, iter->GetMesh()->mPolygonData.Materials[0]->GetTextures()->at(0)->GetTextureID());
		}

		glDrawElements(DrawingMode, iter->GetMesh()->mPolygonData.NumElements, GL_UNSIGNED_INT, 0);// &iter->mPolygonData.Elements[0]);
		// TODO [High] : Handle VAO Binding !

		if (iter->GetMesh()->mPolygonData.Materials[0]->HasTextures())
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	glBindVertexArray(0);

}