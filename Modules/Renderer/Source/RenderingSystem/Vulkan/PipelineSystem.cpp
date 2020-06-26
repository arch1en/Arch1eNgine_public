#include "PipelineSystem.h"

#include "Debug/LogSystem.h"
#include "MemoryManager.h"

void PipelineSystem::CreateGraphicsPipeline
(
	const VkDevice* LogicalDevice,
	VkExtent2D ViewportExtent,
	VkFormat ImageFormat,
	const VkRenderPass* RenderPassHandle,
	const MemoryManager* MemoryManager,
	std::vector<char> ShaderCode_Vertex,
	std::vector<char> ShaderCode_Fragment
)
{
	VkShaderModule ShaderModule_Vertex = mShaderSystem.CreateShaderModule(ShaderCode_Vertex, *LogicalDevice);
	VkShaderModule ShaderModule_Fragment = mShaderSystem.CreateShaderModule(ShaderCode_Fragment, *LogicalDevice);

	VkPipelineShaderStageCreateInfo ShaderStageInfo_Vertex = {};
	ShaderStageInfo_Vertex.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	ShaderStageInfo_Vertex.stage = VK_SHADER_STAGE_VERTEX_BIT;
	ShaderStageInfo_Vertex.module = ShaderModule_Vertex;
	ShaderStageInfo_Vertex.pName = "main";

	VkPipelineShaderStageCreateInfo ShaderStageInfo_Fragment = {};
	ShaderStageInfo_Fragment.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	ShaderStageInfo_Fragment.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	ShaderStageInfo_Fragment.module = ShaderModule_Fragment;
	ShaderStageInfo_Fragment.pName = "main";

	mShaderStages = { ShaderStageInfo_Vertex, ShaderStageInfo_Fragment };

	const VkVertexInputBindingDescription BindingDescription = MemoryManager->GetBindingDescription<Vertex>();
	const std::vector<VkVertexInputAttributeDescription> AttributeDescriptions = MemoryManager->GetAttributeDescription<Vertex>();

	VkPipelineVertexInputStateCreateInfo VertexInputInfo = {};
	VertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	VertexInputInfo.vertexBindingDescriptionCount = 1;
	VertexInputInfo.vertexAttributeDescriptionCount = uint32_t(AttributeDescriptions.size());
	VertexInputInfo.pVertexBindingDescriptions = &BindingDescription;
	VertexInputInfo.pVertexAttributeDescriptions = AttributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo InputAssembly = {};
	InputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	InputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	InputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport Viewport = {};
	Viewport.x = 0.0f;
	Viewport.y = 0.0f;
	Viewport.width = static_cast<float>(ViewportExtent.width);
	Viewport.height = static_cast<float>(ViewportExtent.height);
	Viewport.minDepth = 0.0f;
	Viewport.maxDepth = 1.0f;

	VkRect2D Scissor = {};
	Scissor.offset = { 0,0 };
	Scissor.extent = ViewportExtent;

	VkPipelineViewportStateCreateInfo ViewportState = {};
	ViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	ViewportState.viewportCount = 1;
	ViewportState.pViewports = &Viewport;
	ViewportState.scissorCount = 1;
	ViewportState.pScissors = &Scissor;

	VkPipelineRasterizationStateCreateInfo Rasterizer = {};
	Rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	Rasterizer.depthClampEnable = VK_FALSE;
	Rasterizer.rasterizerDiscardEnable = VK_FALSE;
	Rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	Rasterizer.lineWidth = 1.0f;
	Rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	Rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	Rasterizer.depthBiasEnable = VK_FALSE;
	Rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	Rasterizer.depthBiasClamp = 0.0f; // Optional
	Rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	VkPipelineMultisampleStateCreateInfo Multisampling = {};
	Multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	Multisampling.sampleShadingEnable = VK_FALSE;
	Multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	Multisampling.minSampleShading = 1.f; // Optional
	Multisampling.pSampleMask = nullptr; // Optional
	Multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	Multisampling.alphaToOneEnable = VK_FALSE; // Optional

	VkPipelineColorBlendAttachmentState ColorBlendAttachment = {};
	ColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	ColorBlendAttachment.blendEnable = VK_FALSE;
	ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	ColorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	ColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	ColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	ColorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	VkPipelineColorBlendStateCreateInfo ColorBlendState = {};
	ColorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	ColorBlendState.logicOpEnable = VK_FALSE;
	ColorBlendState.logicOp = VK_LOGIC_OP_COPY; // Optional
	ColorBlendState.attachmentCount = 1;
	ColorBlendState.pAttachments = &ColorBlendAttachment;
	ColorBlendState.blendConstants[0] = 0.f; // OptionalBeginInfo
	ColorBlendState.blendConstants[1] = 0.f; // Optional
	ColorBlendState.blendConstants[2] = 0.f; // Optional
	ColorBlendState.blendConstants[3] = 0.f; // Optional

	CreateDescriptorSetLayout(*LogicalDevice);
	CreatePipelineLayout(*LogicalDevice, &mDescriptorSetLayouts);

	VkGraphicsPipelineCreateInfo PipelineInfo = {};
	PipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	PipelineInfo.stageCount = 2;
	PipelineInfo.pStages = &mShaderStages[0]; // @todo : How to add more handles (if its possible).
	PipelineInfo.pVertexInputState = &VertexInputInfo;
	PipelineInfo.pInputAssemblyState = &InputAssembly;
	PipelineInfo.pViewportState = &ViewportState;
	PipelineInfo.pRasterizationState = &Rasterizer;
	PipelineInfo.pMultisampleState = &Multisampling;
	PipelineInfo.pColorBlendState = &ColorBlendState;
	PipelineInfo.layout = mPipelineLayout;
	PipelineInfo.renderPass = *RenderPassHandle;
	PipelineInfo.subpass = 0;
	PipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	PipelineInfo.basePipelineIndex = -1;

	if (vkCreateGraphicsPipelines(*LogicalDevice, VK_NULL_HANDLE, 1, &PipelineInfo, nullptr, &mPipelineHandle) != VK_SUCCESS) {
		LogVk(LogType::Error, 0, "Graphics pipeline creation failed !");
	}

	vkDestroyShaderModule(*LogicalDevice, ShaderModule_Vertex, nullptr);
	vkDestroyShaderModule(*LogicalDevice, ShaderModule_Fragment, nullptr);

}

