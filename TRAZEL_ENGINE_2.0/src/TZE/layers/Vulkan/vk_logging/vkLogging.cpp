#include "pch.h"
#include "vkLogging.h"

VKAPI_ATTR VkBool32 VKAPI_CALL tze::debugCallback
(
	VkDebugUtilsMessageSeverityFlagBitsEXT messangerServerity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData
)
{
	std::cerr << "validation layer: " << pCallbackData->pMessage << '\n';

	return VK_FALSE;
}

vk::DebugUtilsMessengerEXT tze::makeDebugMessenger(vk::Instance& instance, vk::DispatchLoaderDynamic& dldi)
{
	vk::DebugUtilsMessengerCreateInfoEXT createInfo = vk::DebugUtilsMessengerCreateInfoEXT
	(
		vk::DebugUtilsMessengerCreateFlagsEXT(),
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
		vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
		debugCallback,
		nullptr
	);


	return instance.createDebugUtilsMessengerEXT(createInfo, nullptr, dldi);
}

void tze::logDeviceProperties(const vk::PhysicalDevice& device)
{
	vk::PhysicalDeviceProperties properties = device.getProperties();

	std::cout << "device name: " << properties.deviceName << '\n';

	std::cout << "device type: ";
	switch (properties.deviceType)
	{
	case (vk::PhysicalDeviceType::eCpu):
		TZE_ENGINE_WARN("gpu wasn't chosen to be the physical device");
		std::cout << "CPU\n";
		break;

	case (vk::PhysicalDeviceType::eDiscreteGpu):
		std::cout << "Discrete GPU\n";
		break;

	case (vk::PhysicalDeviceType::eIntegratedGpu):
		std::cout << "Integrated GPU\n";
		break;

	case (vk::PhysicalDeviceType::eVirtualGpu):
		std::cout << "Virtual GPU\n";
		break;

	default:
		std::cout << "Other\n";
	}
}

std::vector<std::string> tze::logTransformBits(vk::SurfaceTransformFlagsKHR bits)
{
	std::vector<std::string> result;

	if (bits & vk::SurfaceTransformFlagBitsKHR::eIdentity)
	{
		result.push_back("identity");
	}
	if (bits & vk::SurfaceTransformFlagBitsKHR::eRotate90)
	{
		result.push_back("90 degree rotation");
	}
	if (bits & vk::SurfaceTransformFlagBitsKHR::eRotate180)
	{
		result.push_back("180 degree rotation");
	}
	if (bits & vk::SurfaceTransformFlagBitsKHR::eRotate270)
	{
		result.push_back("270 degree rotation");
	}
	if (bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirror)
	{
		result.push_back("horizontal mirror");
	}
	if (bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90)
	{
		result.push_back("horizontal mirror and 90 degree rotate");
	}
	if (bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180)
	{
		result.push_back("horizontal mirror and 180 degree rotate");
	}
	if (bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270)
	{
		result.push_back("horizontal mirror and 270 degree rotate");
	}
	if (bits & vk::SurfaceTransformFlagBitsKHR::eInherit)
	{
		result.push_back("eInherited");
	}

	return result;
}

std::vector<std::string> tze::logAlphaCompositesString(vk::CompositeAlphaFlagsKHR bits)
{
	std::vector<std::string> result;

	if (bits & vk::CompositeAlphaFlagBitsKHR::eOpaque)
	{
		result.push_back("opaque (alpha ignored)");
	}
	if (bits & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied)
	{
		result.push_back("pre multiplied (alpha expected to already be multiplied in image)");
	}
	if (bits & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied)
	{
		result.push_back("post multiplied (alpha will be applied during composition)");
	}
	if (bits & vk::CompositeAlphaFlagBitsKHR::eInherit)
	{
		result.push_back("inherited");
	}

	return result;
}

