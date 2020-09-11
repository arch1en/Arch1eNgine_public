#include "ShaderSystem.h"
#include "Debug/LogSystem.h"

#include "FileSystem/FileSystem.h"

// @todo: Loading files should be performed through internal file management system.
std::vector<char> ShaderSystem::LoadShaderFromFile(const std::string& FileName)
{
	FileData Data;
	std::string AssetsDir =	FileSystem::Get()->GetModuleAssetsDir("Renderer");
	ErrorHandle Result = FileSystem::Open(FileSystem::Path(AssetsDir + "/" + FileName).c_str(), Data, FileOpeningOptions::ReadPermission | FileOpeningOptions::BinaryFormat);

	if (Result.Code != 0)
	{
		Log(LogType::Error, 0, "Cannot load shader from file. %s", Result.Msg);
	}

	return Data.Data;
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
