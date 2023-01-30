/* cg3 project
 *
 * Copyright (c) 2023 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2023. 01. 18.
 *
 * checks/chonktion/test/ast_tests --
 *   Tests for correctly chonktion reporting AST warnings.
 */

#include <filesystem>
#include <iostream>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <chonktion/chonktion.hxx>
#include <test-commons/capture_stream.hxx>
#include <test-commons/test_ast_loader.hxx>
#include <test-commons/test_consumer.hxx>

#include <clang/Tooling/Tooling.h>

namespace fs = std::filesystem;

TEST_CASE("files without long functions do not cause warnings",
          "[chonktion]") {
    const auto src = GENERATE(as<fs::path>{},
                              "data/ok.c.ast",
                              "data/ok.cxx.ast",
                              "data/empty.c.ast",
                              "data/empty.cxx.ast");
    REQUIRE(exists(src));
    cg3::test_ast_loader ldr(src);
    cg3::chonktion check(ldr.get_diag());

    ldr.diag_sink->set_info_check([&]([[maybe_unused]] const clang::Diagnostic& x) {
        INFO(src);
        CHECK(false);
    });

    SECTION("doesn't cause any warnings") {
        check.check_ast(ldr.ast);
        CHECK(true);
    }
}

// NOLINTNEXTLINE test-macro
TEST_CASE("files with chonktion violating functions are reported as their respective size category",
          "[chonktion]") {
    const std::string type = GENERATE("big", "huge", "gargantuan");
    const std::string limit = GENERATE("low", "high");
    const std::string language = GENERATE(".c.ast", ".cxx.ast");
    const fs::path src = "data/" + type + "_" + limit + language;

    INFO(src);
    REQUIRE(exists(src));
    cg3::test_ast_loader ldr(src);
    cg3::chonktion check(ldr.get_diag());

    SECTION("each file only has one (the violating) function reported") {
        int warn_count = 0;
        ldr.diag_sink->set_info_check([&]([[maybe_unused]] const clang::Diagnostic& x) {
            ++warn_count;
        });

        check.check_ast(ldr.ast);
        INFO(src);
        CHECK(warn_count == 1);
    }
}
