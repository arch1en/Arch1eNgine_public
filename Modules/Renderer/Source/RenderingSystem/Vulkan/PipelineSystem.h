#ifndef PIPELINESYSTEM_H
#define PIPELINESYSTEM_H

#include <map>
#include <vector>

#include <vulkan/vulkan.h>

#include "ShaderSystem.h"

// REF : https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Render_passes

class MemoryManager;

using DescriptorPoolID = std::string;

struct PipelineSystemCreationInfo
{
	const VkDevice* mLogicalDevice;
	VkExtent2D mViewportExtent;
	VkFormat mImageFormat;
	const VkRenderPass* mRenderPassHandle;
	const MemoryManager* mMemoryManager;
};

struct DescriptorPoolCreateInfo
{
	const VkDevice* mLogicalDevice = nullptr;
	std::vector<VkDescriptorPoolSize> mPoolSizes = {};
	VkDescriptorPoolCreateInfo mPoolCreateInfo = {};
	DescriptorPoolID mDescriptorPoolID = "";
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

	void CreateDescriptorPool(const DescriptorPoolCreateInfo& CreateInfo);
	void UpdateDescriptorSets(const VkDevice* Device, MemoryManager* MemoryManager, uint32_t NumSwapChainImages);
	void CreateDescriptorPoolAndUpdateDescriptorSets(const DescriptorPoolCreateInfo& CreateInfo, MemoryManager* MemoryManager, uint32_t NumSwapChainImages);

	const std::vector<VkDescriptorSetLayout>& GetDescriptorSetLayouts() const;
	const VkDescriptorPool* const GetMainDescriptorPool();
	const VkDescriptorPool* const GetDescriptorPool(DescriptorPoolID ID);
	const std::vector<VkDescriptorSet>* GetDescriptorSets();


private:

	VkPipelineLayout mPipelineLayout;
	ShaderSystem mShaderSystem;
	VkViewport mViewport;
	VkPipeline mPipelineHandle;

	std::vector<VkPipelineShaderStageCreateInfo> mShaderStages;

	// Descriptor Pool/Sets
	std::map<DescriptorPoolID, VkDescriptorPool> mDescriptorPools;
	std::vector<VkDescriptorSet> mDescriptorSets;
	std::vector<VkDescriptorSetLayout> mDescriptorSetLayouts;

};

#endif