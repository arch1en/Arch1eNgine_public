#include "FileSystem.h"

#include <algorithm>
#include <SDL_filesystem.h>

FileSystem::FileSystem()
{
	char* BasePath = SDL_GetBasePath();

	const int Position = FindOccurenceFromString(BasePath, GetPathSeparator(), 4, true);
	char* NewRootDir = new char[Position - 4];
	memcpy(NewRootDir, BasePath, Position);

	RootDir = NewRootDir;

	SDL_free(reinterpret_cast<void*>(const_cast<char*>(BasePath)));
}

FileSystem::~FileSystem()
{
	// Because SDL needs to free SDL_GetBasePath.
}

FileSystem* const FileSystem::Get()
{
	static FileSystem Instance;
	return &Instance;
}

const char* FileSystem::GetRootDir()
{
	return RootDir;
}

std::string FileSystem::GetModuleAssetsDir(const char* ModuleName)
{
	// @todo : Improve the algorithm.

	std::string Result = GetRootDir();
	Result =  Path(Result + "/Modules/" + ModuleName + "/Assets");

	return Result;
}

size_t FileSystem::FindOccurenceFromString(const char* String, const char* Token, int OccurenceNumber, bool Reversed)
{
	std::string SString = String;
	size_t Position;
	if (Reversed)
	{
		Position = strlen(String);
		for (size_t i = 0; i < OccurenceNumber; i++)
		{
			Position = SString.rfind(Token, Position - strlen(Token));
		}
	}
	else
	{
		Position = 0;
		for (size_t i = 0; i < OccurenceNumber; i++)
		{
			Position = SString.find(Token, Position + strlen(Token));
		}
	}

	return Position;
}

std::string FileSystem::ReplaceStringOccurences(std::string Source, const char* ReplaceFrom, const char* ReplaceTo)
{
	size_t ReplaceFromStartPosition = Source.find(ReplaceFrom);

	while (ReplaceFromStartPosition != std::string::npos)
	{
		Source = Source.erase(ReplaceFromStartPosition, strlen(ReplaceFrom));
		Source.insert(ReplaceFromStartPosition, ReplaceTo);
		ReplaceFromStartPosition = Source.find(ReplaceFrom);
	}

	return Source;
}

std::string FileSystem::Path(std::string String)
{
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
	return ReplaceStringOccurences(String, "/", "\\");
#elif defined(PLATFORM_LINUX)
	return ReplaceStringOccurences("\\", "/");
#endif

	return std::string();
}
