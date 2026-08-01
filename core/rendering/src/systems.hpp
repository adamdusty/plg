#pragma once

#include <flecs.h>

namespace core::rendering {

auto generate_draw_commands(flecs::iter& iter) -> void;
auto render_scene(flecs::iter& iter) -> void;

} // namespace core::rendering
