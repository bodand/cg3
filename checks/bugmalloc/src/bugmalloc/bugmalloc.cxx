/* cg3 project
 *
 * Copyright (c) 2022 András Bodor <bodand@pm.me>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * - Neither the name of the copyright holder nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Originally created: 2022-11-25.
 *
 * checks/bugmalloc/src/bugmalloc --
 */

#include <filesystem>
#include <iostream>
#include <string_view>

#include <bugmalloc/bugmalloc.hxx>
#include <cg3-common/hash_storages.hxx>

#include <clang/ASTMatchers/ASTMatchers.h>

using namespace clang::ast_matchers;

namespace {
    template<class... Names>
    auto
    fun_bindings(Names&&... names) {
        // NOLINTNEXTLINE(hicpp-no-array-decay)
        return eachOf(callee(functionDecl(hasName(std::forward<Names>(names))).bind("fun"))...);
    }

    void
    write_missing_debugmallocs(const std::unordered_set<std::filesystem::path>& files) {
        if (files.empty()) return;
        std::cout << "`debugmalloc.h' at least partially missing in the following sources: \n\n\t";
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
    constexpr const static auto terminal_width = 80;
    if (_files_to_report.empty()
        && _tricky_functions.empty()
        && any_called) return;

    std::fill_n(std::ostream_iterator<char>(std::cout), terminal_width, '-');
    std::cout << "\nbugmalloc collected report\n";

    if (!any_called) {
        std::cout << "program does not even try to allocate memory\n";
    }
    write_missing_debugmallocs(_files_to_report);
    write_probably_getarounds(_tricky_functions);

    std::fill_n(std::ostream_iterator<char>(std::cout), terminal_width, '-');
    std::cout << "\n";
}

void
cg3::bugmalloc::add_invalid_file(std::string_view filename) {
    _files_to_report.emplace(filename.data(),
                             filename.data() + filename.size());
}

void
cg3::bugmalloc::add_call(const std::string& fun, std::string_view filename) {
    any_called = true;
    add_invalid_file(filename);

    // if fun is a standard function, just ignore it
    if (auto it = _standard_funcs.find(fun);
        it != _standard_funcs.end()) return;

    _tricky_functions.emplace(std::piecewise_construct,
                              std::forward_as_tuple(fun.data(), fun.size()),
                              std::forward_as_tuple(filename.data(), filename.data() + filename.size()));
}

cg3::bugmalloc::bugmalloc(clang::DiagnosticsEngine* diag)
     : check(diag) {
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
    auto macro_check = callExpr(anyOf(isExpandedFromMacro("malloc"),
                                      isExpandedFromMacro("calloc"),
                                      isExpandedFromMacro("realloc"),
                                      isExpandedFromMacro("free")))
                              .bind("hijacked_call");

    _finder.addMatcher(check, &_malloc_callback);
    _finder.addMatcher(macro_check, &_malloc_callback);
}

void
cg3::bugmalloc::hijacked_call() {
    any_called = true;
}

void
cg3::bugmalloc::match_ast(clang::ASTContext& context) {
    _finder.matchAST(context);
}
