/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-25.
 *
 * src/cg3/runtime_loader --
 */
#include "runtime_loader.hxx"

std::unique_ptr<cg3::check>
cg3::runtime_loader::load_check(cg3::check_types type) {
    assert(type != check_types::COUNT);

    return (*_checks[static_cast<unsigned>(type)])();
}
