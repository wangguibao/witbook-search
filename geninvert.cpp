/*
 * geninvert.cpp
 * @author Wang Guibao
 * @brief This is part of WITBOOK Search Project. The goal is to provide a
 * universal searching utility, with which you can search the Internet for
 * freely available books by title, author name, year etc.
 *
 * This geninvert tool generates invert index records from text databases, like
 * text files.
 *
 * Input is text format, consisting of below fields separated by TABs
 * TITLE    AUTHOR    YEAR    DESCRIPTION    COVER_IMAGE_URL    ONLINE_URL_LIST    DOWNLOAD_URL_LIST
 *
 * where
 * COVER_IMAGE_URL is a book cover image URL
 * ONLINE_URL_LIST is a list of URLs where you can read the book online
 * DOWNLOAD_URL_LIST is a list of URLs where you can download the e-book
 */
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <stdint.h>
#include "util.h"

const int MAX_OFFSET = 5;
const int MAX_RECORD = 100;
const int MAX_URL_NUM = 15;
const int MAX_TERM_LEN = 3;
const int BUF_LEN = 1024;

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

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "Usage: geninvert TEXT_FILE" << std::endl;
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

    int i = 0;
    int j = 0;
    std::string line;
    while (std::getline(fs, line)) {
        p = record;
        *p = '\0';
        // 1. term
        int term_len = rand() % MAX_TERM_LEN + 1;
        p += select_utf8_char(fs, file_size, p, term_len);
        *p = '\t';
        ++p;

        // 2. termsign
        snprintf(p, BUF_LEN - (p - record), "%lu", creat_sign(record, p - record));
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
