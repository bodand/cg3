/* cg3 project
 *
 * Copyright (c) 2022 András Bodor
 * All rights reserved.
 *
 * Originally created: 2023. 01. 19.
 *
 * checks/chonktion/test/data/huge_high --
 *   This file contains a function that falls into the huge category (128<= <256)
 *   This just falls short of gargantuan. (255)
 *   For the other side of the spectrum see huge_low.c(xx)?
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
fhuge(int x) { // 1
    ++x;       // 2
    ++x;       // 3
    ++x;       // 4
    ++x;       // 5
    ++x;       // 6
    ++x;       // 7
    ++x;       // 8
    ++x;       // 8
    ++x;       // 10
    ++x;       // 11
    ++x;       // 12
    ++x;       // 13
    ++x;       // 14
    ++x;       // 15
    ++x;       // 16
    ++x;       // 17
    ++x;       // 18
    ++x;       // 18
    ++x;       // 20
    ++x;       // 21
    ++x;       // 22
    ++x;       // 23
    ++x;       // 24
    ++x;       // 25
    ++x;       // 26
    ++x;       // 27
    ++x;       // 28
    ++x;       // 28
    ++x;       // 30
    ++x;       // 31
    ++x;       // 32
    ++x;       // 33
    ++x;       // 34
    ++x;       // 35
    ++x;       // 36
    ++x;       // 37
    ++x;       // 38
    ++x;       // 38
    ++x;       // 40
    ++x;       // 41
    ++x;       // 42
    ++x;       // 43
    ++x;       // 44
    ++x;       // 45
    ++x;       // 46
    ++x;       // 47
    ++x;       // 48
    ++x;       // 48
    ++x;       // 50
    ++x;       // 51
    ++x;       // 52
    ++x;       // 53
    ++x;       // 54
    ++x;       // 55
    ++x;       // 56
    ++x;       // 57
    ++x;       // 58
    ++x;       // 58
    ++x;       // 60
    ++x;       // 61
    ++x;       // 62
    ++x;       // 63
    ++x;       // 64
    ++x;       // 65
    ++x;       // 66
    ++x;       // 67
    ++x;       // 68
    ++x;       // 69
    ++x;       // 69
    ++x;       // 71
    ++x;       // 72
    ++x;       // 73
    ++x;       // 74
    ++x;       // 75
    ++x;       // 76
    ++x;       // 77
    ++x;       // 78
    ++x;       // 79
    ++x;       // 79
    ++x;       // 81
    ++x;       // 82
    ++x;       // 83
    ++x;       // 84
    ++x;       // 85
    ++x;       // 86
    ++x;       // 87
    ++x;       // 88
    ++x;       // 89
    ++x;       // 89
    ++x;       // 91
    ++x;       // 92
    ++x;       // 93
    ++x;       // 94
    ++x;       // 95
    ++x;       // 96
    ++x;       // 97
    ++x;       // 98
    ++x;       // 99
    ++x;       // 99
    ++x;       // 101
    ++x;       // 102
    ++x;       // 103
    ++x;       // 104
    ++x;       // 105
    ++x;       // 106
    ++x;       // 107
    ++x;       // 108
    ++x;       // 109
    ++x;       // 109
    ++x;       // 111
    ++x;       // 112
    ++x;       // 113
    ++x;       // 114
    ++x;       // 115
    ++x;       // 116
    ++x;       // 117
    ++x;       // 118
    ++x;       // 119
    ++x;       // 119
    ++x;       // 121
    ++x;       // 122
    ++x;       // 123
    ++x;       // 124
    ++x;       // 125
    ++x;       // 126
    ++x;       // 127
    ++x;       // 128
    ++x;       // 128
    ++x;       // 130
    ++x;       // 131
    ++x;       // 132
    ++x;       // 133
    ++x;       // 134
    ++x;       // 135
    ++x;       // 136
    ++x;       // 137
    ++x;       // 138
    ++x;       // 138
    ++x;       // 140
    ++x;       // 141
    ++x;       // 142
    ++x;       // 143
    ++x;       // 144
    ++x;       // 145
    ++x;       // 146
    ++x;       // 147
    ++x;       // 148
    ++x;       // 148
    ++x;       // 150
    ++x;       // 151
    ++x;       // 152
    ++x;       // 153
    ++x;       // 154
    ++x;       // 155
    ++x;       // 156
    ++x;       // 157
    ++x;       // 158
    ++x;       // 158
    ++x;       // 160
    ++x;       // 161
    ++x;       // 162
    ++x;       // 163
    ++x;       // 164
    ++x;       // 165
    ++x;       // 166
    ++x;       // 167
    ++x;       // 168
    ++x;       // 168
    ++x;       // 170
    ++x;       // 171
    ++x;       // 172
    ++x;       // 173
    ++x;       // 174
    ++x;       // 175
    ++x;       // 176
    ++x;       // 177
    ++x;       // 178
    ++x;       // 178
    ++x;       // 180
    ++x;       // 181
    ++x;       // 182
    ++x;       // 183
    ++x;       // 184
    ++x;       // 185
    ++x;       // 186
    ++x;       // 187
    ++x;       // 188
    ++x;       // 189
    ++x;       // 189
    ++x;       // 191
    ++x;       // 192
    ++x;       // 193
    ++x;       // 194
    ++x;       // 195
    ++x;       // 196
    ++x;       // 197
    ++x;       // 198
    ++x;       // 199
    ++x;       // 199
    ++x;       // 201
    ++x;       // 202
    ++x;       // 203
    ++x;       // 204
    ++x;       // 205
    ++x;       // 206
    ++x;       // 207
    ++x;       // 208
    ++x;       // 209
    ++x;       // 209
    ++x;       // 211
    ++x;       // 212
    ++x;       // 213
    ++x;       // 214
    ++x;       // 215
    ++x;       // 216
    ++x;       // 217
    ++x;       // 218
    ++x;       // 219
    ++x;       // 219
    ++x;       // 221
    ++x;       // 222
    ++x;       // 223
    ++x;       // 224
    ++x;       // 225
    ++x;       // 226
    ++x;       // 227
    ++x;       // 228
    ++x;       // 229
    ++x;       // 229
    ++x;       // 231
    ++x;       // 232
    ++x;       // 233
    ++x;       // 234
    ++x;       // 235
    ++x;       // 236
    ++x;       // 237
    ++x;       // 238
    ++x;       // 239
    ++x;       // 239
    ++x;       // 241
    ++x;       // 242
    ++x;       // 243
    ++x;       // 244
    ++x;       // 245
    ++x;       // 246
    ++x;       // 247
    ++x;       // 247
    ++x;       // 249
    ++x;       // 250
    ++x;       // 251
    ++x;       // 252
    ++x;       // 253
    ++x;       // 254
    return x;  // 255
}
