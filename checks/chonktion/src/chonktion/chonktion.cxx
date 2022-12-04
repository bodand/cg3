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

cg3::chonktion::chonktion() {
    auto big_func = functionDecl(hasBody(compoundStmt().bind("body")),
                                 isExpansionInMainFile())
                           .bind("big_func");

    _finder.addMatcher(big_func, this);
}

void
cg3::chonktion::run(const clang::ast_matchers::MatchFinder::MatchResult& result) {
    auto&& srcmgr = *result.SourceManager;
    auto&& nodes = result.Nodes;

    auto big = nodes.getNodeAs<clang::FunctionDecl>("big_func");
    auto body = nodes.getNodeAs<clang::CompoundStmt>("body");

    auto begin_src = body->getBeginLoc();
    auto end_src = body->getEndLoc();
    auto begin = srcmgr.getCharacterData(begin_src);
    auto end = srcmgr.getCharacterData(end_src);
    auto newlines = std::count(begin, end, '\n');

    if (newlines >= 512) {
        handle_long_function(srcmgr, big, 512, "function is inconceivably gargantuan (>= 512 stmts)");
    }
    else if (newlines >= 256) {
        handle_long_function(srcmgr, big, 256, "function is way too huge (>= 256 stmts)");
    }
    else if (newlines >= 128) {
        handle_long_function(srcmgr, big, 128, "function is a tad too big (>= 128 stmts)");
    }
}

void
cg3::chonktion::check_ast(std::vector<std::unique_ptr<clang::ASTUnit>>& units) {
    for (const auto& unit : units) {
        auto& ctx = unit->getASTContext();
        _finder.matchAST(ctx);
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
    if (_big_funcs.empty()) return;

    std::vector<std::string> big_funcs;
    std::vector<std::string> huge_funcs;
    std::vector<std::string> gargantuan_funcs;
    for (const auto& [size, data] : _big_funcs) {
        switch (size) {
        case 128u: big_funcs.push_back(data); continue;
        case 256u: big_funcs.push_back(data); continue;
        case 512u: big_funcs.push_back(data); continue;
        }
        assert(false);
    }

    std::fill_n(std::ostream_iterator<char>(std::cout), 80, '-');
    std::cout << "\nchonktion collected report\n";

    std::cout << "the following files contain the larger-than-wanted functions\n\n";
    print_function_set(128u, big_funcs);
    print_function_set(256u, huge_funcs);
    print_function_set(512u, gargantuan_funcs);

    std::fill_n(std::ostream_iterator<char>(std::cout), 80, '-');
    std::cout << "\n";
}

void
cg3::chonktion::handle_long_function(clang::SourceManager& srcmgr,
                                     const clang::FunctionDecl* func,
                                     unsigned f_len,
                                     std::string_view msg) {
    if (!func) return;

    auto func_name = func->getName().str();

    auto src_begin = func->getBeginLoc();

    auto begin = srcmgr.getCharacterData(src_begin);
    auto end = begin;
    while (*++end != '(');

    auto error_line = src_begin.printToString(srcmgr);

    std::cout << error_line << ": chonktion: " << msg << "\n\t";
    std::copy(begin, end, std::ostream_iterator<char>(std::cout));
    std::cout << "\n\t^";
    std::fill_n(std::ostream_iterator<char>(std::cout), func_name.size() - 1, '~');
    std::cout << "\n";

//    const llvm::Twine& func_data_twine = error_line + ": " + func_name;
//    assert(func_data_twine.isSingleStringRef());
//    auto func_data = func_data_twine.getSingleStringRef();
    auto func_data = error_line + ": " + func_name;

    _big_funcs.emplace(std::piecewise_construct,
                       std::forward_as_tuple(f_len),
                       std::forward_as_tuple(func_data.data(), func_data.size()));
}
