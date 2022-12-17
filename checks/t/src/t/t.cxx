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
                          hasArgument(1, stringLiteral(contains_t()).bind("lit_param")));

    _finder.addMatcher(check, this);
}

void
cg3::t::run(const clang::ast_matchers::MatchFinder::MatchResult& result) {
    auto&& srcmgr = *result.SourceManager;
    auto&& diag = srcmgr.getDiagnostics();
    auto&& nodes = result.Nodes;
    auto t_str_node = nodes.getNodeAs<clang::StringLiteral>("lit_param");
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

    diag.Report(t_str_node->getBeginLoc(), _diag_id)
           << t_str_node->getSourceRange()
           << fixit;
}

void
cg3::t::check_ast(std::vector<std::unique_ptr<clang::ASTUnit>>& units) {
    for (const auto& unit : units) {
        auto& ctx = unit->getASTContext();
        auto& opts = unit->getLangOpts();
        auto pp = unit->getPreprocessorPtr();
        auto& diag_engine = ctx.getDiagnostics();
        auto consumer = diag_engine.getClient();

        consumer->BeginSourceFile(opts, pp.get());

        _diag_id = diag_engine.getCustomDiagID(clang::DiagnosticsEngine::Warning,
                                               "ANSI/ISO C forbids `t' to stand for text-mode in fopen parameter");
        _finder.matchAST(ctx);

        consumer->EndSourceFile();
    }
}
