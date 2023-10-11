cmake_minimum_required(VERSION 3.10)

# Project name
project(NoGame_Game_Engine)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

set(SOURCE_FILES src/main.cpp src/simple_logger.cpp src/vulkan_base/vulkan_device.cpp)

# Find SDL2
add_subdirectory(libs/SDL)

# Find Vulkan
find_package(Vulkan REQUIRED)

# vulkan_tutorial executable
add_executable(NoGame_Game_Engine ${SOURCE_FILES})
target_include_directories(NoGame_Game_Engine PUBLIC libs/SDL/include)
target_link_libraries(NoGame_Game_Engine PUBLIC SDL3::SDL3-shared)
target_include_directories(NoGame_Game_Engine PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(NoGame_Game_Engine PUBLIC ${Vulkan_LIBRARIES})