////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Shader Program
//  @author     : Artur Ostrowski
//  @usage      : Keeps shader program data and handles it.
//  @version    : 1.0.0
//
////////////////////////////////////////
#include "stdafx.h"
#include "ShaderProgram.h"


ShaderProgram::ShaderProgram()
	: mProgramIDm(0)
	, m_vertexShader(0)
	, m_fragmentShader(0)
{

}

void ShaderProgram::Init()
{
	mProgramIDm = glCreateProgram();
}

bool ShaderProgram::LoadShader(const char * path, ShaderType type)
{
	GLint				shaderSuccess = GL_FALSE;
	std::fstream		shaderFile(path);
	std::string			buffer(	(std::istreambuf_iterator<char>(shaderFile)),
								std::istreambuf_iterator<char>() );

	const char * source = buffer.c_str();

	if (type == GL_VERTEX_SHADER && m_vertexShader <= 0)
	{
		// Create shader
		m_vertexShader = glCreateShader(type);
		// Pass it's source to created shader
		glShaderSource(m_vertexShader, 1, &source, NULL);
		// Compile it
		glCompileShader(m_vertexShader);
		// Check for errors
		CheckShaderStatus(m_vertexShader);
	} 
	else if (type == GL_FRAGMENT_SHADER && m_fragmentShader <= 0)
	{
		m_fragmentShader = glCreateShader(type);
		glShaderSource(m_fragmentShader, 1, &source, NULL);
		glCompileShader(m_fragmentShader);
		CheckShaderStatus(m_fragmentShader);
	}
	else
	{
		// TODO implement glshaderinfolog
		SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_WARN, "Error, cannot load %d shader./n"
						, type == GL_VERTEX_SHADER ? "vertex" : type == GL_FRAGMENT_SHADER ? "fragment" : "undefined");
		return false;
	}

	return true;
}

void ShaderProgram::LinkProgram()
{
	GLint programSuccess = GL_FALSE;
	if (mProgramIDm <= 0 || m_vertexShader <= 0 || m_fragmentShader <= 0)
	{
		printf("Error, cannot link program : \n\nmProgramIDm : %u\nm_vertexShader : %u\nm_fragmentShader : %u", mProgramIDm, m_vertexShader, m_fragmentShader);
	}
	glAttachShader(mProgramIDm, m_vertexShader);
	glAttachShader(mProgramIDm, m_fragmentShader);
	glLinkProgram(mProgramIDm);
	glGetProgramiv(mProgramIDm, GL_LINK_STATUS, &programSuccess);
}

void ShaderProgram::Bind()
{
	glUseProgram(mProgramIDm);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Error binding program ! %s\n");
	}

}

void ShaderProgram::Unbind()
{
	glUseProgram(NULL);
}

unsigned int ShaderProgram::GetProgramID() const
{
	return mProgramID;
}

void ShaderProgram::PrintInfo()
{
	GLint maxLength, nAttribs;
	// Query for the number of active attributes
	glGetProgramiv(mProgramIDm, GL_ACTIVE_ATTRIBUTES, &nAttribs);
	// Query for length of the longest attribute name.
	glGetProgramiv(mProgramIDm, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);

	GLchar * name = (GLchar *)malloc(maxLength);

	GLint written, size, location;
	GLenum type;
	printf(" Index | Name\n");
	printf("-----------------------------------------\n");

	// Loop over each index and retrieve information about each attribute.
	for (int i = 0; i < nAttribs; i++)
	{
		glGetActiveAttrib(mProgramIDm, i, maxLength, &written,
			&size, &type, name);
		location = glGetAttribLocation(mProgramIDm, name);
		printf(" %-5d | %s\n", location, name);
	}
	free(name);
}

bool ShaderProgram::CheckShaderStatus(unsigned int shaderID)
{
	GLint shaderSuccess;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderSuccess);
	if (shaderSuccess != GL_TRUE)
	{
		GLint logLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		GLchar * buffer = new GLchar[logLength];
		GLsizei bufferSize;
		glGetShaderInfoLog(shaderID, logLength, &bufferSize, buffer);
		GLint shaderType;
		glGetShaderiv(shaderID, GL_SHADER_TYPE, &shaderType);

		std::string shaderTypeString;
		shaderType == GL_FRAGMENT_SHADER ? shaderTypeString = "Fragment" :
		shaderType == GL_VERTEX_SHADER ? shaderTypeString = "Vertex" :
		shaderTypeString = "Unknown";

		printf("%s shader Log: \n\n%s\n", shaderTypeString.c_str(), buffer);
		SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_WARN, "Error, cannot compile %s shader : %d\n", shaderTypeString, m_fragmentShader);

		delete[] buffer;
		return false;
	}
	else
	{
		return true;
	}

}

bool ShaderProgram::CheckProgramStatus()
{
	GLint programSuccess;
	glGetProgramiv(mProgramIDm, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		GLint logLength;
		glGetProgramiv(mProgramIDm, GL_INFO_LOG_LENGTH, &logLength);
		GLchar * buffer = new GLchar[logLength];
		GLsizei bufferSize;
		glGetProgramInfoLog(mProgramIDm, logLength, &bufferSize, buffer);

		printf("Program Link Log: \n\n%s\n", buffer);
		SDL_LogMessage(SDL_LOG_CATEGORY_RENDER, SDL_LOG_PRIORITY_WARN, "Error, cannot compile fragment shader : %d\n", m_fragmentShader);

		delete[] buffer;
		return false;
	}
	else
	{
		return true;
	}
}

int ShaderProgram::GetUniformLocation(const char* aName) const
{
	if (mProgramIDm == 0)
	{
		Log(DebugType::EDT_Error, "Getting %s uniform location failed. Shared program invalid.", aName);
		return -1;
	}

	return glGetUniformLocation(mProgramIDm, aName);

}