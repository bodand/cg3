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
 * ext/janetxx/src/jxx/values/types --
 *   
 */
#ifndef CG3_TYPES_HXX
#define CG3_TYPES_HXX

#include <cstdint>
#include <string>
#include <string_view>
#include <variant>

#include <janet.h>

namespace jxx {
    template<class T>
    struct janet_traits;

    struct function;
    template<int Size = 32>
    struct integer;
    struct boolean;
    struct string;

    template<>
    struct janet_traits<std::int32_t> {
        static std::int32_t
        to_native(Janet j) { return janet_unwrap_integer(j); }
        using janet_type = integer<32>;
        constexpr const static auto runtime_types = JANET_TFLAG_NUMBER;
    };
    template<>
    struct janet_traits<std::int64_t> {
        static std::int64_t
        to_native(Janet j) { return janet_unwrap_s64(j); }
        using janet_type = integer<64>;
        constexpr const static auto runtime_types = JANET_TFLAG_NUMBER;
    };
    template<>
    struct janet_traits<bool> {
        static bool
        to_native(Janet j) { return janet_unwrap_boolean(j); }
        using janet_type = boolean;
        constexpr const static auto runtime_types = JANET_TFLAG_BOOLEAN;
    };
    template<>
    struct janet_traits<std::string_view> {
        static std::string
        to_native(Janet j) {
            return reinterpret_cast<const char*>(janet_unwrap_string(j));
        }
        using janet_type = string;
        constexpr const static auto runtime_types = JANET_TFLAG_STRING;
    };
    template<class Inner>
    struct janet_traits<std::optional<Inner>> {
        static std::optional<Inner>
        to_native(Janet j) {
            if (janet_checktype(j, JANET_NIL)) return std::nullopt;
            return janet_traits<Inner>::to_native(j);
        }
        using janet_type = typename janet_traits<Inner>::janet_type;
        constexpr const static auto runtime_types = JANET_TFLAG_NIL | janet_traits<Inner>::runtime_type;
    };

    /**
     * An opaque type representing a janet value.
     * For any meaningful access, construct one of the specific classes that
     * represent a meaningful value in the janet vm.
     *
     * Provides helpers, to easily query the type of the stored object, based on
     * the wrapper classes provided in the library.
     */
    struct value {
        value() noexcept : _janet() { }

        explicit(false) value(const Janet& janet) noexcept : _janet(janet) { }

        template<class T>
        [[nodiscard]] bool
        is() const noexcept {
            static_assert(!std::same_as<T, T>,
                          "trying to call invalid is() check function on janet value");
        }

        template<>
        [[nodiscard]] bool
        is<function>() const noexcept {
            return janet_checktypes(_janet, JANET_TFLAG_FUNCTION);
        }

        template<>
        [[nodiscard]] bool
        is<integer<32>>() const noexcept {
            return janet_checktypes(_janet, JANET_TFLAG_NUMBER)
                   && janet_checkint(_janet);
        }

        template<>
        [[nodiscard]] bool
        is<integer<64>>() const noexcept {
            return janet_checktypes(_janet, JANET_TFLAG_NUMBER)
                   && janet_checkint64(_janet);
        }

        template<>
        [[nodiscard]] bool
        is<boolean>() const noexcept {
            return janet_checktypes(_janet, JANET_TFLAG_BOOLEAN);
        }

        template<>
        [[nodiscard]] bool
        is<string>() const noexcept {
            return janet_checktypes(_janet, JANET_TFLAG_STRING);
        }

        explicit operator Janet() noexcept { return _janet; }
        explicit operator Janet() const noexcept { return _janet; }

    private:
        friend function;
        template<int Size>
        friend struct integer;
        friend boolean;
        friend string;

        Janet _janet;
    };
}

#endif
