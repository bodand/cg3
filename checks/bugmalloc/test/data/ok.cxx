/* cg3 project
 *
 * Copyright (c) 2023 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2023. 01. 14.
 *
 * checks/bugmalloc/test/data/ok --
 *   Check for handling bugmalloc checks.
 */

extern "C" void*
malloc(size_t sz);

#include "debugmalloc.h"

void*
allocate() {
    return malloc(42 * sizeof(int));
}
