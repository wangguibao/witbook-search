#include <iostream>
#include <fstream>
#include "invert2index.h"

int main(int argc, char **argv)
{
    FILE *file = fopen(argv[1], "rb");

    Level2IndexBlockHeader level2_index_block_header;
    int term_count = 0;
    UrlnoList *urlno_list = new UrlnoList;
    WeightList *weight_list = new WeightList;
    AttributeList *attribute_list = new AttributeList;
    CountList *count_list = new CountList;
    OffsetList *offset_list = new OffsetList;

    while (!feof(file)) {
        int nread = fread(&level2_index_block_header,
                        sizeof(Level2IndexBlockHeader),
                        1,
                        file);
        if (nread != 1 && !feof(file)) {
            std::cerr << "Corrupted level 2 index block: " << nread << std::endl;
            return -1;
        }
        else if (feof(file)) {
            std::cout << "Traversed level2 index, term_count " << term_count << std::endl;
            return 0;
        }
        ++term_count;

        printf("term %u url_compressed = %u weight_attribute_compressed=%u "
                "count_all_one=%u count_compressed=%u offset_all_zero=%u "
                "offset_compressed=%u\n",
                term_count,
                level2_index_block_header.url_compressed,
                level2_index_block_header.weight_attribute_compressed,
                level2_index_block_header.count_all_one,
                level2_index_block_header.count_compressed,
                level2_index_block_header.offset_all_zero,
                level2_index_block_header.offset_compressed);

        uint32_t url_count;
        size_t n = fread(&url_count, sizeof(url_count), 1, file);
        if (n != 1) {
            std::cerr << "Cannot read url_count" << std::endl;
            return -1;
        }

        n = fread(&urlno_list->urlno[0], sizeof(urlno_t), url_count, file);
        if (n != url_count) {
            std::cerr << "Cannot read urlno_list" << std::endl;
            return -1;
        }

        n = fread(&weight_list->weight[0], sizeof(weight_t), url_count, file);
        if (n != url_count) {
            std::cerr << "Cannot read weight_list" << std::endl;
            return -1;
        }

        n = fread(&attribute_list->attribute[0], sizeof(attribute_t), url_count, file);
        if (n != url_count) {
            std::cerr << "Cannot read attribute_list" << std::endl;
        }

        uint8_t count_width;
        n = fread(&count_width, sizeof(uint8_t), 1, file);
        if (n != 1) {
            std::cerr << "Cannot read count_width" << std::endl;
            return -1;
        }

        uint8_t offset_width;
        n = fread(&offset_width, sizeof(uint8_t), 1, file);
        if (n != 1) {
            std::cerr << "Cannot read offset_width" << std::endl;
            return -1;
        }

        n = fread(&count_list->count[0], sizeof(count_t), url_count, file);
        if (n != url_count) {
            std::cerr << "Cannot read count_list" << std::endl;
            return -1;
        }

        uint32_t offset_count = 0;
        for (unsigned i = 0; i < url_count; ++i) {
            offset_count += count_list->count[i];
        }

        n = fread(&offset_list->offset[0], sizeof(PosOffset), offset_count, file);
        if (n != offset_count) {
            std::cerr << "Cannot read offset_list" << std::endl;
            return -1;
        }

        std::cout << "url_count: " << url_count << std::endl;
        unsigned int offset_index = 0;
        for (unsigned int i = 0; i < url_count; ++i) {
            std::cout << "-----------------------------" << std::endl;
            printf("url_no=%u weight=%u attribute=0x%x count_width=%u offset_width=%u offset_count=%u\n",
                    urlno_list->urlno[i],
                    weight_list->weight[i],
                    attribute_list->attribute[i],
                    count_width,
                    offset_width,
                    count_list->count[i]);
            for (unsigned int j = 0; j < count_list->count[i]; ++j) {
                std::cout << "" << offset_list->offset[offset_index].pos << ":"
                    << offset_list->offset[offset_index].offset << " ";
                ++offset_index;
            }
            std::cout << std::endl;
        }

        if (offset_index != offset_count) {
            std::cerr << "offset_count mismatch " << offset_index << ", " << offset_count << std::endl;
        }

        std::cout << "====================================" << std::endl;
    }

    return 0;
}
