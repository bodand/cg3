/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-29.
 *
 * src/cg3-db/compiler_info --
 */
#ifndef CG3_COMPILER_INFO_HXX
#define CG3_COMPILER_INFO_HXX

#include <filesystem>
#include <string>

namespace cg3 {
    enum class compatibility {
        gcc,
        cl
    };

    compatibility
    guess_compiler(const std::filesystem::path& cc);

    std::string
    get_output_name_option(compatibility cc);
    std::string
    get_compile_flag(compatibility cc);
}

#endif
