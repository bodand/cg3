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

cg3::hunction::hunction() {
    auto check = functionDecl(isDefinition(),
                              isExpansionInMainFile(),
                              isExpansionInFileMatching(R"aa(\.h$)aa"))
                        .bind("header_func");
    _finder.addMatcher(check, this);
}

void
cg3::hunction::run(const clang::ast_matchers::MatchFinder::MatchResult& result) {
    auto&& srcmgr = *result.SourceManager;
    auto&& diag = srcmgr.getDiagnostics();

    const auto* func = result.Nodes.getNodeAs<clang::FunctionDecl>("header_func");
    auto loc = func->getLocation();
    auto fun_name = func->getName();
    auto fun_sz = static_cast<std::int32_t>(fun_name.size());
    auto name_end = loc.getLocWithOffset(fun_sz);

    auto report = diag.Report(loc, _diag_id);
    report.AddString(fun_name);
    report.AddSourceRange(clang::CharSourceRange::getCharRange(loc, name_end));

    auto fname = srcmgr.getFilename(loc);
    _header_functions.emplace(std::piecewise_construct,
                              std::forward_as_tuple(fname.str()),
                              std::forward_as_tuple(fun_name.data(), fun_name.size()));
}

void
cg3::hunction::check_ast(std::vector<std::unique_ptr<clang::ASTUnit>>& units) {
    for (const auto& unit : units) {
        auto& ctx = unit->getASTContext();
        const auto& opts = unit->getLangOpts();
        auto pp = unit->getPreprocessorPtr();
        auto& diag_engine = ctx.getDiagnostics();
        auto* consumer = diag_engine.getClient();

        consumer->BeginSourceFile(opts, pp.get());

        _diag_id = diag_engine.getCustomDiagID(clang::DiagnosticsEngine::Warning,
                                               "function %0 is defined in a header file");
        _finder.matchAST(ctx);

        consumer->EndSourceFile();
    }
}

void
cg3::hunction::collected_report() {
    constexpr const static auto terminal_width = 80;
    if (_header_functions.empty()) return;

    std::fill_n(std::ostream_iterator<char>(std::cout), terminal_width, '-');
    std::cout << "\nhunction collected report\n";

    std::cout << "the following files contain the header-defined functions\n\n";
    for (const auto& [file, var] : _header_functions) {
        std::cout << "\t" << file.string() << ": " << var << "\n";
    }
    std::cout << "\n";

    std::fill_n(std::ostream_iterator<char>(std::cout), terminal_width, '-');
    std::cout << "\n";
}
