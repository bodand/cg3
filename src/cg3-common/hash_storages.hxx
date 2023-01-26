/* cg3 project
 *
 * Copyright (c) 2023 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2023. 01. 26.
 *
 * src/cg3-common/hash_storages --
 *   This header is the stand-in for including
 *   std::unordered_* containers.
 *   It checks for some incompatibilities with some
 *   compiler versions and standard libraries and
 *   stubs in implementations that were missing.
 */

#ifndef CG3_HASH_STORAGES_HXX
#define CG3_HASH_STORAGES_HXX

#include <filesystem>
#include <functional>

#ifdef CG3_STUB_FS_PATH_HASH
template<>
struct std::hash<std::filesystem::path> {
    std::size_t
    operator()(const std::filesystem::path& p) const {
        return std::hash<std::filesystem::path::string_type>{}(p.native());
    }
};
#endif

#include <unordered_map>
#include <unordered_set>

#endif
