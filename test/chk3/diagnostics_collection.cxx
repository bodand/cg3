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

TEST_CASE("diagnostics_collection two default initied chain_iterators are equal",
          "[chk3][diagnostics_collection]") {
    CHECK(cg3::chain_iterator{} == cg3::chain_iterator{});
}

TEST_CASE("diagnostics_collection begin iterator of empty collection is an after end iterator",
          "[chk3][diagnostics_collection]") {
    cg3::diagnostics_collection collection;
    auto beg = collection.begin_chains();
    auto end = collection.end_chains();
    CHECK(beg == end);
}

TEST_CASE("diagnostics_collection's valid chain_iterators are not equal if they point to different chains",
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

    auto it1 = collection.begin_chains();
    auto it2 = collection.begin_chains();
    std::advance(it2, 1);

    CHECK_FALSE(it1 == it2);
}

TEST_CASE("diagnostics_collection's valid chain_iterators return the same check they were created as",
          "[chk3][diagnostics_collection]") {

    auto check = GENERATE(filter([](auto chk) { return chk != cg3::check_types::COUNT; },
                                 from_range(magic_enum::enum_values<cg3::check_types>())));
    INFO(magic_enum::enum_name(check));
    auto diag = cg3::diagnostic(clang::DiagnosticsEngine::Level::Warning,
                                "asd.c",
                                1,
                                2,
                                "warning: do not name your file asd -- makes no sense",
                                "asd.c");

    cg3::diagnostics_collection collection;

    std::ignore = collection.chain_diagnostic(check, diag);
    auto it = collection.begin_chains();

    CHECK(it.current_check_type() == check);
}

TEST_CASE("diagnostics_collection's forward iterator is behaviorally correct",
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

    auto it1 = collection.begin_chains();
    auto it2 = collection.begin_chains();

    CHECK(it1++ == it2);
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

TEST_CASE("diagnostics_collection returns empty chain if chain is created but not chained with anything",
          "[chk3][diagnostics_collection]") {
    auto check = GENERATE(filter([](auto chk) { return chk != cg3::check_types::COUNT; },
                                 from_range(magic_enum::enum_values<cg3::check_types>())));
    INFO(magic_enum::enum_name(check));

    cg3::diagnostics_collection collection;
    std::ignore = collection.new_chain(check);

    auto it = collection.begin_chains();
    REQUIRE_FALSE(it == collection.end_chains());

    SECTION("using direct dereference") {
        auto empty_chain = *it;
        CHECK(empty_chain.empty());
    }

    SECTION("using arrow notation") {
        CHECK(it->empty());
    }
}

// NOLINTNEXTLINE test-macro
TEST_CASE("diagnostics_collection returns nonempty const chain if chain is created and chained with diagnostic",
          "[chk3][diagnostics_collection]") {
    auto check = GENERATE(filter([](auto chk) { return chk != cg3::check_types::COUNT; },
                                 from_range(magic_enum::enum_values<cg3::check_types>())));
    INFO(magic_enum::enum_name(check));
    auto diag = cg3::diagnostic(clang::DiagnosticsEngine::Level::Warning,
                                "asd.c",
                                1,
                                2,
                                "warning: do not name your file asd -- makes no sense",
                                "asd.c");

    cg3::diagnostics_collection collection;
    std::ignore = collection.chain_diagnostic(check, diag);

    const auto it = collection.begin_chains();
    REQUIRE_FALSE(it == collection.end_chains());

    SECTION("using direct dereference") {
        auto chain = *it;
        REQUIRE_FALSE(chain.empty());
        CHECK(chain.size() == 1U);
        CHECK(chain.front().output_text == diag.output_text);
    }

    SECTION("using arrow notation") {
        REQUIRE_FALSE(it->empty());
        CHECK(it->size() == 1U);
        CHECK(it->front().output_text == diag.output_text);
    }
}

TEST_CASE("diagnostics_collection maps diagnostics to checks",
          "[chk3][diagnostics_collection]") {
    auto check = GENERATE(filter([](auto chk) { return chk != cg3::check_types::COUNT; },
                                 from_range(magic_enum::enum_values<cg3::check_types>())));
    INFO(magic_enum::enum_name(check));

    cg3::diagnostics_collection collection;
    CHECK_NOTHROW(collection.map_diagnostic_to_check(2, check));
}

TEST_CASE("diagnostics_collection diagnostic mapping returns nullopt for unmapped diagnostic id",
          "[chk3][diagnostics_collection]") {
    cg3::diagnostics_collection collection;
    CHECK(collection.get_check_from_diagnostic(1) == std::nullopt);
}

TEST_CASE("diagnostics_collection mapped checks can be retrieved",
          "[chk3][diagnostics_collection]") {
    auto check = GENERATE(filter([](auto chk) { return chk != cg3::check_types::COUNT; },
                                 from_range(magic_enum::enum_values<cg3::check_types>())));
    auto id = GENERATE(take(1, random(0U, 65535U)));
    INFO(magic_enum::enum_name(check));
    INFO(id);

    cg3::diagnostics_collection collection;
    collection.map_diagnostic_to_check(id, check);

    auto diag_check = collection.get_check_from_diagnostic(id);
    REQUIRE(diag_check.has_value());

    CHECK(*diag_check == check);
}

TEST_CASE("diagnostics_collection mapped checks can be overwritten with a different check",
          "[chk3][diagnostics_collection]") {
    auto check1 = GENERATE(filter([](auto chk) { return chk != cg3::check_types::COUNT; },
                                  from_range(magic_enum::enum_values<cg3::check_types>())));
    auto check2 = GENERATE_REF(filter([check1](auto chk) { return chk != check1; },
                                      filter([](auto chk) { return chk != cg3::check_types::COUNT; },
                                             from_range(magic_enum::enum_values<cg3::check_types>()))));
    REQUIRE(check1 != check2);

    auto id = GENERATE(take(1, random(0U, 65535U)));
    INFO(magic_enum::enum_name(check1));
    INFO(magic_enum::enum_name(check2));
    INFO(id);

    cg3::diagnostics_collection collection;
    collection.map_diagnostic_to_check(id, check1);

    auto diag_check = collection.get_check_from_diagnostic(id);
    REQUIRE(diag_check.has_value());
    CHECK(*diag_check == check1);

    CHECK_NOTHROW(collection.map_diagnostic_to_check(id, check2));
    diag_check = collection.get_check_from_diagnostic(id);
    REQUIRE(diag_check.has_value());
    CHECK(*diag_check == check2);
}

TEST_CASE("diagnostics_collection mapped checks can be assigned to the same check",
          "[chk3][diagnostics_collection]") {
    auto check = GENERATE(filter([](auto chk) { return chk != cg3::check_types::COUNT; },
                                 from_range(magic_enum::enum_values<cg3::check_types>())));
    auto id = GENERATE(take(1, random(0U, 65535U)));
    INFO(magic_enum::enum_name(check));
    INFO(id);

    cg3::diagnostics_collection collection;
    collection.map_diagnostic_to_check(id, check);

    auto diag_check = collection.get_check_from_diagnostic(id);
    REQUIRE(diag_check.has_value());

    CHECK(*diag_check == check);
    CHECK_NOTHROW(collection.map_diagnostic_to_check(id, check));
}
