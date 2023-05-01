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
 * Originally created: 2023-05-01.
 *
 * ext/janetxx/src/jxx/janet_runtime_exception --
 *   Defines the janet_runtime_exception; a subclass of exception super-classing
 *   all exception types which may be safely thrown from a janet C function.
 */
#ifndef CG3_JANET_RUNTIME_EXCEPTION_HXX
#define CG3_JANET_RUNTIME_EXCEPTION_HXX

#include <exception>

namespace jxx {
    /**
     * A superclass for all exception that may be safely thrown from a janet
     * C function.
     * These provide a capability to turn the C++ exception into a Janet VM
     * panic.
     */
    struct janet_runtime_exception : std::exception {
        /**
         * Turns the current exception into a Janet VM panic.
         *
         * Needs to consider that destructors may not be called properly when
         * a janet_panic* is called.
         */
        [[noreturn]] virtual void
        trigger_panic() const noexcept = 0;
    };
}

#endif
