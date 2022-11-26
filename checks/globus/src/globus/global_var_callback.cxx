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
 * checks/globus/src/globus/global_var_callback --
 */
#include <iostream>

#include <globus/global_var_callback.hxx>
#include <globus/globus.hxx>

void
cg3::global_var_callback::run(const clang::ast_matchers::MatchFinder::MatchResult& result) {
    auto var = result.Nodes.getNodeAs<clang::VarDecl>("global");

    auto name = var->getName();
    auto src_begin = var->getSourceRange().getBegin();
    auto src_end = var->getSourceRange().getEnd();

    auto error_line = src_begin.printToString(*result.SourceManager);

    auto begin = result.SourceManager->getCharacterData(src_begin);
    auto end = result.SourceManager->getCharacterData(src_end);
    if (src_end.isInvalid()) {
        auto fileid = result.SourceManager->getFileID(src_begin);
        auto file_end_src = result.SourceManager->getLocForEndOfFile(fileid);
        auto file_end = result.SourceManager->getCharacterData(file_end_src);
        end = std::find(begin, file_end, ';') + 1;
    }

    auto name_it = std::search(begin, end, name.begin(), name.end());
    auto shift_varname = name_it - begin;

    std::cout << error_line << ": globus: global variable:\n\t";
    std::copy(begin, end, std::ostream_iterator<char>(std::cout));
    std::cout << "\n\t";
    std::fill_n(std::ostream_iterator<char>(std::cout), shift_varname, ' ');
    std::cout << "^";
    std::fill_n(std::ostream_iterator<char>(std::cout), name.size() - 1, '~');
    std::cout << "\n";

    auto filename = result.SourceManager->getFilename(src_begin);
    _globus->add_global(filename.str(), name.str());
}

cg3::global_var_callback::global_var_callback(cg3::globus* globus)
     : _globus(globus) { }
