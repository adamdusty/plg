#pragma once

#include <SDL3/SDL.h>
#include <cstdint>
#include <vector>
#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

namespace core::rendering {

auto create_instance() -> VkInstance;
auto get_available_devices(VkInstance instance) -> std::vector<VkPhysicalDevice>;
auto get_queue_families(VkPhysicalDevice device) -> std::vector<VkQueueFamilyProperties>;
auto select_graphics_queue(const std::vector<VkQueueFamilyProperties>& families) -> std::uint32_t;
auto create_device(VkPhysicalDevice physical_device, std::uint32_t family) -> VkDevice;
auto get_queue(VkDevice device, std::uint32_t index) -> VkQueue;
auto create_allocator(VkInstance inst, VkPhysicalDevice phys_device, VkDevice dev) -> VmaAllocator;
auto get_surface(SDL_Window* window, VkInstance instance) -> VkSurfaceKHR;
auto get_surface_capabilities(VkPhysicalDevice dev, VkSurfaceKHR surf) -> VkSurfaceCapabilitiesKHR;
auto create_swapchain(VkDevice dev, VkSurfaceKHR surf, VkSurfaceCapabilitiesKHR caps)
    -> VkSwapchainKHR;

} // namespace core::rendering
