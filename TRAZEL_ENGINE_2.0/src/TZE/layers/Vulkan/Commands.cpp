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
	_input._logicalDevice.waitIdle();

	for (auto& frame : _input._swapchainFrames)
	{
		_input._logicalDevice.freeCommandBuffers(_commandPool, 1, &frame.commandBuffer);
		_input._logicalDevice.destroyFramebuffer(frame.frameBuffer);
	}

	// Free the main command buffer
	_input._logicalDevice.freeCommandBuffers(_commandPool, 1, &_mainCommandBuffer);


	_input._logicalDevice.destroyCommandPool(_commandPool);
}

void tze::Commands::run()
{
	/*device.waitForFences(1, &swapchainFrames[frameNum].inFlight, VK_TRUE, UINT64_MAX);
	device.resetFences(1, &swapchainFrames[frameNum].inFlight);

	uint32_t imageIndex{ device.acquireNextImageKHR(*swapchain, UINT64_MAX, swapchainFrames[frameNum].imageAvailable, nullptr).value };

	vk::CommandBuffer commandBuffer = swapchainFrames[frameNum].commandBuffer;

	commandBuffer.reset();

	recordDrawCommands(commandBuffer, imageIndex);

	vk::SubmitInfo submitInfo = {};

	vk::Semaphore waitSemaphores[] = { swapchainFrames[frameNum].imageAvailable };
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vk::Semaphore signalSemaphores[] = { swapchainFrames[frameNum].renderFinished };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	try {
		graphicsQueue.submit(submitInfo, swapchainFrames[frameNum].inFlight);
	}
	catch (vk::SystemError err) {
#ifndef Client_MODE
		TZE_ENGINE_ERR("failed to submit draw command buffer!");
#endif
	}


	vk::PresentInfoKHR presentInfo = {};
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	vk::SwapchainKHR swapChains[] = { *swapchain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;


	////////////////////////
	// uint32_t frameNum32_t = frameNum;
	// VkResult result1 = vkAcquireNextImageKHR
	// (
	// 	device,
	// 	*swapchain,
	// 	std::numeric_limits<uint64_t>::max(),
	// 	swapchainFrames[frameNum].imageAvailable,  // must be a not signaled semaphore
	// 	swapchainFrames[frameNum].inFlight,
	// 	&frameNum32_t
	// );
	// if (result1 == VK_ERROR_OUT_OF_DATE_KHR)
	// {
	// 	recreate_swapchain();
	// }
	////////////////////////////////

	if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || mainWindow->wasWindowResized())
	{
		mainWindow->resetWindowResizedFlag();
		recreate_swapchain();
		return;
	}
	frameNum = (frameNum + 1) % maxFramesInFlight;
	result = presentQueue.presentKHR(presentInfo);


	//const auto a = VkPresentInfoKHR(presentInfo);
	//result1 = vkQueuePresentKHR(presentQueue, &a);*/
}


void tze::Commands::makeFrameBuffers()
{
	// In Vulkan, a framebuffer is a collection of image attachments that specify the rendering targets for a specific render pass. 
	// It represents the actual memory that is used to render a frame. The framebuffer must match the number and type of attachments specified in the render pass, 
	// and it is created using the images that are acquired from the swapchain. 
	// Once a framebuffer is created, it can be used to execute the commands that render a frame.
	vk::FramebufferCreateInfo framebufferInfo = {};
	framebufferInfo.width = _input._extent.width;
	framebufferInfo.height = _input._extent.height;;
	framebufferInfo.flags = vk::FramebufferCreateFlags();
	framebufferInfo.renderPass = _input._renderPass;
	framebufferInfo.layers = 1;

	for (auto& frame : _input._swapchainFrames)
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

void tze::Commands::createCommandBuffers()
{
	vk::CommandBufferAllocateInfo alloceInfo = {};
	alloceInfo.commandPool = _commandPool;
	alloceInfo.level = vk::CommandBufferLevel::ePrimary;
	alloceInfo.commandBufferCount = 1;
	
	for (auto& frame : _input._swapchainFrames)
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
