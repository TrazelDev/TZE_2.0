#include "pch.h"
#include "Instance.h"
#include "vk_logging/vkLogging.h"

tze::Instance::Instance(GLFWwindow* window, const std::string& title)
{
	createInstance(title); // creating the instance to the VULKAN API
	
	_dldi = vk::DispatchLoaderDynamic(_instance, vkGetInstanceProcAddr); // creating the dynamic dispatch loader which is in charge of taking the VULKAN commands from the gpu
	DEBUG_CMD(tze::makeDebugMessenger(_instance, _dldi);); // making a debug messenger if this is not a CLIENT_MODE to help debug the application in a easier way

	// creating c style surface because the following function takes a c style VULKAN surface and I don't know another way to do it 
 	VkSurfaceKHR CStyleSurface;
 	if (glfwCreateWindowSurface(_instance, window, nullptr, &CStyleSurface) != VK_SUCCESS)
 	{
 		TZE_ENGINE_ERR("failed to abstract the glfw surface for Vulkan.\n");
 	}
	else
	{
		TZE_ENGINE_INFO("Successfully to abstracted the glfw surface for Vulkan.");
	}
	_surface = CStyleSurface;

	// choosing the physical device for the application usage:
	choosePhysicalDevice();

	// creating a logical device and finding out if the physical device that has been chosen is supporting the graphics and presenting queues
	createLogicalDevice();
}

tze::Instance::~Instance()
{
	_logicalDevice.destroy();

	_instance.destroySurfaceKHR(_surface);
	DEBUG_CMD(_instance.destroyDebugUtilsMessengerEXT(_debugMessenger, nullptr, _dldi);)
	_instance.destroy();
}

void tze::Instance::run()
{
}

vk::PhysicalDevice* tze::Instance::getPhysicalDevice()
{
	return &_physicalDevice;
}

vk::Device* tze::Instance::getLogicalDevice()
{
	return &_logicalDevice;
}

vk::SurfaceKHR* tze::Instance::getSurface()
{
	return &_surface;
}

tze::QueueFamilies* tze::Instance::getQueueFamilies()
{
	return &_indices;
}


bool tze::Instance::supported(std::vector<const char*> extensions, std::vector<const char*>& layers)
{
	std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();

	// printing the supported extensions:
	SHOW_DATA
	(
		std::cout << "Device can support the following extensions:\n";
		for (vk::ExtensionProperties supportedExtension : supportedExtensions)
		{
			std::cout << '\t' << supportedExtension.extensionName << '\n';
		}
	);

	// checking extension support:
	bool found = false;
	for (const char* extension : extensions)
	{
		found = false;
		for (vk::ExtensionProperties supportedExtention : supportedExtensions)
		{
			if (strcmp(extension, supportedExtention.extensionName) == 0)
			{
				found = true;
				
				SHOW_DATA
				(
					TZE_ENGINE_INFO(std::string("Extension \"") + extension + "\" is supported!\n");
				);
			}
		}

		if (!found)
		{
			TZE_ENGINE_WARN(std::string("Extension \"") + extension + "\" is not supported!\n");
			return false;
		}
	}

	// printing the supported layers
	std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();
	SHOW_DATA
	(
		std::cout << "device can support the following layers:\n";
		for (vk::LayerProperties supportedLayer : supportedLayers)
		{
			std::cout << '\t' << supportedLayer.layerName << '\n';
		}
	);

	// check layer support
	for (const char* layer : layers)
	{
		found = false;
		for (vk::LayerProperties supportedLayer : supportedLayers)
		{
			if (strcmp(layer, supportedLayer.layerName) == 0)
			{
				found = true;
				SHOW_DATA(TZE_ENGINE_INFO(std::string("Layer \"") + layer + "\" is supported!\n"););
			}
		}

		if (!found)
		{
			TZE_ENGINE_WARN(std::string("Layer \"") + layer + "\" is not supported!\n");
			return false;
		}
	}

	return true;
}

void tze::Instance::createInstance(const std::string& title)
{
	uint32_t version{ 0 };
	vkEnumerateInstanceVersion(&version);

	SHOW_DATA
	(
		std::cout << "System can support vulkan variants : " << VK_API_VERSION_VARIANT(version) <<
		", major: " << VK_API_VERSION_MAJOR(version) <<
		", minor: " << VK_API_VERSION_MINOR(version) <<
		", pathc: " << VK_API_VERSION_PATCH(version) << "\n";
	);

	version &= ~(0xFFFU);
	version = VK_MAKE_API_VERSION(0, 1, 0, 0);

	vk::ApplicationInfo appInfo = vk::ApplicationInfo
	(
		title.c_str(),
		version,
		title.c_str(),
		version,
		version
	);

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	extensions.push_back("VK_EXT_debug_utils");

	std::vector<const char*> layers;
	layers.push_back("VK_LAYER_KHRONOS_validation");

	if (!supported(extensions, layers))
	{
		TZE_ENGINE_ERR("Something is wrong with one of the extensions or layers for the instance");
		return;
	}

	// Setting up the info that is going to be used in order to construct the instance
	vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo
	(
		vk::InstanceCreateFlags(),
		&appInfo,
		static_cast<uint32_t>(layers.size()),
		layers.data(),
		static_cast<uint32_t>(extensions.size()),
		extensions.data()
	);

	// Creating the instance
	_instance = vk::createInstance(createInfo);
	if (!_instance)
	{
		TZE_ENGINE_ERR("Failed to create an instance");
	}
	else
	{
		TZE_ENGINE_INFO("Created a VULKAN instance");
	}
}

