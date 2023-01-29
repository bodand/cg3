/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022. 12. 04.
 *
 * checks/chonktion/src/chonktion/chonktion --
 */

#include <cassert>
#include <iostream>

#include <chonktion/chonktion.hxx>

#include <clang/ASTMatchers/ASTMatchers.h>
using namespace clang::ast_matchers;

cg3::chonktion::chonktion(clang::DiagnosticsEngine* diag)
     : typed_check<check_types::chonktion>(diag),
       _big_diag(register_warning("function %0 is a tad too big (>= %1 stmts)")),
       _huge_diag(register_warning("function %0 is way too huge (>= %1 stmts)")),
       _gargantuan_diag(register_warning("function %0 is inconceivably gargantuan (>= %1 stmts)")) {
    auto big_func = functionDecl(hasBody(compoundStmt().bind("body")),
                                 isExpansionInMainFile())
                           .bind("big_func");

    _finder.addMatcher(big_func, this);
}

void
cg3::chonktion::run(const clang::ast_matchers::MatchFinder::MatchResult& result) {
    auto&& srcmgr = *result.SourceManager;
    auto&& nodes = result.Nodes;

    const auto* big = nodes.getNodeAs<clang::FunctionDecl>("big_func");
    const auto* body = nodes.getNodeAs<clang::CompoundStmt>("body");

    auto begin_src = body->getBeginLoc();
    auto end_src = body->getEndLoc();
    const auto* begin = srcmgr.getCharacterData(begin_src);
    const auto* end = srcmgr.getCharacterData(end_src);
    auto newlines = std::count(begin, end, '\n');

    if (newlines >= gargantuan_limit) {
        handle_long_function(srcmgr, big, gargantuan_limit, _gargantuan_diag);
    }
    else if (newlines >= huge_limit) {
        handle_long_function(srcmgr, big, huge_limit, _huge_diag);
    }
    else if (newlines >= big_limit) {
        handle_long_function(srcmgr, big, big_limit, _big_diag);
    }
}

namespace {
    void
    print_function_set(unsigned size, const std::vector<std::string>& funcs) {
        if (funcs.empty()) return;

        std::cout << "functions larger than " << size << " statements:\n";
        for (const auto& func : funcs) {
            std::cout << "\t" << func << "\n";
        }
        std::cout << "\n";
    }
}

void
cg3::chonktion::collected_report() {
    constexpr const static auto terminal_width = 80;
    if (_big_funcs.empty()) return;

    std::vector<std::string> big_funcs;
    std::vector<std::string> huge_funcs;
    std::vector<std::string> gargantuan_funcs;
    for (const auto& [size, data] : _big_funcs) {
        if (size >= gargantuan_limit) {
            gargantuan_funcs.push_back(data);
        }
        else if (size >= huge_limit) {
            huge_funcs.push_back(data);
        }
        else if (size >= big_limit) {
            big_funcs.push_back(data);
        }
    }

    std::fill_n(std::ostream_iterator<char>(std::cout), terminal_width, '-');
    std::cout << "\nchonktion collected report\n";

    std::cout << "the following files contain the larger-than-wanted functions\n\n";
    print_function_set(big_limit, big_funcs);
    print_function_set(huge_limit, huge_funcs);
    print_function_set(gargantuan_limit, gargantuan_funcs);

    std::fill_n(std::ostream_iterator<char>(std::cout), terminal_width, '-');
    std::cout << "\n";
}

void
cg3::chonktion::handle_long_function(clang::SourceManager& srcmgr,
                                     const clang::FunctionDecl* func,
                                     unsigned f_len,
                                     check_diagnostic& diag_h) {
    if (!func) return;

    auto loc = func->getLocation();
    auto size_int = static_cast<std::int32_t>(func->getName().size());
    auto end = loc.getLocWithOffset(size_int);
    auto range = clang::CharSourceRange::getCharRange(loc, end);

    diag_h.fire(loc,
                func,
                f_len,
                range);

    auto error_line = func->getBeginLoc().printToString(srcmgr);
    auto func_name = func->getName().str();
    auto func_data = error_line + ": " + func_name;
    _big_funcs.emplace(std::piecewise_construct,
                       std::forward_as_tuple(f_len),
                       std::forward_as_tuple(func_data.data(), func_data.size()));
}

void
cg3::chonktion::match_ast(clang::ASTContext& context) {
    _finder.matchAST(context);
}
