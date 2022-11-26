/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-25.
 *
 * src/cg3/checks --
 *   Incomplete file to be configured by CMake to contain all built checks in an
 *   enum.
 */
#ifndef CG3_CHECKS_IN_HXX
#define CG3_CHECKS_IN_HXX

#include <utility>

#include <llvm/Support/CommandLine.h>

namespace cg3 {
    enum class check_types : int {
        @CG3_CHECK_TYPES_ENUM@,
        COUNT
    };

    template<check_types>
    struct check_description_impl;

    @CG3_CHECK_TYPES_DESCRIPTION_IMPLS@

           template<>
           struct check_description_impl<check_types::COUNT> {
        constexpr const static auto value = "run all compiled checks";
    };

    template<check_types Type>
    constexpr const static auto check_description = check_description_impl<Type>::value;

    template<check_types>
    struct check_name_impl;

    @CG3_CHECK_TYPES_NAME_IMPLS@

           template<>
           struct check_name_impl<check_types::COUNT> {
        constexpr const static auto value = "complete";
    };

    template<check_types Type>
    constexpr const static auto check_name = check_name_impl<Type>::value;

    template<class M, check_types... checks>
    struct make_options_impl {
        auto
        operator()() {
            return maker(
                   clEnumValN(checks, check_name<checks>, check_description<checks>)...);
        }

        M maker;
    };

    template<class M>
    using make_options = make_options_impl<M, @CG3_CHECK_TYPES_NAMESPACED@, check_types::COUNT>;
}

#endif
