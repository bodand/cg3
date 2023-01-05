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
#include <unordered_set>

namespace cg3 {
    struct filter {
        using string_type = std::filesystem::path::string_type;
        using string_iterator = string_type::iterator;

        filter() = default;

        filter(const filter& cp) = default;
        filter(filter&& mv) noexcept = default;

        filter&
        operator=(const filter& cp) = default;
        filter&
        operator=(filter&& mv) noexcept = default;

        virtual bool
        operator()(const std::filesystem::path& file) const = 0;

        virtual std::string
        to_string() = 0;

        virtual ~filter() noexcept = default;

        [[gnu::pure]] static std::unique_ptr<filter>
        only_extensions(const std::unordered_set<std::string>& exts);

        [[gnu::pure]] static std::unique_ptr<filter>
        exclude_file(std::string_view txt);

        [[gnu::pure]] static std::unique_ptr<filter>
        exclude_path(std::string_view txt);
    };

    struct extension_filter final : filter {
        explicit extension_filter(const std::unordered_set<std::string>& exts);

        std::string
        to_string() override;

        bool
        operator()(const std::filesystem::path& file) const override;

    private:
        const std::unordered_set<std::string>& _exts;
    };

    struct substring_filter : filter {
        substring_filter(string_iterator begin,
                         string_iterator end);

    protected:
        [[nodiscard]] bool
        match(const std::filesystem::path::string_type& file_str) const noexcept;

        string_type filter_str;

    private:
        std::boyer_moore_horspool_searcher<string_iterator> _searcher;
    };

    struct file_filter final : substring_filter {
        using substring_filter::substring_filter;

        std::string
        to_string() override;

        bool
        operator()(const std::filesystem::path& file) const override;
    };

    struct path_filter final : substring_filter {
        using substring_filter::substring_filter;

        std::string
        to_string() override;

        bool
        operator()(const std::filesystem::path& file) const override;
    };
}

#endif
