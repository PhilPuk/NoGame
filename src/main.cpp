#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "logger.h"
#include "vulkan_base/vulkan_base.h"

bool handleMessage() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_EVENT_QUIT:
			return false;
		}
	}
	return true;
}

int main() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		LOG_ERROR("Error initializing SDL: ", SDL_GetError());
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Vulkan Tutorial", 1240, 720, SDL_WINDOW_VULKAN);
	if (!window) {
		LOG_ERROR("Error creating SDL window");
		return 1;
	}

	VulkanContext* context = initVulkan();

	while (handleMessage()) {
		//TODO: Render with Vulkan
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}