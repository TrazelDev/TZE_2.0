#pragma once
#include "pch.h"
#include "Core.h"
#include "layers/Layer.h"

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

		void addLayer(Layer* layer);
	private:
		std::vector<Layer*> _layers;
	};

	// this will be defined in the client 
	App* createApp();

}

