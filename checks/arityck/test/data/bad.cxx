/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022. 12. 10.
 *
 * checks/arityck/test/data/bad --
 *   Test data for checking arityck to report functions that failed the check.
 */

int quinary(int a, int b, int c, int d, int e) { // STAY ON LINE 13!!!
    // oh no
    return 42;
}