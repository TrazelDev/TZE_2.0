#include "pch.h"
#include "Swapchain.h"
#include "vk_logging/vkLogging.h"

tze::Swapchain::Swapchain(const swapchainBundle& input)
	: _logicalDevice(input._logicalDevice), _physicalDevice(input._physicalDevice), _surface(input._surface)
{
	createSwapChain(input);
	createFrames();
	createSemaphoresFences();
}

tze::Swapchain::~Swapchain()
{
	destroySwapchain();
}

void tze::Swapchain::recreateSwapChain(uint32_t width, uint32_t height)
{
	_logicalDevice.waitIdle(); // waiting for on going operation to be completed 

	// destroying the and creating the actual swapchain
	makeExtent(width, height);
	_swapchainCreateInfo.oldSwapchain = _swapchain[_currSwapchain];
	_swapchainCreateInfo.imageExtent = _extent;
	_swapchain[(_currSwapchain + 1) %2] = _logicalDevice.createSwapchainKHR(_swapchainCreateInfo);
	destroySwapchain(); // destroying the old swapchain after creating a new one
	// advancing to the swapchain index
	_currSwapchain++;
	_currSwapchain %= 2;

	createFrames();
	createSemaphoresFences();
}

void tze::Swapchain::run() {}

vk::Format& tze::Swapchain::getFormat()
{
	return _format.format;
}

vk::Extent2D& tze::Swapchain::getExtent()
{
	return _extent;
}

std::vector<tze::SwapchainFrame>& tze::Swapchain::getFrames()
{
	return _frames;
}

vk::SwapchainKHR& tze::Swapchain::getSwapchain()
{
	return _swapchain[_currSwapchain];
}

