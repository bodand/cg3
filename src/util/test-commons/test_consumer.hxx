/* cg3 project
 *
 * Copyright (c) 2023 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2023. 01. 11.
 *
 * src/util/test-commons/test_consumer --
 */

#ifndef CG3_TEST_CONSUMER_HXX
#define CG3_TEST_CONSUMER_HXX

#include <clang/Basic/Diagnostic.h>

namespace cg3 {
    struct test_consumer : clang::DiagnosticConsumer {
        void
        HandleDiagnostic(clang::DiagnosticsEngine::Level DiagLevel,
                         const clang::Diagnostic& Info) override {
            _level_check(DiagLevel);
            _info_check(Info);
        }

        template<class Fn>
        void
        set_level_check(Fn&& fn) {
            _level_check = std::forward<Fn>(fn);
        }

        template<class Fn>
        void
        set_info_check(Fn&& fn) {
            _info_check = std::forward<Fn>(fn);
        }

    private:
        std::function<void(clang::DiagnosticsEngine::Level)> _level_check{[](const auto&) {
        }};
        std::function<void(const clang::Diagnostic&)> _info_check{[](const auto&) {
        }};
    };
}

#endif
