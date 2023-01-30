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
        handle_long_function(big, gargantuan_limit, _gargantuan_diag);
    }
    else if (newlines >= huge_limit) {
        handle_long_function(big, huge_limit, _huge_diag);
    }
    else if (newlines >= big_limit) {
        handle_long_function(big, big_limit, _big_diag);
    }
}

void
cg3::chonktion::handle_long_function(const clang::FunctionDecl* func, unsigned f_len, check_diagnostic& diag_h) {
    if (!func) return;

    auto loc = func->getLocation();
    auto size_int = static_cast<std::int32_t>(func->getName().size());
    auto end = loc.getLocWithOffset(size_int);
    auto range = clang::CharSourceRange::getCharRange(loc, end);

    diag_h.fire(loc,
                func,
                f_len,
                range);
}

void
cg3::chonktion::match_ast(clang::ASTContext& context) {
    _finder.matchAST(context);
}
