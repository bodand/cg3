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
 * ext/janetxx/src/jxx/func --
 *   
 */
#ifndef CG3_FUNC_HXX
#define CG3_FUNC_HXX

#include <jxx/impl/dissector.hxx>
#include <jxx/janet_rt.hxx>

#include <janet.h>

namespace jxx {
    namespace meta {
        template<class T>
        using native_to_janet_t = typename janet_traits<T>::janet_type;

        template<class>
        struct size;

        template<template<class...> class L, class... Args>
        struct size<L<Args...>> : std::integral_constant<std::size_t, sizeof...(Args)> { };

        template<class L>
        constexpr const static auto size_v = size<L>::value;

        template<class, class>
        struct add_head;

        template<template<class...> class L, class T, class... Args>
        struct add_head<T, L<Args...>> {
            using type = L<T, Args...>;
        };

        template<class>
        struct separate_optionals;

        template<template<class...> class L>
        struct separate_optionals<L<>> {
            using requireds = L<>;
            using optionals = L<>;
        };

        template<template<class...> class L, class T, class... Args>
        struct separate_optionals<L<T, Args...>> {
            using data = separate_optionals<L<Args...>>;
            using requireds = typename add_head<T, typename data::requireds>::type;
            using optionals = typename data::optionals;
        };

        template<template<class...> class L, class T, class... Args>
        struct separate_optionals<L<std::optional<T>, Args...>> {
            using data = separate_optionals<L<Args...>>;
            using requireds = typename data::requireds;
            using optionals = typename add_head<T, typename data::optionals>::type;
        };

        template<class>
        struct assert_trailing_optionals;

        template<class>
        struct assert_optionals;

        template<template<class...> class L>
        struct assert_trailing_optionals<L<>> : std::true_type { };

        template<template<class...> class L>
        struct assert_optionals<L<>> : std::true_type { };

        template<template<class...> class L, class T, class... Args>
        struct assert_trailing_optionals<L<T, Args...>>
             : std::bool_constant<assert_trailing_optionals<L<Args...>>::value> { };

        template<template<class...> class L, class T, class... Args>
        struct assert_trailing_optionals<L<std::optional<T>, Args...>>
             : std::bool_constant<assert_optionals<L<Args...>>::value> { };

        template<template<class...> class L, class T, class... Args>
        struct assert_optionals<L<std::optional<T>, Args...>>
             : std::bool_constant<assert_optionals<L<Args...>>::value> { };

        template<template<class...> class L, class T, class... Args>
        struct assert_optionals<L<T, Args...>>
             : std::false_type { };
    }

    template<class Fn>
    constexpr auto
    wrap_function(Fn&& fn)
        requires(meta::is_typed_callback<Fn>)
    {
        using input_types = meta::dissect<Fn>;
        static_assert(meta::assert_trailing_optionals<input_types>::value,
                      "optionals may only be present at the end of the parameter list");

        using parameters = typename meta::tmap<meta::native_to_janet_t, input_types>::type;
        using separator = meta::separate_optionals<input_types>;
        using required_parameters = typename separator::requireds;
        using optional_parameters = typename separator::optionals;

        return [fn = std::forward<Fn>(fn)](int argc, Janet* argv) {
            constexpr const int max_size = meta::size_v<parameters>;
            constexpr const int min_size = meta::size_v<required_parameters>;
            constexpr const int opt_size = meta::size_v<optional_parameters>;
            if constexpr (max_size == min_size) {
                janet_fixarity(argc, max_size);
            }
            else {
                janet_arity(argc, min_size, max_size);
            }

            auto mk_args_tuple =
                   [argc, argv]<class... ReqTypes,
                                class... OptTypes,
                                int... Is,
                                int... Js>(meta::tlist<ReqTypes...>,
                                           meta::tlist<OptTypes...>,
                                           std::integer_sequence<int, Is...>,
                                           std::integer_sequence<int, Js...>) {
                       return std::tuple<ReqTypes..., std::optional<OptTypes>...>(
                              janet_traits<ReqTypes>::to_native(argv[Is])...,
                              (Js < argc ? std::optional(janet_traits<OptTypes>::to_native(argv[Js])) : std::nullopt)...);
                   };
            return std::apply(fn,
                              mk_args_tuple(required_parameters{},
                                            optional_parameters{},
                                            std::make_integer_sequence<int, min_size>{},
                                            std::make_integer_sequence<int, opt_size>{}));
        };
    }
}

#define JXX_CALL_WRAPPED_ARGS(argc, argv, ...) \
    jxx::wrap_function(__VA_ARGS__)(argc, argv)

#define JXX_WRAP(...) \
    return JXX_CALL_WRAPPED_ARGS(argc, argv, __VA_ARGS__)

#endif
