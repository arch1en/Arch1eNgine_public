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
	OpenAndReadFromEnd = 1 << 2,
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
	std::string GetModuleAssetsDir(const char* ModuleName);
	size_t FindOccurenceFromString(const char* String, const char* Token, int OccurenceNumber, bool Reversed = false);

	// Convert path 
	static std::string Path(const std::string& String);

	static std::string ReplaceStringOccurences(std::string Source, const char* ReplaceFrom, const char* ReplaceTo);

	static 	ErrorHandle Open(const char* Path, FileData& Data, uint8_t Options);
	static std::vector<char> RetrieveBinaryDataFromFile(const char* ModuleName, const std::string& FileName);

private:

	const char* RootDir = nullptr;

};

#endif 