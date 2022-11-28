/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-27.
 *
 * checks/fio/src/fio/call_pos --
 */

#include <fio/call_pos.hxx>

std::ostream&
cg3::operator<<(std::ostream& os, const cg3::call_pos& pos) {
    return os << pos.file.string() << ":" << pos.row << ":" << pos.col;
}
