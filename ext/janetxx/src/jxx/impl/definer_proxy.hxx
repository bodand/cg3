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
 * Originally created: 2023-04-30.
 *
 * ext/janetxx/src/jxx/impl/definer_proxy --
 *   
 */
#ifndef CG3_DEFINER_PROXY_HXX
#define CG3_DEFINER_PROXY_HXX

#include <concepts>
#include <functional>

#include <jxx/values/types.hxx>

#include <janet.h>

extern "C" void
janet_registry_put(
       JanetCFunction key,
       const char* name,
       const char* name_prefix,
       const char* source_file,
       int32_t source_line);

namespace jxx::impl {
    struct definer_proxy {
        template<class Fn>
        definer_proxy(Fn&& fn, JanetTable* jrt_env, std::string_view name)
             : _deathEvent(std::forward<Fn>(fn)),
               _jrt_env(jrt_env),
               _name(name) {
            if constexpr (JXX_NULL_CHECK_LVL > 0) // NOLINT
                assert(jrt_env != nullptr
                       && "proxy created for null runtime");
        }

        definer_proxy(const definer_proxy& cp) = delete;
        definer_proxy&
        operator=(const definer_proxy& cp) = delete;

        definer_proxy(definer_proxy&& mv) noexcept = delete;
        definer_proxy&
        operator=(definer_proxy&& mv) noexcept = delete;

        decltype(auto)
        operator[](std::string_view docs) {
            _docs = docs;
            return *this;
        }

        void
        operator=(const value& jvalue) noexcept {
            janet_var(_jrt_env, _name.data(), static_cast<Janet>(jvalue), _docs ? _docs->data() : nullptr);
        }

        void
        operator=(JanetCFunction fn) noexcept {
            janet_def(_jrt_env, _name.data(), janet_wrap_cfunction(fn), _docs ? _docs->data() : nullptr);
            janet_registry_put(fn, _name.data(), "cg3", nullptr, 0);
        }

        ~definer_proxy() {
#ifdef JXX_CHECK_DANGLING_RT_PROXY
            _deathEvent();
#endif
        }

    private:
        std::optional<std::string_view> _docs = std::nullopt;
        std::function<void()> _deathEvent;
        JanetTable* _jrt_env;
        std::string_view _name;
    };
}

#endif
