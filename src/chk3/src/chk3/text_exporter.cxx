/* cg3 project
 *
 * Copyright (c) 2023 András Bodor <bodand@pm.me>
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
 * Originally created: 2023-01-30.
 *
 * src/chk3/text_exporter --
 *   
 */

#include <algorithm>
#include <ostream>

#include <chk3/text_exporter.hxx>
#include <magic_enum.hpp>

namespace {
    void
    export_chain(std::ostream& os, const cg3::diagnostics_collection::diag_chain_type& chain) {
        if (chain.empty()) return;
        auto&& diag = chain.front();
        os << "\t" << diag.filename.string() << ":" << diag.line << ":" << diag.column << ": " << diag.output_text << "\n";

        std::for_each(std::next(chain.begin()), chain.end(), [&os, notes = false](const auto& note) mutable {
            if (!notes) {
                os << "\tadditional notes:\n";
                notes = true;
            }
            os << "\t\t" << note.filename.string() << ":" << note.line << ":" << note.column << ": " << note.output_text << "\n";
        });
    }

    void
    print_check(std::ostream& os, cg3::chain_iterator& ps, cg3::chain_iterator& end, const cg3::check_types& current_type) {
        os << "\ncheck " << std::quoted(magic_enum::enum_name(current_type)) << " collected report:\n";
        std::for_each(ps, end, [&os](const auto& chain) {
            export_chain(os, chain);
        });
    }
}

void
cg3::text_exporter::export_diagnostics(cg3::chain_iterator begin,
                                       cg3::chain_iterator end) {
    if (begin == end) return;

    auto current_type = begin.current_check_type();
    auto ps = begin;
    for (; begin != end; ++begin) {
        if (begin.current_check_type() != current_type) {
            print_check(_os, ps, begin, current_type);
            current_type = begin.current_check_type();
            ps = begin;
        }
    }
    print_check(_os, ps, end, current_type);
}
