#include "module.hpp"

#include <SDL3/SDL.h>
#include <format>
#include <glaze/glaze.hpp>
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

namespace plg {

auto read_module_metadata(const std::filesystem::path& path)
    -> std::expected<module_metadata, std::string> {
    if(!fs::exists(path)) {
        return std::unexpected(std::format("Path does not exist: {}", path.string()));
    }
    auto metadata = module_metadata{};
    auto buffer   = std::string();
    auto error    = glz::read_file_json(metadata, path.string(), buffer);

    if(error) {
        return std::unexpected(glz::format_error(error, buffer));
    }

    return metadata;
}

auto load_binary(const std::filesystem::path& path) -> std::expected<module, std::string> {
    if(!fs::exists(path)) {
        return std::unexpected(std::format("File does not exist at path: {}", path.string()));
    }

    auto* obj = SDL_LoadObject(path.string().c_str());
    if(obj == nullptr) {
        return std::unexpected(std::string{"Error loading binary: "} + SDL_GetError());
    }

    auto init   = reinterpret_cast<module::init>(SDL_LoadFunction(obj, "initialize"));
    auto deinit = reinterpret_cast<module::deinit>(SDL_LoadFunction(obj, "deinitialize"));

    return module{obj, init, deinit};
}

auto discover_modules(const std::filesystem::path& root)
    -> std::expected<std::unordered_map<std::filesystem::path, module_metadata>, std::string> {
    std::error_code err;

    if(!fs::exists(root, err) || !fs::is_directory(root, err)) {
        return std::unexpected("Mods directory does not exist");
    }

    auto paths = std::unordered_map<fs::path, module_metadata>();

    auto nspace_dir_it = fs::directory_iterator(root, err);
    for(const auto& nspace_dir: nspace_dir_it) {
        auto name_dir_it = fs::directory_iterator(nspace_dir, err);
        for(const auto& name_dir: name_dir_it) {
            auto module_dir_it = fs::directory_iterator(name_dir, err);
            for(const auto& entry: module_dir_it) {
                // module.txt found, read metadata
                if(entry.is_regular_file(err) && entry.path().filename() == "module.json") {
                    auto metadata = read_module_metadata(entry);
                    if(!metadata || !metadata->is_valid()) {
                        SPDLOG_WARN("Failed to parse metadata at {}", entry.path().string());
                        continue;
                    }

                    paths[entry.path().parent_path()] = metadata.value();
                }
            }
        }
    }

    return paths;
}

} // namespace plg
