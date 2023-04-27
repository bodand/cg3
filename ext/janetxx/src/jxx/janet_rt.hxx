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

#include <cassert>
#ifdef JXX_PERFORM_THREAD_CHECKS
#  include <thread>
#endif

#include "janet.h"

namespace jxx {
    struct janet_rt {
        janet_rt() {
            janet_init();
            // needs janet_init before call
            // NOLINTNEXTLINE(cppcoreguidelines-prefer-member-initializer)
            _env = janet_core_env(nullptr);
        }

        janet_rt(const janet_rt&) = delete;
        janet_rt&
        operator=(const janet_rt&) = delete;

        janet_rt(janet_rt&&) noexcept = default;
        janet_rt&
        operator=(janet_rt&&) noexcept = default;

        ~janet_rt() noexcept {
            assert_thread();
            janet_deinit();
        }

    private:
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
