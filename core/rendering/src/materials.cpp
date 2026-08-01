#include "materials.hpp"

#include "assets.hpp"
#include "components.hpp"
#include <SDL3/SDL.h>
#include <array>
#include <format>

namespace core::rendering {

auto create_phong_pipeline(
    const rendering_context& context,
    SDL_GPUShader* vert_shader,
    SDL_GPUShader* frag_shader,
    SDL_GPUTextureFormat depth_fmt
) -> SDL_GPUGraphicsPipeline* {
    auto vert_attribs = std::array<SDL_GPUVertexAttribute, 3>{
        {{
             .location    = 0,
             .buffer_slot = 0,
             .format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
             .offset      = 0,
         },
         {
             .location    = 1,
             .buffer_slot = 0,
             .format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
             .offset      = sizeof(float) * 3,
         },
         {
             .location    = 2,
             .buffer_slot = 0,
             .format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
             .offset      = sizeof(float) * 6,
         }}
    };

    auto vert_buffer_desc = SDL_GPUVertexBufferDescription{
        .slot               = 0,
        .pitch              = sizeof(float) * 8,
        .input_rate         = SDL_GPU_VERTEXINPUTRATE_VERTEX,
        .instance_step_rate = 0,
    };

    auto swapchain_fmt = SDL_GetGPUSwapchainTextureFormat(context.device, context.window);
    auto color_targets = std::array<SDL_GPUColorTargetDescription, 1>{{
        {
            .format = swapchain_fmt,
            .blend_state =
                {
                    .src_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
                    .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ZERO,
                    .color_blend_op        = SDL_GPU_BLENDOP_ADD,
                    .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
                    .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ZERO,
                    .alpha_blend_op        = SDL_GPU_BLENDOP_ADD,
                    .color_write_mask      = SDL_GPU_COLORCOMPONENT_R | SDL_GPU_COLORCOMPONENT_G
                                      | SDL_GPU_COLORCOMPONENT_B | SDL_GPU_COLORCOMPONENT_A,
                    .enable_blend            = false,
                    .enable_color_write_mask = true,
                },
        },
    }};

    auto create_info = SDL_GPUGraphicsPipelineCreateInfo{
        .vertex_shader   = vert_shader,
        .fragment_shader = frag_shader,
        .vertex_input_state =
            {
                .vertex_buffer_descriptions = &vert_buffer_desc,
                .num_vertex_buffers         = 1,
                .vertex_attributes          = vert_attribs.data(),
                .num_vertex_attributes      = static_cast<Uint32>(vert_attribs.size()),
            },
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .rasterizer_state =
            {
                .fill_mode                  = SDL_GPU_FILLMODE_FILL,
                .cull_mode                  = SDL_GPU_CULLMODE_BACK,
                .front_face                 = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE,
                .depth_bias_constant_factor = 0.0f,
                .depth_bias_clamp           = 0.0f,
                .depth_bias_slope_factor    = 0.0f,
                .enable_depth_bias          = false,
                .enable_depth_clip          = true,
            },
        .multisample_state =
            {
                .sample_count             = SDL_GPU_SAMPLECOUNT_1,
                .sample_mask              = 0,
                .enable_mask              = false,
                .enable_alpha_to_coverage = false,
            },
        .depth_stencil_state =
            {
                .compare_op          = SDL_GPU_COMPAREOP_LESS,
                .back_stencil_state  = {},
                .front_stencil_state = {},
                .compare_mask        = 0,
                .write_mask          = 0,
                .enable_depth_test   = true,
                .enable_depth_write  = true,
                .enable_stencil_test = false,
            },
        .target_info =
            {
                .color_target_descriptions = color_targets.data(),
                .num_color_targets         = static_cast<Uint32>(color_targets.size()),
                .depth_stencil_format      = depth_fmt,
                .has_depth_stencil_target  = true,
            },
        .props = 0,
    };

    return SDL_CreateGPUGraphicsPipeline(context.device, &create_info);
}

} // namespace core::rendering
