#include "PipelineSystem.h"

#include <memory>

#include "Debug/LogSystem.h"
#include "MemoryManager.h"

PipelineSystem::PipelineSystem(const VkDevice* LogicalDevice)
{
	mDescriptorSetLayouts.push_back(CreateDescriptorSetLayout(*LogicalDevice));
}

void PipelineSystem::CreateGraphicsPipeline
(
	PipelineID ID,
	const VkDevice* LogicalDevice,
	VkExtent2D ViewportExtent,
	VkFormat ImageFormat,
	const VkRenderPass* RenderPassHandle,
	const MemoryManager* MemoryManager,
	std::vector<char> ShaderCode_Vertex,
	std::vector<char> ShaderCode_Fragment
)
{
	PipelineData NewPipeline = {};

	VkShaderModule ShaderModule_Vertex = mShaderSystem.CreateShaderModule(ShaderCode_Vertex, *LogicalDevice);
	VkShaderModule ShaderModule_Fragment = mShaderSystem.CreateShaderModule(ShaderCode_Fragment, *LogicalDevice);

	VkPipelineShaderStageCreateInfo ShaderStageInfo_Vertex = {};
	ShaderStageInfo_Vertex.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	ShaderStageInfo_Vertex.stage = VK_SHADER_STAGE_VERTEX_BIT;
	ShaderStageInfo_Vertex.module = ShaderModule_Vertex;
	ShaderStageInfo_Vertex.pName = ID.c_str();

	VkPipelineShaderStageCreateInfo ShaderStageInfo_Fragment = {};
	ShaderStageInfo_Fragment.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	ShaderStageInfo_Fragment.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	ShaderStageInfo_Fragment.module = ShaderModule_Fragment;
	ShaderStageInfo_Fragment.pName = ID.c_str();

	const std::vector<VkPipelineShaderStageCreateInfo> ShaderStages = { ShaderStageInfo_Vertex, ShaderStageInfo_Fragment };

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

	NewPipeline.mPipelineLayout = CreatePipelineLayout(*LogicalDevice, &mDescriptorSetLayouts);
	CreateDescriptorSetLayout(*LogicalDevice);
	CreatePipelineLayout(*LogicalDevice, &mDescriptorSetLayouts);

	VkGraphicsPipelineCreateInfo PipelineInfo = {};
	PipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	PipelineInfo.stageCount = ShaderStages.size();
	PipelineInfo.pStages = &ShaderStages[0]; // @todo : How to add more handles (if its possible).
	PipelineInfo.pVertexInputState = &VertexInputInfo;
	PipelineInfo.pInputAssemblyState = &InputAssembly;
	PipelineInfo.pViewportState = &ViewportState;
	PipelineInfo.pRasterizationState = &Rasterizer;
	PipelineInfo.pMultisampleState = &Multisampling;
	PipelineInfo.pColorBlendState = &ColorBlendState;
	PipelineInfo.layout = NewPipeline.mPipelineLayout;
	PipelineInfo.renderPass = *RenderPassHandle;
	PipelineInfo.subpass = 0;
	PipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	PipelineInfo.basePipelineIndex = -1;

	if (vkCreateGraphicsPipelines(*LogicalDevice, VK_NULL_HANDLE, 1, &PipelineInfo, nullptr, &NewPipeline.mPipelineHandle) != VK_SUCCESS) {
		LogVk(LogType::Error, 0, "Graphics pipeline creation failed !");
	}

	vkDestroyShaderModule(*LogicalDevice, ShaderModule_Vertex, nullptr);
	vkDestroyShaderModule(*LogicalDevice, ShaderModule_Fragment, nullptr);

	mPipelines.insert({ID, std::make_unique<PipelineData>(NewPipeline)});
}

VkPipelineLayout PipelineSystem::CreatePipelineLayout(const VkDevice& Device, const std::vector<VkDescriptorSetLayout>* const DescriptorSetLayouts)
{
	VkPipelineLayoutCreateInfo PipelineLayoutInfo = {};
	PipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	PipelineLayoutInfo.setLayoutCount = uint32_t(DescriptorSetLayouts->size()); // Optional
	PipelineLayoutInfo.pSetLayouts = DescriptorSetLayouts->data(); // Optional
	PipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	PipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	VkPipelineLayout NewPipelineLayout;

	if (vkCreatePipelineLayout(Device, &PipelineLayoutInfo, nullptr, &NewPipelineLayout) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Pipeline layout creation failed !");
	}
	
	return NewPipelineLayout;
}

VkDescriptorSetLayout PipelineSystem::CreateDescriptorSetLayout(const VkDevice& Device)
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

	return NewLayout;
}


void PipelineSystem::CreateDescriptorPoolAndUpdateDescriptorSets
(
	PipelineID ID,
	const VkDevice& LogicalDevice,
	VkDescriptorPoolCreateInfo PoolCreateInfo,
	std::vector<VkDescriptorPoolSize> PoolSizes,
	VkDescriptorSetLayout LayoutToSet,
	MemoryManager* MemoryManager, 
	uint32_t NumSwapChainImages
)
{
	PipelineData* Data = GetPipelineData(ID);

	Data->mDescriptorPool = CreateDescriptorPool(LogicalDevice, PoolCreateInfo);

	std::vector<VkDescriptorSetLayout> LayoutsToSet(NumSwapChainImages, LayoutToSet);

	AllocateDescriptorSets(ID, LogicalDevice, LayoutsToSet, NumSwapChainImages);
	UpdateDescriptorSets(ID, LogicalDevice, MemoryManager, NumSwapChainImages);
}

