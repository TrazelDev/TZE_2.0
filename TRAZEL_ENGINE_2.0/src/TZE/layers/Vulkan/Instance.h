#pragma once
#include "Layers/Layer.h"

namespace tze
{
	class Instance
	{
	public:
		Instance(GLFWwindow* window, const std::string& title = "TRAZEL_ENGINE");
		~Instance();
		
		/// <summary>
		/// the function is checking if the the extensions and layers that were chosen are real extensions for Vulkan instance 
		/// </summary>
		/// <param name="extensions">the extensions for the instance</param>
		/// <param name="layers">the layers for the instance</param>
		/// <returns>bool that says if the extensions and layers are real and supported</returns>
		bool supported(std::vector<const char*> extensions, std::vector<const char*>& layers);
		
	private:
		void createInstance(const std::string& title);

		vk::Instance _instance;
		vk::DispatchLoaderDynamic _dldi; 
		vk::SurfaceKHR _surface;
		DEBUG_CMD(vk::DebugUtilsMessengerEXT _debugMessenger;)
		
	};
}