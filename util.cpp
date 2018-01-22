/*
 * util.cpp
 * @author Wang Guibao
 * @brief various utilities
 */
#include <stdlib.h>
#include "util.h"

void gen_utf8_char(char *p)
{
    int charcode = rand() % (0x9fa5 - 0x4e00) + 0x4e00;
    *p = static_cast<char>(0xe0 | (charcode >> 12));
    *(p + 1) = static_cast<char>(0x80 | ((charcode >> 6) & 0x3f));
    *(p + 2) = static_cast<char>(0x80 | (charcode & 0x3f));
}

uint64_t creat_sign(const char *str, int len) {
    uint64_t sign = 0;
    for (int i = 0; i < len; ++i) {
        sign += (sign * 0x1331) + str[i];
    }

    return sign;
}


