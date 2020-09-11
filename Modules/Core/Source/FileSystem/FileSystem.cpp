#include "FileSystem.h"

#include <algorithm>
#include <fstream>
#include <filesystem>

//temp
#include <ryml.hpp>
//~temp

#include <SDL_filesystem.h>

 auto FileSystem::Open(const char* Path, FileData& Data, uint8_t Options)->ErrorHandle
{
	ErrorHandle Result;

	std::ios_base::openmode OpenMode = 0;
	
	if(Options & FileOpeningOptions::ReadPermission)
	{
		OpenMode |= std::ifstream::in;
	}

	if (Options & FileOpeningOptions::BinaryFormat)
	{
		OpenMode |= std::ios::binary;
	}

	OpenMode |= std::ios::ate; // Start from end, so we can use tellg(); to read the file size.

	std::ifstream File(Path, OpenMode);

	if (!File.is_open())
	{
		Result.Msg = "File opening failed";
		Result.Code = 1;
		return Result;
	}

	if(File.fail())
	{
		Result.Msg = "File corrupted";
		Result.Code = 2;
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
auto FileSystem::RetrieveBinaryDataFromFile(const char* ModuleName, const string& FileName)->std::vector<char>
{
	FileData Data;
	string AssetsDir = FileSystem::Get()->GetModuleAssetsDir(ModuleName);
	ErrorHandle Result = FileSystem::Open(FileSystem::Path(AssetsDir + "/" + FileName).c_str(), Data, FileOpeningOptions::ReadPermission | FileOpeningOptions::BinaryFormat);

	if (Result.Code != 0)
	{
		Log(LogType::Error, 0, "Cannot load binary data from file. %s", Result.Msg);
	}

	return Data.Data;
}

auto FileSystem::GetAssetNameFromPath(const string ModuleName, const string ModuleRelativePath) ->const string
{
#if BUILD_CONFIGURATION >= BUILD_CONFIGURATION_DEBUG
	const string AssetAbsolutePath{ GetAssetAbsolutePath(ModuleName, ModuleRelativePath) };
	if (!VerifyFileExistence(AssetAbsolutePath))
	{
		Log(LogType::Warning, 0, "File non-existing : %s.", AssetAbsolutePath);
	}
#endif

	auto Position = FileSystem::FindOccurenceFromString(Path(ModuleRelativePath).c_str(), GetPathSeparator(), 1, true);

	string Result(ModuleRelativePath.begin() + Position+1, ModuleRelativePath.end());

	return Result;
}

auto FileSystem::GetAssetAbsolutePath(const string ModuleName, const string ModuleRelativePath) -> const string
{
	return Path(FileSystem::Get()->GetModuleAssetsDir(ModuleName.c_str()) / ModuleRelativePath);
}

auto FileSystem::GetAssetShortPathFromPath(const string ModuleName, const string ModuleRelativePath) ->const string
{
	return Path(ModuleName / "Assets" / ModuleRelativePath);
}

auto FileSystem::VerifyFileExistence(const string& AbsolutePath) ->const bool
{
	return std::filesystem::exists(AbsolutePath);
}

auto FileSystem::VerifyFileExtension(const vector<string> Extensions, const string Path) ->const bool
{
	string ReversedExtension{""};

	for(auto iter = Path.rbegin(); iter != Path.rend(); iter++)
	{
		if(*iter == '.')
		{
			break;
		}

		ReversedExtension += *iter;
	}

	string Extension{""};

	for(auto iter = ReversedExtension.rbegin(); iter != ReversedExtension.rend(); iter++)
	{
		Extension += *iter;
	}


	for (auto iter = Extensions.begin(); iter != Extensions.end(); iter++)
	{
		if (iter->compare(Extension) == 0)
		{
			return true;
		}
	}

	return false;
}

FileSystem::FileSystem()
{
	char* BasePath = SDL_GetBasePath();

		
	int Position = static_cast<int>(FindOccurenceFromString(BasePath, GetPathSeparator(), 3, true));
	RootDir.resize(Position);
	for (auto i = 0; i < Position; i++)
	{
		RootDir[i] = BasePath[i];
	}

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

string FileSystem::GetModuleAssetsDir(const char* ModuleName)
{
	// @todo : Improve the algorithm.

	string Result = GetRootDir();
	Result =  Path(Result / "Modules" / ModuleName / "Assets");

	return Result;
}

// [Todo] Add 1 to the result.
auto FileSystem::FindOccurenceFromString(const char* String, const char* Token, int OccurenceNumber, bool Reversed)->const size_t
{
	string SString = String;
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

string FileSystem::ReplaceStringOccurences(string Source, const char* ReplaceFrom, const char* ReplaceTo)
{
	size_t ReplaceFromStartPosition = Source.find(ReplaceFrom);

	while (ReplaceFromStartPosition != string::npos)
	{
		Source = Source.erase(ReplaceFromStartPosition, strlen(ReplaceFrom));
		Source.insert(ReplaceFromStartPosition, ReplaceTo);
		ReplaceFromStartPosition = Source.find(ReplaceFrom);
	}

	return Source;
}

string FileSystem::Path(const string& String)
{
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
	return ReplaceStringOccurences(String, "/", "\\");
#elif defined(PLATFORM_LINUX)
	return ReplaceStringOccurences("\\", "/");
#endif

	return string();
}
