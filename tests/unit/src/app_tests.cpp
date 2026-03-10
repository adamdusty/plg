#include <catch2/catch_test_macros.hpp>

#include "version.hpp"

TEST_CASE("version parsing") {
    SECTION("With annotation") {
        const auto* ver = "1.0.0-annotation";
        auto res        = plg::version::parse(ver);
        REQUIRE(res.has_value());
        CHECK(res->major == 1);
        CHECK(res->minor == 0);
        CHECK(res->patch == 0);
        CHECK(res->annotation.has_value());
        CHECK(res->annotation.value() == "annotation");
    }

    SECTION("Without annotation") {
        const auto* ver = "3.1.4";
        auto res        = plg::version::parse(ver);
        REQUIRE(res.has_value());
        CHECK(res->major == 3);
        CHECK(res->minor == 1);
        CHECK(res->patch == 4);
        CHECK(!res->annotation.has_value());
    }

    SECTION("Invalid major") {
        const auto* ver = "hello.1.4";
        auto res        = plg::version::parse(ver);
        REQUIRE(!res.has_value());
        CHECK(res.error() == "Failed to parse version");
    }

    SECTION("Invalid minor") {
        const auto* ver = "3.a.4";
        auto res        = plg::version::parse(ver);
        REQUIRE(!res.has_value());
        CHECK(res.error() == "Failed to parse version");
    }

    SECTION("Invalid patch") {
        const auto* ver = "3.5.a";
        auto res        = plg::version::parse(ver);
        REQUIRE(!res.has_value());
        CHECK(res.error() == "Failed to parse version");
    }

    SECTION("Annotation has invalid char") {
        const auto* ver = "3.5.6-$";
        auto res        = plg::version::parse(ver);
        REQUIRE(!res.has_value());
        CHECK(res.error() == "Failed to parse version");
    }
}

TEST_CASE("version equality") {
    SECTION("major") {
        auto ver1 = plg::version(1, 1, 1, "pre");
        auto ver2 = plg::version(2, 1, 1, "pre");

        CHECK(ver2 > ver1);
        CHECK(ver1 < ver2);
    }

    SECTION("minor") {
        auto ver1 = plg::version(1, 1, 2, "pre");
        auto ver2 = plg::version(1, 2, 1, "pre");

        CHECK(ver2 > ver1);
        CHECK(ver1 < ver2);
    }

    SECTION("patch") {
        auto ver1 = plg::version(1, 1, 1, "pre");
        auto ver2 = plg::version(1, 1, 2, "pre");

        CHECK(ver2 > ver1);
        CHECK(ver1 < ver2);
    }

    SECTION("equal") {
        auto ver1 = plg::version(1, 1, 1, "pre");
        auto ver2 = plg::version(1, 1, 1, "post");

        CHECK(ver1 == ver2);
    }
}
