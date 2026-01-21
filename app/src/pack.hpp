#pragma once

#include "version.hpp"
#include <expected>
#include <string>
#include <vector>

namespace plg {

struct pack_dependency {
    std::string nspace;
    std::string name;
    version version;

    auto operator==(const pack_dependency&) const -> bool = default;
};

struct pack {
    std::string nspace;
    std::string name;
    std::string description;
    version version;
    std::vector<pack_dependency> dependencies;

    auto operator==(const pack&) const -> bool = default;
};

struct pack_find_error {
    std::string message;
};

using pack_find_result = std::expected<pack, pack_find_error>;
auto find_packs(const std::string& dir)
    -> std::expected<std::vector<pack_find_result>, std::string>;

} // namespace plg
