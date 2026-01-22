#pragma once

#include "dependency.hpp"
#include "version.hpp"
#include <expected>
#include <string>
#include <vector>

namespace plg {

struct pack {
    std::string nspace;
    std::string name;
    std::string description;
    version version;
    std::vector<dependency> dependencies;

    auto operator==(const pack&) const -> bool = default;
};

auto find_packs(const std::string& dir) -> std::expected<std::vector<pack>, std::string>;

} // namespace plg
