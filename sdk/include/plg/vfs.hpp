#pragma once

#include "sdk/export.hpp"
#include <expected>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace plg {

class PLG_SDK_EXPORT virtual_file_system {
public:
    struct mount_point {
        std::string module_id;  // e.g. "core:rendering", "user1234:content_module"
        fs::path physical_path; // Local disk folder e.g. "./mods/user1234/"
        int priority;           // Higher numbers take precedence over lower numbers
    };

    auto mount(std::string_view module_id, const fs::path& physical_path, int priority = 0) -> void;
    auto unmount_module(std::string_view module_id) -> void;

    [[nodiscard]] auto resolve(std::string_view vpath) const
        -> std::expected<fs::path, std::string>;

    [[nodiscard]] auto read_bytes(std::string_view vpath) const
        -> std::expected<std::vector<std::byte>, std::string>;

private:
    std::vector<mount_point> mounts;
};

} // namespace plg
