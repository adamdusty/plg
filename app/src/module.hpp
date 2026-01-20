#pragma once

#include <expected>
#include <string>
#include <vector>

namespace plg {

struct module_manifest {
    std::string name;
    std::string description;

    static auto deserialize(const std::string&) -> std::expected<module_manifest, std::string> {
        return module_manifest{.name = "hello", .description = "world"};
    }
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
