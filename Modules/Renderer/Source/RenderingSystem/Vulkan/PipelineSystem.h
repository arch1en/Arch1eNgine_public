#ifndef PIPELINESYSTEM_H
#define PIPELINESYSTEM_H

#include <map>
#include <vector>

#include <vulkan/vulkan.h>

#include "ShaderSystem.h"

// REF : https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Render_passes

class MemoryManager;
struct ImageData;

using DescriptorPoolID = std::string;

struct PipelineSystemCreationInfo
{
	const VkDevice* mLogicalDevice;
	VkExtent2D mViewportExtent;
	VkFormat mImageFormat;
	const VkRenderPass* mRenderPassHandle;
	const MemoryManager* mMemoryManager;

	std::vector<char> mShaderCode_Vertex;
	std::vector<char> mShaderCode_Fragment;
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

	void CreateDescriptorPool
	(
		const DescriptorPoolID& DescPoolID,
		const VkDevice& LogicalDevice,
		const VkDescriptorPoolCreateInfo& DescriptorPoolCI
	);

	void AllocateDescriptorSets
	(
		const DescriptorPoolID& DescPoolID,
		const VkDevice& LogicalDevice,
		const std::vector<VkDescriptorSetLayout>& LayoutsToSet,
		uint32_t NumSwapChainImages
	);
	
	void UpdateDescriptorSets
	(
		const DescriptorPoolID& DescPoolID,
		const VkDevice& LogicalDevice,
		MemoryManager* MemoryManager,
		uint32_t NumSwapChainImages
	);

	void CreateDescriptorPoolAndUpdateDescriptorSets
	(
		DescriptorPoolID DescPoolID,
		const VkDevice& LogicalDevice,
		VkDescriptorPoolCreateInfo PoolCreateInfo,
		std::vector<VkDescriptorPoolSize> PoolSizes,
		MemoryManager* MemoryManager,
		uint32_t NumSwapChainImages
	);

	const std::vector<VkDescriptorSetLayout>& GetDescriptorSetLayouts() const;
	const VkDescriptorPool* const GetMainDescriptorPool();
	const VkDescriptorPool* const GetDescriptorPool(DescriptorPoolID ID);
	const std::vector<VkDescriptorSet> GetDescriptorSets();

	// When a texture is loaded, it needs to be associated with the pipeline system.
	// Associate loaded texture images with this pipeline, it can be then used in descriptor sets.
	void AssociateImage(const ImageData* Data);
	void DissociateImage(const ImageData* Data);

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
	std::vector<const ImageData*> mAssociatedImageData;

};

#endif