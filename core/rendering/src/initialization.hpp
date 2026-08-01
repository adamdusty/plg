#pragma once

#include "components.hpp"
#include <SDL3/SDL.h>
#include <expected>
#include <flecs.h>
#include <slang.h>

namespace core::rendering {

auto register_components(flecs::world& world) -> void;
auto create_context(SDL_Window* window, bool debug = true) -> rendering_context;
auto create_shader_compiler_context(SDL_GPUDevice* gpu)
    -> std::expected<shader_compiler_context, std::string>;

} // namespace core::rendering
