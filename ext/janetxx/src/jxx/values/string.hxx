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
 * ext/janetxx/src/jxx/values/string --
 *   
 */
#ifndef CG3_STRING_HXX
#define CG3_STRING_HXX

#include <algorithm>
#include <iterator>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>

#include <jxx/values/types.hxx>

#include <janet.h>

namespace jxx {
    struct string {
        constexpr string(std::string_view str)
             : _impl(str.data(), str.size()) { }

        explicit string(value x) {
#ifdef JXX_CHECK_TYPE_CTORS
            assert(x.is<std::remove_cvref_t<decltype(this)>>()
                   && "janet function: trying to construct a function from a non-function");
#endif
            const auto* const str = janet_unwrap_string(x._janet);
            const auto buf_len = janet_string_length(str);
            const auto* const buf = janet_string_head(str)->data;

            _impl.reserve(buf_len);
            std::ranges::copy_n(buf, buf_len, std::back_inserter(_impl));
        }

        constexpr std::string_view
        to_native() noexcept { return {_impl.data(), _impl.size()}; }
        constexpr std::string_view
        to_native() const noexcept { return {_impl.data(), _impl.size()}; }

    private:
        std::string _impl{};
    };
}

#endif
