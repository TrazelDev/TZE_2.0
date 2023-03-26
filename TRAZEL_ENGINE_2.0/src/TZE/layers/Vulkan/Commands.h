#pragma once
#include "Swapchain.h"

namespace tze
{
	struct CommandsInput
	{
		vk::Device& _logicalDevice;
		vk::PhysicalDevice& _physicalDevice;
		vk::SurfaceKHR& _surface;
		vk::RenderPass& _renderPass;
		vk::Extent2D _extent;
		std::vector<SwapchainFrame>& _swapchainFrames;
		QueueFamilies& _queueFamilies;

		CommandsInput(vk::Device& logicalDevice, vk::PhysicalDevice& physicalDevice, vk::SurfaceKHR& surface,
			vk::RenderPass& renderPass, vk::Extent2D& extent, std::vector<SwapchainFrame>& swapchainFrames,
			QueueFamilies& queueFamilies) :
			_logicalDevice(logicalDevice), _physicalDevice(physicalDevice), _surface(surface), _renderPass(renderPass),
			_extent(extent), _swapchainFrames(swapchainFrames), _queueFamilies(queueFamilies) { }
	};

	class Commands
	{
	public:
		Commands(const CommandsInput& input);
		~Commands();

		void run();
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

		const CommandsInput& _input;
		vk::CommandPool _commandPool;
		vk::CommandBuffer _mainCommandBuffer;
	};
}
