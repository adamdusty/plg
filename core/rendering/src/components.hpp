#pragma once

#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <tiny_obj_loader.h>
#include <vector>

namespace core::rendering {

struct vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

struct model {
    tinyobj::attrib_t attrib;
    std::vector<vertex> vertices;
    std::vector<std::uint16_t> indices;
};

} // namespace core::rendering
