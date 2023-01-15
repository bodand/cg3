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


extern "C" void*
malloc(size_t sz);

int bad() {
    auto leaked = malloc(42);
    return 0;
}

int bad2() {
    auto leaked = malloc(42);
    return 0;
}
