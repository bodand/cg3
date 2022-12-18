/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2022. 12. 18.
 *
 * src/cg3-common/filter --
 */

#include <cg3-common/filter.hxx>

cg3::filter::filter(string_iterator begin, cg3::filter::string_iterator end)
     : _searcher(begin, end) { }

namespace {
    [[gnu::pure]] std::filesystem::path::string_type
    convert_to_native_string(std::string_view str) {
        auto converter = std::filesystem::path(str.data(), str.data() + str.size());
        std::filesystem::path::string_type converted_str = converter;
        return converted_str;
    }
}

std::unique_ptr<cg3::filter>
cg3::filter::file(std::string_view txt) {
    auto converted_str = convert_to_native_string(txt);
    return std::make_unique<cg3::file_filter>(converted_str.begin(), converted_str.end());
}

std::unique_ptr<cg3::filter>
cg3::filter::path(std::string_view txt) {
    auto converted_str = convert_to_native_string(txt);
    return std::make_unique<cg3::path_filter>(converted_str.begin(), converted_str.end());
}

bool
cg3::file_filter::operator()(const std::filesystem::path& file) const noexcept {
    std::filesystem::path::string_type file_str = file.filename();
    auto found = std::search(file_str.begin(), file_str.end(), _searcher);
    return found != file_str.end();
}

bool
cg3::path_filter::operator()(const std::filesystem::path& file) const noexcept {
    std::filesystem::path::string_type file_str = file;
    auto found = std::search(file_str.begin(), file_str.end(), _searcher);
    return found != file_str.end();
}
