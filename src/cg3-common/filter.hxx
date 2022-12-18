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

#ifndef CG3_FILTER_HXX
#define CG3_FILTER_HXX

#include <algorithm>
#include <filesystem>
#include <functional>
#include <string_view>

namespace cg3 {
    struct file_filter;
    struct path_filter;

    struct filter {
        using string_iterator = std::filesystem::path::string_type::iterator;

        filter(string_iterator begin,
               string_iterator end);

        virtual bool
        operator()(const std::filesystem::path& file) const noexcept = 0;

        virtual ~filter() noexcept = default;

        [[gnu::pure]] static std::unique_ptr<filter>
        file(std::string_view txt);

        [[gnu::pure]] static std::unique_ptr<filter>
        path(std::string_view txt);

    protected:
        std::boyer_moore_horspool_searcher<string_iterator> _searcher;
    };

    struct file_filter final : filter {
        using filter::filter;

        bool
        operator()(const std::filesystem::path& file) const noexcept override;
    };

    struct path_filter final : filter {
        using filter::filter;

        bool
        operator()(const std::filesystem::path& file) const noexcept override;
    };
}

#endif
