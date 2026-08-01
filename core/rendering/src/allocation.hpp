#pragma once

#include "components.hpp"
#include "gpu.hpp"
#include <SDL3/SDL.h>
#include <expected>
#include <unordered_map>
#include <vector>

namespace core::rendering {

using mesh_id = uint32_t;

struct mesh_allocation {
    uint32_t vertex_offset_bytes;
    uint32_t vertex_count;
    uint32_t stride;
    uint32_t index_offset_bytes;
    uint32_t index_count;
};

struct geometry_allocator {
    SDL_GPUBuffer* vertex_buffer = nullptr;
    SDL_GPUBuffer* index_buffer  = nullptr;

    uint32_t current_vertex_offset = 0;
    uint32_t current_index_offset  = 0;

    std::unordered_map<mesh_id, mesh_allocation> meshes;

    auto allocate_mesh(SDL_GPUDevice* dev, SDL_GPUCommandBuffer* cmd, const mesh_geometry& mesh)
        -> std::expected<mesh_allocation, std::string> {

        if(mesh.stride == 0) {
            return std::unexpected("Mesh stride cannot be 0");
        }

        size_t vertex_bytes = mesh.vertices.size();
        size_t index_bytes  = mesh.indices.size() * sizeof(uint16_t);

        if(vertex_buffer == nullptr) {
            return std::unexpected("Geometry vertex buffer is null");
        }

        if(index_buffer == nullptr) {
            return std::unexpected("Geometry index buffer is null");
        }

        // Align vertex offset to 4 bytes (Required by GPU storage buffers)
        uint32_t aligned_vertex_offset = (current_vertex_offset + 3) & ~3u;
        uint32_t aligned_index_offset  = (current_index_offset + 1) & ~1u;

        mesh_allocation alloc{};
        alloc.stride              = mesh.stride;
        alloc.vertex_offset_bytes = aligned_vertex_offset;
        alloc.vertex_count        = static_cast<uint32_t>(vertex_bytes / mesh.stride);
        alloc.index_offset_bytes  = aligned_index_offset;
        alloc.index_count         = static_cast<uint32_t>(mesh.indices.size());

        if(vertex_bytes > 0) {
            auto vertex_upload_result = upload_to_buffer(
                dev, cmd, vertex_buffer, mesh.vertices.data(), vertex_bytes, aligned_vertex_offset
            );
            if(!vertex_upload_result) {
                return std::unexpected("Vertex upload failed: " + vertex_upload_result.error());
            }
            current_vertex_offset = aligned_vertex_offset + static_cast<uint32_t>(vertex_bytes);
        }

        if(index_bytes > 0) {
            auto index_upload_result = upload_to_buffer(
                dev, cmd, index_buffer, mesh.indices.data(), index_bytes, aligned_index_offset
            );
            if(!index_upload_result) {
                return std::unexpected("Index upload failed: " + index_upload_result.error());
            }
            current_index_offset = aligned_index_offset + static_cast<uint32_t>(index_bytes);
        }

        return alloc;
    }
};

// class geometry_allocator {
// public:
//     static auto create(SDL_GPUDevice* device, uint32_t size, uint32_t max_indices)
//         -> std::expected<geometry_allocator, std::string> {
//         SDL_GPUBufferCreateInfo vertex_buffer_ci = {
//             .usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ, // Storage buffer for vertex
//             pulling .size  = size, .props = 0,
//         };
//         auto* vertex_buffer = SDL_CreateGPUBuffer(device, &vertex_buffer_ci);

//         SDL_GPUBufferCreateInfo indexInfo = {
//             .usage = SDL_GPU_BUFFERUSAGE_INDEX,
//             .size  = static_cast<uint32_t>(sizeof(uint16_t) * max_indices),
//             .props = 0,
//         };
//         auto* index_buffer = SDL_CreateGPUBuffer(device, &indexInfo);

//         if(vertex_buffer == nullptr || index_buffer == nullptr) {
//             return std::unexpected(SDL_GetError());
//         }

//         return geometry_allocator(device, vertex_buffer, index_buffer);
//     }

//     auto upload_mesh(
//         SDL_GPUCommandBuffer* cmd,
//         const std::vector<std::byte>& verts,
//         const std::vector<uint16_t>& ind
//     ) -> mesh_allocation {
//         auto alloc = mesh_allocation{
//             .index_count   = static_cast<uint32_t>(ind.size()),
//             .first_index   = current_index_offset,
//             .vertex_offset = current_vertex_offset,
//         };

//         uint32_t vert_offset  = alloc.vertex_offset + static_cast<uint32_t>(verts.size());
//         uint32_t index_offset = alloc.first_index * sizeof(uint16_t);

//         auto res = upload_to_buffer(device, cmd, vertices, verts.data(), verts.size(),
//         vert_offset); if(!res.has_value()) {
//             SDL_Log("%s", res.error().c_str());
//         }
//         res = upload_to_buffer(device, cmd, indices, ind.data(), ind.size(), index_offset);
//         if(!res.has_value()) {
//             SDL_Log("%s", res.error().c_str());
//         }

//         current_vertex_offset += static_cast<uint32_t>(verts.size());
//         current_index_offset += static_cast<uint32_t>(ind.size());

//         return alloc;
//     }

//     [[nodiscard]] constexpr auto vertex_buffer() const -> SDL_GPUBuffer* { return vertices; }
//     [[nodiscard]] constexpr auto index_buffer() const -> SDL_GPUBuffer* { return vertices; }

// private:
//     SDL_GPUDevice* device   = nullptr;
//     SDL_GPUBuffer* vertices = nullptr;
//     SDL_GPUBuffer* indices  = nullptr;

//     uint32_t current_vertex_offset = 0; // Monotonically increasing baseVertex counter
//     uint32_t current_index_offset  = 0; // Monotonically increasing firstIndex counter

//     geometry_allocator(SDL_GPUDevice* dev, SDL_GPUBuffer* vert, SDL_GPUBuffer* ind) :
//         device(dev), vertices(vert), indices(ind) {}
// };
} // namespace core::rendering
