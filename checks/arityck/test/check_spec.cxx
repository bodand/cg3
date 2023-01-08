/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022. 12. 08.
 *
 * checks/arityck/test/check_spec --
 */

#include <concepts>

#include <arityck/arityck.hxx>
#include <boost/ut.hpp>
#include <chk3/check.hxx>
using namespace boost::ut;

[[maybe_unused]] const suite test_check_spec = [] { // NOLINT
    "arityck conforms to check"_test = [] {
        "convertible to check*"_test = [] {
            expect(std::convertible_to<cg3::arityck*, cg3::check*>);
        };

        "default constructible"_test = [] {
            expect(std::is_default_constructible_v<cg3::arityck>);
        };

        "loader<arityck>::type is the arityck type"_test = [] {
            expect(std::same_as<cg3::loader<cg3::check_types::arityck>::type,
                                cg3::arityck>);
        };
    };
};
