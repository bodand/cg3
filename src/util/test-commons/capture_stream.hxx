/* cg3 project
 *
 * Copyright (c) 2023 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2023. 01. 11.
 *
 * src/util/test-commons/capture_stream --
 */

#ifndef CG3_CAPTURE_STREAM_HXX
#define CG3_CAPTURE_STREAM_HXX

#include <ostream>
#include <sstream>
#include <string>

namespace cg3 {
    struct reset_stream {
        reset_stream(const reset_stream&) = delete;
        reset_stream(reset_stream&&) noexcept = delete;
        reset_stream&
        operator=(const reset_stream&) = delete;
        reset_stream&
        operator=(reset_stream&&) noexcept = delete;

        explicit reset_stream(std::ostream* ostrm, std::streambuf* buf)
             : _ostrm(ostrm),
               _buf(buf) { }

        ~reset_stream() noexcept {
            _ostrm->rdbuf(_buf);
        }

    private:
        std::ostream* _ostrm;
        std::streambuf* _buf;
    };

    template<std::ostream* OStrm, class Fn, class... Args>
    std::string
    capture_stream(Fn&& fn, Args&&... args) {
        const std::ostringstream ss;
        { // REDIRECTION SCOPE
            auto buf_buf [[maybe_unused]] = reset_stream(OStrm, OStrm->rdbuf(ss.rdbuf()));
            std::forward<Fn>(fn)(std::forward<Args>(args)...);
        }
        return ss.str();
    }
}

#endif
