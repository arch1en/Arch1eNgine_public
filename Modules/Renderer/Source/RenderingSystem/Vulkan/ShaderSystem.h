#ifndef SHADERSYSTEM_H
#define SHADERSYSTEM_H

#include <vulkan/vulkan_core.h>

#include <iostream>
#include <vector>

class ShaderSystem
{
public:

	VkShaderModule CreateShaderModule(const std::vector<char>& Code, const VkDevice& Device);
	std::vector<char> LoadShaderFromFile(const std::string& FileName);

private:

};

#endif