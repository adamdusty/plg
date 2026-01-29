#include <catch2/catch_test_macros.hpp>

#include "module.hpp"
#include "pack.hpp"
#include <vector>

TEST_CASE("find_modules") {
    SECTION("modules are present") {
        auto res = plg::find_modules("modules");
        REQUIRE(res.has_value());

        auto actual = *res;

        auto expected = std::vector<plg::module>{
            plg::module{
                .manifest  = {"nspace1", "testmod1", {0, 1, 0}, std::nullopt, std::nullopt, {}},
                .directory = "modules/nspace1/testmod1",
            },
            plg::module{
                .manifest  = {"nspace2", "testmod2", {0, 1, 0}, std::nullopt, std::nullopt, {}},
                .directory = "modules/nspace2/testmod2",
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
        auto modules = plg::find_modules("modules/nspace1/testmod1/manifest.toml");
        CHECK(!modules.has_value());
    }
}

TEST_CASE("Find packs") {
    SECTION("packs are present") {
        auto packs = plg::find_packs("packs");
        REQUIRE(packs.has_value());

        auto expected = std::vector<plg::pack>{
            {
                .nspace            = "test",
                .name              = "pack1",
                .version           = plg::version(1, 2, 3, "alpha"),
                .short_description = "A pack for testing",
                .long_description  = "A longer description of this pack",
                .dependencies =
                    {
                        {.nspace = "test", .name = "module1", .version = {1, 2, 3}},
                        {.nspace = "test", .name = "module2", .version = {0, 2, 4}},
                    },
            },
            {
                .nspace            = "test",
                .name              = "pack2",
                .version           = plg::version(3, 2, 5),
                .short_description = "A second pack for testing",
                .long_description  = "A longer description of this second pack",
                .dependencies      = {
                    {.nspace = "test", .name = "pack2module1", .version = {10, 25, 859}},
                    {.nspace = "test", .name = "pack2module2", .version = {3, 0, 1024}},
                },
            },
        };

        for(const auto& pack: expected) {
            CHECK(std::find(packs->begin(), packs->end(), pack) != packs->end());
        }
    }
}