void PipelineSystem::CreatePipelineLayout(const VkDevice& Device, const std::vector<VkDescriptorSetLayout>* const DescriptorSetLayouts)
{
	VkPipelineLayoutCreateInfo PipelineLayoutInfo = {};
	PipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	PipelineLayoutInfo.setLayoutCount = uint32_t(DescriptorSetLayouts->size()); // Optional
	PipelineLayoutInfo.pSetLayouts = DescriptorSetLayouts->data(); // Optional
	PipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	PipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(Device, &PipelineLayoutInfo, nullptr, &mPipelineLayout) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Pipeline layout creation failed !");
	}
}

void PipelineSystem::CreateDescriptorSetLayout(const VkDevice& Device)
{
	VkDescriptorSetLayoutBinding LayoutBindingUBO = {};
	LayoutBindingUBO.binding = 0;
	LayoutBindingUBO.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	LayoutBindingUBO.descriptorCount = 1;
	LayoutBindingUBO.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	LayoutBindingUBO.pImmutableSamplers = nullptr; // Optional

	VkDescriptorSetLayoutBinding LayoutBindingSampler = {};
	LayoutBindingSampler.binding = 1;
	LayoutBindingSampler.descriptorCount = 1;
	LayoutBindingSampler.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	LayoutBindingSampler.pImmutableSamplers = nullptr;
	LayoutBindingSampler.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 2>  Bindings = { LayoutBindingUBO, LayoutBindingSampler };

	VkDescriptorSetLayoutCreateInfo LayoutCI = {};
	LayoutCI.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	LayoutCI.bindingCount = static_cast<uint32_t>(Bindings.size());
	LayoutCI.pBindings = Bindings.data();

	VkDescriptorSetLayout NewLayout;

	if (vkCreateDescriptorSetLayout(Device, &LayoutCI, nullptr, &NewLayout) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Descriptor set layout creation failed!");
	}

	mDescriptorSetLayouts.push_back(NewLayout);
}


