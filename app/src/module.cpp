#include "module.hpp"

#include <filesystem>
#include <format>

namespace fs = std::filesystem;

namespace plg {

auto find_available_modules(const std::string& dir)
    -> std::expected<std::vector<module>, std::string_view> {

    // fs::file_status status;
    // try {
    //     status = fs::status(dir);
    // } catch(fs::filesystem_error& e) {
    //     return std::unexpected(e.what());
    // }

    // if(status.type() != fs::file_type::directory) {
    //     return std::unexpected(std::format("Directory does not exist: {}", dir));
    // }
}

} // namespace plg
