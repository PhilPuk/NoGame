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

	while (handleMessage()) 
	{
		//TODO: Render with Vulkan
	}

	VKA(vkDeviceWaitIdle(context->device));
	destroySwapchain(context, &swapchain);
	VK(vkDestroySurfaceKHR(context->instance, surface, 0));
	exitVulkan(context);

	SDL_DestroyWindow(window);
	SDL_Quit();

	LOG_INFO("Exiting NoGame_Game_Engine\n\n");
	return 0;
}