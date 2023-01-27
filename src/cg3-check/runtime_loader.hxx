/* cg3 project
 *
 * Copyright (c) 2022 András Bodor <bodand@pm.me>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * - Neither the name of the copyright holder nor the names of its contributors
 *   may be used to endorse or promote products derived from this software
 *   without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Originally created: 2022-11-25.
 *
 * src/cg3-check/runtime_loader --
 */
#ifndef CG3_RUNTIME_LOADER_HXX
#define CG3_RUNTIME_LOADER_HXX

#include <chk3/check_includes.hxx>
#include <chk3/checks.hxx>
#include <magic_enum.hpp>

namespace cg3 {
    struct check_maker {
        constexpr check_maker() = default;

        check_maker(const check_maker&) = default;
        check_maker(check_maker&&) noexcept = default;

        check_maker&
        operator=(const check_maker&) = default;
        check_maker&
        operator=(check_maker&&) noexcept = default;

        virtual std::unique_ptr<check>
        operator()(clang::DiagnosticsEngine* diag) = 0;

        virtual ~check_maker() = default;
    };

    template<class Loader>
    struct check_maker_t final : check_maker {
        using check_type = typename Loader::type;

        constexpr check_maker_t() = default;

        std::unique_ptr<check>
        operator()(clang::DiagnosticsEngine* diag) override {
            return std::make_unique<check_type>(diag);
        }
    };

    template<>
    struct check_maker_t<loader<check_types::COUNT>> final : check_maker {
        std::unique_ptr<check>
        operator()([[maybe_unused]] clang::DiagnosticsEngine* diag) override {
            return nullptr;
        }
    };

    struct runtime_loader {
        explicit runtime_loader(clang::DiagnosticsEngine* diag)
             : _diag(diag) {
            magic_enum::enum_for_each<check_types>([&_checks = _checks](auto val) {
                constexpr check_types type = val;
                if (type == check_types::COUNT) return;
                _checks[static_cast<unsigned>(type)] = std::make_unique<check_maker_t<loader<val>>>();
            });
        }

        std::unique_ptr<cg3::check>
        load_check(check_types type);

    private:
        clang::DiagnosticsEngine* _diag;
        std::array<std::unique_ptr<check_maker>,
                   static_cast<unsigned>(check_types::COUNT)>
               _checks;
    };
}

#endif
