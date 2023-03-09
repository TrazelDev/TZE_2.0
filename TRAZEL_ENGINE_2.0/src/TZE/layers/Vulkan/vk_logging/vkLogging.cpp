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
	vk::PhysicalDeviceProperties proprties = device.getProperties();

	std::cout << "device name: " << proprties.deviceName << '\n';

	std::cout << "device type: ";
	switch (proprties.deviceType)
	{
	case (vk::PhysicalDeviceType::eCpu):
		TZE_ENGINE_WARN("gpu wasn't choosen to be the physical device");
		std::cout << "CPU\n";
		break;

	case (vk::PhysicalDeviceType::eDiscreteGpu):
		std::cout << "Discrte GPU\n";
		break;

	case (vk::PhysicalDeviceType::eIntegratedGpu):
		std::cout << "Integrated GPU\n";
		break;

	case (vk::PhysicalDeviceType::eVirtualGpu):
		std::cout << "Virtuel GPU\n";
		break;

	default:
		std::cout << "Other\n";
	}
}

