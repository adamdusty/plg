#pragma once

#include "dependency.hpp"
#include "version.hpp"
#include <expected>
#include <optional>
#include <string>
#include <vector>

namespace plg {

struct pack {
    std::string nspace;
    std::string name;
    version version;
    std::optional<std::string> short_description;
    std::optional<std::string> long_description;
    std::vector<dependency> dependencies;

    auto operator==(const pack&) const -> bool = default;
};

auto find_packs(const std::string& dir) -> std::expected<std::vector<pack>, std::string>;

} // namespace plg
