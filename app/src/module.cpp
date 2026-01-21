#include "module.hpp"

#include <algorithm>
#include <filesystem>
#include <format>
#include <istream>
#include <toml++/toml.hpp>

namespace fs = std::filesystem;

namespace plg {

auto module_manifest::deserialize(std::istream& toml)
    -> std::expected<module_manifest, std::string> {
    auto table = toml::table{};
    try {
        table = toml::parse(toml);
    } catch(const toml::parse_error& e) {
        return std::unexpected(e.what());
    }

    auto nspace = table["namespace"].value<std::string>();
    if(!nspace) {
        return std::unexpected("Missing required field: `namespace`");
    }

    auto name = table["name"].value<std::string>();
    if(!name) {
        return std::unexpected("Missing required field: `name`");
    }

    auto desc = table["description"].value<std::string>();
    if(!desc) {
        return std::unexpected("Missing required field: `description`");
    }

    return module_manifest{.nspace = *nspace, .name = *name, .description = *desc};
}

auto find_modules(const std::string& dir)
    -> std::expected<std::vector<module_find_result>, std::string> {

    fs::directory_iterator directory_it;
    try {
        directory_it = fs::directory_iterator(dir);
    } catch(const fs::filesystem_error& e) {
        return std::unexpected(e.what());
    }

    // Get sub-directories of 'dir' that contain a manifest.json
    auto module_directories = std::vector<fs::directory_entry>();
    std::copy_if(
        begin(directory_it),
        end(directory_it),
        std::back_inserter(module_directories),
        [](const fs::directory_entry& entry) {
            return entry.is_directory() && fs::exists(entry.path() / "manifest.toml");
        }
    );

    auto modules = std::vector<module_find_result>();
    std::transform(
        module_directories.begin(),
        module_directories.end(),
        std::back_inserter(modules),
        [](const fs::directory_entry& entry) -> module_find_result {
            auto manifest_file_path = entry.path() / "manifest.toml";
            auto file               = std::ifstream(manifest_file_path);
            if(!file) {
                auto msg =
                    std::format("Unable to open manifest.toml at: {}", entry.path().string());
                return std::unexpected(module_find_error{msg});
            }

            auto man = module_manifest::deserialize(file);
            if(!man.has_value()) {
                return std::unexpected(module_find_error{man.error()});
            }

            return module{
                .nspace      = man->nspace,
                .name        = man->name,
                .description = man->description,
                .directory   = entry.path().generic_string(),
            };
        }
    );

    return modules;
}

} // namespace plg
