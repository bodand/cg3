/* cg3 project
 *
 * Copyright (c) 2023 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2023. 01. 18.
 *
 * checks/bugmalloc/test/data/tricky --
 *   Tricky calls either call to allocating functions that are not provided
 *   by debugmalloc so are likely misused, or describe the memory leak
 *   that keeps happening.
 *   Other calls are just system-calls to allocating primitives that can
 *   circumvent debugmalloc rendering it useless.
 */

extern "C" char*
strdup(const char* s);

// irrelevant
#include "debugmalloc.h"

char*
new_thingy() {
    return strdup("thingy");
}
