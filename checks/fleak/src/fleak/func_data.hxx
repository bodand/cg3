/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022. 11. 30.
 *
 * checks/fleak/src/fleak/func_data --
 */

#ifndef CG3_FUNC_DATA_HXX
#define CG3_FUNC_DATA_HXX

#include <algorithm>
#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include <chk3/check.hxx>
#include <chk3/loader.hxx>

#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Basic/SourceLocation.h>
#include <llvm/ADT/StringRef.h>

namespace cg3 {
    struct func_data {
        func_data(llvm::StringRef name,
                  clang::SourceLocation srcloc,
                  std::filesystem::path file,
                  const unsigned int row,
                  const unsigned int col,
                  const func_data* calls = nullptr)
             : name(name.begin(), name.end()),
               srcloc(srcloc),
               file(std::move(file)),
               row(row),
               col(col),
               calls(calls) { }

        func_data(llvm::StringRef name,
                  std::filesystem::path file,
                  const unsigned int row,
                  const unsigned int col,
                  const func_data* calls = nullptr)
             : name(name.begin(), name.end()),
               file(std::move(file)),
               row(row),
               col(col),
               calls(calls) { }

        const std::string name;
        const std::optional<clang::SourceLocation> srcloc{};
        const std::filesystem::path file;
        const unsigned row;
        const unsigned col;
        const func_data* calls;

        void
        set_checked() const noexcept { _checked = true; }

        bool
        get_checked() const noexcept { return _checked; }

    private:
        mutable bool _checked{};

        friend bool
        operator==(const func_data& fd, std::string_view str) {
            return fd.name == str;
        }

        friend bool
        operator==(std::string_view str, const func_data& fd) {
            return str == fd.name;
        }

        friend bool
        operator==(const func_data& fd, const func_data& fd2) {
            return fd.name == fd2.name;
        }
    };
}

namespace std {
    template<>
    struct hash<cg3::func_data> {
        auto
        operator()(const cg3::func_data& fd) const noexcept(noexcept(_str(fd.name))) {
            return _str(fd.name);
        }

    private:
        hash<string> _str{};
    };
}

#endif
