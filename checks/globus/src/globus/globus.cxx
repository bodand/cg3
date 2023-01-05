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
 * checks/globus/src/globus/globus --
 */

#include <iostream>

#include <globus/globus.hxx>

#include <clang/ASTMatchers/ASTMatchers.h>
using namespace clang::ast_matchers;

void
cg3::globus::add_global(std::string_view filename, std::string varname) {
    _globals.emplace(std::piecewise_construct,
                     std::forward_as_tuple(filename.data(), filename.data() + filename.size()),
                     std::forward_as_tuple(varname.data(), varname.size()));
}

void
cg3::globus::collected_report() {
    if (_globals.empty()) return;

    std::fill_n(std::ostream_iterator<char>(std::cout), 80, '-');
    std::cout << "\nglobus collected report\n";

    std::cout << "the following files contain the shown global variables\n\n";
    for (const auto& [file, var] : _globals) {
        std::cout << "\t" << file << ": " << var << "\n";
    }
    std::cout << "\n";

    std::fill_n(std::ostream_iterator<char>(std::cout), 80, '-');
    std::cout << "\n";
}

cg3::globus::globus() {
    auto check = varDecl(hasGlobalStorage(),
                         unless(isStaticLocal()),
                         isExpansionInMainFile())
                        .bind("global");

    _finder.addMatcher(check, &_global_callback);
}

void
cg3::globus::check_ast(std::vector<std::unique_ptr<clang::ASTUnit>>& units) {
    for (const auto& unit : units) {
        auto& ctx = unit->getASTContext();
        const auto& opts = unit->getLangOpts();
        auto pp = unit->getPreprocessorPtr();
        auto& diag_engine = ctx.getDiagnostics();
        auto *consumer = diag_engine.getClient();

        consumer->BeginSourceFile(opts, pp.get());

        _global_callback.configure_engine(diag_engine);
        _finder.matchAST(ctx);

        consumer->EndSourceFile();
    }
}
