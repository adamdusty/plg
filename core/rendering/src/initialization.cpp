#include "initialization.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <cstdint>

namespace core::rendering {

static inline auto check(VkResult res) -> void {
    if(res != VK_SUCCESS) {
        SDL_Log("Vulkan error");
    }
}

auto create_instance() -> VkInstance {
    auto app_info = VkApplicationInfo{
        .sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext            = nullptr,
        .pApplicationName = "test",
        .apiVersion       = VK_API_VERSION_1_3,
    };

    std::uint32_t instance_extension_count = 0;
    const auto* instance_extensions = SDL_Vulkan_GetInstanceExtensions(&instance_extension_count);

    auto create_info = VkInstanceCreateInfo{
        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo        = &app_info,
        .enabledExtensionCount   = instance_extension_count,
        .ppEnabledExtensionNames = instance_extensions,
    };

    VkInstance instance = nullptr;
    check(vkCreateInstance(&create_info, nullptr, &instance));

    return instance;
}

} // namespace core::rendering
