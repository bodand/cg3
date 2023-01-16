/* cg3 project
 *
 * Copyright (c) 2023 András Bodor
 * All rights reserved.
 * Licensed under the BSD 3-Clause license.
 *
 * Originally created: 2023. 01. 14.
 *
 * checks/bugmalloc/test/data/debugmalloc --
 *   Stub debugmalloc.h file, used for testing.
 *   This defines macros for malloc/calloc/realloc/free which are
 *   expected to be overwritten by the real debugmalloc header.
 */

#ifndef CG3_DEBUGMALLOC_H
#define CG3_DEBUGMALLOC_H

#ifdef __cplusplus
extern "C"
{
#endif

    void*
    malloc(unsigned long long total_sz);
    void*
    calloc(unsigned long long obj_cnt, unsigned long long obj_sz);
    void*
    realloc(void* p, unsigned long long new_sz);
    void
    free(void* p);

    inline void*
    dbg_malloc(unsigned long long total_sz) {
        return malloc(total_sz);
    }
    void*
    dbg_calloc(unsigned long long obj_cnt, unsigned long long obj_sz) {
        return calloc(obj_cnt, obj_sz);
    }
    void*
    dbg_realloc(void* p, unsigned long long new_sz) {
        return realloc(p, new_sz);
    }
    void
    dbg_free(void* p) {
        free(p);
    }


#ifdef __cplusplus
}
#endif

#define malloc(x) dbg_malloc(x)
#define calloc(x, y) dbg_calloc(x, y)
#define realloc(x, y) dbg_realloc(x, y)
#define free(x) dbg_free(x)

#endif
