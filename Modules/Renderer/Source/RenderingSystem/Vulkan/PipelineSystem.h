#ifndef PIPELINESYSTEM_H
#define PIPELINESYSTEM_H

#include <unordered_map>
#include <vector>

#include <vulkan/vulkan.h>

#include "ShaderSystem.h"

// REF : https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Render_passes

class MemoryManager;
struct ImageData;

using DescriptorPoolID = std::string;
using PipelineID = std::string;

// Requires reconstruction - This struct will be destroyed and created again during various events (eg. window resizing).
struct PipelineData
{
	PipelineID mID;

	VkPipeline mPipelineHandle;
	VkPipelineLayout mPipelineLayout;

	// Descriptor Pool/Sets
	VkDescriptorPool mDescriptorPool;
	// One descriptor set per swapchain image.
	std::vector<VkDescriptorSet> mDescriptorSets;
};

class PipelineSystem
{
public:

	PipelineSystem() = delete;
	explicit PipelineSystem(const VkDevice* LogicalDevice);

	VkDescriptorSetLayout CreateDescriptorSetLayout(const VkDevice& Device);
	VkPipelineLayout CreatePipelineLayout(const VkDevice& Device, const std::vector<VkDescriptorSetLayout>* const DescriptorSetLayouts);

	void CreateGraphicsPipeline
	(
		PipelineID ID,
		const VkDevice* LogicalDevice,
		VkExtent2D ViewportExtent,
		VkFormat ImageFormat,
		const VkRenderPass* RenderPassHandle,
		const MemoryManager* MemoryManager,
		std::vector<char> ShaderCode_Vertex,
		std::vector<char> ShaderCode_Fragment
	);

	void CleanUp(const VkDevice& Device);
	void Destroy(const VkDevice& Device);

	PipelineData*				GetPipelineData(const PipelineID& ID) const;

	VkDescriptorPool CreateDescriptorPool
	(
		const VkDevice& LogicalDevice,
		const VkDescriptorPoolCreateInfo& DescriptorPoolCI
	);

	void AllocateDescriptorSets
	(
		const PipelineID& DescPoolID,
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
		VkDescriptorSetLayout LayoutToSet,
		MemoryManager* MemoryManager,
		uint32_t NumSwapChainImages
	);

	const std::vector<VkDescriptorSetLayout>& GetDescriptorSetLayouts() const;

	// When a texture is loaded, it needs to be associated with the pipeline system.
	// Associate loaded texture images with this pipeline, it can be then used in descriptor sets.
	void AssociateImage(const PipelineID& ID, const ImageData* Data);
	void DissociateImage(const PipelineID& ID, const ImageData* Data);

private:

	ShaderSystem mShaderSystem;

	std::vector<VkDescriptorSetLayout> mDescriptorSetLayouts;
	std::unordered_map<PipelineID, std::unique_ptr<PipelineData>> mPipelines;
	std::unordered_map<PipelineID, std::vector<const ImageData*>> mAssociatedImageData;

	// [todo] : Description sets are beign destroyed frequently, and they are dependant on the image association. We cannot break the association, or it will be lost in the ether, and texture will not be shown.
	// Figure out how to fix that.
	void CreateImageAssociationCategory(const PipelineID ID);
	void DestroyImageAssociationCategory(const PipelineID ID);

	std::vector<VkPipelineShaderStageCreateInfo> mShaderStages;

};

#endif