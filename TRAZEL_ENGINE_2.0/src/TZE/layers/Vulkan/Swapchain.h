#pragma once
#include "layers/Layer.h"
#include "Instance.h"

namespace tze
{
	struct swapchainBundle
	{
		vk::Device& logicalDevice;
		vk::PhysicalDevice& physicalDevice;
		vk::SurfaceKHR& surface;
		uint32_t width;
		uint32_t height;
		QueueFamilies indices;
	};

	struct SwapchainFrame
	{
		vk::Image image;
		vk::ImageView imageView;
		vk::Framebuffer frameBuffer;
		vk::CommandBuffer commandBuffer;
		vk::Semaphore imageAvailable, renderFinished;
		vk::Fence inFlight;
	};

	class Swapchain
	{
	public:
		Swapchain(const swapchainBundle& input);
		~Swapchain();

		// the function is responsible of creating the VULKAN swap chain object
		void createSwapChain(const swapchainBundle& input);
	private:
		void querySwapchainSupport(const vk::SurfaceCapabilitiesKHR& capabilities,
			const std::vector<vk::SurfaceFormatKHR>& formats, std::vector<vk::PresentModeKHR> presentModes);
		vk::Device& _logicalDevice;
		vk::PhysicalDevice& _physicalDevice;
		vk::SurfaceKHR& _surface;

		// swapchain related:
		// the swap chain is responsible for loading the image that we want to display into a buffer and not directly on the screen to avoid screen taring
		// so what swap chain is doing is that we load the next image for display into the buffer and then switch between the buffer that currently presenting to the screen
		// and then load to it and switch again this insures that there is no screen taring
		vk::SwapchainKHR _swapchain;
		vk::SurfaceFormatKHR _format;
		vk::PresentModeKHR _presentMode;
		vk::Extent2D _extent;
		uint32_t _imageCount; // the amount of images that the are going to be swapped with each other
	};
}
