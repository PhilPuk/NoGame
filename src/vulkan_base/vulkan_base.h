#include "../logger.h"

#include <vulkan/vulkan.h>
#include<cassert>
#include<vector>

#define ASSERT_VULKAN(val) if (val != VK_SUCCESS) {LOG_ERROR("Vulkan error: ", val); assert(false);}

#ifndef VK
#define VK(f) (f)
#endif

#ifndef VKA
#define VKA(f) ASSERT_VULKAN(VK(f))
#endif

#define ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))

//#ifndef VULKAN_INFO_OUTPUT
//#define VULKAN_INFO_OUTPUT
//#endif

struct VulkanQueue
{
	VkQueue queue;
	uint32_t familyIndex;
};

struct VulkanSwapchain
{
	VkSwapchainKHR swapchain;
	uint32_t width;
	uint32_t height;
	VkFormat format;
	std::vector<VkImage> images;
	std::vector<VkImageView> imageViews;
};

struct VulkanPipeline
{
	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;
};

struct VulkanContext 
{
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkPhysicalDeviceProperties physicalDeviceProperties;
	VkDevice device;
	VulkanQueue graphicsQueue;
};

VulkanContext* initVulkan(uint32_t instanceExtensionCount, const char** instanceExtensions, uint32_t deviceExtensionCount, const char** devicesExtensions);
void exitVulkan(VulkanContext* context);

VulkanSwapchain createSwapchain(VulkanContext* context, VkSurfaceKHR surface, VkImageUsageFlags usage);
void destroySwapchain(VulkanContext* context, VulkanSwapchain* swapchain);

VkRenderPass createRenderPass(VulkanContext* context, VkFormat format);
void destroyRenderPass(VulkanContext* context, VkRenderPass renderPass);

VulkanPipeline createPipeline(VulkanContext* context, const char* vertexShaderFilename, const char* fragmentShaderFilename, VkRenderPass renderPass, uint32_t width, uint32_t height);
void destroyPipeline(VulkanContext* context, VulkanPipeline* pipeline);
