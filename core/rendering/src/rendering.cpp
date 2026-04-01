#include "rendering/export.hpp"

#include "components.hpp"
#include "initialization.hpp"
#include <SDL3/SDL.h>
#include <cstdint>
#include <cstring>
#include <flecs.h>
#include <glm/mat4x4.hpp>
#include <plg/sdk.hpp>
#include <tiny_obj_loader.h>
#include <volk.h>
#include <vulkan/vulkan.h>

constexpr std::uint32_t device_index     = 0;
constexpr std::uint32_t frames_in_flight = 2;

struct shader_data {
    glm::mat4 projection;
    glm::mat4 view;
    std::array<glm::mat4, 3> model;
    glm::vec4 light_pos    = {0.0f, -10.0f, 10.0f, 0.0f};
    std::uint32_t selected = 1;
};

struct shader_data_buffer {
    VmaAllocation allocation          = VK_NULL_HANDLE;
    VmaAllocationInfo allocation_info = {};
    VkBuffer buffer                   = VK_NULL_HANDLE;
    VkDeviceAddress address           = {};
};

extern "C" {
    CORE_RENDERING_EXPORT auto initialize(ecs_world_t* wld) -> void {
        SDL_Log("Initializing core rendering module");

        auto world = flecs::world{wld};
        world.component<core::rendering::vertex>();

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

        auto attrib    = tinyobj::attrib_t{};
        auto shapes    = std::vector<tinyobj::shape_t>();
        auto materials = std::vector<tinyobj::material_t>();
        if(!tinyobj::LoadObj(
               &attrib,
               &shapes,
               &materials,
               nullptr,
               nullptr,
               R"(/home/ad/dev/plg/core/rendering/assets/teapot.obj)"
           )) {
            SDL_Log("Failed to load obj");
        }

        SDL_Log("Loaded mesh");

        const VkDeviceSize index_count = shapes[0].mesh.indices.size();
        auto vertices                  = std::vector<core::rendering::vertex>();
        auto indices                   = std::vector<std::uint16_t>();

        for(auto& index: shapes.at(0).mesh.indices) {
            auto vert = core::rendering::vertex{
                .position =
                    {
                        attrib.vertices.at(index.vertex_index * 3),
                        -attrib.vertices.at(index.vertex_index * 3 + 1),
                        attrib.vertices.at(index.vertex_index * 3 + 2),
                    },
                .normal =
                    {
                        attrib.vertices.at(index.normal_index * 3),
                        -attrib.vertices.at(index.normal_index * 3 + 1),
                        attrib.vertices.at(index.normal_index * 3 + 2),
                    },
                .uv = {
                    attrib.texcoords.at(index.texcoord_index * 2),
                    1.0 - attrib.texcoords.at(index.texcoord_index * 2 + 1),
                },
            };

            vertices.emplace_back(vert);
            indices.emplace_back(indices.size());
        }

        VkDeviceSize vertex_buffer_size = (sizeof(core::rendering::vertex) * vertices.size());
        VkDeviceSize index_buffer_size  = (sizeof(std::uint16_t) * indices.size());
        auto buffer_create_info         = VkBufferCreateInfo{
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size  = vertex_buffer_size + index_buffer_size,
            .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        };

        auto alloc_info = VmaAllocationCreateInfo{
            .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
                   | VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT
                   | VMA_ALLOCATION_CREATE_MAPPED_BIT,
            .usage = VMA_MEMORY_USAGE_AUTO,
        };
        auto buffer_alloc_info = VmaAllocationInfo{};

        VkBuffer geometry_buffer        = nullptr;
        VmaAllocation buffer_allocation = nullptr;
        vmaCreateBuffer(
            allocator,
            &buffer_create_info,
            &alloc_info,
            &geometry_buffer,
            &buffer_allocation,
            &buffer_alloc_info
        );

        std::memcpy(buffer_alloc_info.pMappedData, vertices.data(), vertex_buffer_size);
        std::memcpy(
            ((char*)buffer_alloc_info.pMappedData) + vertex_buffer_size,
            vertices.data(),
            vertex_buffer_size
        );

        auto shader_data_buffers = std::array<shader_data_buffer, frames_in_flight>();
        // auto command_buffers     = std::array<VkCommandBuffer, frames_in_flight>();

        for(auto i = 0; i < frames_in_flight; i++) {
            auto buffer_ci = VkBufferCreateInfo{
                .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                .size  = sizeof(shader_data),
                .usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
            };

            auto alloc_ci = VmaAllocationCreateInfo{
                .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
                       | VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT
                       | VMA_ALLOCATION_CREATE_MAPPED_BIT,
                .usage = VMA_MEMORY_USAGE_AUTO,
            };

            vmaCreateBuffer(
                allocator,
                &buffer_ci,
                &alloc_ci,
                &shader_data_buffers.at(i).buffer,
                &shader_data_buffers.at(i).allocation,
                &shader_data_buffers.at(i).allocation_info
            );

            auto buffer_info = VkBufferDeviceAddressInfo{
                .sType  = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
                .buffer = shader_data_buffers.at(i).buffer,
            };
            shader_data_buffers.at(i).address = vkGetBufferDeviceAddress(device, &buffer_info);
        }

        auto semaphore_create_info = VkSemaphoreCreateInfo{
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        };

        auto fence_create_info = VkFenceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        };

        auto fences             = std::array<VkFence, frames_in_flight>();
        auto present_semaphores = std::array<VkSemaphore, frames_in_flight>();
        auto render_semaphores  = std::vector<VkSemaphore>(swapchain_images.size());

        for(auto i = 0; i < frames_in_flight; i++) {
            vkCreateFence(device, &fence_create_info, nullptr, &fences.at(i));
            vkCreateSemaphore(device, &semaphore_create_info, nullptr, &present_semaphores.at(i));
        }

        for(auto& semaphore: render_semaphores) {
            vkCreateSemaphore(device, &semaphore_create_info, nullptr, &semaphore);
        }

        SDL_Log("Created semaphores and fences");

        auto command_pool    = core::rendering::create_command_pool(device, queue_index);
        auto command_buffers = core::rendering::create_command_buffers(
            device, command_pool, frames_in_flight
        );

        SDL_Log("Command pool and buffers created");
    }

    CORE_RENDERING_EXPORT auto deinitialize(ecs_world_t* _) -> void {}
}
