#include "pch.h"
#include "app.h"

#include "Layers/Window/Window.h"
#include "layers/Vulkan/Instance.h"
#include "layers/Vulkan/Swapchain.h"

namespace tze {
	App::App()
		: _window(), _instance(_window.getWindow()), 
		_width(_window.getWidth()), _height(_window.getWidth()),
		_swapchain(swapchainBundle(*_instance.getLogicalDevice(), *_instance.getPhysicalDevice(), 
			*_instance.getSurface(), *_instance.getQueueFamilies(), *_width, *_height))
	{
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
			_instance.run();
			_swapchain.run();
		}

		TZE_ENGINE_INFO("Successfully closed the window");
	}

	void App::addObject(GameObject* gameObject)
	{
		_gameObjects.push_back(gameObject);
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