void tze::Swapchain::createSwapChain(const swapchainBundle& input)
{
	_capabilities = _physicalDevice.getSurfaceCapabilitiesKHR(_surface);
	std::vector<vk::SurfaceFormatKHR> formats = _physicalDevice.getSurfaceFormatsKHR(_surface);
	std::vector<vk::PresentModeKHR> presentModes = _physicalDevice.getSurfacePresentModesKHR(_surface);
	SHOW_DATA(querySwapchainSupport(_capabilities, formats, presentModes););

	// choosing the format of the colors in the swapchain:
	bool flag = true;
	for (vk::SurfaceFormatKHR currFormat : formats)
	{
		// the eB8G8R8A8Unorm && eSrgbNonlinear are good options since they are widly used maybe change in the future as we grow
		if (currFormat.format == vk::Format::eB8G8R8A8Unorm && currFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
		{
			_format = currFormat;
			flag = false;
			break;
		}
	}
	if (flag)
	{
		_format = formats[0];
		TZE_ENGINE_WARN("the swapchain format that you wanted wasn't found so the first one is used");
	}

	// choosing a present mode: ( eFifo - present mode that insure that there will be no taring and displays the images in the order they came to the swapchain,
	// eMailbox - similar present mode but if a new image came to the swap chain then the old one will be deleted this makes the application feel faster and insures less latency)
	flag = true;
	for (vk::PresentModeKHR currPresentMode : presentModes)
	{
		if (currPresentMode == vk::PresentModeKHR::eMailbox)
		{
			flag = false;
			_presentMode = currPresentMode;
		}
	}
	if (flag)
	{
		TZE_ENGINE_WARN("the swap chain desired way present wasn't found resulted to use eFifo");
		_presentMode = vk::PresentModeKHR::eFifo;
	}

	makeExtent(input._width, input._height);

	_imageCount = std::min(_capabilities.maxImageCount, _capabilities.minImageCount + 1);

	_swapchainCreateInfo = vk::SwapchainCreateInfoKHR(
		/* flags */ vk::SwapchainCreateFlagsKHR(),
		/* surface */ input._surface,
		/* image count */ _imageCount,
		/* image format */ _format.format,
		/* color space */ _format.colorSpace,
		/* image extent */ _extent,
		/* image array layers */ 1,
		/* image usage */ vk::ImageUsageFlagBits::eColorAttachment
	);

	if (input._indices.graphicsIndex.value() != input._indices.presentIndex.value())
	{
		uint32_t queueFamilyIndices[] = { input._indices.graphicsIndex.value(), input._indices.presentIndex.value() };
		_swapchainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		_swapchainCreateInfo.queueFamilyIndexCount = 2;
		_swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		_swapchainCreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
	}
	_swapchainCreateInfo.oldSwapchain = vk::SwapchainKHR(nullptr); // there is no old swapchain so t

	try
	{
		_swapchain[_currSwapchain] = _logicalDevice.createSwapchainKHR(_swapchainCreateInfo);
		TZE_ENGINE_INFO("successfully created the swapchain");
	}
	catch (vk::SystemError err)
	{
		TZE_ENGINE_ERR("failed to create swapchain");
	}
}

void tze::Swapchain::querySwapchainSupport(const vk::SurfaceCapabilitiesKHR& capabilities,
	const std::vector<vk::SurfaceFormatKHR>& formats, std::vector<vk::PresentModeKHR> presentModes)
{
	std::cout << "swapChain can supported the following surface capabilities:\n";

	std::cout << "\tminimum image cout: " << capabilities.minImageCount << '\n';
	std::cout << "\tmaximum image cout: " << capabilities.maxImageCount << '\n';

	std::cout << "\tcurrent extent: \n";
	std::cout << "\t\twidth: " << capabilities.currentExtent.width << '\n';
	std::cout << "\t\theight: " << capabilities.currentExtent.height << '\n';

	std::cout << "\tminimum supported extent:\n";
	std::cout << "\t\twidth: " << capabilities.minImageExtent.width << '\n';
	std::cout << "\t\theight: " << capabilities.minImageExtent.height << '\n';

	std::cout << "\maximum supported extent:\n";
	std::cout << "\t\twidth: " << capabilities.maxImageExtent.width << '\n';
	std::cout << "\t\theight: " << capabilities.maxImageExtent.height << '\n';

	std::cout << "\tmaximum image array layers: " << capabilities.maxImageArrayLayers << '\n';

	std::cout << "\tsupported transform:\n";
	std::vector<std::string> stringList = logTransformBits(capabilities.supportedTransforms);
	for (std::string line : stringList)
	{
		std::cout << "\t\t" << line << '\n';
	}

	std::cout << "\tcurrent transform:\n";
	stringList = logTransformBits(capabilities.currentTransform);
	for (std::string line : stringList)
	{
		std::cout << "\t\t" << line << '\n';
	}

	std::cout << "\tsupported alpha operations:\n";
	stringList = logAlphaCompositesString(capabilities.supportedCompositeAlpha);
	for (std::string line : stringList)
	{
		std::cout << "\t\t" << line << '\n';
	}

	std::cout << "\tsupported image usage:\n";
	stringList = logImageUsageBits(capabilities.supportedUsageFlags);
	for (std::string line : stringList)
	{
		std::cout << "\t\t" << line << "\n";
	}

	for (vk::SurfaceFormatKHR supportedFormat : formats)
	{
		std::cout << "supported pixel format: " << vk::to_string(supportedFormat.format) << '\n';
		std::cout << "supported color space: " << vk::to_string(supportedFormat.colorSpace) << '\n';
	}

	for (vk::PresentModeKHR presentMode : presentModes)
	{
		std::cout << '\t' << logPresentMode(presentMode) << '\n';
	}
}

void tze::Swapchain::makeExtent(uint32_t width, uint32_t height)
{
	_capabilities = _physicalDevice.getSurfaceCapabilitiesKHR(_surface);

	// checking what should be the extent:
	if (_capabilities.currentExtent.width != UINT32_MAX)
	{
		_extent = _capabilities.currentExtent;
	}
	else
	{
		_extent.width = std::min(_capabilities.maxImageExtent.width, std::max(_capabilities.minImageExtent.width, width));
		_extent.height = std::min(_capabilities.maxImageExtent.height, std::max(_capabilities.minImageExtent.height, height));
	}
}

void tze::Swapchain::createFrames()
{
	std::vector<vk::Image> images = _logicalDevice.getSwapchainImagesKHR(_swapchain[_currSwapchain]);
	_frames.resize(images.size());

	for (size_t i = 0; i < images.size(); i++)
	{
		vk::ImageViewCreateInfo createInfo = {};
		createInfo.image = images[i];
		createInfo.viewType = vk::ImageViewType::e2D;
		createInfo.components.r = vk::ComponentSwizzle::eIdentity;
		createInfo.components.g = vk::ComponentSwizzle::eIdentity;
		createInfo.components.b = vk::ComponentSwizzle::eIdentity;
		createInfo.components.a = vk::ComponentSwizzle::eIdentity;
		createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;
		createInfo.format = _format.format;

		_frames[i].image = images[i];
		_frames[i].imageView = _logicalDevice.createImageView(createInfo);
	}
}

void tze::Swapchain::createSemaphoresFences()
{
	for (SwapchainFrame& frame : _frames)
	{
		frame.inFlight = createFence();
		frame.imageAvailable = createSemaphore();
		frame.renderFinished = createSemaphore();
	}
}

vk::Fence tze::Swapchain::createFence()
{
	vk::FenceCreateInfo fenceInfo = {};
	fenceInfo.flags = vk::FenceCreateFlags() | vk::FenceCreateFlagBits::eSignaled;

	try
	{
		return _logicalDevice.createFence(fenceInfo);
	}
	catch (vk::SystemError err)
	{
		TZE_ENGINE_ERR("Failed to create fence");
		return nullptr;
	}
}

vk::Semaphore tze::Swapchain::createSemaphore()
{
	vk::SemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.flags = vk::SemaphoreCreateFlags();

	try
	{
		return _logicalDevice.createSemaphore(semaphoreInfo);
	}
	catch (vk::SystemError err)
	{
		TZE_ENGINE_ERR("Failed to create semaphore");
		return nullptr;
	}
}

void tze::Swapchain::destroySwapchain()
{
	for (SwapchainFrame& frame : _frames)
	{
		_logicalDevice.destroyImageView(frame.imageView);

		// destroying all of the fences and semaphores
		_logicalDevice.destroyFence(frame.inFlight);
		_logicalDevice.destroySemaphore(frame.imageAvailable);
		_logicalDevice.destroySemaphore(frame.renderFinished);
	}

	_logicalDevice.destroySwapchainKHR(_swapchain[_currSwapchain]);
}