void tze::Instance::choosePhysicalDevice()
{
	std::vector<vk::PhysicalDevice> avalibleDevices = _instance.enumeratePhysicalDevices();

	for (const vk::PhysicalDevice& device : avalibleDevices)
	{
		SHOW_DATA(logDeviceProperties(device););
		
		if (checkPhysicalDevice(device))
		{
			TZE_ENGINE_INFO("Device supports the requested extensions!\n");
			_physicalDevice = device;
			return;
		}
		else
		{
			TZE_ENGINE_WARN("WARNING: device can't support the requested extensions!\n");
		}
	}

	TZE_ENGINE_ERR("ERROR: there is no suitable physical devices");
}

bool tze::Instance::checkPhysicalDevice(const vk::PhysicalDevice& device) const
{
	// a device is suitable if it can present to the screen, is support the swapchain extension
	const std::vector<const char*> requestedExtentions = 
	{ 
		VK_KHR_SWAPCHAIN_EXTENSION_NAME 
	};


	// this printing the requested extensions for the physical device ( probably gpu)
	SHOW_DATA
	(
		std::cout << "we are requesting device extensions:\n";
		for (const char* extention : requestedExtentions)
		{
			std::cout << "\t\"" << extention << "\"\n";
		}
	)


	return checkDeviceExtensionsSupport(device, requestedExtentions);
}

bool tze::Instance::checkDeviceExtensionsSupport(const vk::PhysicalDevice& device, const std::vector<const char*>& requestedExtensions) const
{
	// check if the given device can satisfy a list requested device extensions
	std::set<std::string> requiredExtentions(requestedExtensions.begin(), requestedExtensions.end());

	SHOW_DATA(std::cout << "device can support extensions:\n";);

	for (vk::ExtensionProperties& extention : device.enumerateDeviceExtensionProperties())
	{
		SHOW_DATA(std::cout << "\t\"" << extention.extensionName << "\"\n";);

		// if this extensions is one the extensions that the device needs to support than removing it from the set of requested extensions 
		requiredExtentions.erase(extention.extensionName);
	}

	// if the set is empty then all required extensions have been found
	return requiredExtentions.empty();
}

void tze::Instance::createLogicalDevice()
{
	checkAvailableQueues();
	std::vector<uint32_t> uniqueIndices;
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo;
	float queuePriority = 1.0f;

	uniqueIndices.push_back(_indices.graphicsIndex.value());
	if (_indices.graphicsIndex.value() != _indices.presentIndex.value()) // if they don't have the same index then pushing both
	{
		uniqueIndices.push_back(_indices.presentIndex.value());
	}

	for (uint32_t queueFamilyIndex : uniqueIndices)
	{
		queueCreateInfo.push_back
		(
			vk::DeviceQueueCreateInfo
			(
				vk::DeviceQueueCreateFlags(),
				queueFamilyIndex,
				1,
				&queuePriority
			)
		);
	}

	std::vector<const char*> deviceExtentions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();
	std::vector<const char*> enabledLayers;

	DEBUG_CMD(enabledLayers.push_back("VK_LAYER_KHRONOS_validation");); // adding a validation layer that checks for error at debugging stage

	vk::DeviceCreateInfo deviceInfo = vk::DeviceCreateInfo
	(
		vk::DeviceCreateFlags(),
		queueCreateInfo.size(), queueCreateInfo.data(),
		enabledLayers.size(), enabledLayers.data(),
		deviceExtentions.size(), deviceExtentions.data(),
		&deviceFeatures
	);

	try
	{
		_logicalDevice = _physicalDevice.createDevice(deviceInfo);
		if (_logicalDevice)
		{
			TZE_ENGINE_INFO("GPU has been successfully abstracted! ");
		}
		else
		{
			TZE_ENGINE_ERR("Device creation failed!");
		}
	}
	catch (vk::SystemError err)
	{
		TZE_ENGINE_ERR("Device creation failed!");
	}
}

void tze::Instance::checkAvailableQueues()
{
	std::vector<vk::QueueFamilyProperties> queueFamilies = _physicalDevice.getQueueFamilyProperties();

	SHOW_DATA(std::cout << "System can support " << queueFamilies.size() << " queue families.\n";);

	int index = 0;
	for (const vk::QueueFamilyProperties& queueFamily : queueFamilies)
	{
		// checking if the queue flags of the physical device that we are using has the ability to preform and run graphics drawing command
		// by making an AND bit wise command and checking if the graphics command queue bit is turned on in the bits the physical device supports:
		if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
		{
			_indices.graphicsIndex = index;

			// printing the index of the queue family of graphics:
			SHOW_DATA(TZE_ENGINE_INFO(std::string("Queue family ") + std::to_string(index) + " is suitable for graphics drawing and rendering"););
		}

		// checking if it supporting the the presenting queue commands ( here we are using the surface command because it is not enough if the physical supports it) 
		if (_physicalDevice.getSurfaceSupportKHR(index, _surface))
		{
			_indices.presentIndex = index;

			// printing the index of the queue family of presenting:
			SHOW_DATA(TZE_ENGINE_INFO(std::string("Queue family ") + std::to_string(index) + " is suitable for presenting"););
		}

		// if the the indexes of the presenting and graphics have been found than returning those indexes:
		if (_indices.hasEssentials())
		{
			TZE_ENGINE_INFO("Successfully found the graphics and presenting queues");
			return;
		}
		index++;
	}

	TZE_ENGINE_ERR("physical device is not supporting the necessary necessity");
}



