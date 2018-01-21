/*
 * geninvert.cpp
 * @author Wang Guibao
 * @brief Generates pesudo inverted index records
 */
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <stdint.h>

const int MAX_OFFSET = 5;
const int MAX_RECORD = 100;
const int MAX_URL_NUM = 15;
const int MAX_TERM_LEN = 3;
const int BUF_LEN = 1024;

/**
 * Generate a utf-8 character
 * The rule is:
 * 1) randomly select a char-code between 0x4e00 - 0x9fa5
 * 2) Apply the bits to bellow 3-byte sequence
 * 0b1110xxxx 0b10xxxxxx 0b10xxxxxx
 */
void gen_utf8_char(char *p)
{
    int charcode = rand() % (0x9fa5 - 0x4e00) + 0x4e00;
    *p = static_cast<char>(0xe0 | (charcode >> 12));
    *(p + 1) = static_cast<char>(0x80 | ((charcode >> 6) & 0x3f));
    *(p + 2) = static_cast<char>(0x80 | (charcode & 0x3f));
}

int select_utf8_char(std::ifstream &fs, int file_size, char *p, int term_len)
{
    int len = 0;
    for (int i = 0; i < term_len; ++i) {
        int pos = rand() % file_size;
        fs.seekg(pos);
        unsigned char c = fs.get();

        while (c < 0xe0 || c > 0xef) {
            c = fs.get();
            if (c == 0xff) {
                fs.clear();
                pos = rand() % file_size;
                fs.seekg(pos);
            }
        }

        *(p + 3 * i) = c;
        *(p + 3 * i + 1) = fs.get();
        *(p + 3 * i + 2) = fs.get();
        len += 3;
    }

    return len;
}

unsigned int creat_sign(char *str, int len) {
    uint64_t sign = 0;
    for (int i = 0; i < len; ++i) {
        sign += (sign * 0x1331) + str[i];
    }

    return sign;
}

int main(int argc, char **argv)
{
    if (argc != 4) {
        std::cerr << "Usage: geninvert TEXT_FILE RECORD_NUM URL_NUM" << std::endl;
        return -1;
    }

    std::ifstream fs(argv[1], std::ios::in);
    if (fs.bad()) {
        std::cerr << "Error reading " << argv[1] << std::endl;
        return -1;
    }
    fs.seekg(0, std::ios::end);
    int file_size = fs.tellg();
    fs.seekg(0, std::ios::beg);

    int record_num = MAX_RECORD;
    record_num = atoi(argv[2]);
    if (record_num <= 0) {
        record_num = MAX_RECORD;
    }

    int url_num = atoi(argv[3]);
    if (url_num <= 0) {
        url_num = MAX_URL_NUM;
    }


    srand(time(NULL));

    int i = 0;
    int j = 0;
    char record[BUF_LEN];
    char *p = NULL;
    for (i = 0; i < record_num; ++i) {
        p = record;
        *p = '\0';
        // 1. term
        int term_len = rand() % MAX_TERM_LEN + 1;
        p += select_utf8_char(fs, file_size, p, term_len);
        *p = '\t';
        ++p;

        // 2. termsign
        snprintf(p, BUF_LEN - (p - record), "%u", creat_sign(record, p - record));
        p += strlen(p);
        *p = '\t';
        ++p;

        // 3. urlno
        snprintf(p, BUF_LEN - (p - record), "%u", rand() % url_num);
        p += strlen(p);
        *p = '\t';
        ++p;

        // 4. weight
        snprintf(p, BUF_LEN - (p - record), "%u", rand() % 0xffff);
        p += strlen(p);
        *p = '\t';
        ++p;

        // 5. attribute
        snprintf(p, BUF_LEN - (p - record), "%u", rand() % 0xff);
        p += strlen(p);
        *p = '\t';
        ++p;

        // 6. pos & offset list
        int offset_size = rand() % 0xff + 1;
        for (j = 0; j < offset_size && (p - record < BUF_LEN - 32); ++j) {
            snprintf(p, BUF_LEN - (p - record), "%u:%u|", rand() % 4, rand() % 0x3fff);
            p +=strlen(p);
        }
        // Remove last '|' and mark end of record;
        p[strlen(p) - 1] = '\0';

        std::cout << record << std::endl;
    }

    return 0;
}

