#ifndef FILESYSTEM_H
#define FILESYSTEM_H

constexpr int MAX_ARRAY_BUFFER_SIZE = 256;

#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
constexpr const char* GetPathSeparator() { return "\\"; };
#elif defined(PLATFORM_LINUX)
constexpr const char* GetPathSeparator() { return "/"; };
#endif

#include <string>

class FileSystem
{
public:
	
	FileSystem();
	virtual ~FileSystem();

	static FileSystem* const Get();
	
	const char* GetRootDir();
	std::string GetModuleAssetsDir(const char* ModuleName);
	size_t FindOccurenceFromString(const char* String, const char* Token, int OccurenceNumber, bool Reversed = false);

	// Convert path 
	static std::string Path(std::string String);

	static std::string ReplaceStringOccurences(std::string Source, const char* ReplaceFrom, const char* ReplaceTo);

private:

	const char* RootDir = nullptr;

};

#endif 