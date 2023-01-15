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
    auto&& nodes = result.Nodes;
    if (nodes.getNodeAs<clang::CallExpr>("hijacked_call")) {
        _check->hijacked_call();
        return;
    }

    const auto* allocator = nodes.getNodeAs<clang::CallExpr>("allocator_call");
    const auto* fun = nodes.getNodeAs<clang::FunctionDecl>("fun");

    auto&& diag = result.Context->getDiagnostics();
    auto&& srcmgr = result.SourceManager;

    auto begin_loc = allocator->getBeginLoc();
    auto report = diag.Report(begin_loc, _diagnostic_id);

    report.AddString(fun->getName());
    report.AddSourceRange(clang::CharSourceRange::getCharRange(
           allocator->getSourceRange()));

    auto called = fun->getName();
    auto fname = srcmgr->getFilename(begin_loc);
    _check->add_call(called.str(), fname.str());
}

cg3::invalid_malloc_callback::invalid_malloc_callback(cg3::bugmalloc* check)
     : _check(check) { }

void
cg3::invalid_malloc_callback::configure_engine(clang::DiagnosticsEngine& diag_engine) {
    _diagnostic_id = diag_engine.getCustomDiagID(clang::DiagnosticsEngine::Warning,
                                                 "unchecked call to allocating function '%0'");
}
