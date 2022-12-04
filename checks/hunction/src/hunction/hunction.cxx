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
    auto func = result.Nodes.getNodeAs<clang::FunctionDecl>("header_func");

    auto begin_src = func->getBeginLoc();
    auto end_src = func->getEndLoc();

    auto begin = srcmgr.getCharacterData(begin_src);
    auto end = srcmgr.getCharacterData(end_src);

    auto error_line = begin_src.printToString(srcmgr);

    std::cout << error_line << ": hunction: function defined in header:\n\t";
    std::copy(begin, end + 1, std::ostream_iterator<char>(std::cout));
    std::cout << "\n";

    auto fname = srcmgr.getFilename(begin_src);
    auto name = func->getName();
    _header_functions.emplace(std::piecewise_construct,
                              std::forward_as_tuple(fname.str()),
                              std::forward_as_tuple(name.data(), name.size()));
}

void
cg3::hunction::check_ast(std::vector<std::unique_ptr<clang::ASTUnit>>& units) {
    for (const auto& unit : units) {
        auto& ctx = unit->getASTContext();
        _finder.matchAST(ctx);
    }
}

void
cg3::hunction::collected_report() {
    if (_header_functions.empty()) return;

    std::fill_n(std::ostream_iterator<char>(std::cout), 80, '-');
    std::cout << "\nhunction collected report\n";

    std::cout << "the following files contain the header-defined functions\n\n";
    for (const auto& [file, var] : _header_functions) {
        std::cout << "\t" << file.string() << ": " << var << "\n";
    }
    std::cout << "\n";

    std::fill_n(std::ostream_iterator<char>(std::cout), 80, '-');
    std::cout << "\n";
}
