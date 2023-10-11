#include "../logger.h"

#include <vulkan/vulkan.h>
#include<cassert>

#define ASSERT_VULKAN(val) if (val != VK_SUCCESS) {LOG_ERROR("Vulkan error: ", val); assert(false);}

#ifndef VK
#define VK(f) (f)
#endif

#ifndef VKA
#define VKA(f) ASSERT_VULKAN(VK(f))
#endif

//#ifndef VULKAN_INFO_OUTPUT
//#define VULKAN_INFO_OUTPUT
//#endif

#define ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))

struct VulkanContext 
{
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkPhysicalDeviceProperties physicalDeviceProperties;
};

VulkanContext* initVulkan(uint32_t instanceExtensionCount, const char** instanceExtensions);