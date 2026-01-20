#pragma once

#include <expected>
#include <istream>
#include <string>
#include <vector>

namespace plg {

struct module_manifest {
    std::string name;
    std::string description;

    auto operator==(const module_manifest&) const -> bool = default;

    static auto deserialize(std::istream& toml) -> std::expected<module_manifest, std::string>;
};

struct module {
    std::string name;
    std::string description;
    std::string directory;

    auto operator==(const module&) const -> bool = default;
};

using module_find_result = std::expected<module, std::string>;
auto find_modules(const std::string& dir)
    -> std::expected<std::vector<module_find_result>, std::string>;

} // namespace plg
