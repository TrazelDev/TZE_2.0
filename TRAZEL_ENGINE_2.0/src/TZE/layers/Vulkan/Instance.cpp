#include "pch.h"
#include "Instance.h"
#include "vk_logging/vkLogging.h"

tze::Instance::Instance(GLFWwindow* window, const std::string& title)
{
	createInstance(title);
	
	_dldi = vk::DispatchLoaderDynamic(_instance, vkGetInstanceProcAddr);
	DEBUG_CMD(tze::makeDebugMessenger(_instance, _dldi););


 	VkSurfaceKHR c_stayle_surface;
 	if (glfwCreateWindowSurface(_instance, window, nullptr, &c_stayle_surface) != VK_SUCCESS)
 	{
 		TZE_ENGINE_ERR("failed to abstract the glfw surface for Vulkan.\n");
 	}
 	else
 	{
 		TZE_ENGINE_INFO("Successfully to abstracted the glfw surface for Vulkan."); 
 	}

	_surface = c_stayle_surface;
}

tze::Instance::~Instance()
{
	_instance.destroy();
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
	if (_instance)
	{
		TZE_ENGINE_ERR("Failed to create an instance");
	}
}
