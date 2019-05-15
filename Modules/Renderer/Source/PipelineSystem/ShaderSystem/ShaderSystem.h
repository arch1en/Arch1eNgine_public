#ifndef SHADERSYSTEM_H
#define SHADERSYSTEM_H

#include <vector>

class ShaderSystem
{
public:

	VkShaderModule CreateShaderModule(const std::vector<char>& Code);
	std::vector<char> LoadShaderFromFile(const std::string& FileName);

private:


};

#endif