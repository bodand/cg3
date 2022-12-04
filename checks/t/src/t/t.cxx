/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022. 12. 04.
 *
 * checks/t/src/t/t --
 */

#include <iostream>

#include <t/t.hxx>

#include <clang/ASTMatchers/ASTMatchers.h>
using namespace clang::ast_matchers;

namespace {
    struct string_contains_t_interface final : internal::SingleNodeMatcherInterface<clang::StringLiteral> {
        bool
        matchesNode(const clang::StringLiteral& node) const override {
            auto str = node.getString();
            auto it = std::find(str.begin(), str.end(), 't');
            return it != str.end();
        }
    };

    struct string_contains_t final : internal::Matcher<clang::StringLiteral> {
        explicit string_contains_t(internal::MatcherInterface<clang::StringLiteral>* implementation)
             : Matcher(implementation) { }
    };

    string_contains_t
    contains_t() {
        return string_contains_t{
               new string_contains_t_interface()};
    }

}

cg3::t::t() {
    auto check = callExpr(callee(functionDecl(hasName("fopen"))),
                          hasArgument(1, stringLiteral(contains_t())))
                        .bind("bad_call");

    _finder.addMatcher(check, this);
}

void
cg3::t::run(const clang::ast_matchers::MatchFinder::MatchResult& result) {
    auto&& srcmgr = *result.SourceManager;
    auto expr = result.Nodes.getNodeAs<clang::CallExpr>("bad_call");

    auto begin_src = expr->getBeginLoc();
    auto end_src = expr->getEndLoc();

    auto begin = srcmgr.getCharacterData(begin_src);
    auto end = srcmgr.getCharacterData(end_src);

    auto error_line = begin_src.printToString(srcmgr);

    std::cout << error_line << ": t: found t:\n\t";
    std::copy(begin, end + 1, std::ostream_iterator<char>(std::cout));
    std::cout << "\n";
}

void
cg3::t::check_ast(std::vector<std::unique_ptr<clang::ASTUnit>>& units) {
    for (const auto& unit : units) {
        auto& ctx = unit->getASTContext();
        _finder.matchAST(ctx);
    }
}
