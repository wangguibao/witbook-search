#pragma once

#include <string>
#include <vector>
namespace witbook {
namespace search {
namespace util {
/**
 * Generate a utf-8 character, storing it to p
 * The rule is:
 * 1) randomly select a char-code between 0x4e00 - 0x9fa5
 * 2) Apply the bits to bellow 3-byte sequence
 * 0b1110xxxx 0b10xxxxxx 0b10xxxxxx
 */
void gen_utf8_char(char *p);

/**
 * Create 64bit sign from a byte string of length len
 */
uint64_t creat_sign(const char *str, int len);

void string_split(
        const std::string& str,
        const char delimiter,
        std::vector<std::string>& ret_vec);

/**
 * is_ascii
 * @brief check if a string is of pure ascii characters (code 0 - 127)
 */
bool is_ascii(const std::string& str);

}   // End namespace util
}   // End namespace search
}   // End namespace witbook
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
