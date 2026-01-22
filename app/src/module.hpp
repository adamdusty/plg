#pragma once

#include "dependency.hpp"
#include "version.hpp"
#include <expected>
#include <filesystem>
#include <istream>
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

    static auto parse(std::istream& toml) -> std::expected<module_manifest, std::string>;
};

struct module {
    std::string nspace;
    std::string name;
    std::string description;
    version version;
    std::filesystem::path directory;

    auto operator==(const module&) const -> bool = default;
};

auto find_modules(const std::string& dir) -> std::expected<std::vector<module>, std::string>;

} // namespace plg
