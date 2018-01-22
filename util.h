#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>
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
#endif
