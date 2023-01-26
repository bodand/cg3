/* cg3 project
 *
 * Copyright (c) 2023 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2023. 01. 26.
 *
 * cmake/try/fs_path_hash --
 *   See if std::hash<std::filesystem::path> is an existing thing.
 *   If this does not compile, we define it to be the hash of the
 *   string returned by the path::string() member fn.
 */
#include <filesystem>
#include <string>
#include <unordered_map>

int
main() {
    std::unordered_multimap<std::filesystem::path, std::string> things{};
}
