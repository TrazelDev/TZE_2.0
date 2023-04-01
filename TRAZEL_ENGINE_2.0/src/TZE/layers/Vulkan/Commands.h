#pragma once
#include "Swapchain.h"
#include "Pipeline/Pipeline.h"

namespace tze
{
	struct CommandsInput
	{
		vk::Device& _logicalDevice;
		vk::PhysicalDevice& _physicalDevice;
		vk::SurfaceKHR& _surface;
		Pipeline& _pipeline;
		QueueFamilies& _queueFamilies;
		Swapchain& _swapchain;

		CommandsInput(vk::Device& logicalDevice, vk::PhysicalDevice& physicalDevice, vk::SurfaceKHR& surface, 
			QueueFamilies& queueFamilies, Pipeline& pipeline, Swapchain& swapchain) :
			_logicalDevice(logicalDevice), _physicalDevice(physicalDevice), _surface(surface), _pipeline(pipeline),
			_swapchain(swapchain), _queueFamilies(queueFamilies) { }
	};

	class Commands
	{
	public:
		Commands(const CommandsInput& input);
		~Commands();

		void run();

		/// <summary>
		/// this function is recreating both the frame and command buffer that assigned for each frame 
		/// </summary>
		void recreateBuffers();
	private:
		/// <summary>
		/// function that create frameBuffers a framebuffer is a collection of images that specify the rendering targets for the renderPass
		/// </summary>
		void makeFrameBuffers();

		/// <summary>
		/// function that creates command pool that in charge of managing the command buffers which are places to store commands that are going to be sent to the gpu
		/// </summary>
		void makeCommandPool();

		/// <summary>
		/// creating the command buffers
		/// </summary>
		void createCommandBuffers();

		/// <summary>
		/// this function is destroying both the frame buffers and command buffers that are allocated to each frame
		/// </summary>
		void destroyBuffers();


		const CommandsInput& _input;
		vk::CommandPool _commandPool;
		vk::CommandBuffer _mainCommandBuffer;
	};
}
