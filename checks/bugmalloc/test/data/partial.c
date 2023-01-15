/* cg3 project
 *
 * Copyright (c) 2023 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2023. 01. 15.
 *
 * checks/bugmalloc/test/data/partial --
 *   Checks if files containing some correct and some invalid
 *   malloc calls is handled correctly by bugmalloc.
 *   This is unlikely to happen, as the following code is quite
 *   weird to happen in real life, and I cannot think of a better
 *   way to construct this phenomenon...?
 */


void*
malloc(size_t sz);

int bad() {
    void* leaked = malloc(42);
    return 0;
}

#include "debugmalloc.h"

int ok() {
    void* leaked = malloc(0);
    return 0;
}
