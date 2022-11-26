/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-26.
 *
 * checks/bugmalloc/src/bugmalloc/invalid_malloc_callback --
 */

#include <iomanip>
#include <iostream>

#include <bugmalloc/bugmalloc.hxx>
#include <bugmalloc/invalid_malloc_callback.hxx>

void
cg3::invalid_malloc_callback::run(const clang::ast_matchers::MatchFinder::MatchResult& result) {
    auto allocator = result.Nodes.getNodeAs<clang::CallExpr>("allocator_call");
    auto src_begin = allocator->getSourceRange().getBegin();
    auto src_end = allocator->getSourceRange().getEnd();

    auto error_line = src_begin.printToString(*result.SourceManager);

    auto begin = result.SourceManager->getCharacterData(src_begin);
    auto end = result.SourceManager->getCharacterData(src_end);

    auto fun_end = std::find_if_not(begin, end + 1, [](auto ch) {
        return std::isalnum(ch);
    });
    auto fun_len = fun_end - begin;

    auto fname = result.SourceManager->getFilename(src_begin);
    _check->add_invalid_file(fname.str());

    std::cout << error_line << ": bugmalloc: unchecked call to allocating function:\n\t";
    std::copy(begin, end + 1, std::ostream_iterator<char>(std::cout));
    std::cout << "\n\t^";
    std::fill_n(std::ostream_iterator<char>(std::cout), fun_len - 1, '~');
    std::cout << "\n";

    auto fun = result.Nodes.getNodeAs<clang::FunctionDecl>("fun");
    auto called = fun->getName();
    _check->add_call(called.str(), fname.str());
}

cg3::invalid_malloc_callback::invalid_malloc_callback(cg3::bugmalloc* check)
     : _check(check) { }
