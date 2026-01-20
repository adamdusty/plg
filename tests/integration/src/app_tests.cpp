#include <catch2/catch_test_macros.hpp>

#include "module.hpp"
#include <vector>

TEST_CASE("find_available_modules") {
    SECTION("modules are present") {
        auto res = plg::find_modules("modules");
        REQUIRE(res.has_value());

        auto actual = std::vector<plg::module>();
        for(auto& mod: *res) {
            if(mod) {
                actual.emplace_back(mod.value());
            }
        }

        auto expected = std::vector<plg::module>{
            plg::module{
                .name        = "testmod1",
                .description = "A module for integration testing",
                .directory   = "modules/testmod1"
            },
            plg::module{
                .name        = "testmod2",
                .description = "A module for integration testing",
                .directory   = "modules/testmod2"
            },
        };

        for(auto& mod: expected) {
            CHECK(std::find(actual.begin(), actual.end(), mod) != actual.end());
        }
    }

    SECTION("directory path does not exist") {
        auto modules = plg::find_modules("dir/that/does/not/exist");
        CHECK(!modules.has_value());
    }

    SECTION("path is not a directory") {
        auto modules = plg::find_modules("modules/testmod1/manifest.json");
        CHECK(!modules.has_value());
    }
}
