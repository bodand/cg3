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
 * Originally created: 2023-04-28.
 *
 * ext/janetxx/src/jxx/values/boolean --
 *   
 */
#ifndef CG3_BOOLEAN_HXX
#define CG3_BOOLEAN_HXX

#include <jxx/values/types.hxx>

#include <janet.h>

namespace jxx {
    struct boolean {
        constexpr explicit(false) boolean(bool b) noexcept : _value(b) { }

        explicit boolean(const value& val) noexcept {
#ifdef JXX_CHECK_TYPE_CTORS
            assert(x.is<std::remove_cvref_t<decltype(this)>>()
                   && "janet function: trying to construct an integer from a non-integer");
#endif
            _value = janet_unwrap_boolean(val._janet);
        }

        constexpr operator bool() noexcept { return _value; }
        constexpr operator bool() const noexcept { return _value; }

        constexpr bool
        to_native() noexcept { return _value; }
        constexpr bool
        to_native() const noexcept { return _value; }

    private:
        bool _value;
    };
}

#endif
