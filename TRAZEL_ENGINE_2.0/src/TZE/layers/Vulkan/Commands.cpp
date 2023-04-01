#include "pch.h"
#include "Commands.h"
#include "Instance.h"

tze::Commands::Commands(const CommandsInput& input) : _input(input)
{
	makeFrameBuffers();
	makeCommandPool();
	createCommandBuffers();
}

tze::Commands::~Commands()
{
	destroyBuffers();

	// Free the main command buffer
	_input._logicalDevice.freeCommandBuffers(_commandPool, 1, &_mainCommandBuffer);
	_input._logicalDevice.destroyCommandPool(_commandPool);
}

void tze::Commands::run()
{
	
}


void tze::Commands::makeFrameBuffers()
{
	// In Vulkan, a framebuffer is a collection of image attachments that specify the rendering targets for a specific render pass. 
	// It represents the actual memory that is used to render a frame. The framebuffer must match the number and type of attachments specified in the render pass, 
	// and it is created using the images that are acquired from the swapchain. 
	// Once a framebuffer is created, it can be used to execute the commands that render a frame.

	vk::Extent2D& thisExtent = _input._swapchain.getExtent();
	vk::FramebufferCreateInfo framebufferInfo = {};
	framebufferInfo.width = thisExtent.width;
	framebufferInfo.height = thisExtent.height;;
	framebufferInfo.flags = vk::FramebufferCreateFlags();
	framebufferInfo.renderPass = _input._pipeline.getRenderPass();
	framebufferInfo.layers = 1;

	for (auto& frame : _input._swapchain.getFrames())
	{
		std::vector<vk::ImageView> attachments
		{
			frame.imageView
		};

		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();

		try
		{
			frame.frameBuffer = (_input._logicalDevice.createFramebuffer(framebufferInfo));
		}
		catch (vk::SystemError err)
		{
			TZE_ENGINE_ERR("failed to create framebuffer");
		}
	}
}

void tze::Commands::createCommandBuffers()
{
	vk::CommandBufferAllocateInfo alloceInfo = {};
	alloceInfo.commandPool = _commandPool;
	alloceInfo.level = vk::CommandBufferLevel::ePrimary;
	alloceInfo.commandBufferCount = 1;
	
	for (auto& frame : _input._swapchain.getFrames())
	{
		try
		{
			frame.commandBuffer = _input._logicalDevice.allocateCommandBuffers(alloceInfo)[0];
			SHOW_DATA(TZE_ENGINE_INFO("allocated command buffer"););
		}
		catch (vk::SystemError err)
		{
			TZE_ENGINE_ERR("failed to allocate command buffer");
		}
	}

	try
	{
		_mainCommandBuffer = _input._logicalDevice.allocateCommandBuffers(alloceInfo)[0];
		TZE_ENGINE_INFO("allocated main command buffer");
	}
	catch (vk::SystemError err)
	{
		TZE_ENGINE_ERR("failed tp allocate main command buffer");
	}
}

void tze::Commands::destroyBuffers()
{
	_input._logicalDevice.waitIdle();

	for (auto& frame : _input._swapchain.getFrames())
	{
		_input._logicalDevice.freeCommandBuffers(_commandPool, 1, &frame.commandBuffer);
		_input._logicalDevice.destroyFramebuffer(frame.frameBuffer);
	}
}

void tze::Commands::recreateBuffers()
{
	destroyBuffers();
	makeFrameBuffers();
	createCommandBuffers();
}

void tze::Commands::makeCommandPool()
{
	// command pool is in charge of managing the memory of the vulkan commandBuffers that are in charge of storing commands that are going to be sent to the 
	// gpu for execution 
	vk::CommandPoolCreateInfo commandPoolInfo = {};
	// this flag says that command buffers will reset only by the vkResetCommandBuffer 
	// function and this flag improves performance since we can reuse commandbuffers instead of destroying them
	commandPoolInfo.flags = vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer; 
	commandPoolInfo.queueFamilyIndex = _input._queueFamilies.graphicsIndex.value();

	try
	{
		_commandPool = _input._logicalDevice.createCommandPool(commandPoolInfo);
	}
	catch (vk::SystemError err)
	{
		TZE_ENGINE_ERR("failed to create command pool");
	}
}

