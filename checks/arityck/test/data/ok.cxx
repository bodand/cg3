/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2022. 12. 10.
 *
 * checks/arityck/test/data/ok --
 *   Test data for checking arityck not reporing errors on correct functoins.
 */

int nullary() {
    return 42;
}

int unary(int x) {
    return x;
}

int binary(int a, int b) {
    return a + b;
}

int ternary(int a, int b, int c) {
    return binary(a, binary(b, c));
}

int quaternary(int a, int b, bool c, int d) {
    return ternary(
                binary(
                    unary(a),
                    nullary()),
                b,
                binary(c ? 1 : 1,
                       d));
}
