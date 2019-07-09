#ifndef PIPELINESYSTEM_H
#define PIPELINESYSTEM_H

#include <vulkan/vulkan.h>
#include "ShaderSystem/ShaderSystem.h"

// REF : https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Render_passes

struct PipelineSystemCreationInfo
{

};

class PipelineSystem
{
public:

	virtual	~PipelineSystem();

	void CreatePipelineLayout(const VkDevice& Device);
	void CreateRenderPass(const VkDevice& Device, const VkFormat& ImageFormat);

	void CreateGraphicsPipeline(const VkDevice& Device, const VkExtent2D& ViewportExtent, const VkFormat& ImageFormat);

	void CleanUp(const VkDevice& Device);
private:

	VkRenderPass mRenderPass;
	VkPipelineLayout mPipelineLayout;
	ShaderSystem mShaderSystem;
	VkViewport mViewport;
	VkPipeline mPipelineHandle;

	std::vector<VkPipelineShaderStageCreateInfo> mShaderStages;

};

#endif