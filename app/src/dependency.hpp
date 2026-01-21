#pragma once

#include "version.hpp"
#include <string>

namespace plg {

struct dependency {
    std::string nspace;
    std::string name;
    version version;

    auto operator==(const dependency&) const -> bool = default;
};

} // namespace plg
