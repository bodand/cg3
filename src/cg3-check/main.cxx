/* cg3 project
 *
 * Copyright (c) 2022 András Bodor <bodand@pm.me>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * - Neither the name of the copyright holder nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Originally created: 2022-11-25.
 *
 * src/cg3-check/main --
 */
#include <iostream>
#include <tuple>

#include <cg3-check/runtime_loader.hxx>
#include <chk3/checks.hxx>
#include <magic_enum.hpp>

#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/CommandLine.h>

using namespace llvm;
using namespace clang::tooling;
using namespace clang::ast_matchers;

namespace {
    cl::OptionCategory g_cg3_category("cg3 options");              // NOLINT literally llvm having a god-awful cli parser
    cl::extrahelp g_common_help(CommonOptionsParser::HelpMessage); // NOLINT literally llvm having a god-awful cli parser

    constexpr const auto make_opt_values = [](auto&&... args) {
        return cl::values(std::forward<decltype(args)>(args)...);
    };
    cl::list<cg3::check_types> g_cg3_checks(cl::cat(g_cg3_category), // NOLINT literally llvm having a god-awful cli parser
                                            cl::desc("Checks available: "),
                                            cg3::make_options<decltype(make_opt_values)>{make_opt_values}());

    std::vector<std::unique_ptr<clang::ASTUnit>>
    make_ast_units(ClangTool& tool) {
        tool.appendArgumentsAdjuster(getClangSyntaxOnlyAdjuster());

        std::vector<std::unique_ptr<clang::ASTUnit>> ast_units;
        tool.buildASTs(ast_units);

        return ast_units;
    }

    std::vector<std::unique_ptr<cg3::check>>
    get_requested_checks(cg3::runtime_loader& loader) {
        std::vector<std::unique_ptr<cg3::check>> checks;

        // if --complete is passed, do all checks
        if (auto it = std::find(g_cg3_checks.begin(),
                                g_cg3_checks.end(),
                                cg3::check_types::COUNT);
            it != g_cg3_checks.end()) {
            auto const& all_values = magic_enum::enum_values<cg3::check_types>();
            for (const auto& check : all_values) {
                if (check == cg3::check_types::COUNT) continue;

                checks.push_back(loader.load_check(check));
            }
        }
        else {
            std::transform(g_cg3_checks.begin(),
                           g_cg3_checks.end(),
                           std::back_inserter(checks),
                           [&loader](auto check) {
                               return loader.load_check(check);
                           });
        }

        return checks;
    }
}

int
main(int argc, const char** argv) try {
    auto maybe_parser = CommonOptionsParser::create(argc, argv, g_cg3_category);

    handleAllErrors(maybe_parser.takeError(),
                    [](const ErrorInfoBase& err) {
                        std::cerr << "fatal error: llvm: " << err.message() << "\n";
                    });
    if (!maybe_parser) {
        return 1;
    }

    auto&& parser = maybe_parser.get();

    ClangTool tool(parser.getCompilations(),
                   parser.getSourcePathList());
    auto ast_units = make_ast_units(tool);

    cg3::runtime_loader loader;
    auto checks = get_requested_checks(loader);

    for (const auto& check : checks) {
        check->check_ast(ast_units);
    }

    std::cerr << std::flush;
    std::cout << std::flush;
    std::clog << std::flush;

    // end reports
    for (const auto& check : checks) {
        check->collected_report();
    }

    return 0;
} catch (const std::exception& ex) {
    // do not throw in exception handling code
    std::ignore = std::fputs("fatal: ", stderr);
    std::ignore = std::fputs(ex.what(), stderr);
    return 1;
}
