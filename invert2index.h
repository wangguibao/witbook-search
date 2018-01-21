#ifndef INVERTTOINDEX_H_INCLDED
#define INVERTTOINDEX_H_INCLUDED
#include <stdint.h>
#include <map>
#include <vector>

const int TERM_LEN = 128 * 3;
const int MAX_OFFSET = 255;
const int MAX_URL_PER_TERM = 100000;

typedef struct PosOffset {
    uint16_t pos:2;
    uint16_t offset:14;
} PosOffset;

typedef struct Invert {
    uint32_t urlno;
    uint16_t weight;
    uint8_t attribute;
    uint8_t offset_count;
    PosOffset pos_offset[MAX_OFFSET];
} Invert;

typedef std::map<uint64_t, std::vector<Invert> > InvertIndex;

typedef struct _Level2IndexBlockHeader {
    uint8_t url_compressed:1;
    uint8_t weight_attribute_compressed:1;
    uint8_t count_all_one:1;
    uint8_t count_compressed:1;
    uint8_t offset_all_zero:1;
    uint8_t offset_compressed:1;
    uint8_t padding:2;
} Level2IndexBlockHeader;

typedef uint32_t urlno_t;
typedef uint16_t weight_t;
typedef uint8_t attribute_t;
typedef uint8_t count_t;

typedef struct _UrlnoList {
    urlno_t urlno[MAX_URL_PER_TERM];
} UrlnoList;

typedef struct _WeightList {
    weight_t weight[MAX_URL_PER_TERM];
} WeightList;

typedef struct _AttributeList {
    attribute_t attribute[MAX_URL_PER_TERM];
} AttributeList;

typedef struct _CountList {
    count_t count[MAX_URL_PER_TERM];
} CountList;

typedef struct _OffsetList {
    PosOffset offset[MAX_URL_PER_TERM * MAX_OFFSET];
} OffsetList;

const int INVERT_INDEX_UNIT_SIZE = sizeof(Level2IndexBlockHeader)
                                    + sizeof(uint32_t)
                                    + sizeof(UrlnoList)
                                    + sizeof(WeightList)
                                    + sizeof(AttributeList)
                                    + sizeof(CountList)
                                    + sizeof(OffsetList);

#endif // INVERTTOINDEX_H_INCLUDED
