#include <catch2/catch_test_macros.hpp>

#include "module.hpp"
#include <vector>

TEST_CASE("find_available_modules") {
    SECTION("modules are present") {
        auto actual = plg::find_modules("modules");

        auto expected = std::vector<plg::module_find_result>{
            plg::module{
                .name        = "testmod1",
                .description = "A mod for integration testing",
                .directory   = "./modules/testmod1"
            },
            plg::module{
                .name        = "testmod2",
                .description = "A mod for integration testing",
                .directory   = "./modules/testmod2"
            },
        };

        CHECK(*actual == expected);
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
