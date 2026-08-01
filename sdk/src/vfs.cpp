#include "plg/vfs.hpp"

#include <expected>
#include <filesystem>
#include <fstream>

namespace plg {

auto virtual_file_system::mount(
    std::string_view module_id, const fs::path& physical_path, int priority
) -> void {
    fs::path abs_path = fs::exists(physical_path) ? fs::canonical(physical_path) : physical_path;

    mounts.emplace_back(std::string(module_id), abs_path, priority);

    std::sort(mounts.begin(), mounts.end(), [](const mount_point& a, const mount_point& b) {
        return a.priority > b.priority;
    });
}

auto virtual_file_system::unmount_module(std::string_view module_id) -> void {
    std::erase_if(mounts, [module_id](const mount_point& point) {
        return point.module_id == module_id;
    });
}

auto virtual_file_system::resolve(std::string_view vpath) const
    -> std::expected<fs::path, std::string> {
    auto clean_vpath = fs::path{vpath};

    if(clean_vpath.is_absolute()) {
        clean_vpath = clean_vpath.relative_path();
    }

    for(const auto& mount: mounts) {
        fs::path candidate = mount.physical_path / clean_vpath;
        if(fs::exists(candidate) && fs::is_regular_file(candidate)) {
            return candidate;
        }
    }

    return std::unexpected("VFS: File not found in any mounted paths: " + std::string(vpath));
}

auto virtual_file_system::read_bytes(std::string_view vpath) const
    -> std::expected<std::vector<std::byte>, std::string> {
    auto resolved_path = resolve(vpath);
    if(!resolved_path) {
        return std::unexpected(resolved_path.error());
    }

    auto file = std::basic_ifstream<std::byte>(*resolved_path, std::ios::binary | std::ios::ate);
    if(!file.is_open()) {
        return std::unexpected("VFS: Failed to open resolved file: " + resolved_path->string());
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    auto buffer = std::vector<std::byte>(static_cast<size_t>(size));
    if(!file.read(buffer.data(), size)) {
        return std::unexpected("VFS: Failed to read file data: " + resolved_path->string());
    }

    return buffer;
}

} // namespace plg
