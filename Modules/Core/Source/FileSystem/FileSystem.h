#ifndef FILESYSTEM_H
#define FILESYSTEM_H

constexpr int MAX_ARRAY_BUFFER_SIZE = 256;

#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
constexpr const char* GetPathSeparator() { return "\\"; };
#elif defined(PLATFORM_LINUX)
constexpr const char* GetPathSeparator() { return "/"; };
#endif

#include <string>
#include <memory>

#include "Utilities.h"

struct FileData
{
	std::vector<char> Data;
	size_t Size;
};

enum class FileOpeningOptions : uint8_t
{
	// Permissions
	ReadPermission = 1 << 0,
	WritePermission = 1 << 1,
	// Formats
	BinaryFormat = 1 << 3
};
TypeSafeEnumBitmask(FileOpeningOptions);


class FileSystem
{
public:
	
	FileSystem();
	FileSystem(const FileSystem&) = delete;
	FileSystem(FileSystem&&) = delete;
	FileSystem& operator=(const FileSystem&) = delete;
	FileSystem& operator=(FileSystem&&) = delete;

	virtual ~FileSystem();

	static FileSystem* const Get();
	
	const char* GetRootDir();
	string GetModuleAssetsDir(const char* ModuleName);

	// Convert path 
	static string Path(const string& String);

	static string ReplaceStringOccurences(string Source, const char* ReplaceFrom, const char* ReplaceTo);

	static auto FindOccurenceFromString(const char* String, const char* Token, int OccurenceNumber, bool Reversed = false)->const size_t;

	static auto Open(const char* Path, FileData& Data, uint8_t Options)-> ErrorHandle;
	static auto RetrieveBinaryDataFromFile(const char* ModuleName, const string& FileName)->std::vector<char>;

	static auto GetAssetNameFromPath(const string ModuleName, const string ModuleRelativePath)-> const string;
	static auto GetAssetAbsolutePath(const string ModuleName, const string ModuleRelativePath)-> const string;
	static auto GetAssetShortPathFromPath(const string ModuleName, const string ModuleRelativePath)->const string;

	static auto VerifyFileExistence(const string& AbsolutePath)->const bool;
	static auto VerifyFileExtension(const vector<string> Extensions, const string Path)->const bool;

private:

	string RootDir;

};

#endif 