void PipelineSystem::CreateDescriptorPoolAndUpdateDescriptorSets
(
	DescriptorPoolID DescPoolID,
	const VkDevice& LogicalDevice,
	VkDescriptorPoolCreateInfo PoolCreateInfo,
	std::vector<VkDescriptorPoolSize> PoolSizes,
	MemoryManager* MemoryManager, 
	uint32_t NumSwapChainImages
)
{
	CreateDescriptorPool(DescPoolID, LogicalDevice, PoolCreateInfo);

	std::vector<VkDescriptorSetLayout> LayoutsToSet(NumSwapChainImages, GetDescriptorSetLayouts()[0]);

	AllocateDescriptorSets(DescPoolID, LogicalDevice, LayoutsToSet, NumSwapChainImages);
	UpdateDescriptorSets(DescPoolID, LogicalDevice, MemoryManager, NumSwapChainImages);
}

void PipelineSystem::CreateDescriptorPool(const DescriptorPoolID& DescPoolID, const VkDevice& LogicalDevice, const VkDescriptorPoolCreateInfo& DescriptorPoolCI)
{
	Assert(DescPoolID.compare("") != 0, "Descriptor pool ID cannot be empty!");

	VkDescriptorPool Pool;

	if (vkCreateDescriptorPool(LogicalDevice, &DescriptorPoolCI, nullptr, &Pool) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Descriptor pool creation failed!");
	}

	mDescriptorPools.insert({ DescPoolID, Pool });
}

void PipelineSystem::AllocateDescriptorSets(const DescriptorPoolID& DescPoolID, const VkDevice& LogicalDevice, const std::vector<VkDescriptorSetLayout>& LayoutsToSet, uint32_t NumSwapChainImages)
{
	VkDescriptorSetAllocateInfo AllocInfo = {};

	AllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	AllocInfo.descriptorPool = *GetDescriptorPool(DescPoolID);
	AllocInfo.descriptorSetCount = NumSwapChainImages;
	AllocInfo.pSetLayouts = LayoutsToSet.data();

	mDescriptorSets.resize(NumSwapChainImages);

	if (vkAllocateDescriptorSets(LogicalDevice, &AllocInfo, mDescriptorSets.data()) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Descriptor sets allocation failed!");
	}
}

