#ifndef PIPELINESYSTEM_H
#define PIPELINESYSTEM_H

#include <vulkan/vulkan.h>
#include "ShaderSystem/ShaderSystem.h"

// REF : https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Render_passes

struct PipelineSystemCreationInfo
{
	const VkDevice* mLogicalDevice;
	VkExtent2D mViewportExtent;
	VkFormat mImageFormat;
	const VkRenderPass* mRenderPassHandle;
};

class PipelineSystem
{
public:

	void CreatePipelineLayout(const VkDevice& Device);

	void CreateGraphicsPipeline(const PipelineSystemCreationInfo& CreationInfo);

	void CleanUp(const VkDevice& Device);
	void Destroy(const VkDevice& Device);

	const VkPipeline* GetPipelineHandle() const;

private:

	VkPipelineLayout mPipelineLayout;
	ShaderSystem mShaderSystem;
	VkViewport mViewport;
	VkPipeline mPipelineHandle;

	std::vector<VkPipelineShaderStageCreateInfo> mShaderStages;

};

#endif