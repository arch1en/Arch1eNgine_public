#include "FileSystem.h"

#include <string>

#include "stb_image.h"

#include "Debug/LogSystem.h"

stbi_uc* EngineFileSystem::LoadTextureImage(const std::string& Path, size_t& ImageSize)
{
    int TextureWidth = -1;
    int TextureHeight = -1;
    int TextureChannel = -1;
    stbi_uc* pixels = stbi_load(Path.c_str(), &TextureWidth, &TextureHeight, &TextureChannel, STBI_rgb_alpha);

    ImageSize = TextureWidth * TextureHeight * 4;

    if (!pixels) {
       Log(LogType::Fatal, "failed to load texture image!");
    }

    return pixels;
}

void EngineFileSystem::CleanTextureImage(stbi_uc* Pixels)
{
    stbi_image_free(Pixels);
}