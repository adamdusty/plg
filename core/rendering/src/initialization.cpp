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

auto get_surface_capabilities(VkPhysicalDevice dev, VkSurfaceKHR surf) -> VkSurfaceCapabilitiesKHR {
    VkSurfaceCapabilitiesKHR caps;
    check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dev, surf, &caps));

    return caps;
}

auto create_swapchain(VkDevice dev, VkSurfaceKHR surf, VkSurfaceCapabilitiesKHR caps)
    -> VkSwapchainKHR {
    auto image_format = VK_FORMAT_B8G8R8A8_SRGB;
    auto create_info  = VkSwapchainCreateInfoKHR{
        .sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface          = surf,
        .minImageCount    = caps.minImageCount,
        .imageFormat      = image_format,
        .imageColorSpace  = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        .imageExtent      = {.width = 640, .height = 480},
        .imageArrayLayers = 1,
        .imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform     = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode      = VK_PRESENT_MODE_FIFO_KHR,
        .oldSwapchain     = nullptr,
    };

    VkSwapchainKHR swapchain = nullptr;
    check(vkCreateSwapchainKHR(dev, &create_info, nullptr, &swapchain));

    return swapchain;
}

auto get_swapchain_images(VkDevice dev, VkSwapchainKHR swapchain) -> std::vector<VkImage> {
    std::uint32_t count = 0;
    check(vkGetSwapchainImagesKHR(dev, swapchain, &count, nullptr));
    auto images = std::vector<VkImage>(count);
    check(vkGetSwapchainImagesKHR(dev, swapchain, &count, images.data()));

    return images;
}

auto get_depth_format(VkPhysicalDevice dev) -> VkFormat {
    auto formats = std::vector<VkFormat>{
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT,
    };

    auto format = VK_FORMAT_UNDEFINED;

    for(auto fmt: formats) {
        auto props = VkFormatProperties2{.sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2};
        vkGetPhysicalDeviceFormatProperties2(dev, fmt, &props);
        if((props.formatProperties.optimalTilingFeatures
            & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
           != 0U) {
            format = fmt;
            break;
        }
    }

    return format;
}

auto create_depth_image(VmaAllocator allocator, VkFormat format) -> VkImage {
    auto create_info = VkImageCreateInfo{
        .sType     = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .format    = format,
        .extent{.width = 640, .height = 480, .depth = 1},
        .mipLevels     = 1,
        .arrayLayers   = 1,
        .samples       = VK_SAMPLE_COUNT_1_BIT,
        .tiling        = VK_IMAGE_TILING_OPTIMAL,
        .usage         = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };

    auto alloc_info = VmaAllocationCreateInfo{
        .flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
        .usage = VMA_MEMORY_USAGE_AUTO,
    };

    VmaAllocation image_allocation = nullptr;
    VkImage image                  = nullptr;
    check(vmaCreateImage(allocator, &create_info, &alloc_info, &image, &image_allocation, nullptr));

    return image;
}

auto create_depth_view(VkDevice dev, VkImage image, VkFormat format) -> VkImageView {
    auto create_info = VkImageViewCreateInfo{
        .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image            = image,
        .viewType         = VK_IMAGE_VIEW_TYPE_2D,
        .format           = format,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
            .levelCount = 1,
            .layerCount = 1,
        },
    };

    VkImageView view = nullptr;
    check(vkCreateImageView(dev, &create_info, nullptr, &view));

    return view;
}

auto create_command_pool(VkDevice dev, std::uint32_t qfi) -> VkCommandPool {
    auto create_info = VkCommandPoolCreateInfo{
        .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = qfi,
    };

    VkCommandPool pool = nullptr;
    check(vkCreateCommandPool(dev, &create_info, nullptr, &pool));

    return pool;
}

auto create_command_buffers(VkDevice dev, VkCommandPool pool, std::uint32_t count)
    -> std::vector<VkCommandBuffer> {

    auto create_info = VkCommandBufferAllocateInfo{
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = pool,
        .commandBufferCount = count,
    };

    auto buffers = std::vector<VkCommandBuffer>(count);
    check(vkAllocateCommandBuffers(dev, &create_info, buffers.data()));

    return buffers;
}

} // namespace core::rendering
