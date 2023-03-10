#pragma once

namespace tze
{
		VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback
		(
			VkDebugUtilsMessageSeverityFlagBitsEXT messangerServerity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData
		);


		vk::DebugUtilsMessengerEXT makeDebugMessenger(vk::Instance& instance, vk::DispatchLoaderDynamic& dldi);

		void logDeviceProperties(const vk::PhysicalDevice& device);

		// logging functions:
		std::vector<std::string> logTransformBits(vk::SurfaceTransformFlagsKHR bits);
		std::vector<std::string> logAlphaCompositesString(vk::CompositeAlphaFlagsKHR bits);
		std::vector<std::string> logImageUsageBits(vk::ImageUsageFlags bits);
		std::string logPresentMode(vk::PresentModeKHR presentMode);

}