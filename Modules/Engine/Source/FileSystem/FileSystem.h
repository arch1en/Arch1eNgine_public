#ifndef ENGINE_FILESYSTEM_H
#define ENGINE_FILESYSTEM_H

class EngineFileSystem
{
public:
	// Needs to be paired with CleanTextureImage.
	static stbi_uc* LoadTextureImage(const std::string& Path, size_t& ImageSize);
	static void CleanTextureImage(stbi_uc* Pixels);
};

#endif // ENGINE_FILESYSTEM_H