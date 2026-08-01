#pragma once

#include "components.hpp"
#include <cstdint>
#include <vector>

namespace core::rendering {

constexpr auto generate_cube_mesh() {
    auto mesh_data = mesh{};

    constexpr float pos = 0.5f;
    constexpr float neg = -0.5f;

    struct vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

    // 24 Vertices: 6 faces * 4 vertices each (for unique normals)
    auto verts = std::array<vertex, 24>{
        {// Front Face (+Z) — Normal (0, 0, 1)
         {{neg, neg, pos}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
         {{pos, neg, pos}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
         {{pos, pos, pos}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
         {{neg, pos, pos}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

         // Back Face (-Z) — Normal (0, 0, -1)
         {{pos, neg, neg}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
         {{neg, neg, neg}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
         {{neg, pos, neg}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
         {{pos, pos, neg}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},

         // Top Face (+Y) — Normal (0, 1, 0)
         {{neg, pos, pos}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
         {{pos, pos, pos}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
         {{pos, pos, neg}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
         {{neg, pos, neg}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},

         // Bottom Face (-Y) — Normal (0, -1, 0)
         {{neg, neg, neg}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
         {{pos, neg, neg}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
         {{pos, neg, pos}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
         {{neg, neg, pos}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},

         // Right Face (+X) — Normal (1, 0, 0)
         {{pos, neg, pos}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
         {{pos, neg, neg}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
         {{pos, pos, neg}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
         {{pos, pos, pos}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

         // Left Face (-X) — Normal (-1, 0, 0)
         {{neg, neg, neg}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
         {{neg, neg, pos}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
         {{neg, pos, pos}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
         {{neg, pos, neg}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}
        },
    };

    auto bytes = verts.size() * sizeof(float);
    mesh_data.vertices.reserve(bytes);
    std::memcpy(mesh_data.vertices.data(), verts.data(), bytes);

    // 36 Indices: 6 faces * 2 triangles * 3 vertices (CCW Winding)
    uint16_t offset  = 0;
    size_t index_ptr = 0;

    mesh_data.indices = std::vector<uint16_t>(36);
    for(int face = 0; face < 6; ++face) {
        // First triangle
        mesh_data.indices[index_ptr++] = offset + 0;
        mesh_data.indices[index_ptr++] = offset + 1;
        mesh_data.indices[index_ptr++] = offset + 2;

        // Second triangle
        mesh_data.indices[index_ptr++] = offset + 0;
        mesh_data.indices[index_ptr++] = offset + 2;
        mesh_data.indices[index_ptr++] = offset + 3;

        offset += 4; // Move to next quad
    }

    return mesh_data;
}
} // namespace core::rendering
