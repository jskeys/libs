#ifndef __PARSER__H
#define __PARSER__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

const uint16_t k_sync_word = 0x5A55;
const uint32_t k_type_bytes = 2;
const uint32_t k_length_bytes = 4;

typedef enum
{
    SYNC,
    TYPE,
    LENGTH,
    VALUE,
} TLVParserState_t;

typedef struct
{
    uint16_t type;
    uint32_t length;
    char* value;
} TLVPacket_t;

typedef struct {
    TLVParserState_t state;
    uint16_t sync;
    uint32_t count;
} TLVParser_t;

void TLVParser_Init(TLVParser_t*);
uint8_t TLVParser_Parse(TLVParser_t*, TLVPacket_t*, char);

#ifdef __cplusplus
}
#endif

#endif
