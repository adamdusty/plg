#include "assets.hpp"

#include <SDL3/SDL.h>

namespace core::rendering {

auto load_obj(const std::string& path) -> model {

    auto attrib    = tinyobj::attrib_t{};
    auto shapes    = std::vector<tinyobj::shape_t>();
    auto materials = std::vector<tinyobj::material_t>();
    if(!tinyobj::LoadObj(&attrib, &shapes, &materials, nullptr, nullptr, path.c_str())) {
        SDL_Log("Failed to load obj: %s", path.c_str());
        return {};
    }

    auto mod = model{};

    for(auto& index: shapes.at(0).mesh.indices) {
        auto vert = core::rendering::vertex{
            .position =
                {
                    attrib.vertices.at(index.vertex_index * 3),
                    -attrib.vertices.at(index.vertex_index * 3 + 1),
                    attrib.vertices.at(index.vertex_index * 3 + 2),
                },
            .normal =
                {
                    attrib.vertices.at(index.normal_index * 3),
                    -attrib.vertices.at(index.normal_index * 3 + 1),
                    attrib.vertices.at(index.normal_index * 3 + 2),
                },
            .uv = {
                attrib.texcoords.at(index.texcoord_index * 2),
                1.0 - attrib.texcoords.at(index.texcoord_index * 2 + 1),
            },
        };

        mod.vertices.emplace_back(vert);
        mod.indices.emplace_back(mod.indices.size());
    }

    return mod;
}

} // namespace core::rendering
