/*
 * invert2ind2.cpp
 * @author Wang Guibao
 * @brief Convert invert index to level2 disk format
 *
 * Assuming input format:
 * term \t termsign \t urlno \t weight \t attribute \t pos:offset|pos:offset|...
 * which is already sorted by termsign
 *
 */
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include "invert2index.h"

char invert_index_unit_buf[INVERT_INDEX_UNIT_SIZE];

bool sort_by_urlno(Invert &lhs, Invert &rhs)
{
    return lhs.urlno < rhs.urlno;
}

int main(int argc, char **argv)
{
    InvertIndex invert_index;

    char term[TERM_LEN];
    std::ofstream of("ind2.0", std::ios::out);
#if 1
    // 1. Read in individual invert record
    while (!feof(stdin)) {
#else
    FILE *file = fopen(argv[1], "r");
    while (!feof(file)) {
#endif
        uint64_t termsign = 0;
        uint32_t urlno = 0;
        uint32_t weight = 0;
        uint32_t attribute = 0;
        uint32_t pos;
        uint32_t offset;

        int nread = scanf("%s %I64u %u %u %u", term, &termsign, &urlno, &weight, &attribute);
        if (nread != 5) {
            continue;
        }

        Invert invert;
        invert.urlno = urlno;
        invert.weight = weight;
        invert.attribute = attribute;

        int offset_count = 0;
        while (offset_count < MAX_OFFSET) {
            int n = scanf("%u:%u|", &pos, &offset);
            if (n != 2) {
                break;
            }
            invert.post_offset[offset_count].pos = pos;
            invert.post_offset[offset_count].offset = offset;
            ++offset_count;
        }
        invert.offset_count = offset_count;
        invert_index[termsign].push_back(invert);
    }

    // 2. Generate level2 index
    // std::map keeps order by key which is termsign
    for (auto &individual_term_invert : invert_index) {
        uint64_t termsign = individual_term_invert.first;
        Level2IndexHeader level2_index_header;
        UrlnoList urlno_list;
        WeightList weight_list;
        AttributeList attribute_list;
        CountList count_list;
        OffsetList offset_list;
        int url_count = 0;
        int offset_count = 0;
        int count_all_one = 1;
        int offset_all_zero = 1;


        std::vector<Invert> &url_vec = individual_term_invert.second;
        std::sort(url_vec.begin(), url_vec.end(), sort_by_urlno);

        for (auto &individual_url : url_vec) {
            urlno_list.url[url_count] = individual_url.urlno;
            weight_list.weight[url_count] = individual_url.weight;
            attribute_list.attribute[url_count] = individual_url.attribute;
            count_list.count[url_count] = individual_url.offset_count;
            if (offset_count != 1) {
                count_all_one = 0;
            }
            for (auto pos_offset : individual_url.post_offset) {
                offset_list.offset[offset_count].pos = pos_offset.pos;
                offset_list.offset[offset_count].offset = pos_offset.offset;
                if (pos_offset.offset != 0) {
                    offset_all_zero = 0;
                }
                ++offset_count;
            }
            ++url_count;
        }

        level2_index_header.url_compressed = 0;
        level2_index_header.weight_attribute_compressed = 0;
        level2_index_header.count_all_one = count_all_one;
        level2_index_header.count_compressed = 0;
        level2_index_header.offset_all_zero = offset_all_zero;
        level2_index_header.offset_compressed = 0;

        int buf_used = 0;
        memcpy(invert_index_unit_buf, &level2_index_header, sizeof(level2_index_header));
        buf_used += sizeof(level2_index_header);
        memcpy(invert_index_unit_buf + buf_used, &url_count, sizeof(url_count));
        buf_used += sizeof(url_count);
        memcpy(invert_index_unit_buf + buf_used, (void *)&urlno_list, sizeof(urlno_t) * url_count);
        buf_used += sizeof(urlno_t) * url_count;
        memcpy(invert_index_unit_buf + buf_used, (void *)&weight_list, sizeof(weight_t) * url_count);
        buf_used += sizeof(weight_t) * url_count;
        memcpy(invert_index_unit_buf + buf_used, (void *)&attribute_list, sizeof(attribute_t) * url_count);
        buf_used += sizeof(attribute_t) * url_count;

        uint8_t count_width = sizeof(count_t);
        memcpy(invert_index_unit_buf + buf_used, &(count_width), sizeof(count_width));
        buf_used += sizeof(count_width);

        uint8_t offset_width = sizeof(PosOffset);
        memcpy(invert_index_unit_buf + buf_used, &(offset_width), sizeof(offset_width));
        buf_used += sizeof(offset_width);

        memcpy(invert_index_unit_buf + buf_used, (void *)&count_list, sizeof(count_t) * url_count);
        buf_used += sizeof(count_t) * url_count;

        memcpy(invert_index_unit_buf + buf_used, (void *)&offset_list, sizeof(PosOffset) * offset_count);
        buf_used += sizeof(PosOffset) * offset_count;

        of.write(invert_index_unit_buf, buf_used);
    }

    of.close();
}
