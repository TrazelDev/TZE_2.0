#include "pch.h"
#include "app.h"

#include "Layers/Window/Window.h"
#include "layers/Vulkan/Instance.h"
#include "layers/Vulkan/Swapchain.h"

namespace tze {
	App::App()
		: _window(), _instance(_window.getWindow()),
		_width(_window.getWidth()), _height(_window.getWidth()),
		_swapchain(*new swapchainBundle(*_instance.getLogicalDevice(), *_instance.getPhysicalDevice(),
			*_instance.getSurface(), *_instance.getQueueFamilies(), *_width, *_height)),
		_pipeline(*new pipelineInput(*_instance.getLogicalDevice(), _swapchain.getFormat(), _swapchain.getExtent())),
		commands(*new CommandsInput(*_instance.getLogicalDevice(), *_instance.getPhysicalDevice(),
			*_instance.getSurface(), _pipeline.getRenderPass(), _swapchain.getExtent(),
			_swapchain.getFrames(), *_instance.getQueueFamilies())),
		_renderer(*new RendererInput(*_instance.getLogicalDevice(), _swapchain.getSwapchain(), _swapchain.getFrames(), _pipeline.getRenderPass(),
		_swapchain.getExtent(), *_window.getWidth(), *_window.getHeight(), _pipeline.getPipeline(), _pipeline.getLayout(), *_instance.getQueueFamilies()))
	{
		// pipelineInput input = pipelineInput(*_instance.getLogicalDevice(), _swapchain.getFormat(), _swapchain.getExtent());
		/// Pipeline(input);
	}

	App::~App()
	{
		// TODO: delete the layers
	}

	void App::appInit()
	{
	}

	void App::run()
	{
		bool* closingFlag = _window.closeWindowFlag();

		while (!*closingFlag)
		{
			_window.run();
			// _instance.run();
			// _swapchain.run();
			_renderer.run();
		}

		TZE_ENGINE_INFO("Successfully closed the window");
	}

	vk::PhysicalDevice& App::getPhysicalDevice()
	{
		return *_instance.getPhysicalDevice();
	}
	vk::Device& App::getLogicalDevice()
	{
		return *_instance.getLogicalDevice();
	}

	vk::SurfaceKHR& App::getSurface()
	{
		return *_instance.getSurface();
	}

	QueueFamilies& App::getQueueIndices()
	{
		return *_instance.getQueueFamilies();
	}
}