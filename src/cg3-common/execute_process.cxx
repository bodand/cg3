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


#include <string>

#include <cg3-common/execute_process.hxx>
#include <reproc++/drain.hpp>
#include <reproc++/reproc.hpp>

int
cg3::execute_process(const std::vector<std::string_view>& args,
                     std::istream& input,
                     std::ostream& output,
                     std::ostream& error) {
    reproc::process proc;
    reproc::options opts;
    opts.redirect.err.type = reproc::redirect::pipe;

    if (auto ec = proc.start(args, opts);
        ec != std::errc{}) throw std::system_error(ec);

    std::uint8_t buf[4096];
    while (input.read(reinterpret_cast<char*>(buf), std::size(buf))) {
        auto red = input.gcount();
        proc.write(buf, red);
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
