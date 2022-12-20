/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2022. 12. 20.
 *
 * src/cg3/cli --
 */

#ifndef CG3_CLI_WITH_VECTOR_DATA_HXX
#define CG3_CLI_WITH_VECTOR_DATA_HXX

#include <info/cli/types/type_data.hxx>
template<class T>
struct info::cli::type_data<std::vector<T>> {
    constexpr const static bool allow_nothing = type_data<T>::allow_nothing;
    constexpr const static std::string_view default_value = type_data<T>::default_value;
    constexpr const static int length = type_data<T>::length;
    constexpr const static std::string_view type_name = meta::type_name<T>();
    constexpr const static parse_type expected_type = type_data<T>::expected_type;
};

#include <info/cli.hxx>

#endif
