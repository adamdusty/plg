#pragma once

#include "allocation.hpp"
#include "configuration.hpp"
#include <SDL3/SDL.h>
#include <array>
#include <expected>
#include <spdlog/spdlog.h>
#include <string>

namespace core::rendering {

struct draw_command {
    uint32_t index_count   = 0;
    uint32_t first_index   = 0;
    uint32_t vertex_offset = 0;

    // glm::mat4 transform     = {};
    // material_data* mat_data = nullptr;

    // draw_command(
    //     uint32_t icnt, uint32_t fst_idx, uint32_t vert_offset, glm::mat4 trns, material_data* mat
    // ) :
    //     index_count(icnt), first_index(fst_idx), vertex_offset(vert_offset), transform(trns),
    //     mat_data(mat) {}
};

struct frame_resources {
    SDL_GPUCommandBuffer* cmd = nullptr;
    std::vector<draw_command> draw_commands;
};

struct application_resources {
    SDL_Window* window                     = nullptr;
    SDL_GPUDevice* device                  = nullptr;
    std::shared_ptr<spdlog::logger> logger = nullptr;
    geometry_allocator allocator           = {};

    std::array<frame_resources, frames_in_flight> frames = {};
    uint8_t current_frame                                = 0;
};

auto create_application_resources(SDL_Window* win, bool debug = true, const char* driver = nullptr)
    -> std::expected<application_resources, std::string>;

} // namespace core::rendering
