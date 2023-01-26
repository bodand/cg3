/* cg3 project
 *
 * Copyright (c) 2023 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2023. 01. 26.
 *
 * test/chk3/diagnostics_collection --
 *   Tests for the diagnostics collection classes.
 */

#include <tuple>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <catch2/generators/catch_generators_range.hpp>
#include <chk3/checks.hxx>
#include <chk3/diagnostics_collection.hxx>
#include <magic_enum.hpp>

TEST_CASE("diagnostic constructed without relevant identifier !has_identifier()",
          "[chk3][diagnostic]") {
    auto diag = cg3::diagnostic(clang::DiagnosticsEngine::Level::Warning,
                                "asd.c",
                                1,
                                2,
                                "warning: do not name your file asd -- makes no sense",
                                "asd.c");
    CHECK_FALSE(diag.has_identifier());
    CHECK_FALSE(diag.relevant_identifier);
}

TEST_CASE("diagnostic constructed with relevant identifier has_identifier()",
          "[chk3][diagnostic]") {
    auto diag = cg3::diagnostic(clang::DiagnosticsEngine::Level::Warning,
                                "asd.c",
                                1,
                                2,
                                "warning: do not name your file asd -- makes no sense",
                                "asd.c",
                                "asd");
    CHECK(diag.has_identifier());
    CHECK(*diag.relevant_identifier == "asd");
}

TEST_CASE("diagnostics_collection returns different chains for calling new_chain twice for the same check type",
          "[chk3][diagnostics_collection]") {
    auto check = GENERATE(filter([](auto chk) { return chk != cg3::check_types::COUNT; },
                                 from_range(magic_enum::enum_values<cg3::check_types>())));

    cg3::diagnostics_collection collection;

    auto chain0 = collection.new_chain(check);
    auto chain1 = collection.new_chain(check);

    CHECK(chain0 != chain1);
}

TEST_CASE("diagnostics_collection returns different chains for different check types",
          "[chk3][diagnostics_collection]") {
    auto check1 = GENERATE(filter([](auto chk) { return chk != cg3::check_types::COUNT; },
                                  from_range(magic_enum::enum_values<cg3::check_types>())));
    auto check2 = GENERATE_REF(filter([check1](auto chk) { return chk != check1; },
                                      filter([](auto chk) { return chk != cg3::check_types::COUNT; },
                                             from_range(magic_enum::enum_values<cg3::check_types>()))));
    REQUIRE(check1 != check2);

    cg3::diagnostics_collection collection;

    auto chain0 = collection.new_chain(check1);
    auto chain1 = collection.new_chain(check2);

    CHECK(chain0 != chain1);
}

TEST_CASE("diagnostics_collection increases its chain count after a new_chain call",
          "[chk3][diagnostics_collection]") {
    auto check = GENERATE(take(3,
                               filter([](auto chk) { return chk != cg3::check_types::COUNT; },
                                      map([](auto chk_int) { return static_cast<cg3::check_types>(chk_int); },
                                          random(0, static_cast<int>(cg3::check_types::COUNT))))));
    auto diag = cg3::diagnostic(clang::DiagnosticsEngine::Level::Warning,
                                "asd.c",
                                1,
                                2,
                                "warning: do not name your file asd -- makes no sense",
                                "asd.c");

    cg3::diagnostics_collection collection;
    auto before = collection.chain_count();
    CHECK(before == 0);

    std::ignore = collection.new_chain(check);
    auto after = collection.chain_count();
    CHECK(after == 1);
}

TEST_CASE("diagnostics_collection returns the same chain if appending to a chain",
          "[chk3][diagnostics_collection]") {
    auto check = GENERATE(take(1,
                               filter([](auto chk) { return chk != cg3::check_types::COUNT; },
                                      map([](auto chk_int) { return static_cast<cg3::check_types>(chk_int); },
                                          random(0, static_cast<int>(cg3::check_types::COUNT))))));
    auto diag = cg3::diagnostic(clang::DiagnosticsEngine::Level::Warning,
                                "asd.c",
                                1,
                                2,
                                "warning: do not name your file asd -- makes no sense",
                                "asd.c");

    cg3::diagnostics_collection collection;

    auto chain = collection.new_chain(check);
    auto chain_ret = collection.chain_diagnostic(chain, diag);

    CHECK(chain == chain_ret);
}

TEST_CASE("diagnostics_collection allows iterating through all chains",
          "[chk3][diagnostics_collection]") {
    auto check = GENERATE(filter([](auto chk) { return chk != cg3::check_types::COUNT; },
                                 from_range(magic_enum::enum_values<cg3::check_types>())));
    INFO(magic_enum::enum_name(check));
    auto diag0 = cg3::diagnostic(clang::DiagnosticsEngine::Level::Warning,
                                 "asd.c",
                                 1,
                                 2,
                                 "warning: do not name your file asd -- makes no sense",
                                 "asd.c");
    auto diag1 = cg3::diagnostic(clang::DiagnosticsEngine::Level::Error,
                                 "asd1.c",
                                 1,
                                 2,
                                 "error: really do not name your file asd -- makes no sense",
                                 "asd1.c");

    cg3::diagnostics_collection collection;

    std::ignore = collection.chain_diagnostic(check, diag0);
    std::ignore = collection.chain_diagnostic(check, diag1);

    auto chain_cnt = static_cast<std::size_t>(std::distance(collection.begin_chains(),
                                                            collection.end_chains()));
    auto resulting_cnt = collection.chain_count();
    CHECK(chain_cnt == resulting_cnt);
}
