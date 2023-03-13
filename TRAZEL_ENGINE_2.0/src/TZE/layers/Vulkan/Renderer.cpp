#include "pch.h"
#include "Renderer.h"

tze::Renderer::Renderer(const RendererInput& input) : _input(input)
{
	_graphicsQueue = _input._logicalDevice.getQueue(_input._indices.graphicsIndex.value(), 0);
	_presentQueue = _input._logicalDevice.getQueue(_input._indices.presentIndex.value(), 0);
	_maxFramesInFlight = uint32_t(_input._swapchainFrames.size());
	_frameNum  = 0;
}

tze::Renderer::~Renderer()
{

}

void tze::Renderer::addGameObjects(GameObject* obj)
{
	_gameObjects.push_back(obj);
}

void tze::Renderer::run()
{
	_input._logicalDevice.waitForFences(1, &_input._swapchainFrames[_frameNum].inFlight, VK_TRUE, UINT64_MAX);
	_input._logicalDevice.resetFences(1, &_input._swapchainFrames[_frameNum].inFlight);

	auto [result, imageIndex] = _input._logicalDevice.acquireNextImageKHR(_input._swapchain, UINT64_MAX, _input._swapchainFrames[_frameNum].imageAvailable, nullptr);
	if (result != vk::Result::eSuccess)
	{
		TZE_ENGINE_ERR("failed to acquire the next image");
	}

	vk::CommandBuffer commandBuffer = _input._swapchainFrames[_frameNum].commandBuffer;

	commandBuffer.reset();
	recordDrawCommands(commandBuffer, imageIndex);
	
	vk::SubmitInfo submitInfo = {};

	vk::Semaphore waitSemaphores[] = { _input._swapchainFrames[_frameNum].imageAvailable };
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vk::Semaphore signalSemaphores[] = { _input._swapchainFrames[_frameNum].renderFinished };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	try {
		_graphicsQueue.submit(submitInfo, _input._swapchainFrames[_frameNum].inFlight);
	}
	catch (vk::SystemError err) {
		TZE_ENGINE_ERR("failed to submit draw command buffer!");
	}


	vk::PresentInfoKHR presentInfo = {};
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	vk::SwapchainKHR swapChains[] = { _input._swapchain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;


	////////////////////////
	// uint32_t _frameNum 32_t = _frameNum ;
	// VkResult result1 = vkAcquireNextImageKHR
	// (
	// 	device,
	// 	*swapchain,
	// 	std::numeric_limits<uint64_t>::max(),
	// 	swapchainFrames[_frameNum ].imageAvailable,  // must be a not signaled semaphore
	// 	swapchainFrames[_frameNum ].inFlight,
	// 	&_frameNum 32_t
	// );
	// if (result1 == VK_ERROR_OUT_OF_DATE_KHR)
	// {
	// 	recreate_swapchain();
	// }
	////////////////////////////////

	// if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || mainWindow->wasWindowResized())
	// {
	// 	mainWindow->resetWindowResizedFlag();
	// 	recreate_swapchain();
	// 	return;
	// }
	_frameNum  = (_frameNum  + 1) % _maxFramesInFlight;
	_presentQueue.presentKHR(presentInfo);


	//const auto a = VkPresentInfoKHR(presentInfo);
	//result1 = vkQueuePresentKHR(presentQueue, &a);
}

void tze::Renderer::recordDrawCommands(const vk::CommandBuffer& commandBuffer, uint32_t imageIndex)
{
	vk::CommandBufferBeginInfo beginInfo = {};

	try
	{
		commandBuffer.begin(beginInfo);
	}
	catch (vk::SystemError err)
	{
		TZE_ENGINE_ERR("failed to begin recording command buffer");
	}

	vk::RenderPassBeginInfo renderpassInfo = {};
	renderpassInfo.renderPass = _input._renderpass;
	renderpassInfo.framebuffer = _input._swapchainFrames[imageIndex].frameBuffer;
	renderpassInfo.renderArea.offset.x = 0;
	renderpassInfo.renderArea.offset.y = 0;
	renderpassInfo.renderArea.extent = _input._extent;

	vk::ClearValue clearColor = { std::array<float, 4> {0.1f, 0.1f, 0.1f, 1.0f} };
	renderpassInfo.clearValueCount = 1;
	renderpassInfo.pClearValues = &clearColor;

	commandBuffer.beginRenderPass(&renderpassInfo, vk::SubpassContents::eInline);


	// viewport and scissor:
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = _input._width;
	viewport.height = _input._height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{ {0, 0}, {_input._width, _input._height} };

	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);


	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, _input._pipeline);
	renderGameObj(commandBuffer);
	commandBuffer.endRenderPass();

	try
	{
		commandBuffer.end();
	}
	catch (vk::SystemError err)
	{
		TZE_ENGINE_ERR("failed to finish command buffer");
	}
}



void tze::Renderer::renderGameObj(vk::CommandBuffer commandBuffer)
{
	for (GameObject* gameObject : _gameObjects)
	{
		gameObject->_transform_2D._rotation = glm::mod(gameObject->_transform_2D._rotation + 0.0005f, glm::two_pi<float>());

		SimplePushConstantData push;
		push.offset = gameObject->_transform_2D._translation;
		push.color = gameObject->_color;
		push.transform = gameObject->_transform_2D.mat2();

		vkCmdPushConstants
		(
			commandBuffer,
			_input._layout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(SimplePushConstantData),
			&push
		);

		gameObject->_model->bind(commandBuffer);
		gameObject->_model->draw(commandBuffer);
	}
}