VkDescriptorPool PipelineSystem::CreateDescriptorPool(const VkDevice& LogicalDevice, const VkDescriptorPoolCreateInfo& DescriptorPoolCI)
{
	VkDescriptorPool Pool;

	if (vkCreateDescriptorPool(LogicalDevice, &DescriptorPoolCI, nullptr, &Pool) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Descriptor pool creation failed!");
	}

	return Pool;
}

void PipelineSystem::AllocateDescriptorSets(const PipelineID& ID, const VkDevice& LogicalDevice, const std::vector<VkDescriptorSetLayout>& LayoutsToSet, uint32_t NumSwapChainImages)
{
	PipelineData* Data = GetPipelineData(ID);

	VkDescriptorSetAllocateInfo AllocInfo = {};

	AllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	AllocInfo.descriptorPool = Data->mDescriptorPool;
	AllocInfo.descriptorSetCount = NumSwapChainImages;
	AllocInfo.pSetLayouts = LayoutsToSet.data();

	Data->mDescriptorSets.resize(NumSwapChainImages);

	if (vkAllocateDescriptorSets(LogicalDevice, &AllocInfo, Data->mDescriptorSets.data()) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Descriptor sets allocation failed!");
	}
}

void PipelineSystem::UpdateDescriptorSets(const PipelineID& ID, const VkDevice& LogicalDevice, MemoryManager* MemoryManager, uint32_t NumSwapChainImages)
{
	PipelineData* Data = GetPipelineData(ID);

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
		UboDescriptorWrite.dstSet = Data->mDescriptorSets[i];
		UboDescriptorWrite.dstBinding = Binding++;
		UboDescriptorWrite.dstArrayElement = 0;
		UboDescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		UboDescriptorWrite.descriptorCount = 1;
		UboDescriptorWrite.pBufferInfo = &BufferInfo;
		UboDescriptorWrite.pImageInfo = nullptr;	// Optional
		UboDescriptorWrite.pTexelBufferView = nullptr; // Optional

		DescriptorWrites.push_back(UboDescriptorWrite);

		if (mAssociatedImageData.empty() == false)
		{
			std::vector<const ImageData*>& AssociatedImageDataRef = mAssociatedImageData.at(ID);

			for (const ImageData* ImageDataRef : AssociatedImageDataRef)
			{
				VkDescriptorImageInfo ImageInfo = {};
				ImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				ImageInfo.imageView = ImageDataRef->mTextureImageView;
				ImageInfo.sampler = ImageDataRef->mTextureSampler;

				VkWriteDescriptorSet ImageDescriptorWrite = {};

				ImageDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				ImageDescriptorWrite.dstSet = Data->mDescriptorSets[i];
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
	for (auto& i : mPipelines)
	{
		if (i.second->mPipelineHandle != VK_NULL_HANDLE)
		{
			vkDestroyPipeline(Device, i.second->mPipelineHandle, nullptr);
			i.second->mPipelineHandle = VK_NULL_HANDLE;
		}

		if (i.second->mPipelineLayout != VK_NULL_HANDLE)
		{
			vkDestroyPipelineLayout(Device, i.second->mPipelineLayout, nullptr);
			i.second->mPipelineLayout = VK_NULL_HANDLE;
		}

		vkDestroyDescriptorPool(Device, i.second->mDescriptorPool, nullptr);
		// Descriptor sets will be automatically freed when descriptor pool is destroyed.
	}

	mPipelines.erase(mPipelines.begin(), mPipelines.end());
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

PipelineData* PipelineSystem::GetPipelineData(const PipelineID& ID) const
{
	return mPipelines.at(ID).get();
}

const std::vector<VkDescriptorSetLayout>& PipelineSystem::GetDescriptorSetLayouts() const
{
	return mDescriptorSetLayouts;
}

void PipelineSystem::AssociateImage(const PipelineID& ID, const ImageData* Data)
{
	auto Found = mAssociatedImageData.find(ID);


	if (Found == mAssociatedImageData.end())
	{
		CreateImageAssociationCategory(ID);
	}

	auto& AssociatedImageDataRef = mAssociatedImageData.at(ID);

	auto Iterator = std::find(AssociatedImageDataRef.begin(), AssociatedImageDataRef.end(), Data);
	if (Iterator == AssociatedImageDataRef.end())
	{
		AssociatedImageDataRef.push_back(Data);
	}
	else
	{
		LogVk(LogType::Error, 0, "Image was already associated!");
	}
}

void PipelineSystem::DissociateImage(const PipelineID& ID, const ImageData* Data)
{
	auto AssociatedImageDataRef = mAssociatedImageData.at(ID);

	auto Iterator = std::find(AssociatedImageDataRef.begin(), AssociatedImageDataRef.end(), Data);
	if (Iterator != AssociatedImageDataRef.end())
	{
		AssociatedImageDataRef.erase(Iterator);
	}
	else
	{
		LogVk(LogType::Error, 0, "Image could not be dissociated, missing from the association array!");
	}

	if (AssociatedImageDataRef.empty())
	{
		DestroyImageAssociationCategory(ID);
	}
}

void PipelineSystem::CreateImageAssociationCategory(const PipelineID ID)
{
	auto Found = mAssociatedImageData.find(ID);
	if (Found == mAssociatedImageData.end())
	{
		mAssociatedImageData.insert({ID, std::vector<const ImageData*>()});
	}
}

void PipelineSystem::DestroyImageAssociationCategory(const PipelineID ID)
{
	auto Found = mAssociatedImageData.find(ID);
	mAssociatedImageData.erase(Found);
}
