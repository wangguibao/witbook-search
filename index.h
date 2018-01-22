#ifndef INDEX_H
#define INDEX_H
#include <stdint.h>

const int TERM_LEN = 128 * 3;
const int MAX_OFFSET = 255;
const int MAX_URL_PER_TERM = 100000;
const int LEVEL1_INDEX_BLOCK_SIZE = 512;
const int LEVEL1_INDEX_BLOCK_MAX_TERMS = 255;

typedef uint32_t urlno_t;
typedef uint16_t weight_t;
typedef uint8_t attribute_t;
typedef uint8_t count_t;
typedef uint64_t termsign_t;
typedef uint32_t offset_t;

#pragma pack(push, 1)
typedef struct _Level1IndexBlockHeader{
    uint64_t termsign0;                 // First termsign in this block
    uint32_t offset0;                   // File offset of first termsign
    uint8_t termsign_same_bit_width:3;  // Number of same bits of termsigns in this block
    uint8_t offset_same_bit_width:5;    // Number of same bits of offsets in this block
    uint8_t term_count;                 // Number of termsigns (and offsets) in this block
} Level1IndexBlockHeader;
#pragma pack(pop)

typedef struct _Level2IndexBlockHeader {
    uint8_t url_compressed:1;
    uint8_t weight_attribute_compressed:1;
    uint8_t count_all_one:1;
    uint8_t count_compressed:1;
    uint8_t offset_all_zero:1;
    uint8_t offset_compressed:1;
    uint8_t padding:2;
} Level2IndexBlockHeader;

typedef struct PosOffset {
    uint16_t pos:2;
    uint16_t offset:14;
} PosOffset;

#endif
