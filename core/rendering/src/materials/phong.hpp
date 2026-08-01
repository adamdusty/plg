#pragma once

#include "components.hpp"
#include <SDL3/SDL.h>
#include <expected>
#include <string>

namespace core::rendering::materials {

constexpr auto* shader = "";

constexpr auto phong_ci = SDL_GPUGraphicsPipelineCreateInfo{
    .vertex_shader      = nullptr,
    .fragment_shader    = nullptr,
    .vertex_input_state = {},
    .primitive_type     = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
    .rasterizer_state =
        {
            .fill_mode                  = SDL_GPU_FILLMODE_FILL,
            .cull_mode                  = SDL_GPU_CULLMODE_BACK,
            .front_face                 = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE,
            .depth_bias_constant_factor = 0.0f,
            .depth_bias_clamp           = 0.0f,
            .depth_bias_slope_factor    = 0.0f,
            .enable_depth_bias          = false,
            .enable_depth_clip          = false,
            .padding1                   = 0,
            .padding2                   = 0,
        },
    .multisample_state =
        {
            .sample_count             = SDL_GPU_SAMPLECOUNT_1,
            .sample_mask              = 0,
            .enable_mask              = false,
            .enable_alpha_to_coverage = false,
            .padding2                 = 0,
            .padding3                 = 0,
        },
    .depth_stencil_state =
        {
            .compare_op          = SDL_GPU_COMPAREOP_LESS,
            .enable_depth_test   = true,
            .enable_depth_write  = true,
            .enable_stencil_test = false,
        },
    .target_info =
        {
            .color_target_descriptions = nullptr,
            .num_color_targets         = 0,
            .depth_stencil_format      = SDL_GPU_TEXTUREFORMAT_INVALID,
            .has_depth_stencil_target  = true,
        },
    .props = 0,
};

auto phong(const rendering_context& context, const shader_compiler_context& compiler)
    -> std::expected<SDL_GPUGraphicsPipeline*, std::string>;

} // namespace core::rendering::materials
