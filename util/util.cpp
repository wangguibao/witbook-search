/*
 * util.cpp
 * @author Wang Guibao
 * @brief various utilities
 */
#include <cstdlib>
#include "util.h"

namespace witbook {
namespace search {
namespace util {
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

void string_split(
        const std::string& str,
        const char delimiter,
        std::vector<std::string>& ret_vec) {
    ret_vec.clear();

    std::string slice;
    std::string::size_type spos = 0;
    std::string::size_type epos = 0;

    while ((epos = str.find(delimiter, spos)) != std::string::npos) {
        ret_vec.push_back(str.substr(spos, epos);
        spos = epos + 1;
    }

    ret_vec.push_back(str.substr(spos));

    return;
}

bool is_ascii(const std::string& str) {
    return std::all_of(
            str.begin(),
            str.end(),
            [](const char c) {return static_cast<unsigned char>(c) <= 127});
}
}   // End namespace util
}   // End namespace search
}   // End namespace witbook

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
