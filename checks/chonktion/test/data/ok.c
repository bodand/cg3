/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2023. 01. 18.
 *
 * checks/chonktion/test/data/empty --
 *   This file contains only functions which are under the (default) maximal limit.
 *   At the time of writing big limit is 64.
 */

void
f0() { }

int
f5() {
    int x = 11;
    int y = 22;
    int z = 33;
    return x + y - z == 0;
}

int
f63(int x) {  // 1
    ++x;      // 2
    ++x;      // 3
    ++x;      // 4
    ++x;      // 5
    ++x;      // 6
    ++x;      // 7
    ++x;      // 8
    ++x;      // 8
    ++x;      // 10
    ++x;      // 11
    ++x;      // 12
    ++x;      // 13
    ++x;      // 14
    ++x;      // 15
    ++x;      // 16
    ++x;      // 17
    ++x;      // 18
    ++x;      // 18
    ++x;      // 20
    ++x;      // 21
    ++x;      // 22
    ++x;      // 23
    ++x;      // 24
    ++x;      // 25
    ++x;      // 26
    ++x;      // 27
    ++x;      // 28
    ++x;      // 28
    ++x;      // 30
    ++x;      // 31
    ++x;      // 32
    ++x;      // 33
    ++x;      // 34
    ++x;      // 35
    ++x;      // 36
    ++x;      // 37
    ++x;      // 38
    ++x;      // 38
    ++x;      // 40
    ++x;      // 41
    ++x;      // 42
    ++x;      // 43
    ++x;      // 44
    ++x;      // 45
    ++x;      // 46
    ++x;      // 47
    ++x;      // 48
    ++x;      // 48
    ++x;      // 50
    ++x;      // 51
    ++x;      // 52
    ++x;      // 53
    ++x;      // 54
    ++x;      // 55
    ++x;      // 56
    ++x;      // 57
    ++x;      // 58
    ++x;      // 58
    ++x;      // 60
    ++x;      // 61
    ++x;      // 62
    return x; // 63
}
