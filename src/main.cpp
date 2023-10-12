#define SDL_MAIN_HANDLED
#include <SDL.h>
#include<SDL_vulkan.h>

#include "logger.h"
#include "vulkan_base/vulkan_base.h"

bool handleMessage()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			return false;
		}
	}
	return true;
}

int main() 
{
	LOG_INFO("Starting NoGame_Game_Engine");
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		LOG_ERROR("Error initializing SDL: ", SDL_GetError());
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("NoGame_Game_Engine", 1280, 720, SDL_WINDOW_VULKAN);
	if (!window) 
	{
		LOG_ERROR("Error creating SDL window");
		return 1;
	}

	uint32_t instanceExtensionCount = 0;
	SDL_Vulkan_GetInstanceExtensions(&instanceExtensionCount, 0);
	const char** enabledInstanceExtensions = new const char*[instanceExtensionCount];
	SDL_Vulkan_GetInstanceExtensions(&instanceExtensionCount, enabledInstanceExtensions);

	const char* enabledDeviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	VulkanContext* context = initVulkan(instanceExtensionCount, enabledInstanceExtensions, ARRAY_COUNT(enabledDeviceExtensions), enabledDeviceExtensions);
	VkSurfaceKHR surface;
	SDL_Vulkan_CreateSurface(window, context->instance, &surface);
	VulkanSwapchain swapchain = createSwapchain(context, surface, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);

	VkRenderPass renderPass = createRenderPass(context, swapchain.format);
	std::vector<VkFramebuffer> framebuffers(swapchain.images.size());
	//framebuffers.resize(swapchain.images.size());
	for (uint32_t i = 0; i < swapchain.images.size(); ++i)
	{
		VkFramebufferCreateInfo createInfo = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
		createInfo.renderPass = renderPass;
		createInfo.attachmentCount = 1;
		createInfo.pAttachments = &swapchain.imageViews[i];
		createInfo.width = swapchain.width;
		createInfo.height = swapchain.height;
		createInfo.layers = 1;

		VKA(vkCreateFramebuffer(context->device, &createInfo, 0, &framebuffers[i]));
	}

	while (handleMessage()) 
	{
		//TODO: Render with Vulkan
	}

	VKA(vkDeviceWaitIdle(context->device));
	for (uint32_t i = 0; i < framebuffers.size(); ++i)
	{
		VK(vkDestroyFramebuffer(context->device, framebuffers[i], 0));
	}
	framebuffers.clear();
	destroyRenderPass(context, renderPass);
	destroySwapchain(context, &swapchain);
	VK(vkDestroySurfaceKHR(context->instance, surface, 0));
	exitVulkan(context);

	SDL_DestroyWindow(window);
	SDL_Quit();

	LOG_INFO("Exiting NoGame_Game_Engine\n\n");
	return 0;
}