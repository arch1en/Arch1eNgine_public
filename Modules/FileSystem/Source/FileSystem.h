#ifndef FILESYSTEM_H
#define FILESYSTEM_H

class FileSystem
{
public:

	static FileSystem* const Get();
	
	const char* GetRootDir();
};

#endif 