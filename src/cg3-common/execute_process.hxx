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

#ifndef CG3_EXECUTE_PROCESS_HXX
#define CG3_EXECUTE_PROCESS_HXX

#include <istream>
#include <ostream>
#include <string_view>
#include <vector>

namespace cg3 {
    int
    execute_process(const std::vector<std::string_view>& args,
                    std::istream& input,
                    std::ostream& output,
                    std::ostream& error);
}

#endif
