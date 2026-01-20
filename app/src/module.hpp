#pragma once

#include <expected>
#include <string>
#include <string_view>
#include <vector>

namespace plg {

struct module_manifest {
    std::string name;
};

struct module {};

auto find_available_modules(const std::string& dir)
    -> std::expected<std::vector<module>, std::string_view>;

} // namespace plg
