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

#include <cg3-common/path_finder.hxx>

namespace fs = std::filesystem;

namespace {
    std::vector<fs::path>
    find_files_flat(const fs::path& dir,
                    const std::vector<std::unique_ptr<cg3::filter>>& filters) {
        if (!is_directory(dir)) return {dir};
        std::vector<fs::path> ret;
        std::copy_if(fs::directory_iterator(dir),
                     fs::directory_iterator(),
                     std::back_inserter(ret),
                     [&filters](auto entry) {
                         auto& file = entry.path();
                         return std::all_of(filters.begin(), filters.end(), [&file](const auto& filter_ptr) {
                             return (*filter_ptr)(file);
                         });
                     });
        return ret;
    }

    std::vector<fs::path>
    find_files_recursive(const fs::path& dir,
                         const std::vector<std::unique_ptr<cg3::filter>>& filter,
                         int depth) {
        if (!is_directory(dir)) return {dir};
        std::vector<fs::path> ret;
        for (auto it = fs::recursive_directory_iterator(dir);
             it != fs::recursive_directory_iterator();
             ++it) {
            if (it.depth() > depth) continue;

            auto& file = it->path();
            if (std::all_of(filter.begin(), filter.end(), [&file](const auto& filter_ptr) {
                    return (*filter_ptr)(file);
                }))
                ret.push_back(file);
        }
        return ret;
    }
}

std::vector<std::filesystem::path>
cg3::find_files(const std::filesystem::path& dir,
                const std::vector<std::unique_ptr<cg3::filter>>& filters,
                int depth) {
    if (depth == 0) return find_files_flat(dir, filters);
    return find_files_recursive(dir, filters, depth);
}
