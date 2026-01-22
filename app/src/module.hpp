#pragma once

#include "dependency.hpp"
#include "version.hpp"
#include <expected>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace plg {

struct module_manifest {
    std::string nspace;
    std::string name;
    version version;
    std::optional<std::string> short_description;
    std::optional<std::string> long_description;
    std::vector<dependency> dependencies;

    auto operator==(const module_manifest&) const -> bool = default;
};

struct module {
    module_manifest manifest;
    std::filesystem::path directory;

    auto operator==(const module&) const -> bool = default;
};

auto find_modules(const std::string& dir) -> std::expected<std::vector<module>, std::string>;

} // namespace plg