void PipelineSystem::UpdateDescriptorSets(const DescriptorPoolID& DescPoolID, const VkDevice& LogicalDevice, MemoryManager* MemoryManager, uint32_t NumSwapChainImages)
{
	for (size_t i = 0; i < NumSwapChainImages; i++)
	{
		int Binding = 0;

		VkDescriptorBufferInfo BufferInfo = {};
		BufferInfo.buffer = (*MemoryManager->GetUniformBufferData())[i]->mBuffer;
		BufferInfo.offset = 0;
		BufferInfo.range = sizeof(UniformBufferObject);

		std::vector<VkWriteDescriptorSet> DescriptorWrites = {};

		VkWriteDescriptorSet UboDescriptorWrite = {};

		UboDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		UboDescriptorWrite.dstSet = mDescriptorSets[i];
		UboDescriptorWrite.dstBinding = Binding++;
		UboDescriptorWrite.dstArrayElement = 0;
		UboDescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		UboDescriptorWrite.descriptorCount = 1;
		UboDescriptorWrite.pBufferInfo = &BufferInfo;
		UboDescriptorWrite.pImageInfo = nullptr;	// Optional
		UboDescriptorWrite.pTexelBufferView = nullptr; // Optional

		DescriptorWrites.push_back(UboDescriptorWrite);

		if (mAssociatedImageData.size() > 0)
		{
			for (const ImageData* ImageDataRef : mAssociatedImageData)
			{
				VkDescriptorImageInfo ImageInfo = {};
				ImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				ImageInfo.imageView = ImageDataRef->mTextureImageView;
				ImageInfo.sampler = ImageDataRef->mTextureSampler;

				VkWriteDescriptorSet ImageDescriptorWrite = {};

				ImageDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				ImageDescriptorWrite.dstSet = mDescriptorSets[i];
				ImageDescriptorWrite.dstBinding = Binding++;
				ImageDescriptorWrite.dstArrayElement = 0;
				ImageDescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				ImageDescriptorWrite.descriptorCount = 1;
				ImageDescriptorWrite.pImageInfo = &ImageInfo;

				DescriptorWrites.push_back(ImageDescriptorWrite);
			}
		}

		vkUpdateDescriptorSets(LogicalDevice, static_cast<uint32_t>(DescriptorWrites.size()), DescriptorWrites.data(), 0, nullptr);
	}
}


void PipelineSystem::CleanUp(const VkDevice& Device)
{
	if (mPipelineHandle != VK_NULL_HANDLE)
	{
		vkDestroyPipeline(Device, mPipelineHandle, nullptr);
		mPipelineHandle = VK_NULL_HANDLE;
	}

	if (mPipelineLayout != VK_NULL_HANDLE)
	{
		vkDestroyPipelineLayout(Device, mPipelineLayout, nullptr);
		mPipelineLayout = VK_NULL_HANDLE;
	}

	for (auto iter = mDescriptorPools.rbegin(); iter != mDescriptorPools.rend(); ++iter)
	{
		vkDestroyDescriptorPool(Device, iter->second, nullptr);
	}
	mDescriptorPools.clear();
}

void PipelineSystem::Destroy(const VkDevice& Device)
{
	CleanUp(Device);

	for (auto& i : mDescriptorSetLayouts)
	{
		vkDestroyDescriptorSetLayout(Device, i, nullptr);
	}

	mDescriptorSetLayouts.erase(mDescriptorSetLayouts.begin(), mDescriptorSetLayouts.end());
}

const VkPipeline* PipelineSystem::GetPipelineHandle() const
{
	return &mPipelineHandle;
}

const VkPipelineLayout* PipelineSystem::GetPipelineLayout() const
{
	return &mPipelineLayout;
}

const std::vector<VkDescriptorSetLayout>& PipelineSystem::GetDescriptorSetLayouts() const
{
	return mDescriptorSetLayouts;
}

const VkDescriptorPool* const PipelineSystem::GetMainDescriptorPool()
{
	return GetDescriptorPool("main");
}

const VkDescriptorPool* const PipelineSystem::GetDescriptorPool(DescriptorPoolID ID) 
{
	return &mDescriptorPools[ID];
}

const std::vector<VkDescriptorSet> PipelineSystem::GetDescriptorSets()
{
	return mDescriptorSets;
}

void PipelineSystem::AssociateImage(const ImageData* Data)
{
	auto Iterator = std::find(mAssociatedImageData.begin(), mAssociatedImageData.end(), Data);
	if (Iterator == mAssociatedImageData.end())
	{
		mAssociatedImageData.push_back(Data);
	}
	else
	{
		LogVk(LogType::Error, 0, "Image was already associated!");
	}
}

void PipelineSystem::DissociateImage(const ImageData* Data)
{
	auto Iterator = std::find(mAssociatedImageData.begin(), mAssociatedImageData.end(), Data);
	if (Iterator != mAssociatedImageData.end())
	{
		mAssociatedImageData.erase(Iterator);
	}
	else
	{
		LogVk(LogType::Error, 0, "Image could not be dissociated, missing from the association array!");
	}
}
