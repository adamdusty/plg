#include "module.hpp"

#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

namespace plg {

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
        fs::begin(directory_it),
        fs::end(directory_it),
        std::back_inserter(module_directories),
        [](const fs::directory_entry& entry) {
            return entry.is_directory() && fs::exists(entry.path() / "manifest.json");
        }
    );

    auto modules = std::vector<module_find_result>();
    std::transform(
        module_directories.begin(),
        module_directories.end(),
        std::back_inserter(modules),
        [](const fs::directory_entry& entry) -> module_find_result {
            auto man = module_manifest::deserialize((entry.path() / "manifest.json").string());
            if(!man.has_value()) {
                return std::unexpected(man.error());
            }

            return module{
                .name        = man->name,
                .description = man->description,
                .directory   = entry.path().string(),
            };
        }
    );

    return modules;
}

} // namespace plg
