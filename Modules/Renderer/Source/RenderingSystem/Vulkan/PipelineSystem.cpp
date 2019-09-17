#include "PipelineSystem.h"

#include "LogSystem.h"



void PipelineSystem::CreateGraphicsPipeline(const PipelineSystemCreationInfo& CreationInfo)
{
	auto ShaderCode_Vertex = mShaderSystem.LoadShaderFromFile("Shaders/Main.vert.spv");
	auto ShaderCode_Fragment = mShaderSystem.LoadShaderFromFile("Shaders/Main.frag.spv");

	VkShaderModule ShaderModule_Vertex = mShaderSystem.CreateShaderModule(ShaderCode_Vertex, *CreationInfo.mLogicalDevice);
	VkShaderModule ShaderModule_Fragment = mShaderSystem.CreateShaderModule(ShaderCode_Fragment, *CreationInfo.mLogicalDevice);

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

	VkPipelineVertexInputStateCreateInfo VertexInputInfo = {};
	VertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	VertexInputInfo.vertexBindingDescriptionCount = 0;
	VertexInputInfo.vertexAttributeDescriptionCount = 0;

	VkPipelineInputAssemblyStateCreateInfo InputAssembly = {};
	InputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	InputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	InputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport Viewport = {};
	Viewport.x = 0.0f;
	Viewport.y = 0.0f;
	Viewport.width = static_cast<float>(CreationInfo.mViewportExtent.width);
	Viewport.height = static_cast<float>(CreationInfo.mViewportExtent.height);
	Viewport.minDepth = 0.0f;
	Viewport.maxDepth = 1.0f;

	VkRect2D Scissor = {};
	Scissor.offset = { 0,0 };
	Scissor.extent = CreationInfo.mViewportExtent;

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
	Rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
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

	VkPipelineLayoutCreateInfo PipelineLayoutInfo = {};
	PipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	PipelineLayoutInfo.setLayoutCount = 0; // Optional
	PipelineLayoutInfo.pSetLayouts = nullptr; // Optional
	PipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	PipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(*CreationInfo.mLogicalDevice, &PipelineLayoutInfo, nullptr, &mPipelineLayout) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Pipeline layout creation failed !");
	}

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
	PipelineInfo.renderPass = *CreationInfo.mRenderPassHandle;
	PipelineInfo.subpass = 0;
	PipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	PipelineInfo.basePipelineIndex = -1;

	if (vkCreateGraphicsPipelines(*CreationInfo.mLogicalDevice, VK_NULL_HANDLE, 1, &PipelineInfo, nullptr, &mPipelineHandle) != VK_SUCCESS) {
		LogVk(LogType::Error, 0, "Graphics pipeline creation failed !");
	}

	vkDestroyShaderModule(*CreationInfo.mLogicalDevice, ShaderModule_Vertex, nullptr);
	vkDestroyShaderModule(*CreationInfo.mLogicalDevice, ShaderModule_Fragment, nullptr);

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
}

void PipelineSystem::Destroy(const VkDevice& Device)
{
	CleanUp(Device);
}

const VkPipeline* PipelineSystem::GetPipelineHandle() const
{
	return &mPipelineHandle;
}
