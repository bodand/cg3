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
            const auto* it = std::find(str.begin(), str.end(), 't');
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

cg3::t::t(clang::DiagnosticsEngine* diag)
     : typed_check<check_types::t>(diag),
       _invalid_fopen_diag(register_warning("ANSI/ISO C forbids `t' to stand for text-mode in fopen parameter")) {
    auto check = callExpr(callee(functionDecl(hasName("fopen"))),
                          hasArgument(1, stringLiteral(contains_t()).bind("lit_param")));

    _finder.addMatcher(check, this);
}

void
cg3::t::run(const clang::ast_matchers::MatchFinder::MatchResult& result) {
    auto&& nodes = result.Nodes;
    const auto* t_str_node = nodes.getNodeAs<clang::StringLiteral>("lit_param");
    auto t_str = t_str_node->getString();

    std::string t_less;
    t_less.reserve(t_str.size());

    t_less.push_back('"');
    std::copy_if(t_str.begin(), t_str.end(), std::back_inserter(t_less), [](char c) {
        return c != 't';
    });
    t_less.push_back('"');

    auto fixit = clang::FixItHint::CreateReplacement(t_str_node->getSourceRange(),
                                                     t_less);
    _invalid_fopen_diag.fire(t_str_node->getBeginLoc(),
                             t_str_node->getSourceRange(),
                             fixit);
}

void
cg3::t::match_ast(clang::ASTContext& context) {
    _finder.matchAST(context);
}
