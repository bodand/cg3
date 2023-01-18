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
    const std::string src = GENERATE("data/ok.c.ast",
                                     "data/ok.cxx.ast",
                                     "data/empty.c.ast",
                                     "data/empty.cxx.ast");
    REQUIRE(exists(fs::path(src)));
    cg3::test_ast_loader ldr(src);
    cg3::chonktion check;

    ldr.diag_sink->set_info_check([&]([[maybe_unused]] const clang::Diagnostic& x) {
        INFO(src);
        CHECK(false);
    });

    SECTION("doesn't cause any warnings") {
        check.check_ast(ldr.ast);
        CHECK(true);
    }

    SECTION("doesn't print output") {
        check.check_ast(ldr.ast);
        auto written = cg3::capture_stream<&std::cout>([&] {
            check.collected_report();
        });

        INFO(src);
        INFO(written);
        CHECK(written.empty());
    }
}
