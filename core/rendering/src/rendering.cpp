#include "rendering/export.hpp"

#include "initialization.hpp"
#include <SDL3/SDL.h>
#include <cstdint>
#include <flecs.h>
#include <plg/sdk.hpp>
#include <volk.h>
#include <vulkan/vulkan.h>

constexpr std::uint32_t device_index = 0;

extern "C" {
    CORE_RENDERING_EXPORT auto initialize(ecs_world_t* wld) -> void {
        SDL_Log("Initializing core rendering module");

        auto world = flecs::world{wld};

        volkInitialize();

        SDL_Log("Creating instance");
        auto* instance = core::rendering::create_instance();
        if(instance != nullptr) {
            SDL_Log("Created instance successfully");
        } else {
            SDL_Log("Failed to create instance");
        }

        volkLoadInstance(instance);
        auto devices                     = core::rendering::get_available_devices(instance);
        VkPhysicalDevice physical_device = devices.at(device_index);

        VkPhysicalDeviceProperties2 device_properties{
            .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2
        };
        vkGetPhysicalDeviceProperties2(physical_device, &device_properties);

        SDL_Log("Selected device: %s", device_properties.properties.deviceName);

        auto queue_families = core::rendering::get_queue_families(physical_device);
        auto queue_index    = core::rendering::select_graphics_queue(queue_families);

        auto device = core::rendering::create_device(physical_device, queue_index);
        volkLoadDevice(device);
        SDL_Log("Created logical device");

        auto queue = core::rendering::get_queue(device, queue_index);
        SDL_Log("Acquired queue");

        auto allocator = core::rendering::create_allocator(instance, physical_device, device);
        SDL_Log("Created VMA allocator");

        auto window       = world.get<plg::window>();
        auto surface      = core::rendering::get_surface(window, instance);
        auto surface_caps = core::rendering::get_surface_capabilities(physical_device, surface);
        SDL_Log("Acquired surface");

        auto swapchain = core::rendering::create_swapchain(device, surface, surface_caps);
        SDL_Log("Created swapchain");

        auto swapchain_images      = core::rendering::get_swapchain_images(device, swapchain);
        auto swapchain_image_views = std::vector<VkImageView>(swapchain_images.size());

        auto depth_fmt  = core::rendering::get_depth_format(physical_device);
        auto depth_img  = core::rendering::create_depth_image(allocator, depth_fmt);
        auto depth_view = core::rendering::create_depth_view(device, depth_img, depth_fmt);
    }

    CORE_RENDERING_EXPORT auto deinitialize(ecs_world_t* _) -> void {}
}
