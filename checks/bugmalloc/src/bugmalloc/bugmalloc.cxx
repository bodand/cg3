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
}

cg3::bugmalloc::bugmalloc(clang::DiagnosticsEngine* diag)
     : typed_check<cg3::check_types::bugmalloc>(diag) {
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
cg3::bugmalloc::match_ast(clang::ASTContext& context) {
    _finder.matchAST(context);
}
