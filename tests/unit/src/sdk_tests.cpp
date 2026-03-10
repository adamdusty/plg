#include <catch2/catch_test_macros.hpp>

#include <plg/sdk.hpp>

TEST_CASE("Hello sdk") {
    REQUIRE(plg::version() == 0);
}
