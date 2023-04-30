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
 * ext/janetxx/janet_rt --
 *   Specifies the janet runtime object which acts as a RAII object around the
 *   internals of the janet VM.
 */
#ifndef CG3_JANET_RT_HXX
#define CG3_JANET_RT_HXX

#include <bit>
#include <cassert>
#include <cstdlib>
#include <string>
#include <string_view>
#include <type_traits>
#ifdef JXX_PERFORM_THREAD_CHECKS
#  include <thread>
#endif

#include <info/expected.hpp>
#include <jxx/values/types.hxx>

#include "janet.h"

namespace jxx {
    struct janet_rt {
        template<std::invocable<> Fn>
            requires(std::same_as<JanetTable*, std::invoke_result_t<Fn>>)
        static janet_rt
        delayed_load_with(Fn&& init_fn) {
            constexpr const static int overload_selector_helper = 42;
            janet_init();
            return janet_rt(std::forward<Fn>(init_fn)(), overload_selector_helper);
        }

        janet_rt(JanetTable* replacements = nullptr) {
            janet_init();
            // needs janet_init before call
            // NOLINTNEXTLINE(cppcoreguidelines-prefer-member-initializer)
            _env = janet_core_env(replacements);
        }

        janet_rt(const janet_rt&) = delete;
        janet_rt&
        operator=(const janet_rt&) = delete;

        janet_rt(janet_rt&&) noexcept = default;
        janet_rt&
        operator=(janet_rt&&) noexcept = default;

        template<class Alloc>
        [[nodiscard]] info::expected<value, value>
        exec(const std::basic_string<char, std::char_traits<char>, Alloc>& code) {
            assert_thread();
            return exec_internal(static_cast<const std::uint8_t*>(code.c_str()),
                                 static_cast<std::int32_t>(code.size()));
        }

        [[nodiscard]] info::expected<value, value>
        exec(const std::basic_string_view<char, std::char_traits<char>>& code) {
            assert_thread();
            return exec_internal(std::bit_cast<const std::uint8_t*>(code.data()),
                                 static_cast<std::int32_t>(code.size()));
        }

        ~janet_rt() noexcept {
            assert_thread();
            janet_deinit();
        }

        /** temporary, refactoring helper */
        [[deprecated("temporary")]] JanetTable*
        get_env() const {
            return _env;
        }

    private:
        /**
         * A private constructor that does not call janet_init().
         * Not to be called by not-privileged users, for they will break it.
         * Intended to be called by the static constructor helpers, who
         * therefore must call janet_init in all cases.
         */
        janet_rt(JanetTable* repl, int overload_helper)
             : _env(janet_core_env(repl)) {
            std::ignore = overload_helper;
        }

        [[nodiscard]] info::expected<value, value>
        exec_internal(const std::uint8_t* code,
                      std::int32_t code_sz) {
            if constexpr (JXX_NULL_CHECK_LVL > 0) assert(_env != nullptr
                                                         && "janet exec: invalid runtime");
            if constexpr (JXX_NULL_CHECK_LVL > 1) assert(code != nullptr
                                                         && "janet exec: code is nullptr");

            Janet result;
            auto stat = janet_dobytes(_env, code, code_sz, "runtime-object", &result);

            if (stat != JANET_SIGNAL_OK) return info::INFO_UNEXPECTED(value(result));
            return value(result);
        }

#ifdef JXX_PERFORM_THREAD_CHECKS
        void
        assert_thread() {
            assert(std::this_thread::get_id() == _created_on
                   && "janet runtime used on different thread than it was created on");
        }
#else
        void
        assert_thread() { }
#endif

#ifdef JXX_PERFORM_THREAD_CHECKS
        std::thread::id _created_on = std::this_thread::get_id();
#endif
        JanetTable* _env = nullptr;
    };
}

#endif
