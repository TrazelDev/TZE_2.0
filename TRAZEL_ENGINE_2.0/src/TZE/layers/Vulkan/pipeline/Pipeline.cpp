#include "pch.h"
#include "Pipeline.h"
#include "layers/Vulkan/Objects/Model.h"

tze::Pipeline::Pipeline(const pipelineInput& input,
	const std::string& vertexShaderPath, const std::string& fragmentShaderPath) :
	_input(input), _vertexShaderPath(vertexShaderPath), _fragmentShaderPath(fragmentShaderPath)
{
	createPipeLine(_input._swapchainExtent);
}

tze::Pipeline::~Pipeline()
{
	destroyPipeline();
}

void tze::Pipeline::run()
{
}

void tze::Pipeline::recreatePipeline(vk::Extent2D& swapchainExtent)
{
	destroyPipeline();
	createPipeLine(swapchainExtent);
}

vk::RenderPass& tze::Pipeline::getRenderPass()
{
	return _renderPass;
}

vk::Pipeline& tze::Pipeline::getPipeline()
{
	return _graphicsPipeline;
}

vk::PipelineLayout& tze::Pipeline::getLayout()
{
	return _layout;
}

void tze::Pipeline::createPipeLine(vk::Extent2D& swapchainExtent)
{
	vk::GraphicsPipelineCreateInfo pipelineInfo = {};
	vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
	std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;
	vk::PipelineShaderStageCreateInfo vertexShaderInfo;
	vk::Viewport viewport = {};
	vk::Rect2D scissor = {};
	vk::PipelineViewportStateCreateInfo viewportState = {};
	vk::PipelineRasterizationStateCreateInfo rasterizer = {};
	vk::ShaderModule vertexShader;
	vk::ShaderModule fragmentShader;
	vk::PipelineShaderStageCreateInfo fragmentShaderInfo = {};
	vk::PipelineMultisampleStateCreateInfo multisampling = {};
	vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
	vk::PipelineColorBlendStateCreateInfo colorBlending = {};
	vk::DynamicState dynamicStates[] =
	{
		vk::DynamicState::eViewport,
		vk::DynamicState::eScissor
	};
	vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo;
	// getting the vertex shader attributes and binding:
	auto bindingDescription = Model::Vertex::getBindingDescriptions();
	auto attributeDescription = Model::Vertex::getAttributeDescriptions();

	// creating the vertex creating info:
	vertexInputInfo.flags = vk::PipelineVertexInputStateCreateFlags();
	vertexInputInfo.sType = vk::StructureType::ePipelineVertexInputStateCreateInfo; // VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexAttributeDescriptionCount = uint32_t(attributeDescription.size());
	vertexInputInfo.vertexBindingDescriptionCount = uint32_t(bindingDescription.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescription.data();
	vertexInputInfo.pVertexBindingDescriptions = bindingDescription.data();

	// Assembly input (the way we take the vertices and assemble the shapes on the screen):
	inputAssemblyInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
	inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;

	vertexShader = createModule(_vertexShaderPath);
	vertexShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
	vertexShaderInfo.stage = vk::ShaderStageFlagBits::eVertex;
	vertexShaderInfo.module = vertexShader;
	vertexShaderInfo.pName = "main";
	shaderStages.push_back(vertexShaderInfo);

	// creating a view port: ( view port is a place on the screen that we can render to a multiple view ports can be created to simulate a few things on the screen)
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = swapchainExtent.width;
	viewport.height = swapchainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	// creating a scissor: it is similar to viewport but can not be change probably)
	scissor.offset.x = 0.0f;
	scissor.offset.y = 0.0f;
	scissor.extent = swapchainExtent;

	// creating a view port states because there could be more than one viewport on the screen:
	viewportState.flags = vk::PipelineViewportStateCreateFlags();
	viewportState.viewportCount = 1;
	viewportState.pViewports = nullptr;
	viewportState.scissorCount = 1;
	viewportState.pScissors = nullptr;

	// rasterizer creation:
	// here are some properties to know for the future (chatGpt credit)
	/*
	Polygon mode: Determines how polygons are rendered. The options are Fill (default), Line, and Point.
	Culling: Determines which faces of polygons are rendered. The options are None, Front, Back, and FrontAndBack. Back-face culling is commonly used to improve performance by discarding fragments that are not visible.
	Winding order: Determines the order in which vertices are interpreted for front-facing polygons. The options are Clockwise (default) and CounterClockwise.
	Depth bias: Adds a constant offset to the depth value of fragments. This is useful for avoiding z-fighting (where fragments overlap and flicker) in certain situations.
	Depth clipping: Determines whether fragments outside of the
	depth range should be clipped or clamped. Clipping discards fragments that are outside the depth range, while clamping keeps them within the range.
	Depth bias slope factor and constant factor: These factors are used to adjust the depth bias based on the slope of the polygon relative to the view direction. This can be used to avoid artifacts such as "stair-stepping" along edges.
	Line width: Sets the width of lines in pixels.
	Rasterizer discard: Determines whether all fragments generated by the rasterizer are discarded. This is useful for certain rendering techniques, such as occlusion culling.
	Multisampling: Determines how multiple samples within a pixel are combined to produce the final color and depth values.
	Sample shading: Determines the number of shading samples used per pixel. This can be used to improve the quality of shading, particularly when using antialiasing.
	Alpha-to-coverage: Determines how alpha values are used to combine fragments when multisampling is enabled. This can be used to produce smoother edges for objects with alpha textures.
	*/
	rasterizer.flags = vk::PipelineRasterizationStateCreateFlags();
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = vk::PolygonMode::eFill;
	rasterizer.lineWidth = 1.0;
	rasterizer.cullMode = vk::CullModeFlagBits::eBack;
	rasterizer.frontFace = vk::FrontFace::eClockwise;
	rasterizer.depthBiasEnable = VK_FALSE;

	// creation of fragment shader:
	fragmentShader = createModule(_fragmentShaderPath);
	fragmentShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
	fragmentShaderInfo.stage = vk::ShaderStageFlagBits::eFragment;
	fragmentShaderInfo.module = fragmentShader;
	fragmentShaderInfo.pName = "main";
	shaderStages.push_back(fragmentShaderInfo);

	// multisampling: ( a technique used to fix curved or diagonal lines to make them smoother by taking multiple pixels from an area and
	// averaging them out to make it smoother as I said )
	multisampling.flags = vk::PipelineMultisampleStateCreateFlags();
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

	// color blend ( probably change in the future starting values only) :
	colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlending.flags = vk::PipelineColorBlendStateCreateFlags();
	colorBlending.logicOp = vk::LogicOp::eCopy;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	// dynamic create info:
	dynamicStateCreateInfo.sType = vk::StructureType::ePipelineDynamicStateCreateInfo;
	dynamicStateCreateInfo.pDynamicStates = dynamicStates;
	dynamicStateCreateInfo.dynamicStateCount = /*sizeof(dynamicStates)*/ 2;
	pipelineInfo.pDynamicState = &dynamicStateCreateInfo;

	_layout = makePipelineLayout();
	_renderPass = makeRenderPass();

	// putting the info inside of the pipeline creation info structure
	pipelineInfo.flags = vk::PipelineCreateFlags();
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.stageCount = shaderStages.size();
	pipelineInfo.pStages = shaderStages.data();
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.layout = _layout;
	pipelineInfo.renderPass = _renderPass;
	pipelineInfo.basePipelineHandle = nullptr;

	try
	{
		auto pair = _input._logicalDevice.createGraphicsPipeline(nullptr, pipelineInfo);
		if (pair.result != vk::Result::eSuccess)
		{
			TZE_ENGINE_ERR("failed to create graphics pipeline!");
		}
		else
		{
			_graphicsPipeline = pair.value;
			TZE_ENGINE_INFO("successfully created a graphics pipeline");
		}
	}
	catch (vk::SystemError)
	{
		TZE_ENGINE_ERR("failed to create graphics pipeline!");
	}

	_input._logicalDevice.destroyShaderModule(vertexShader);
	_input._logicalDevice.destroyShaderModule(fragmentShader);
}

void tze::Pipeline::destroyPipeline()
{
	_input._logicalDevice.waitIdle();
	_input._logicalDevice.destroyPipeline(_graphicsPipeline);
	_input._logicalDevice.destroyPipelineLayout(_layout);
	_input._logicalDevice.destroyRenderPass(_renderPass);
}

vk::ShaderModule tze::Pipeline::createModule(const std::string& filename)
{
	std::vector<char> sourceCode = readFile(filename); // getting the content of the files

	// putting the creating info of the shader module:
	vk::ShaderModuleCreateInfo moduleInfo = {};
	moduleInfo.flags = vk::ShaderModuleCreateFlags();
	moduleInfo.codeSize = sourceCode.size();
	moduleInfo.pCode = reinterpret_cast<const uint32_t*>(sourceCode.data());

	try
	{
		// creating the shader module:
		return _input._logicalDevice.createShaderModule(moduleInfo);
	}
	catch (vk::SystemError err)
	{
		TZE_ENGINE_ERR(std::string("Failed to create shader module for \"") + filename + "\"");
	}
}

std::vector<char> tze::Pipeline::readFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		TZE_ENGINE_ERR(std::string("failed to load \"") + filename + "\"");
	}

	// reading the content of the file into the vector of chars called buffer:
	size_t fileSize{
		static_cast<size_t>(file.tellg())
	};
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}

