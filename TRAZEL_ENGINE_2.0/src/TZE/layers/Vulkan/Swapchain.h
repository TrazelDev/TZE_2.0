#pragma once
#include "layers/Layer.h"
#include "Instance.h"

namespace tze
{
	struct swapchainBundle
	{
		vk::Device& _logicalDevice;
		vk::PhysicalDevice& _physicalDevice;
		vk::SurfaceKHR& _surface;
		uint32_t _width;
		uint32_t _height;
		QueueFamilies& _indices;

		swapchainBundle(vk::Device& logicalDevice, vk::PhysicalDevice& physicalDevice, vk::SurfaceKHR& surface, QueueFamilies& indices,
			uint32_t _width, uint32_t height)
			: _logicalDevice(logicalDevice), _physicalDevice(physicalDevice), _surface(surface), _indices(indices)
		{
		}
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

	class Swapchain : public Layer
	{
	public:
		Swapchain(const swapchainBundle& input);
		~Swapchain();

		void run() override;
		void recreateSwapChain(uint32_t width, uint32_t height);

		vk::Format& getFormat();
		vk::Extent2D& getExtent();
		std::vector<SwapchainFrame>& getFrames();
		vk::SwapchainKHR& getSwapchain();

	private:
		// the function is responsible of creating the VULKAN swap chain object
		void createSwapChain(const swapchainBundle& input);
		void querySwapchainSupport(const vk::SurfaceCapabilitiesKHR& capabilities,
			const std::vector<vk::SurfaceFormatKHR>& formats, std::vector<vk::PresentModeKHR> presentModes);

		void makeExtent(uint32_t width, uint32_t height);
		void createFrames();

		// understand what it is:
		void createSemaphoresFences();
		vk::Fence createFence();
		vk::Semaphore createSemaphore();

		/// <summary>
		// destroy swapchain related variables: 
		/// </summary>
		void destroySwapchain();

		vk::Device& _logicalDevice;
		vk::PhysicalDevice& _physicalDevice;
		vk::SurfaceKHR& _surface;

		// swapchain related:
		// the swap chain is responsible for loading the image that we want to display into a buffer and not directly on the screen to avoid screen taring
		// so what swap chain is doing is that we load the next image for display into the buffer and then switch between the buffer that currently presenting to the screen
		// and then load to it and switch again this insures that there is no screen taring
		vk::SurfaceCapabilitiesKHR _capabilities;
		vk::SwapchainCreateInfoKHR _swapchainCreateInfo;
		vk::SwapchainKHR _swapchain[2];
		int _currSwapchain{0};
		vk::SurfaceFormatKHR _format;
		vk::PresentModeKHR _presentMode;
		vk::Extent2D _extent;
		uint32_t _imageCount; // the amount of images that the are going to be swapped with each other
		std::vector<SwapchainFrame> _frames;
	};
}
