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
		// std::vector<std::string> log_transform_bits(vk::SurfaceTransformFlagsKHR bits);
		// 
		// 
		// std::vector<std::string> log_alpha_composite_string(vk::CompositeAlphaFlagsKHR bits);
		// 
		// 
		// std::vector<std::string> log_image_usage_bits(vk::ImageUsageFlags bits);

}