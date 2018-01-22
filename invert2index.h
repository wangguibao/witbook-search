#ifndef INVERTTOINDEX_H
#define INVERTTOINDEX_H
#include <map>
#include <vector>
#include "index.h"

typedef struct Invert {
    uint32_t urlno;
    uint16_t weight;
    uint8_t attribute;
    uint8_t offset_count;
    PosOffset pos_offset[MAX_OFFSET];
} Invert;

typedef std::map<uint64_t, std::vector<Invert> > InvertIndex;

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

typedef std::map<uint64_t, uint32_t> Level2IndexTermOffsetMap;
typedef std::map<uint64_t, Level1IndexBlockHeader> Level1IndexBlockHeaders;

#endif // INVERTTOINDEX_H
