#ifndef PIPELINESYSTEM_H
#define PIPELINESYSTEM_H

#include <vulkan/vulkan.h>
#include "ShaderSystem.h"

// REF : https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Render_passes

class MemoryManager;

struct PipelineSystemCreationInfo
{
	const VkDevice* mLogicalDevice;
	VkExtent2D mViewportExtent;
	VkFormat mImageFormat;
	const VkRenderPass* mRenderPassHandle;
	const MemoryManager* mMemoryManager;
};

class PipelineSystem
{
public:

	void CreateDescriptorSetLayout(const VkDevice& Device);
	void CreatePipelineLayout(const VkDevice& Device, const std::vector<VkDescriptorSetLayout>* const DescriptorSetLayouts);

	void CreateGraphicsPipeline(const PipelineSystemCreationInfo& CreationInfo);

	void CleanUp(const VkDevice& Device);
	void Destroy(const VkDevice& Device);

	const VkPipeline*			GetPipelineHandle() const;
	const VkPipelineLayout*		GetPipelineLayout() const;

	const std::vector<VkDescriptorSetLayout>& GetDescriptorSetLayouts() const;

private:

	VkPipelineLayout mPipelineLayout;
	ShaderSystem mShaderSystem;
	VkViewport mViewport;
	VkPipeline mPipelineHandle;

	std::vector<VkPipelineShaderStageCreateInfo> mShaderStages;
	std::vector<VkDescriptorSetLayout> mDescriptorSetLayouts;
};

#endif