#include "FileSystem.h"

#include <algorithm>
#include <fstream>
#include <SDL_filesystem.h>

ErrorHandle FileSystem::Open(const char* Path, FileData& Data, uint8_t Options)
{
	ErrorHandle Result;

	std::ios_base::openmode OpenMode = 0;

	if (Options & FileOpeningOptions::OpenAndReadFromEnd)
	{
		OpenMode |= std::ios::ate;
	}

	if (Options & FileOpeningOptions::BinaryFormat)
	{
		OpenMode |= std::ios::binary;
	}

	std::ifstream File(Path, OpenMode);

	if (!File.is_open())
	{
		Result.Msg = "File opening failed";
		Result.Code = 1;
		return Result;
	}

	size_t FileSize = static_cast<size_t>(File.tellg());
	std::vector<char> Buffer(FileSize);

	File.seekg(0);
	File.read(Buffer.data(), FileSize);

	File.close();

	Data.Size = FileSize;
	Data.Data = Buffer;

	return Result;
}

// [Todo] Adapt name to mention that we are extracting data from the assets dir folder or make this function load from absolute path.
std::vector<char> FileSystem::RetrieveBinaryDataFromFile(const char* ModuleName, const std::string& FileName)
{
	FileData Data;
	std::string AssetsDir = FileSystem::Get()->GetModuleAssetsDir(ModuleName);
	ErrorHandle Result = FileSystem::Open(FileSystem::Path(AssetsDir + "/" + FileName).c_str(), Data, FileOpeningOptions::OpenAndReadFromEnd | FileOpeningOptions::BinaryFormat);

	if (Result.Code != 0)
	{
		Log(LogType::Error, 0, "Cannot load binary data from file. %s", Result.Msg);
	}

	return Data.Data;
}

FileSystem::FileSystem()
{
	char* BasePath = SDL_GetBasePath();

	std::string Path = BasePath;
	const int Position = static_cast<int>(FindOccurenceFromString(Path.c_str(), GetPathSeparator(), 3, true));
	
	std::string NewDir = Path.substr(0, Position);

	RootDir = NewDir;

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
	return RootDir.c_str();
}

std::string FileSystem::GetModuleAssetsDir(const char* ModuleName)
{
	// @todo : Improve the algorithm.

	std::string Result = GetRootDir();
	Result =  Path(Result / "Modules" / ModuleName / "Assets");

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

std::string FileSystem::Path(const std::string& String)
{
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
	return ReplaceStringOccurences(String, "/", "\\");
#elif defined(PLATFORM_LINUX)
	return ReplaceStringOccurences("\\", "/");
#endif

	return std::string();
}
