#ifndef __PARSER__H
#define __PARSER__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifndef TLV_MAX_LENGTH
#define TLV_MAX_LENGTH 1024
#endif

// Forward declare `struct TLVParser`
typedef struct TLVParser *TLVParser_t;

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
    char value[TLV_MAX_LENGTH];
} TLVPacket_t;


void TLVParser_Init(TLVParser_t);
uint8_t TLVParser_Parse(TLVParser_t, TLVPacket_t*, char);

#ifdef __cplusplus
}
#endif

#endif
