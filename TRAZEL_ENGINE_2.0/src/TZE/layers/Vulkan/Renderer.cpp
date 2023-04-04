#include "pch.h"
#include "Renderer.h"

tze::Renderer::Renderer(const RendererInput& input) : _input(input), 
	_pipeline(*new pipelineInput(_input._logicalDevice, _input._swapchain.getFormat(), _input._swapchain.getExtent())),
	_commands(*new CommandsInput(_input._logicalDevice, _input._indices, _pipeline, _input._swapchain))
{
	_graphicsQueue = _input._logicalDevice.getQueue(_input._indices.graphicsIndex.value(), 0);
	_presentQueue = _input._logicalDevice.getQueue(_input._indices.presentIndex.value(), 0);
	_maxFramesInFlight = uint32_t(_input._swapchain.getFrames().size());
	_frameNum = 0;
}

tze::Renderer::~Renderer()
{
	for (auto obj : _gameObjects)
	{
		delete obj;
	}
}

void tze::Renderer::addGameObjects(GameObject* obj)
{
	_gameObjects.push_back(obj);
}

void tze::Renderer::run()
{
	if (_input._window.wasWindowMinimized())
	{
		return;
	}
	if (_input._window.wasWindowResized())
	{
		recreationOfWindowSize();
		return;
	}

	_input._logicalDevice.waitForFences(1, &_input._swapchain.getFrames()[_frameNum].inFlight, VK_TRUE, UINT64_MAX);
	_input._logicalDevice.resetFences(1, &_input._swapchain.getFrames()[_frameNum].inFlight);

	auto [result, imageIndex] = _input._logicalDevice.acquireNextImageKHR(_input._swapchain.getSwapchain(), UINT64_MAX, _input._swapchain.getFrames()[_frameNum].imageAvailable, nullptr);

	if (result != vk::Result::eSuccess)
	{
		TZE_ENGINE_ERR("failed to acquire the next image");
	}

	vk::CommandBuffer commandBuffer = _input._swapchain.getFrames()[_frameNum].commandBuffer;
	
	commandBuffer.reset();
	recordDrawCommands(commandBuffer, imageIndex);
	
	vk::SubmitInfo submitInfo = {};
	
	vk::Semaphore waitSemaphores[] = { _input._swapchain.getFrames()[_frameNum].imageAvailable };
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	
	vk::Semaphore signalSemaphores[] = { _input._swapchain.getFrames()[_frameNum].renderFinished };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;
	
	try {
		_graphicsQueue.submit(submitInfo, _input._swapchain.getFrames()[_frameNum].inFlight);
	}
	catch (vk::SystemError err) {
		TZE_ENGINE_ERR("failed to submit draw command buffer!");
	}
	
	vk::PresentInfoKHR presentInfo = {};
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	
	vk::SwapchainKHR swapChains[] = { _input._swapchain.getSwapchain()};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	
	presentInfo.pImageIndices = &imageIndex;
	

	_frameNum = (_frameNum + 1) % _maxFramesInFlight;
	_presentQueue.presentKHR(presentInfo);

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
	renderpassInfo.renderPass = _pipeline.getRenderPass();
	renderpassInfo.framebuffer = _input._swapchain.getFrames()[imageIndex].frameBuffer;
	renderpassInfo.renderArea.offset.x = 0;
	renderpassInfo.renderArea.offset.y = 0;
	renderpassInfo.renderArea.extent = _input._swapchain.getExtent();

	vk::ClearValue clearColor = { std::array<float, 4> {0.1f, 0.1f, 0.1f, 1.0f} };
	renderpassInfo.clearValueCount = 1;
	renderpassInfo.pClearValues = &clearColor;

	commandBuffer.beginRenderPass(&renderpassInfo, vk::SubpassContents::eInline);

	// viewport and scissor:
	vk::Viewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = *_input._window.getWidth();
	viewport.height = *_input._window.getHeight();
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vk::Rect2D scissor{ {0, 0}, {*_input._window.getWidth(), *_input._window.getHeight()} };

	commandBuffer.setViewport(0, 1, &viewport);
	commandBuffer.setScissor(0, 1, &scissor);


	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, _pipeline.getPipeline(1));
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

void tze::Renderer::renderGameObj(const vk::CommandBuffer& commandBuffer)
{
	int i = 0;
	for (auto& obj : _gameObjects) {
		i += 1;
		// obj->_transform_2D._rotation = glm::mod<float>(obj->_transform_2D._rotation + 0.001f * i, 2.f * glm::pi<float>());
		//obj->_transform_2D._rotation = glm::mod<float>(obj->_transform_2D._rotation + 0.00003f * i, 2.f * glm::pi<float>());
	}

	for (GameObject* gameObject : _gameObjects)
	{
		// gameObject->_transform_2D.runChanges();

		SimplePushConstantData push;
		push.offset = gameObject->_transform_2D._translation;
		push.color = gameObject->_color;
		push.transform = gameObject->_transform_2D.mat2();

		commandBuffer.pushConstants(_pipeline.getLayout(), vk::ShaderStageFlagBits::eVertex |
			vk::ShaderStageFlagBits::eFragment, 0, sizeof(SimplePushConstantData), &push);

		gameObject->_model->bind(commandBuffer);
		gameObject->_model->draw(commandBuffer);
	}
}

void tze::Renderer::recreationOfWindowSize()
{
	bool wasMin = false; // var that says if the window was resized or minimized 

	while (*_input._window.getHeight() == 0 || *_input._window.getWidth() == 0)
	{
		glfwWaitEvents();
		wasMin = true;
	}
	
	_input._swapchain.recreateSwapChain(*_input._window.getWidth(), *_input._window.getHeight());
	_pipeline.recreatePipeline(_input._swapchain.getExtent());
	_commands.recreateBuffers();
	_input._window.resetWindowResizedFlag();
}
