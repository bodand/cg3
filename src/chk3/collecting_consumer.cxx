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
 *  - Redistributions in binary form must reproduce the above copyright notice,
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
 * src/chk3/collecting_consumer --
 */

#include <algorithm>
#include <filesystem>
#include <iterator>
#include <string>

#include <chk3/collecting_consumer.hxx>

#include <clang/Basic/SourceManager.h>
#include <llvm/ADT/SmallString.h>

namespace fs = std::filesystem;

namespace {
    std::string
    find_relevant_code(unsigned line, llvm::MemoryBufferRef file_buf) {
        constexpr const static auto context_size = 3;

        // get lines [line - context_size; min(line + context_size, buffer_lines)]
        const auto begin_line = line - context_size;
        const auto end_line = line + context_size;

        const auto* buf_pos = file_buf.getBufferStart();
        auto line_cnt = 0U;
        do {
            std::advance(buf_pos, 1);
            if (buf_pos == file_buf.getBufferEnd()) return "";
            buf_pos = std::find(buf_pos, file_buf.getBufferEnd(), '\n');
            if (buf_pos == file_buf.getBufferEnd()) return ""; // something is not right
            ++line_cnt;
        } while (line_cnt != begin_line);

        const auto* const buf_begin = buf_pos;
        do {
            std::advance(buf_pos, 1);
            if (buf_pos == file_buf.getBufferEnd()) break;
            buf_pos = std::find(buf_pos, file_buf.getBufferEnd(), '\n');
            if (buf_pos == file_buf.getBufferEnd()) break;
            ++line_cnt;
        } while (line_cnt != end_line);

        std::string buf;
        buf.reserve(static_cast<std::size_t>(buf_pos - buf_begin));
        std::copy(buf_begin, buf_pos, std::back_inserter(buf));

        return buf;
    }
}

cg3::collecting_consumer::collecting_consumer(cg3::diagnostics_collection* collection)
     : _collection(collection) {
    assert(_collection);
}

void
cg3::collecting_consumer::HandleDiagnostic(clang::DiagnosticsEngine::Level DiagLevel,
                                           const clang::Diagnostic& Info) {
    auto&& diags = Info.getDiags();
    auto&& src_mgr = diags->getSourceManager();

    auto id = Info.getID();
    auto maybe_check = _collection->get_check_from_diagnostic(id);
    if (!maybe_check) return;
    auto check = *maybe_check;

    auto location = Info.getLocation();
    auto filename = src_mgr.getFilename(location);
    auto line = src_mgr.getPresumedLineNumber(location);
    auto column = src_mgr.getPresumedColumnNumber(location);

    auto file_id = src_mgr.getFileID(location);
    auto maybe_file_buf = src_mgr.getBufferOrNone(file_id);
    std::string diag_context;
    if (maybe_file_buf) diag_context = find_relevant_code(line, *maybe_file_buf);

    constexpr const static auto buffer_stack_size = 512; // based on basically nothing
    llvm::SmallString<buffer_stack_size> message_buf;
    Info.FormatDiagnostic(message_buf);

    const auto diag = diagnostic(DiagLevel,
                                 fs::path(filename.data(), filename.data() + filename.size()),
                                 line,
                                 column,
                                 message_buf.str().str(),
                                 diag_context);

    ++NumWarnings;
    if (DiagLevel == clang::DiagnosticsEngine::Note) {
        assert(_last_chain && "note generated when there is no diagnostic to chain it to");

        _last_chain = _collection->chain_diagnostic(*_last_chain, diag);
        return;
    }
    _last_chain = _collection->chain_diagnostic(check, diag);
}
