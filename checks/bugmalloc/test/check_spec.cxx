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

#include <bugmalloc/bugmalloc.hxx>
#include <catch2/catch_test_macros.hpp>
#include <chk3/check.hxx>

TEST_CASE("bugmalloc conforms to check", "[bugmalloc]") {
    REQUIRE(std::convertible_to<cg3::bugmalloc*, cg3::check*>);
}

TEST_CASE("bugmalloc is default constructible", "[bugmalloc]") {
    REQUIRE(std::is_default_constructible_v<cg3::bugmalloc>);
}

TEST_CASE("loader<bugmalloc>::type is bugmalloc", "[bugmalloc]") {
    REQUIRE(std::same_as<cg3::loader<cg3::check_types::bugmalloc>::type,
                         cg3::bugmalloc>);
}
