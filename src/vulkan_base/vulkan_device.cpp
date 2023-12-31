#include "vulkan_base.h"

bool initVulkanInstance(VulkanContext* context, uint32_t instanceExtensionCount, const char** instanceExtensions)
{
	uint32_t layerPropertyCount = 0;
	VKA(vkEnumerateInstanceLayerProperties(&layerPropertyCount, 0));
	VkLayerProperties* layerProperties = new VkLayerProperties[layerPropertyCount];
	VKA(vkEnumerateInstanceLayerProperties(&layerPropertyCount, layerProperties));
	for (uint32_t i = 0; i < layerPropertyCount; ++i)
	{
#ifdef VULKAN_INFO_OUTPUT
		LOG_INFO(layerProperties[i].layerName);
		LOG_INFO(layerProperties[i].description);
#endif
	}
	delete[] layerProperties;

	const char* enabledLayers[] = { 
		"VK_LAYER_KHRONOS_validation",
	};

#ifdef VULKAN_INFO_OUTPUT
	uint32_t availableInstanceExtensionCount = 0;
	VKA(vkEnumerateInstanceExtensionProperties(0, &availableInstanceExtensionCount, 0));
	VkExtensionProperties* instanceExtensionProperties = new VkExtensionProperties[availableInstanceExtensionCount];
	VKA(vkEnumerateInstanceExtensionProperties(0, &availableInstanceExtensionCount, instanceExtensionProperties));
	for (uint32_t i = 0; i < availableInstanceExtensionCount; ++i)
	{
		LOG_INFO(instanceExtensionProperties[i].extensionName);
	}
	delete[] instanceExtensionProperties;
#endif

	VkApplicationInfo applicationInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
	applicationInfo.pApplicationName = "Vulkan Tutorial";
	applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1); // 0.0.1
	applicationInfo.apiVersion = VK_API_VERSION_1_3;

	VkInstanceCreateInfo createInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
	createInfo.pApplicationInfo = &applicationInfo;
	createInfo.enabledLayerCount = ARRAY_COUNT(enabledLayers);
	createInfo.ppEnabledLayerNames = enabledLayers;
	createInfo.enabledExtensionCount = instanceExtensionCount;
	createInfo.ppEnabledExtensionNames = instanceExtensions;

	if (VK(vkCreateInstance(&createInfo, 0, &context->instance)) != VK_SUCCESS) 
	{
		LOG_ERROR("Error creating vulkan instance");
		return false;
	}

	return true;
}

bool selectPhysicalDevice(VulkanContext* context)
{
	uint32_t physicalDeviceCount = 0;
	VKA(vkEnumeratePhysicalDevices(context->instance, &physicalDeviceCount, 0));

	if (physicalDeviceCount == 0)
	{
		LOG_ERROR("Failed to find GPUs with Vulkan Support!");
		context->physicalDevice = 0;
		return false;
	}

	VkPhysicalDevice* physicalDevices = new VkPhysicalDevice[physicalDeviceCount];
	VKA(vkEnumeratePhysicalDevices(context->instance, &physicalDeviceCount, physicalDevices));
	LOG_INFO("Found ", physicalDeviceCount, " GPU(s):");

	for (uint32_t i = 0; i < physicalDeviceCount; ++i)
	{
#ifdef VULKAN_INFO_OUTPUT
		VkPhysicalDeviceProperties properties = {};
		vkGetPhysicalDeviceProperties(physicalDevices[i], &properties);
		LOG_INFO("GPU ", i, ": ", properties.deviceName);
#endif
	}

	context->physicalDevice = physicalDevices[0];
	VK(vkGetPhysicalDeviceProperties(context->physicalDevice, &context->physicalDeviceProperties));
	LOG_INFO("Selected GPU: ", context->physicalDeviceProperties.deviceName);

	delete[] physicalDevices;

	return true;
}

bool createLogicalDevice(VulkanContext* context, uint32_t deviceExtensionCount, const char** devicesExtensions)
{
	//Queues
	uint32_t numQueueFamilies = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(context->physicalDevice, &numQueueFamilies, 0);
	VkQueueFamilyProperties* queueFamilyProperties = new VkQueueFamilyProperties[numQueueFamilies];
	vkGetPhysicalDeviceQueueFamilyProperties(context->physicalDevice, &numQueueFamilies, queueFamilyProperties);	

	uint32_t graphicsQueueIndex = 0;
	for (uint32_t i = 0; i < numQueueFamilies; ++i)
	{
		VkQueueFamilyProperties queueFamily = queueFamilyProperties[i];
		if (queueFamily.queueCount > 0)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				graphicsQueueIndex = i;
				break;
			}
		}
	}

	float priorities[] = { 1.0f };
	VkDeviceQueueCreateInfo queueCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
	queueCreateInfo.queueFamilyIndex = graphicsQueueIndex;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = priorities;

	VkPhysicalDeviceFeatures enabledFeatures = {};
	VkDeviceCreateInfo createInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
	createInfo.queueCreateInfoCount = 1;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.enabledExtensionCount = deviceExtensionCount;
	createInfo.ppEnabledExtensionNames = devicesExtensions;
	createInfo.pEnabledFeatures = &enabledFeatures;

	if (vkCreateDevice(context->physicalDevice, &createInfo, 0, &context->device))
	{
		LOG_ERROR("Failed to create vulkan logical device");
		return false;
	}

	// Acquire graphics queues
	context->graphicsQueue.familyIndex = graphicsQueueIndex;
	VK(vkGetDeviceQueue(context->device, graphicsQueueIndex, 0, &context->graphicsQueue.queue));

	return true;
}

VulkanContext* initVulkan(uint32_t instanceExtensionCount, const char** instanceExtensions, uint32_t deviceExtensionCount, const char** devicesExtensions)
{
	VulkanContext* context = new VulkanContext;

	if (!initVulkanInstance(context, instanceExtensionCount, instanceExtensions))
	{
		return 0;
	}

	if (!selectPhysicalDevice(context))
	{
		return 0;
	}

	if (!createLogicalDevice(context, deviceExtensionCount, devicesExtensions))
	{
		return 0;
	}

	return context;
}

void exitVulkan(VulkanContext* context)
{
	VKA(vkDeviceWaitIdle(context->device));
	VK(vkDestroyDevice(context->device, 0));

	vkDestroyInstance(context->instance, 0);
}