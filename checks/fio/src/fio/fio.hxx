/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-27.
 *
 * checks/fio/src/fio/fio --
 */
#ifndef CG3_FIO_HXX
#define CG3_FIO_HXX

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include <cg3-common/hash_storages.hxx>
#include <chk3/check.hxx>
#include <chk3/loader.hxx>
#include <fio/call_pos.hxx>
#include <fio/io_op_callback.hxx>

namespace cg3 {
    struct fio final : check {
        fio();
        void
        check_ast(std::vector<std::unique_ptr<clang::ASTUnit>>& units) override;

        void
        collected_report() override;

        void
        add_io_call(std::string_view io_call, std::string_view file, unsigned row, unsigned col);
        void
        add_opener_call(std::string_view opener, std::string_view file, unsigned row, unsigned col);
        void
        add_closer_call(std::string_view closer, std::string_view file, unsigned row, unsigned col);

    private:
        void
        failed_report() const;

        [[nodiscard]] bool
        success_report() const;

        struct io_call {
            static io_call
            input(std::string_view fn) {
                return {fn, false};
            }

            static io_call
            output(std::string_view fn) {
                return {fn, true};
            }

            friend bool
            operator==(const io_call& io, std::string_view fn) {
                return io.fun == fn;
            }
            friend bool
            operator==(std::string_view fn, const io_call& io) {
                return io.fun == fn;
            }

            std::string_view fun;
            bool out;
        };


        struct io_routine {
            template<class... Io>
            io_routine(std::string opener,
                       std::string closer,
                       Io... io)
                 : opener(std::move(opener)),
                   closer(std::move(closer)),
                   io_functions{std::move(io)...} {
            }

            [[nodiscard, gnu::pure]] bool
            opens_with(std::string_view fun) const;
            [[nodiscard, gnu::pure]] bool
            closes_with(std::string_view fun) const;
            [[nodiscard, gnu::pure]] bool
            io_function_of(std::string_view fun) const;

            void
            add_open_in_file(call_pos&& file);
            void
            add_close_in_file(call_pos&& file);
            void
            add_call_in_file(std::string_view call,
                             call_pos&& file);

            [[gnu::pure]] explicit
            operator bool() const noexcept;

            const std::string opener;
            const std::string closer;
            const std::vector<io_call> io_functions;
            int opened = 0;
            std::vector<call_pos> opened_in{};
            int closed = 0;
            std::vector<call_pos> closed_in{};
            int input_called = 0;
            int output_called = 0;
            std::unordered_multimap<std::string, call_pos> io_calls{};
        };
        static void
        open_close_stat(const cg3::fio::io_routine& io,
                        const std::string io_routine::*io_type,
                        int io_routine::*invoked,
                        std::vector<call_pos> io_routine::*call_files);

        clang::ast_matchers::MatchFinder _finder{};
        io_op_callback _io_op_callback{this};
        // XXX currently this check does not handle io_uring on Linux
        //     natch, this has 0 probability of happening in a prog1 hf,
        //     but not impossible
        // XXX configuring this causes immense pain, should have some external
        //     format which is read at runtime
        std::vector<io_routine> _call_data{
               {"fopen",
                "fclose",
                io_call::output("fputs"),
                io_call::output("fputc"),
                io_call::output("fprintf"),
                io_call::output("vfprintf"),
                io_call::output("fwrite"),
                io_call::input("fgets"),
                io_call::input("fgetc"),
                io_call::input("fscanf"),
                io_call::input("vfscanf"),
                io_call::input("fread")},
               {"open",
                "close",
                io_call::output("write"),
                io_call::output("pwrite"),
                io_call::output("writev"),
                io_call::output("pwritev"),
                io_call::input("read"),
                io_call::input("pread"),
                io_call::input("readv"),
                io_call::input("preadv")},
               {"CreateFile",
                // this is actually an AW function but this part of the code
                // pretends it is not
                "CloseHandle",
                io_call::output("WriteFile"),
                io_call::output("WriteFileEx"),
                io_call::output("WriteFileGather"),
                io_call::input("ReadFile"),
                io_call::input("ReadFileEx"),
                io_call::input("ReadFileScatter")}};
    };

    template<>
    struct loader<check_types::fio> {
        using type = fio;
    };
}

#endif
