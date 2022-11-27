/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022-11-27.
 *
 * checks/fio/src/fio/call_pos --
 */
#ifndef CG3_CALL_POS_HXX
#define CG3_CALL_POS_HXX

#include <filesystem>
#include <ostream>

namespace cg3 {
    struct call_pos {
        std::filesystem::path file;
        int row;
        int col;

        friend std::ostream&
        operator<<(std::ostream&& os, const call_pos&);
    };
}

#endif
