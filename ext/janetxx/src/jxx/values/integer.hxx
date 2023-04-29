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
 * ext/janetxx/src/jxx/values/integer --
 *   
 */
#ifndef CG3_INTEGER_HXX
#define CG3_INTEGER_HXX

#include <type_traits>

#include <jxx/values/types.hxx>

#include <janet.h>

namespace jxx {
    template<int Size>
    struct integer {
        static_assert(Size == 32 || Size == 64, "integer may only be 32 or 64 bit long");
        using underlying_type = std::conditional_t<Size == 32,
                                                   std::int32_t,
                                                   std::int64_t>;

        constexpr explicit(false) integer(underlying_type val) noexcept : _value(val) { }

        explicit integer(const value& val) noexcept {
#ifdef JXX_CHECK_TYPE_CTORS
            assert(x.is<std::remove_cvref_t<decltype(this)>>()
                   && "janet function: trying to construct an integer from a non-integer");
#endif
            unwrap(val._janet, _value);
        }

        [[nodiscard]] constexpr explicit(Size != 32) operator integer<32>() noexcept {
            return _value;
        }
        [[nodiscard]] constexpr explicit(false) operator integer<64>() noexcept {
            return static_cast<std::int64_t>(_value);
        }

        operator value() noexcept { return wrap(_value); }
        operator const value() const noexcept { return wrap(_value); }

        constexpr explicit operator underlying_type() noexcept { return _value; }
        constexpr explicit operator underlying_type() const noexcept { return _value; }

        constexpr underlying_type
        to_native() noexcept { return _value; }
        constexpr underlying_type
        to_native() const noexcept { return _value; }

    private:
        static void
        unwrap(Janet value, std::int32_t& res) noexcept {
            res = janet_unwrap_integer(value);
        }
        static void
        unwrap(Janet value, std::int64_t& res) noexcept {
            res = janet_unwrap_s64(value);
        }

        static Janet
        wrap(std::int32_t res) noexcept {
            return janet_wrap_integer(res);
        }
        static Janet
        wrap(std::int64_t res) noexcept {
            return janet_wrap_s64(res);
        }

        underlying_type _value;
    };


    template<int Size_>
    constexpr bool
    operator==(integer<Size_> a, integer<Size_> b) {
        return a.to_native() == b.to_native();
    }

    template<int Size1, int Size2>
    constexpr bool
    operator==(integer<Size1> a, integer<Size2> b)
        requires(Size1 < Size2)
    {
        return static_cast<typename integer<Size2>::underlying_type>(a.to_native()) == b.to_native();
    }

    template<int Size1, int Size2>
    constexpr bool
    operator==(integer<Size1> a, integer<Size2> b)
        requires(Size2 < Size1)
    {
        return a.to_native() == static_cast<typename integer<Size1>::underlying_type>(b.to_native());
    }

    template<int Size_>
    constexpr bool
    operator==(integer<Size_> a, typename integer<Size_>::underlying_type b) {
        return a.to_native() == b;
    }

    template<int Size_>
    constexpr bool
    operator==(typename integer<Size_>::underlying_type a, integer<Size_> b) {
        return a == b.to_native();
    }

    template<int Size_>
    constexpr bool
    operator!=(integer<Size_> a, integer<Size_> b) {
        return !(a == b);
    }

    template<int Size1, int Size2>
    constexpr bool
    operator!=(integer<Size1> a, integer<Size2> b) {
        return !(a == b);
    }

    template<int Size_>
    constexpr bool
    operator!=(integer<Size_> a, typename integer<Size_>::underlying_type b) {
        return !(a == b);
    }

    template<int Size_>
    constexpr bool
    operator!=(typename integer<Size_>::underlying_type a, integer<Size_> b) {
        return !(a == b);
    }

}

#endif
