/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-29.
 *
 * src/cg3-db/fixup_compiler --
 */
#ifndef CG3_FIXUP_COMPILER_HXX
#define CG3_FIXUP_COMPILER_HXX

#include <filesystem>

#include <cg3-common/find_executable.hxx>

namespace cg3 {
    inline void
    fixup_compiler(std::filesystem::path& cc) {
        cc = find_executable(cc);
    }
}

#endif
