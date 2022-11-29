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

namespace cg3 {
    void
    fixup_compiler(std::filesystem::path& cc);
}

#endif
