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
#include "Mesh/MeshBase.h"
#include "IO/Paths.h"

Renderer::Renderer()
{
	DrawingMode = GL_TRIANGLES;
}

Renderer::~Renderer()
{

}

void Renderer::Initiate()
{
	mShaderProgram.Init();

	mShaderProgram.LoadShader((an::Paths::GetInstance().GetPathAssets() + "\\Shaders\\hello_glsl.vert").c_str(), GL_VERTEX_SHADER);
	mShaderProgram.LoadShader((an::Paths::GetInstance().GetPathAssets() + "\\Shaders\\hello_glsl.frag").c_str(), GL_FRAGMENT_SHADER);

	mShaderProgram.LinkProgram();
	mShaderProgram.CheckProgramStatus();

	mModelUniformLocation = mShaderProgram.GetUniformLocation("model");
	mViewUniformLocation = mShaderProgram.GetUniformLocation("view");
	mProjectionUniformLocation = mShaderProgram.GetUniformLocation("projection");

	// TEMP TEXTURE SETUP
	glUniform1i(mShaderProgram.GetUniformLocation("aTexture"), 0);
	// ~TEMP TEXTURE SETUP

	mShaderProgram.Bind();
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

		if (iter->GetMesh()->HasTexture())
		{
			glBindTexture(GL_TEXTURE_2D, iter->GetMesh()->GetTexture()->GetTextureID());
		}

		glDrawElements(DrawingMode, iter->GetMesh()->mPolygonData.Elements.size(), GL_UNSIGNED_INT, 0);// &iter->mPolygonData.Elements[0]);
		// TODO [High] : Handle VAO Binding !

		if (iter->GetMesh()->HasTexture())
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	glBindVertexArray(0);

}