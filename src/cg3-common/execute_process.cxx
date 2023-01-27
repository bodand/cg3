/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2022. 12. 19.
 *
 * src/cg3-common/execute_process --
 */


#include <cassert>
#include <string>

#include <cg3-common/execute_process.hxx>
#include <reproc++/drain.hpp>
#include <reproc++/reproc.hpp>

int
cg3::execute_process(const std::vector<std::string_view>& args,
                     std::istream& input,
                     std::ostream& output,
                     std::ostream& error) {
    assert(!args.empty());

    reproc::process proc;
    reproc::options opts;
    opts.redirect.err.type = reproc::redirect::pipe;

    if (auto ec = proc.start(args, opts);
        ec != std::errc{}) throw std::system_error(ec);

    constexpr const static auto io_buffer_size = 4096;
    std::uint8_t buf[io_buffer_size];
    while (input.read(reinterpret_cast<char*>(buf), std::size(buf))) {
        auto red = input.gcount();
        proc.write(static_cast<std::uint8_t*>(buf), static_cast<std::size_t>(red));
    }
    proc.close(reproc::stream::in);

    reproc::sink::ostream out_strm(output);
    reproc::sink::ostream err_strm(error);
    if (auto ec = reproc::drain(proc, out_strm, err_strm);
        ec != std::errc{}) throw std::system_error(ec);

    auto [status, ec] = proc.wait(reproc::infinite);
    if (ec != std::errc{}) throw std::system_error(ec);

    return status;
}

int
cg3::execute_process(const std::vector<std::string_view>& args) {
    assert(!args.empty());

    reproc::process proc;
    reproc::options opts;
    opts.redirect.out.type = reproc::redirect::parent;
    opts.redirect.err.type = reproc::redirect::parent;
    opts.redirect.in.type = reproc::redirect::parent;

    if (auto ec = proc.start(args, opts);
        ec != std::errc{}) throw std::system_error(ec);

    auto [status, ec] = proc.wait(reproc::infinite);
    if (ec != std::errc{}) throw std::system_error(ec);

    return status;
}
