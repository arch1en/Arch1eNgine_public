////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @name       : Paths
//  @author     : Artur Ostrowski
//  @usage      : Singleton, keeps data concerning paths to various directories in the project.
//  @version    : 1.0.0
//
////////////////////////////////////////
//#include "stdafx.h"
#include "Paths.h"

#include <SDL_filesystem.h>
#include "Logger.h"

Paths::Paths()
{
	std::string Path = SDL_GetBasePath();
	
	for (int i = 0; i < 3; i++)
	{
		std::size_t Found = Path.rfind("\\", Path.length() - 2);
		if (Found != std::string::npos)
		{
			Path.erase(Found + 1, Path.length());
		}
		else
		{
			Log(LogType::Error, "Failed to trim the basic path string. Investigate !");
			return;
		}
	}

	PathAssets = Path + "Assets\\";
	PathConfig = Path + "Config\\";
	PathSourceCode = Path + "SourceCode\\";
	PathLogs = Path + "Logs\\";
}