std::vector<std::string> tze::logImageUsageBits(vk::ImageUsageFlags bits)
{
	std::vector<std::string> result;

	if (bits & vk::ImageUsageFlagBits::eTransferSrc) {
		result.push_back("transfer src: image can be used as the source of a transfer command.");
	}
	if (bits & vk::ImageUsageFlagBits::eTransferDst) {
		result.push_back("transfer dst: image can be used as the destination of a transfer command.");
	}
	if (bits & vk::ImageUsageFlagBits::eSampled) {
		result.push_back("sampled: image can be used to create a VkImageView suitable for occupying a \
VkDescriptorSet slot either of type VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE or \
VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, and be sampled by a shader.");
	}
	if (bits & vk::ImageUsageFlagBits::eStorage) {
		result.push_back("storage: image can be used to create a VkImageView suitable for occupying a \
VkDescriptorSet slot of type VK_DESCRIPTOR_TYPE_STORAGE_IMAGE.");
	}
	if (bits & vk::ImageUsageFlagBits::eColorAttachment) {
		result.push_back("color attachment: image can be used to create a VkImageView suitable for use as \
a color or resolve attachment in a VkFramebuffer.");
	}
	if (bits & vk::ImageUsageFlagBits::eDepthStencilAttachment) {
		result.push_back("depth/stencil attachment: image can be used to create a VkImageView \
suitable for use as a depth/stencil or depth/stencil resolve attachment in a VkFramebuffer.");
	}
	if (bits & vk::ImageUsageFlagBits::eTransientAttachment) {
		result.push_back("transient attachment: implementations may support using memory allocations \
with the VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT to back an image with this usage. This \
bit can be set for any image that can be used to create a VkImageView suitable for use as \
a color, resolve, depth/stencil, or input attachment.");
	}
	if (bits & vk::ImageUsageFlagBits::eInputAttachment) {
		result.push_back("input attachment: image can be used to create a VkImageView suitable for \
occupying VkDescriptorSet slot of type VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT; be read from \
a shader as an input attachment; and be used as an input attachment in a framebuffer.");
	}
	if (bits & vk::ImageUsageFlagBits::eFragmentDensityMapEXT) {
		result.push_back("fragment density map: image can be used to create a VkImageView suitable \
for use as a fragment density map image.");
	}
	if (bits & vk::ImageUsageFlagBits::eFragmentShadingRateAttachmentKHR) {
		result.push_back("fragment shading rate attachment: image can be used to create a VkImageView \
suitable for use as a fragment shading rate attachment or shading rate image");
	}

	return result;
}

std::string tze::logPresentMode(vk::PresentModeKHR presentMode)
{
	if (presentMode == vk::PresentModeKHR::eImmediate) {
		return "immediate: the presentation engine does not wait for a vertical blanking period \
to update the current image, meaning this mode may result in visible tearing. No internal \
queuing of presentation requests is needed, as the requests are applied immediately.";
	}
	if (presentMode == vk::PresentModeKHR::eMailbox) {
		return "mailbox: the presentation engine waits for the next vertical blanking period \
to update the current image. Tearing cannot be observed. An internal single-entry queue is \
used to hold pending presentation requests. If the queue is full when a new presentation \
request is received, the new request replaces the existing entry, and any images associated \
with the prior entry become available for re-use by the application. One request is removed \
from the queue and processed during each vertical blanking period in which the queue is non-empty.";
	}
	if (presentMode == vk::PresentModeKHR::eFifo) {
		return "fifo: the presentation engine waits for the next vertical blanking \
period to update the current image. Tearing cannot be observed. An internal queue is used to \
hold pending presentation requests. New requests are appended to the end of the queue, and one \
request is removed from the beginning of the queue and processed during each vertical blanking \
period in which the queue is non-empty. This is the only value of presentMode that is required \
to be supported.";
	}
	if (presentMode == vk::PresentModeKHR::eFifoRelaxed) {
		return "relaxed fifo: the presentation engine generally waits for the next vertical \
blanking period to update the current image. If a vertical blanking period has already passed \
since the last update of the current image then the presentation engine does not wait for \
another vertical blanking period for the update, meaning this mode may result in visible tearing \
in this case. This mode is useful for reducing visual stutter with an application that will \
mostly present a new image before the next vertical blanking period, but may occasionally be \
late, and present a new image just after the next vertical blanking period. An internal queue \
is used to hold pending presentation requests. New requests are appended to the end of the queue, \
and one request is removed from the beginning of the queue and processed during or after each \
vertical blanking period in which the queue is non-empty.";
	}
	if (presentMode == vk::PresentModeKHR::eSharedDemandRefresh) {
		return "shared demand refresh: the presentation engine and application have \
concurrent access to a single image, which is referred to as a shared presentable image. \
The presentation engine is only required to update the current image after a new presentation \
request is received. Therefore the application must make a presentation request whenever an \
update is required. However, the presentation engine may update the current image at any point, \
meaning this mode may result in visible tearing.";
	}
	if (presentMode == vk::PresentModeKHR::eSharedContinuousRefresh) {
		return "shared continuous refresh: the presentation engine and application have \
concurrent access to a single image, which is referred to as a shared presentable image. The \
presentation engine periodically updates the current image on its regular refresh cycle. The \
application is only required to make one initial presentation request, after which the \
presentation engine must update the current image without any need for further presentation \
requests. The application can indicate the image contents have been updated by making a \
presentation request, but this does not guarantee the timing of when it will be updated. \
This mode may result in visible tearing if rendering to the image is not timed correctly.";
	}
	return "none/undefined";
}


