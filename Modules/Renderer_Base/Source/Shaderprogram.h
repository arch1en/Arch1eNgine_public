////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Shader Program
//  @author     : Artur Ostrowski
//  @usage      : Keeps shader program data and handles it.
//  @version    : 1.0.0
//
////////////////////////////////////////
#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Utilities/RendererUtilities.h"

namespace Renderer
{
	class MODULE_API ShaderProgram
	{

	public:

		ShaderProgram();

		void	Init();
		bool	LoadShader(const char * path, ShaderType type);
		void	LinkProgram();
		void	Bind();
		void	Unbind();
		void	PrintInfo();
		unsigned int	GetProgramID() const;

		bool		CheckProgramStatus();
		bool		CheckShaderStatus(unsigned int shaderID);

		int			GetUniformLocation(const char* aName) const;

	private:

		unsigned int		mProgramID;
		unsigned int		m_vertexShader;
		unsigned int		m_fragmentShader;
	};

}


#endif