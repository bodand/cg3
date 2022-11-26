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
 * src/cg3/checks --
 *   Incomplete file to be configured by CMake to contain all built checks in an
 *   enum.
 */
#ifndef CG3_CHECKS_IN_HXX
#define CG3_CHECKS_IN_HXX

#include <utility>

#include <llvm/Support/CommandLine.h>

namespace cg3 {
    enum class check_types : int {
        @CG3_CHECK_TYPES_ENUM@,
        COUNT
    };

    template<check_types>
    struct check_description_impl;

    @CG3_CHECK_TYPES_DESCRIPTION_IMPLS@

           template<>
           struct check_description_impl<check_types::COUNT> {
        constexpr const static auto value = "run all compiled checks";
    };

    template<check_types Type>
    constexpr const static auto check_description = check_description_impl<Type>::value;

    template<check_types>
    struct check_name_impl;

    @CG3_CHECK_TYPES_NAME_IMPLS@

           template<>
           struct check_name_impl<check_types::COUNT> {
        constexpr const static auto value = "complete";
    };

    template<check_types Type>
    constexpr const static auto check_name = check_name_impl<Type>::value;

    template<class M, check_types... checks>
    struct make_options_impl {
        auto
        operator()() {
            return maker(
                   clEnumValN(checks, check_name<checks>, check_description<checks>)...);
        }

        M maker;
    };

    template<class M>
    using make_options = make_options_impl<M, @CG3_CHECK_TYPES_NAMESPACED@, check_types::COUNT>;
}

#endif
