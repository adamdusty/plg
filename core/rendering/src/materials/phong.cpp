#include "materials/phong.hpp"

#include "assets.hpp"

namespace core::rendering::materials {

auto phong(const rendering_context& context, const shader_compiler_context& compiler)
    -> std::expected<SDL_GPUGraphicsPipeline*, std::string> {

    auto res = create_gpu_shader_pair(
        context.device, compiler, "phong", "memory://phong.slang", shader
    );
    if(!res) {
        return std::unexpected(res.error());
    }
    auto [vert, frag] = res.value();

    auto info            = phong_ci;
    info.vertex_shader   = vert;
    info.fragment_shader = frag;

    auto format            = SDL_GetGPUSwapchainTextureFormat(context.device, context.window);
    auto color_target_desc = SDL_GPUColorTargetDescription{
        .format      = format,
        .blend_state = {.color_write_mask = 0xF},
    };

    info.target_info.color_target_descriptions = &color_target_desc;
    info.target_info.num_color_targets         = 1;

    auto* pipeline = SDL_CreateGPUGraphicsPipeline(context.device, &phong_ci);
    if(pipeline == nullptr) {
        return std::unexpected(SDL_GetError());
    }

    return pipeline;
}

} // namespace core::rendering::materials
