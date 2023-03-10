#include "pch.h"
#include "Swapchain.h"
#include "vk_logging/vkLogging.h"

tze::Swapchain::Swapchain(const swapchainBundle& input)
	: _logicalDevice(input.logicalDevice), _physicalDevice(input.physicalDevice), _surface(input.surface)
{
	createSwapChain(input);

	std::vector<vk::Image> images = _logicalDevice.getSwapchainImagesKHR(_swapchain);
	std::vector<SwapchainFrame> frames(images.size());

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

		frames[i].image = images[i];
		frames[i].imageView = _logicalDevice.createImageView(createInfo);
	}
	

}

tze::Swapchain::~Swapchain()
{

}

void tze::Swapchain::createSwapChain(const swapchainBundle& input)
{
	vk::SurfaceCapabilitiesKHR capabilities = _physicalDevice.getSurfaceCapabilitiesKHR(_surface);;
	std::vector<vk::SurfaceFormatKHR> formats = _physicalDevice.getSurfaceFormatsKHR(_surface);;
	std::vector<vk::PresentModeKHR> presentModes = _physicalDevice.getSurfacePresentModesKHR(_surface);;
	SHOW_DATA(querySwapchainSupport(););




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

	// checking what should be the extent:
	if (capabilities.currentExtent.width != UINT32_MAX)
	{
		_extent = capabilities.currentExtent;
	}
	else
	{
		_extent.width = std::min(capabilities.maxImageExtent.width, std::max(capabilities.minImageExtent.width, input.width));
		_extent.height = std::min(capabilities.maxImageExtent.height, std::max(capabilities.minImageExtent.height, input.height));
	}

	_imageCount = std::min(capabilities.maxImageCount, capabilities.minImageCount + 1);

	vk::SwapchainCreateInfoKHR createInfo(
		/* flags */ vk::SwapchainCreateFlagsKHR(),
		/* surface */ input.surface,
		/* image count */ _imageCount,
		/* image format */ _format.format,
		/* color space */ _format.colorSpace,
		/* image extent */ _extent,
		/* image array layers */ 1,
		/* image usage */ vk::ImageUsageFlagBits::eColorAttachment
	);

	if (input.indices.graphicsIndex.value() != input.indices.presentIndex.value())
	{
		uint32_t queueFamilyIndices[] = { input.indices.graphicsIndex.value(), input.indices.presentIndex.value() };
		createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = vk::SharingMode::eExclusive;
	}
	createInfo.oldSwapchain = vk::SwapchainKHR(nullptr); // for now it has the value null but in the future when it will support window resizing then it will be different

	try
	{
		_swapchain = _logicalDevice.createSwapchainKHR(createInfo);
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

