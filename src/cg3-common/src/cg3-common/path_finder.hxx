/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2022. 12. 18.
 *
 * src/cg3-common/path_finder --
 */

#ifndef CG3_PATH_FINDER_HXX
#define CG3_PATH_FINDER_HXX

#include <filesystem>
#include <memory>
#include <vector>

#include <cg3-common/filter.hxx>

namespace cg3 {
    std::vector<std::filesystem::path>
    find_files(const std::filesystem::path& dir,
               const std::vector<std::unique_ptr<cg3::filter>>& filters,
               int depth = 0);
}

#endif
