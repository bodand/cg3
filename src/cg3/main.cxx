/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-25.
 *
 * src/cg3/main --
 */
#include <iostream>

#include <cg3/runtime_loader.hxx>
#include <chk3/checks.hxx>
#include <magic_enum.hpp>

#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/CommandLine.h>

using namespace llvm;
using namespace clang::tooling;
using namespace clang::ast_matchers;

namespace {
    cl::OptionCategory g_cg3_category("cg3 options");
    cl::extrahelp g_common_help(CommonOptionsParser::HelpMessage);

    constexpr const auto make_opt_values = [](auto&&... args) {
        return cl::values(std::forward<decltype(args)>(args)...);
    };
    cl::list<cg3::check_types> g_cg3_checks(cl::cat(g_cg3_category),
                                            cl::desc("Checks available: "),
                                            cg3::make_options<decltype(make_opt_values)>{make_opt_values}());
}

std::vector<std::unique_ptr<clang::ASTUnit>>
make_ast_units(CommonOptionsParser& parser) {
    ClangTool tool(parser.getCompilations(),
                   parser.getSourcePathList());

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

int
main(int argc, const char** argv) {
    auto maybe_parser = CommonOptionsParser::create(argc, argv, g_cg3_category);

    handleAllErrors(maybe_parser.takeError(),
                    [](const ErrorInfoBase& err) {
                        std::cerr << "fatal error: llvm: " << err.message() << "\n";
                    });
    if (!maybe_parser) {
        return 1;
    }

    auto&& parser = maybe_parser.get();
    auto ast_units = make_ast_units(parser);
    cg3::runtime_loader loader;
    auto checks = get_requested_checks(loader);

    for (const auto& check : checks) {
        auto finder = check->create_finder();

        for (auto const& ast : ast_units) {
            auto& ctx = ast->getASTContext();
            finder->matchAST(ctx);
        }
    }

    // end reports
    for (const auto& check : checks) {
        check->collected_report();
    }

    return 0;
}
