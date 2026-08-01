#include "initialization.hpp"

#include "allocator.hpp"
#include "components.hpp"
#include <SDL3/SDL.h>
#include <cassert>
#include <expected>
#include <flecs.h>
#include <slang.h>

namespace core::rendering {

auto register_components(flecs::world& world) -> void {
    world.component<core::rendering::mesh>();
    world.component<core::rendering::mesh_allocation>();
    world.component<core::rendering::material_data>();
    world.component<core::rendering::rendering_context>();
}

auto create_context(SDL_Window* window, bool debug) -> rendering_context {
    auto context   = rendering_context{};
    context.window = window;

    context.device = SDL_CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL, debug, nullptr
    );

    if(context.device == nullptr) {
        SDL_Log("%s", SDL_GetError());
    } else {
        SDL_Log("Created gpu device");
    }

    if(!SDL_ClaimWindowForGPUDevice(context.device, window)) {
        SDL_Log("%s", SDL_GetError());
    } else {
        SDL_Log("GPU Device claimed window");
    }

    return context;
}

auto create_shader_compiler_context(SDL_GPUDevice* gpu)
    -> std::expected<shader_compiler_context, std::string> {
    auto formats = SDL_GetGPUShaderFormats(gpu);
    auto context = shader_compiler_context{};

    auto global_desc = SlangGlobalSessionDesc{};
    auto res         = slang::createGlobalSession(&global_desc, &context.global_session);

    if(SLANG_FAILED(res) || context.global_session == nullptr) {
        return std::unexpected("Failed to create slang global session");
    }

    auto target_desc = slang::TargetDesc{};
    if((formats & SDL_GPU_SHADERFORMAT_SPIRV) != 0) {
        target_desc.format    = SLANG_SPIRV;
        context.shader_format = SDL_GPU_SHADERFORMAT_SPIRV;
    } else if((formats & SDL_GPU_SHADERFORMAT_DXIL) != 0) {
        target_desc.format    = SLANG_DXIL;
        context.shader_format = SDL_GPU_SHADERFORMAT_DXIL;
    } else if((formats & SDL_GPU_SHADERFORMAT_MSL) != 0) {
        target_desc.format    = SLANG_METAL;
        context.shader_format = SDL_GPU_SHADERFORMAT_MSL;
    } else if((formats & SDL_GPU_SHADERFORMAT_METALLIB) != 0) {
        target_desc.format    = SLANG_METAL;
        context.shader_format = SDL_GPU_SHADERFORMAT_METALLIB;
    } else {
        return std::unexpected("Unsupported shader format");
    }

    SDL_Log("SHADER FORMAT: %d", target_desc.format);

    auto session_desc = slang::SessionDesc{
        .targets     = &target_desc,
        .targetCount = 1,
    };
    res = context.global_session->createSession(session_desc, &context.session);
    if(SLANG_FAILED(res) || context.session == nullptr) {
        return std::unexpected("Failed to create slang session");
    }

    return context;
}

} // namespace core::rendering
