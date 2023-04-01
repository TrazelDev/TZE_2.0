#pragma once
#include "pch.h"
#include "Core.h"
#include "layers/Layer.h"
#include "layers/Window/Window.h"
#include "layers/Vulkan/Objects/GameObj.h"
#include "layers/Vulkan/Instance.h"
#include "layers/Vulkan/Swapchain.h"
#include "layers/Vulkan/pipeline/Pipeline.h"
#include "layers/Vulkan/Commands.h"
#include "layers/Vulkan/Renderer.h"

#define WINDOW_LAYER_INDEX 0
#define INSTANCE_LAYER_INDEX 1

namespace tze
{
	class TZE_API App
	{
	public:
		/// <summary>
		/// creating the window and the initializing the the VULKAN instance and making everything related to both logical and physical devices
		/// </summary>
		App();
		virtual ~App();

		/// <summary>
		/// this is a function that continues to create the application after the client has finished to add what he wants and initialized the application
		/// </summary>
		void appInit();
		void run();

		vk::PhysicalDevice& getPhysicalDevice();
		vk::Device& getLogicalDevice();
		vk::SurfaceKHR& getSurface();
		QueueFamilies& getQueueIndices();

	protected:
		Window _window;
		uint32_t* _width;
		uint32_t* _height;

		// vulkan:
		Instance  _instance;
		Swapchain _swapchain;
		Pipeline  _pipeline;
		Commands  _commands;
		Renderer  _renderer;
	};

	// this will be defined in the client
	App* createApp();
}
