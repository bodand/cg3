/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-25.
 *
 * checks/bugmalloc/src/bugmalloc --
 */

#include <filesystem>
#include <iostream>
#include <string_view>
#include <unordered_set>

#include <bugmalloc/bugmalloc.hxx>

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>

using namespace clang::ast_matchers;

namespace {
    template<class... Names>
    auto
    fun_bindings(Names&&... names) {
        return eachOf(callee(functionDecl(hasName(std::forward<Names>(names))).bind("fun"))...);
    }
}

MatchFinder*
cg3::bugmalloc::create_finder() {
    auto not_in_dbgm = unless(isExpansionInFileMatching("debugmalloc\\.h"));

    // clang-format off
    auto check = callExpr(fun_bindings(// supported
                                       "malloc", "calloc", "realloc", "free",
                                       // C23, debugmalloc fails with them
                                       "strdup", "strndup",
                                       // POSIX platform
                                       "mmap", "munmap",
                                       "brk", "sbrk",
                                       // Win32 platform
                                       "LocalAlloc", "GlobalAlloc",
                                       "LocalFree", "GlobalFree",
                                       "VirtualAlloc", "VirtualAlloc2",
                                       "VirtualFree",
                                       "VirtualAllocEx",
                                       "VirtualFreeEx"),
                          not_in_dbgm,
                          isExpansionInMainFile())
                        .bind("allocator_call");
    // clang-format on

    _finder.addMatcher(check, &_malloc_callback);
    return &_finder;
}

namespace {
    void
    write_missing_debugmallocs(const std::unordered_set<std::filesystem::path>& files) {
        if (files.empty()) return;
        std::cout << "`debugmalloc.h' missing in the following sources: \n\n\t";
        std::copy(files.begin(),
                  files.end(),
                  std::ostream_iterator<std::filesystem::path>(std::cout, "\n\t"));
        std::cout << "\n";
    }

    void
    write_probably_getarounds(const std::unordered_multimap<std::string,
                                                            std::filesystem::path>& funcs) {
        if (funcs.empty()) return;

        std::cout << "The following files call memory management functions which are non-trivial:\n\n";
        for (const auto& [func, file] : funcs) {
            std::cout << "\t" << file << ": " << func << "\n";
        }
        std::cout << "\n";
    }
}

void
cg3::bugmalloc::collected_report() {
    if (_files_to_report.empty()
        && _tricky_functions.empty()
        && !any_called) return;

    std::fill_n(std::ostream_iterator<char>(std::cout), 80, '-');
    std::cout << "\nbugmalloc collected report\n";

    if (!any_called) {
        std::cout << "program does not even try to allocate memory\n";
    }
    write_missing_debugmallocs(_files_to_report);
    write_probably_getarounds(_tricky_functions);

    std::fill_n(std::ostream_iterator<char>(std::cout), 80, '-');
}

void
cg3::bugmalloc::add_invalid_file(std::string_view filename) {
    _files_to_report.emplace(filename.data(),
                             filename.data() + filename.size());
}

void
cg3::bugmalloc::add_call(const std::string& fun, std::string_view filename) {
    any_called = true;
    // if fun is a standard function, just ignore it
    if (auto it = _standard_funcs.find(fun);
        it != _standard_funcs.end()) return;

    _tricky_functions.emplace(std::piecewise_construct,
                              std::forward_as_tuple(fun.data(), fun.size()),
                              std::forward_as_tuple(filename.data(), filename.data() + filename.size()));
}
