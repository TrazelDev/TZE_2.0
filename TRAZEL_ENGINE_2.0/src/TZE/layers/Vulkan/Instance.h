#pragma once
#include "Layers/Layer.h"

namespace tze
{
	// structure of queue families which holds the indexes of families of command which the most important one's are:
	// graphical commands - responsible for running draw and running related gpu command 
	// present commands - responsible for running commands that related to presentations of the images on the screen
	// TO CHECK: Compute Queue, Transfer Queue, Sparse Binding Queue
	struct QueueFamilies
	{
		std::optional<uint32_t> graphicsIndex;
		std::optional<uint32_t> presentIndex;

		/// <summary>
		/// the function is checking if the two most important queues which are the ability to present and the ability to run graphical draw commands 
		/// </summary>
		/// <returns>if the indexes of the graphical draw queues and the present queues in the physical device have been found</returns>
		bool hasEssentials()
		{
			return graphicsIndex.has_value() && presentIndex.has_value();
		}
	};

	// this a class that is responsible of creating the VULKAN instance and in charge of creating 
	// managing both the logical and the physical devices
	class Instance : public Layer
	{
	public:
		Instance(GLFWwindow* window, const std::string& title = "TRAZEL_ENGINE");
		~Instance();
		
		void run() override;


		vk::PhysicalDevice* getPhysicalDevice();
		vk::Device* getLogicalDevice();
		vk::SurfaceKHR* getSurface();
		QueueFamilies* getQueueFamilies();

	private:
		/// <summary>
		/// the function is checking if the the extensions and layers that were chosen are real extensions for Vulkan instance 
		/// </summary>
		/// <param name="extensions">the extensions for the instance</param>
		/// <param name="layers">the layers for the instance</param>
		/// <returns>bool that says if the extensions and layers are real and supported</returns>
		bool supported(std::vector<const char*> extensions, std::vector<const char*>& layers);
		/// <summary>
		/// In charge of creating the VULKAN instance 
		/// </summary>
		/// <param name="title">the title of the engine</param>
		void createInstance(const std::string& title);
		
		/// <summary>
		/// the function is going through the physical devices that are connected to the system
		/// </summary>
		void choosePhysicalDevice();
		/// <summary>
		/// the function is getting a physical device and checking if it suitable for the application needs 
		/// </summary>
		/// <param name="device">the device that we want to check</param>
		/// <returns>if the physical device is suitible </returns>
		bool checkPhysicalDevice(const vk::PhysicalDevice& device) const;
		/// <summary>
		/// function that is getting a physical device and a vector of extensions and checks if the device is supporting all of the extensions 
		/// </summary>
		/// <param name="device">the device that we want to check</param>
		/// <param name="requestedExtensions">a vector with the extensions that the physical device needs to support</param>
		/// <returns>if the device supports the extensions or not</returns>
		bool checkDeviceExtensionsSupport(const vk::PhysicalDevice& device, const std::vector<const char*>& requestedExtensions) const;
		
		void createLogicalDevice();
		/// <summary>
		/// checking if the queue families in the physical device supporting the necessary queue families of graphics and presenting
		/// </summary>
		void checkAvailableQueues();


		// Instance related:
		vk::Instance _instance;  // instance to the VULKAN API
		vk::SurfaceKHR _surface; // the surface of the VULKAN on the window ( surface is essentially a way place where the VULKAN API can render on the screen)
		vk::DispatchLoaderDynamic _dldi; // dynamic dispatch loader a way to take the function of the gpu and use them

		// Debug:
		DEBUG_CMD(vk::DebugUtilsMessengerEXT _debugMessenger;) // a way for VULKAN errors or warnings to be written but only in debug mode to not impact the performance of the application
		
		// Device related:
		vk::PhysicalDevice _physicalDevice;
		vk::Device _logicalDevice;
		QueueFamilies _indices;
	};
}