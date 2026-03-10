#include "initialization.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <cstdint>
#include <vector>
#include <vk_mem_alloc.h>
#include <volk.h>

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

auto get_available_devices(VkInstance instance) -> std::vector<VkPhysicalDevice> {
    std::uint32_t device_count = 0;
    check(vkEnumeratePhysicalDevices(instance, &device_count, nullptr));
    auto devices = std::vector<VkPhysicalDevice>(device_count);
    check(vkEnumeratePhysicalDevices(instance, &device_count, devices.data()));

    return devices;
}

auto get_queue_families(VkPhysicalDevice device) -> std::vector<VkQueueFamilyProperties> {
    std::uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
    auto queue_families = std::vector<VkQueueFamilyProperties>(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

    return queue_families;
}

auto select_graphics_queue(const std::vector<VkQueueFamilyProperties>& families) -> std::uint32_t {
    std::uint32_t index = 0;
    for(std::uint32_t i = 0; i < families.size(); i++) {
        if((families.at(i).queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0U) {
            index = i;
            break;
        }
    }

    return index;
}

auto create_device(VkPhysicalDevice physical_device, std::uint32_t family) -> VkDevice {
    auto extensions = std::vector<const char*>{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    auto queue_priorities  = 1.0F;
    auto queue_create_info = VkDeviceQueueCreateInfo{
        .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = family,
        .queueCount       = 1,
        .pQueuePriorities = &queue_priorities,
    };

    auto enabled12_features = VkPhysicalDeviceVulkan12Features{
        .sType              = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
        .descriptorIndexing = VK_TRUE,
        .shaderSampledImageArrayNonUniformIndexing = VK_TRUE,
        .descriptorBindingVariableDescriptorCount  = VK_TRUE,
        .runtimeDescriptorArray                    = VK_TRUE,
        .bufferDeviceAddress                       = VK_TRUE,
    };

    auto enabled13_features = VkPhysicalDeviceVulkan13Features{
        .sType            = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
        .pNext            = &enabled12_features,
        .synchronization2 = VK_TRUE,
        .dynamicRendering = VK_TRUE,
    };

    auto enabled_features = VkPhysicalDeviceFeatures{
        .samplerAnisotropy = VK_TRUE,
    };

    auto create_info = VkDeviceCreateInfo{
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext                   = &enabled13_features,
        .queueCreateInfoCount    = 1,
        .pQueueCreateInfos       = &queue_create_info,
        .enabledExtensionCount   = static_cast<std::uint32_t>(extensions.size()),
        .ppEnabledExtensionNames = extensions.data(),
        .pEnabledFeatures        = &enabled_features,
    };

    VkDevice device = nullptr;
    check(vkCreateDevice(physical_device, &create_info, nullptr, &device));

    return device;
}

auto get_queue(VkDevice device, std::uint32_t index) -> VkQueue {
    VkQueue queue = nullptr;
    vkGetDeviceQueue(device, index, 0, &queue);
    return queue;
}

auto create_allocator(VkInstance inst, VkPhysicalDevice phys_device, VkDevice dev) -> VmaAllocator {
    auto functions = VmaVulkanFunctions{
        .vkGetInstanceProcAddr = vkGetInstanceProcAddr,
        .vkGetDeviceProcAddr   = vkGetDeviceProcAddr,
        .vkCreateImage         = vkCreateImage,
    };

    auto create_info = VmaAllocatorCreateInfo{
        .flags            = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
        .physicalDevice   = phys_device,
        .device           = dev,
        .pVulkanFunctions = &functions,
        .instance         = inst,
    };

    VmaAllocator allocator = nullptr;
    check(vmaCreateAllocator(&create_info, &allocator));

    return allocator;
}

auto get_surface(SDL_Window* window, VkInstance instance) -> VkSurfaceKHR {
    VkSurfaceKHR surface = nullptr;
    if(!SDL_Vulkan_CreateSurface(window, instance, nullptr, &surface)) {
        SDL_Log("Vulkan error");
    }

    return surface;
}

} // namespace core::rendering
