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

#include "filter.hxx"

#include <cg3-common/filter.hxx>

namespace {
    [[gnu::pure]] std::filesystem::path::string_type
    convert_to_native_string(std::string_view str) {
        auto converter = std::filesystem::path(str.data(), str.data() + str.size());
        std::filesystem::path::string_type converted_str = converter;
        return converted_str;
    }
}

std::unique_ptr<cg3::filter>
cg3::filter::exclude_file(std::string_view txt) {
    auto converted_str = convert_to_native_string(txt);
    return std::make_unique<cg3::file_filter>(converted_str.begin(), converted_str.end());
}

std::unique_ptr<cg3::filter>
cg3::filter::exclude_path(std::string_view txt) {
    auto converted_str = convert_to_native_string(txt);
    return std::make_unique<cg3::path_filter>(converted_str.begin(), converted_str.end());
}

std::unique_ptr<cg3::filter>
cg3::filter::only_extensions(const std::unordered_set<std::string>& exts) {
    return std::make_unique<cg3::extension_filter>(exts);
}

std::string
cg3::filter::to_string() {
    return "";
}

bool
cg3::file_filter::operator()(const std::filesystem::path& file) const {
    const std::filesystem::path::string_type file_str = file.filename();
    return !match(file_str);
}

#if defined(_WIN32) || defined(_WIN64)
// NOLINTNEXTLINE literally undoable any other way
#  define CG3_PATH_STR(s) L##s
#else
// NOLINTNEXTLINE literally undoable any other way
#  define CG3_PATH_STR(s) s
#endif

std::string
cg3::file_filter::to_string() {
    const std::filesystem::path conv(CG3_PATH_STR("--file-filter=") + filter_str);
    return conv.string();
}

bool
cg3::path_filter::operator()(const std::filesystem::path& file) const {
    const std::filesystem::path::string_type file_str = file;
    return !match(file_str);
}

std::string
cg3::path_filter::to_string() {
    const std::filesystem::path conv(CG3_PATH_STR("--path-filter=") + filter_str);
    return conv.string();
}

#undef CG3_PATH_STR

cg3::substring_filter::substring_filter(cg3::filter::string_iterator begin,
                                        cg3::filter::string_iterator end)
     : filter_str(begin, end), // NOLINT(performance-unnecessary-value-param) trivially copiable
       _searcher(filter_str.begin(), filter_str.end()) { }

bool
cg3::substring_filter::match(const std::filesystem::path::string_type& file_str) const noexcept {
    auto found = std::search(file_str.begin(), file_str.end(), _searcher);
    return found != file_str.end();
}

cg3::extension_filter::extension_filter(const std::unordered_set<std::string>& exts)
     : _exts(exts) { }

bool
cg3::extension_filter::operator()(const std::filesystem::path& file) const {
    auto ext_str = file.extension().string();
    auto it = _exts.find(ext_str);
    return it != _exts.end();
}

std::string
cg3::extension_filter::to_string() {
    return filter::to_string();
}
