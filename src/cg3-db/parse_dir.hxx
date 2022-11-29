/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-28.
 *
 * src/cg3-cc/parse_dir --
 */
#ifndef CG3_PARSE_DIR_HXX
#define CG3_PARSE_DIR_HXX

#include <algorithm>
#include <filesystem>
#include <unordered_set>

namespace cg3 {
    bool
    is_source_file(const std::filesystem::directory_entry& entry) {
        static std::unordered_set<std::string> known_extensions{
               ".c",
               ".cpp",
               ".cxx",
               ".cc",
               ".C",
               ".c++"};

        if (!entry.is_regular_file()) return false;
        auto ext = entry.path().extension();
        auto it = known_extensions.find(ext.string());
        return it != known_extensions.end();
    }

    template<class Filter>
    bool
    match_filters(const std::string_view str, const Filter& filters) {
        return std::none_of(filters.begin(), filters.end(), [&str](const auto& filter) {
            auto it = std::search(str.begin(), str.end(), filter);
            return it != str.end();
        });
    }

    template<class OutIt, class Filter>
    void
    parse_dir(const std::filesystem::path& dir,
              OutIt out,
              const Filter& file_filts,
              const Filter& path_filts) {
        std::copy_if(std::filesystem::directory_iterator(dir),
                     std::filesystem::directory_iterator{},
                     out,
                     [&file_filts, &path_filts](const auto& entry) {
                         const auto& fpath = entry.path();

                         auto full_str = fpath.string();
                         if (!match_filters(full_str, path_filts)) return false;

                         auto name_str = fpath.filename().string();
                         if (!match_filters(name_str, file_filts)) return false;

                         return is_source_file(entry);
                     });
    }

    template<class OutIt, class Filter>
    void
    parse_recursive_dir(int depth,
                        const std::filesystem::path& dir,
                        OutIt out,
                        const Filter& file_filts,
                        const Filter& path_filts) {
        auto rec_it = std::filesystem::recursive_directory_iterator(dir);
        std::copy_if(rec_it,
                     std::filesystem::recursive_directory_iterator{},
                     out,
                     [&](const auto& entry) {
                         if (rec_it.depth() > depth) return false;
                         const auto& fpath = entry.path();

                         auto full_str = fpath.string();
                         if (!match_filters(full_str, path_filts)) return false;

                         auto name_str = fpath.filename().string();
                         if (!match_filters(name_str, file_filts)) return false;

                         return is_source_file(entry);
                     });
    }
}

#endif
