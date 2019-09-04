#include "ShaderSystem.h"
#include "LogSystem.h"

#include <fstream>

#include "FileSystem.h"

// @todo: Loading files should be performed through internal file management system.
std::vector<char> ShaderSystem::LoadShaderFromFile(const std::string& FileName)
{
	std::string AssetsDir =	FileSystem::Get()->GetModuleAssetsDir("Renderer");
	std::ifstream File(FileSystem::Path(AssetsDir + "/" + FileName), std::ios::ate | std::ios::binary);

	if (!File.is_open())
	{
		LogVk(LogType::Error, 0, "File opening failed");
		return std::vector<char>();
	}

	size_t FileSize = static_cast<size_t>(File.tellg());
	std::vector<char> Buffer(FileSize);

	File.seekg(0);
	File.read(Buffer.data(), FileSize);

	File.close();

	return Buffer;
}

VkShaderModule ShaderSystem::CreateShaderModule(const std::vector<char>& Code, const VkDevice& Device)
{
	VkShaderModuleCreateInfo CreateInfo = {};
	CreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	CreateInfo.codeSize = Code.size();
	CreateInfo.pCode = reinterpret_cast<const uint32_t*>(Code.data());

	VkShaderModule ShaderModule;
	if (vkCreateShaderModule(Device, &CreateInfo, nullptr, &ShaderModule) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Shader module creation failed.");
	}

	return ShaderModule;
}
