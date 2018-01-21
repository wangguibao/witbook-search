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

InvertIndex g_invert_index;

char invert_index_unit_buf[INVERT_INDEX_UNIT_SIZE];

bool sort_by_urlno(Invert &lhs, Invert &rhs)
{
    return lhs.urlno < rhs.urlno;
}

int read_records(const char *filename)
{
    char term[TERM_LEN] = {'\0'};
    FILE *file = fopen(filename, "r");
    uint64_t termsign = 0;
    uint32_t urlno = 0;
    uint32_t weight = 0;
    uint32_t attribute = 0;
    uint32_t pos;
    uint32_t offset;
    int nread = 0;
    int nrecord = 0;

    while ((nread = fscanf(file, "%s %lu %u %u %u", term, &termsign, &urlno, &weight, &attribute)) == 5) {
        Invert invert;
        invert.urlno = urlno;
        invert.weight = weight;
        invert.attribute = attribute;

        int offset_count = 0;
        int n = 0;
        while ((n = fscanf(file, "%u:%u|", &pos, &offset) == 2) && offset_count < MAX_OFFSET) {
            invert.pos_offset[offset_count].pos = pos;
            invert.pos_offset[offset_count].offset = offset;
            ++offset_count;
        }
        invert.offset_count = offset_count;
        g_invert_index[termsign].push_back(invert);
        nrecord++;
    }

    fclose(file);

    printf("Read %u records\n", nrecord);
    return 0;
}

int main(int argc, char **argv)
{
    std::ofstream of("ind2.0", std::ios::out);

    // 1. Read records
    int ret = read_records(argv[1]);
    if (ret != 0) {
        printf("Error reading invert records\n");
        return -1;
    }
    // 2. Generate level2 index
    // std::map keeps order by key which is termsign
    Level2IndexBlockHeader level2_index_block_header;
    UrlnoList *urlno_list = new UrlnoList;
    WeightList *weight_list = new WeightList;
    AttributeList *attribute_list = new AttributeList;
    CountList *count_list = new CountList;
    OffsetList *offset_list = new OffsetList;
    for (auto &individual_term_invert : g_invert_index) {
        uint64_t termsign = individual_term_invert.first;
        int url_count = 0;
        int offset_count = 0;
        int count_all_one = 1;
        int offset_all_zero = 1;


        std::vector<Invert> &url_vec = individual_term_invert.second;
        std::sort(url_vec.begin(), url_vec.end(), sort_by_urlno);

        for (auto &individual_url : url_vec) {
            urlno_list->urlno[url_count] = individual_url.urlno;
            weight_list->weight[url_count] = individual_url.weight;
            attribute_list->attribute[url_count] = individual_url.attribute;
            count_list->count[url_count] = individual_url.offset_count;
            if (count_list->count[url_count] != 1) {
                count_all_one = 0;
            }
            for (int i = 0; i < individual_url.offset_count; ++i) {
                offset_list->offset[offset_count].pos = individual_url.pos_offset[i].pos;
                offset_list->offset[offset_count].offset = individual_url.pos_offset[i].offset;
                if (offset_list->offset[offset_count].offset != 0) {
                    offset_all_zero = 0;
                }
                ++offset_count;
            }
            ++url_count;
        }

        level2_index_block_header.url_compressed = 0;
        level2_index_block_header.weight_attribute_compressed = 0;
        level2_index_block_header.count_all_one = count_all_one;
        level2_index_block_header.count_compressed = 0;
        level2_index_block_header.offset_all_zero = offset_all_zero;
        level2_index_block_header.offset_compressed = 0;

        int buf_used = 0;
        memcpy(invert_index_unit_buf, &level2_index_block_header, sizeof(level2_index_block_header));
        buf_used += sizeof(level2_index_block_header);

        memcpy(invert_index_unit_buf + buf_used, &url_count, sizeof(url_count));
        buf_used += sizeof(url_count);

        memcpy(invert_index_unit_buf + buf_used, (void *)&urlno_list->urlno[0], sizeof(urlno_t) * url_count);
        buf_used += sizeof(urlno_t) * url_count;

        memcpy(invert_index_unit_buf + buf_used, (void *)&weight_list->weight[0], sizeof(weight_t) * url_count);
        buf_used += sizeof(weight_t) * url_count;

        memcpy(invert_index_unit_buf + buf_used, (void *)&attribute_list->attribute[0], sizeof(attribute_t) * url_count);
        buf_used += sizeof(attribute_t) * url_count;

        uint8_t count_width = sizeof(count_t);
        memcpy(invert_index_unit_buf + buf_used, &(count_width), sizeof(count_width));
        buf_used += sizeof(count_width);

        uint8_t offset_width = sizeof(PosOffset);
        memcpy(invert_index_unit_buf + buf_used, &(offset_width), sizeof(offset_width));
        buf_used += sizeof(offset_width);

        memcpy(invert_index_unit_buf + buf_used, (void *)&count_list->count[0], sizeof(count_t) * url_count);
        buf_used += sizeof(count_t) * url_count;

        memcpy(invert_index_unit_buf + buf_used, (void *)&offset_list->offset[0], sizeof(PosOffset) * offset_count);
        buf_used += sizeof(PosOffset) * offset_count;

        of.write(invert_index_unit_buf, buf_used);
    }

    of.close();
}
