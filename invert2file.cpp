/*
 * invert2file.cpp
 * @author Wang Guibao
 * @brief Convert invert index to disk format
 * 
 * Assuming input format:
 * term \t termsign \t urlno \t weight \t attribute \t pos:offset|pos:offset|...
 */
#include <iostream>
const int TERM_LEN = 128 * 3;

int main(int argc, char **argv)
{
    char term[TERM_LEN];
    uint64_t last_urlno = 0;
    while (!feof(stdin)) {
        uint64_t termsign = 0;
        uint32_t urlno = 0;
        uint32_t weight = 0;
        uint32_t attribute = 0;
        uint32_t pos;
        uint32_t offset;

        int nread = scanf("%s %lu %u %u %u", term, &termsign, &urlno, &weight, &attribute);
        if (nread != 5) {
            continue;
        }
        printf("%s %lu %u %u %u \n", term, termsign, urlno, weight, attribute);
        while (1) {
            int n = scanf("%u:%u|", &pos, &offset);
            if (n != 2) {
                break;
            }
            printf("%u:%u\n", pos, offset);
        }
        last_urlno = urlno;
        urlno = last_urlno;
    }
}
