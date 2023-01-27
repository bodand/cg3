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
 * src/cg3/check --
 */

#include <chk3/check.hxx>

namespace {
    struct source_file_processing {
        explicit source_file_processing(clang::ASTUnit* unit)
             : _consumer(unit->getASTContext().getDiagnostics().getClient()) {
            const auto& opts = unit->getLangOpts();
            const auto& pp = unit->getPreprocessorPtr().get();
            _consumer->BeginSourceFile(opts, pp);
        }

        source_file_processing(const source_file_processing&) = delete;
        source_file_processing&
        operator=(const source_file_processing&) = delete;
        source_file_processing(source_file_processing&&) noexcept = delete;
        source_file_processing&
        operator=(source_file_processing&&) noexcept = delete;

        ~source_file_processing() {
            _consumer->EndSourceFile();
        }

    private:
        clang::DiagnosticConsumer* _consumer;
    };
}

void
cg3::check::match_ast([[maybe_unused]] clang::ASTContext& context) {
    /* nop by default */
}

void
cg3::check::check_ast(std::vector<std::unique_ptr<clang::ASTUnit>>& units) {
    for (const auto& unit : units) {
        assert(unit.get() != nullptr);

        auto& ctx = unit->getASTContext();

        const source_file_processing proc_handle(unit.get());
        match_ast(ctx);
    }
}
