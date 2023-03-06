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

