/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2022. 12. 19.
 *
 * src/cg3-common/find_executable --
 */

#ifndef CG3_FIND_EXECUTABLE_HXX
#define CG3_FIND_EXECUTABLE_HXX

#include <filesystem>

namespace cg3 {
    std::filesystem::path
    find_executable(const std::filesystem::path& exe);
}

#endif
