#include "FileSystem.h"

#include <SDL_filesystem.h>

FileSystem* const FileSystem::Get()
{
	static FileSystem Instance;
	return &Instance;
}

const char* FileSystem::GetRootDir()
{
	return SDL_GetBasePath();
}