#ifndef __TLV_H__
#define __TLV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifndef TLV_MAX_LENGTH
#define TLV_MAX_LENGTH 1024
#endif

// Forward declare `struct TLVParser`
typedef struct TLVParser_t *TLVParser_t;

typedef struct
{
    uint16_t type;
    uint32_t length;
    char value[TLV_MAX_LENGTH];
} TLVPacket_t;

void TLVParser_Init(TLVParser_t, uint16_t sync_word);
uint8_t TLVParser_Parse(TLVParser_t, TLVPacket_t*, char);

#ifdef __cplusplus
}
#endif

#endif
