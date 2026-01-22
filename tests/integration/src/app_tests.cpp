#include <catch2/catch_test_macros.hpp>

#include "module.hpp"
#include <vector>

TEST_CASE("find_modules") {
    SECTION("modules are present") {
        auto res = plg::find_modules("modules");
        REQUIRE(res.has_value());

        auto actual = *res;

        auto expected = std::vector<plg::module>{
            plg::module{
                .manifest  = {"test", "testmod1", {0, 1, 0}, std::nullopt, std::nullopt, {}},
                .directory = "modules/testmod1",
            },
            plg::module{
                .manifest  = {"test", "testmod2", {0, 1, 0}, std::nullopt, std::nullopt, {}},
                .directory = "modules/testmod2",
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
        auto modules = plg::find_modules("modules/testmod1/manifest.toml");
        CHECK(!modules.has_value());
    }
}

TEST_CASE("Find packs") {
    // SECTION("packs are present") {
    //     auto packs = plg::find_packs("packs");
    //     REQUIRE(packs.has_value());

    //     auto expected = std::vector<plg::pack>{
    //         {
    //             .nspace      = "test",
    //             .name        = "pack1",
    //             .description = "test pack 1",
    //             .version     = plg::version(0, 1, 0),
    //             .dependencies =
    //                 {
    //                     {.nspace = "test", .name = "mod1", .version = {0, 1, 0}},
    //                     {.nspace = "test", .name = "mod2", .version = {0, 1, 0}},
    //                 },
    //         },
    //         {
    //             .nspace       = "test",
    //             .name         = "pack2",
    //             .description  = "test pack 2",
    //             .version      = plg::version(0, 1, 0),
    //             .dependencies = {
    //                 {.nspace = "test", .name = "mod3", .version = {0, 1, 0}},
    //                 {.nspace = "test", .name = "mod4", .version = {0, 1, 0}},
    //             },
    //         },
    //     };

    //     for(const auto& pack: expected) {
    //         CHECK(std::find(packs->begin(), packs->end(), pack) != packs->end());
    //     }
    // }
}
