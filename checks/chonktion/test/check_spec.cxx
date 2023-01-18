/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2023. 01. 14.
 *
 * checks/bugmalloc/test/check_spec --
 */

#include <concepts>

#include <catch2/catch_test_macros.hpp>
#include <chk3/check.hxx>
#include <chonktion/chonktion.hxx>

TEST_CASE("chonktion conforms to check", "[chonktion]") {
    REQUIRE(std::convertible_to<cg3::chonktion*, cg3::check*>);
}

TEST_CASE("chonktion is default constructible", "[chonktion]") {
    REQUIRE(std::is_default_constructible_v<cg3::chonktion>);
}

TEST_CASE("loader<chonktion>::type is chonktion", "[chonktion]") {
    REQUIRE(std::same_as<cg3::loader<cg3::check_types::chonktion>::type,
                         cg3::chonktion>);
}
