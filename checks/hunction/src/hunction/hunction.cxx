/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022. 12. 04.
 *
 * checks/hunction/src/hunction/hunction --
 */

#include <iostream>

#include <hunction/hunction.hxx>

#include <clang/ASTMatchers/ASTMatchers.h>
using namespace clang::ast_matchers;

cg3::hunction::hunction(clang::DiagnosticsEngine* diag)
     : typed_check<check_types::hunction>(diag),
       _header_def_fun_diag(register_warning("function %0 is defined in a header file")) {
    auto check = functionDecl(isDefinition(),
                              isExpansionInMainFile(),
                              isExpansionInFileMatching(R"aa(\.h$)aa"))
                        .bind("header_func");
    _finder.addMatcher(check, this);
}

void
cg3::hunction::run(const clang::ast_matchers::MatchFinder::MatchResult& result) {
    auto&& srcmgr = *result.SourceManager;

    const auto* func = result.Nodes.getNodeAs<clang::FunctionDecl>("header_func");
    auto loc = func->getLocation();
    auto fun_name = func->getName();
    auto fun_sz = static_cast<std::int32_t>(fun_name.size());
    auto name_end = loc.getLocWithOffset(fun_sz);

    _header_def_fun_diag.fire(loc,
                              func,
                              clang::CharSourceRange::getCharRange(loc, name_end));
}

void
cg3::hunction::match_ast(clang::ASTContext& context) {
    _finder.matchAST(context);
}
