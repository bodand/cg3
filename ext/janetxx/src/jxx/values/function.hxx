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
 * ext/janetxx/src/jxx/values/function --
 *   
 */
#ifndef CG3_FUNCTION_HXX
#define CG3_FUNCTION_HXX

#include <cassert>
#include <vector>

#include <info/expected.hpp>
#include <jxx/values/types.hxx>

#include <janet.h>

namespace jxx {
    struct function {
        explicit function(value x) {
#ifdef JXX_CHECK_TYPE_CTORS
            assert(x.is<function>()
                   && "janet function: trying to construct a function from a non-function");
#endif
            _impl = janet_unwrap_function(x._janet);
        }

        operator value() {
            if constexpr (JXX_NULL_CHECK_LVL > 1) // NOLINT configuration
                assert(_impl != nullptr
                       && "janet function: trying to convert invalid function to generic value");
            return janet_wrap_function(_impl);
        }

        operator const value() const {
            if constexpr (JXX_NULL_CHECK_LVL > 1) // NOLINT configuration
                assert(_impl != nullptr
                       && "janet function: trying to convert invalid function to generic value");
            return janet_wrap_function(_impl);
        }

        template<std::same_as<value>... Values>
        info::expected<value, value>
        operator()(Values&&... values) {
            if constexpr (JXX_NULL_CHECK_LVL > 0) // NOLINT configuration
                assert(_impl != nullptr
                       && "janet function: trying to call null janet function");

            std::vector<Janet> args;
            args.reserve(sizeof...(values));
            (args.push_back(values), ...);

            Janet result;

            auto stat = janet_pcall(_impl, sizeof...(values), args.data(), &result, nullptr);
            if (stat != JANET_SIGNAL_OK) return info::INFO_UNEXPECTED{value(result)};
            return value(result);
        }

    private:
        JanetFunction* _impl = nullptr;
    };
}

#endif