vk::PipelineLayout tze::Pipeline::makePipelineLayout()
{
	vk::PushConstantRange pushConstantRange = {};
	pushConstantRange.stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);

	vk::PipelineLayoutCreateInfo layoutInfo;
	layoutInfo.flags = vk::PipelineLayoutCreateFlags();
	layoutInfo.setLayoutCount = 0;
	layoutInfo.pushConstantRangeCount = 1;
	layoutInfo.pPushConstantRanges = &pushConstantRange;

	try
	{
		return _input._logicalDevice.createPipelineLayout(layoutInfo);
	}
	catch (vk::SystemError err)
	{
		TZE_ENGINE_ERR("Failed to create pipeline layout!");
	};
}

vk::RenderPass tze::Pipeline::makeRenderPass()
{
	vk::AttachmentDescription colorAttachment = {};
	colorAttachment.flags = vk::AttachmentDescriptionFlags();
	colorAttachment.format = _input._swapchainImageFormat;
	colorAttachment.samples = vk::SampleCountFlagBits::e1;
	colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
	colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
	colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

	vk::AttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

	vk::SubpassDescription subpass = {};
	subpass.flags = vk::SubpassDescriptionFlags();
	subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	vk::RenderPassCreateInfo renderpassInfo = {};
	renderpassInfo.flags = vk::RenderPassCreateFlags();
	renderpassInfo.attachmentCount = 1;
	renderpassInfo.pAttachments = &colorAttachment;
	renderpassInfo.subpassCount = 1;
	renderpassInfo.pSubpasses = &subpass;

	try
	{
		return _input._logicalDevice.createRenderPass(renderpassInfo);
	}
	catch (vk::SystemError err)
	{
		TZE_ENGINE_ERR("failed to create RenderPass!");
	}
}