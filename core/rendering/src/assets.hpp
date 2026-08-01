#pragma once

#include "components.hpp"
#include <SDL3/SDL.h>
#include <expected>
#include <ktx.h>
#include <string>
#include <tiny_obj_loader.h>
#include <vector>

namespace core::rendering {

auto compile_shader(
    const shader_compiler_context& context,
    const std::string& name,
    const std::string& path,
    const std::string& src,
    const std::string& entry_point
) -> std::expected<std::vector<uint8_t>, std::string>;

auto create_gpu_shader(
    SDL_GPUDevice* device,
    const shader_compiler_context& context,
    const std::string& name,
    const std::string& path,
    const std::string& src,
    const std::string& entry_point,
    SDL_GPUShaderStage stage
) -> std::expected<SDL_GPUShader*, std::string>;

auto create_gpu_shader_pair(
    SDL_GPUDevice* device,
    const shader_compiler_context& context,
    const std::string& name,
    const std::string& path,
    const std::string& src
) -> std::expected<std::pair<SDL_GPUShader*, SDL_GPUShader*>, std::string>;

} // namespace core::rendering
