#include "systems.hpp"

#include "components.hpp"
#include "rendering.hpp"
#include <SDL3/SDL.h>
#include <flecs.h>
#include <glm/mat4x4.hpp>

namespace core::rendering {

struct alignas(16) vertex_push_constants {
    glm::mat4 transform;
    uint32_t vertex_offset;
    uint32_t pad0;
    uint32_t pad1;
    uint32_t pad2;

    vertex_push_constants(glm::mat4 trns, uint32_t offset) :
        transform(trns), vertex_offset(offset), pad0(), pad1(), pad2() {}
};

auto generate_draw_commands(flecs::iter& iter) -> void {
    // auto& app   = iter.world().get_mut<application_resources>();
    // auto& frame = app.frames.at(app.current_frame++);
    // frame.draw_commands.emplace_back();
    // frame.draw_commands.clear();

    // while(iter.next()) {
    //     const auto& meshes     = iter.field<const mesh_geometry>(0);
    //     const auto& materials  = iter.field<const material_data>(1);
    //     const auto& transforms = iter.field<const transform>(2);

    //     for(auto idx: iter) {
    //         auto trns = transforms[idx];
    //         auto mesh = meshes[idx];
    //         auto mat  = materials[idx];

    //         if(mat.material->pipeline == nullptr) {
    //             continue;
    //         }

    //         context.render_queue.emplace_back(
    //             mesh.index_count, mesh.first_index, mesh.vertex_offset, trns.data, &mat
    //         );
    //     }
    // }
}

auto render_scene(flecs::iter& iter) -> void {
    // auto context = iter.world().get<rendering_context>();

    // // TODO: Sort draw queue by material ID
    // auto draw_commands = std::vector<draw_command>();

    // // Get new command buffer for frame
    // SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(context.device);
    // if(cmd == nullptr) {
    //     iter.fini();
    //     return;
    // }

    // // Get swapchain texture for frame
    // SDL_GPUTexture* swapchain_texture = nullptr;
    // SDL_WaitAndAcquireGPUSwapchainTexture(
    //     cmd, context.window, &swapchain_texture, nullptr, nullptr
    // );
    // if(swapchain_texture == nullptr) {
    //     iter.fini();
    //     return;
    // }

    // // Set up target for frame
    // auto ctgt = SDL_GPUColorTargetInfo{
    //     .texture     = swapchain_texture,
    //     .clear_color = SDL_FColor{0.05f, 0.05f, 0.07f, 1.0f},
    //     .load_op     = SDL_GPU_LOADOP_CLEAR,
    //     .store_op    = SDL_GPU_STOREOP_STORE,
    // };

    // // Begin render pass
    // auto* pass = SDL_BeginGPURenderPass(cmd, &ctgt, 1, nullptr);
    // if(pass == nullptr) {
    //     iter.fini();
    //     return;
    // }

    // // Bind geometry buffers
    // SDL_BindGPUVertexStorageBuffers(pass, 0, &context.geometry_storage_buffer, 1);
    // auto index_binding = SDL_GPUBufferBinding{.buffer = context.index_storage_buffer, .offset =
    // 0}; SDL_BindGPUIndexBuffer(pass, &index_binding, SDL_GPU_INDEXELEMENTSIZE_16BIT);

    // SDL_GPUGraphicsPipeline* active_pipeline = nullptr;
    // material_data* active_material           = nullptr;

    // // Iterate over draw commands
    // for(auto& draw: draw_commands) {
    //     if(draw.mat_data->material->pipeline == nullptr || draw.mat_data == nullptr) {
    //         continue;
    //     }

    //     // Update pipeline if different
    //     if(draw.mat_data->material->pipeline != active_pipeline) {
    //         active_pipeline = draw.mat_data->material->pipeline;
    //         SDL_BindGPUGraphicsPipeline(pass, active_pipeline);
    //         active_material = nullptr;
    //     }

    //     // Update material data if different
    //     if(active_material != draw.mat_data) {
    //         active_material = draw.mat_data;

    //         for(const auto& tex: active_material->texture_bindings) {
    //             auto binding = SDL_GPUTextureSamplerBinding{
    //                 .texture = tex.texture,
    //                 .sampler = tex.sampler,
    //             };

    //             SDL_BindGPUFragmentSamplers(pass, tex.slot, &binding, 1);
    //         }
    //     }

    //     // Push frag uniform data if necessary
    //     if(!active_material->frag_uniform_data.empty()) {
    //         SDL_PushGPUFragmentUniformData(
    //             cmd,
    //             0,
    //             active_material->frag_uniform_data.data(),
    //             static_cast<uint32_t>(active_material->frag_uniform_data.size())
    //         );
    //     }

    //     // Send push constants to GPU (vertex offset and object transform)
    //     auto push = vertex_push_constants(draw.transform, draw.vertex_offset);
    //     SDL_PushGPUVertexUniformData(cmd, 0, &push, sizeof(vertex_push_constants));

    //     // Draw
    //     SDL_DrawGPUIndexedPrimitives(pass, draw.index_count, 1, draw.first_index, 0, 0);
    // }

    // SDL_EndGPURenderPass(pass);

    // SDL_SubmitGPUCommandBuffer(cmd);
}

} // namespace core::rendering
