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
 * src/chk3/diagnostics_exporter --
 *   The base class used to represent the different exporters, which may be used
 */
#ifndef CG3_DIAGNOSTICS_EXPORTER_HXX
#define CG3_DIAGNOSTICS_EXPORTER_HXX

#include <chk3/diagnostics_collection.hxx>

namespace cg3 {
    struct diagnostics_exporter {
        virtual void
        export_diagnostics(chain_iterator begin, chain_iterator end) = 0;

        diagnostics_exporter() = default;

        diagnostics_exporter(const diagnostics_exporter& cp) = delete;
        diagnostics_exporter(diagnostics_exporter&& mv) noexcept = delete;

        diagnostics_exporter&
        operator=(const diagnostics_exporter& cp) = delete;
        diagnostics_exporter&
        operator=(diagnostics_exporter&& mv) noexcept = delete;

        virtual ~diagnostics_exporter() noexcept = default;
    };
}

#endif //CG3_DIAGNOSTICS_EXPORTER_HXX
