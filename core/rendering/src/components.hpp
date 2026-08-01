#pragma once

#include <array>
#include <glm/mat4x4.hpp>
#include <vector>


namespace core::rendering {

struct transform {
    glm::mat4 transform = {};
};

struct mesh_geometry {
    uint32_t stride;
    std::vector<std::byte> vertices;
    std::vector<uint16_t> indices;
};

struct material_data {
    std::array<float, 4> color = {};
};

} // namespace core::rendering
