/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-25.
 *
 * src/cg3/runtime_loader --
 */
#ifndef CG3_RUNTIME_LOADER_HXX
#define CG3_RUNTIME_LOADER_HXX

#include <chk3/check_includes.hxx>
#include <chk3/checks.hxx>
#include <magic_enum.hpp>

namespace cg3 {
    struct runtime_loader {
        struct check_maker {
            constexpr check_maker() = default;

            virtual std::unique_ptr<check>
            operator()() = 0;

            virtual ~check_maker() = default;
        };

        template<class Loader>
        struct check_maker_t final : check_maker {
            using check_type = typename Loader::type;

            constexpr check_maker_t() = default;

            std::unique_ptr<check>
            operator()() override {
                return std::make_unique<check_type>();
            }
        };

        template<>
        struct check_maker_t<loader<check_types::COUNT>> final : check_maker {
            std::unique_ptr<check>
            operator()() override {
                return nullptr;
            }
        };

        runtime_loader() {
            magic_enum::enum_for_each<check_types>([&_checks = _checks](auto val) {
                constexpr check_types type = val;
                if (type == check_types::COUNT) return;
                _checks[static_cast<unsigned>(type)] = std::make_unique<check_maker_t<loader<val>>>();
            });
        }


        std::unique_ptr<cg3::check>
        load_check(check_types type);

    private:
        std::array<std::unique_ptr<check_maker>,
                   static_cast<unsigned>(check_types::COUNT)>
               _checks;
    };
}

#endif
