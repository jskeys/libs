#ifndef __TLV_H__
#define __TLV_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#ifndef TLV_MAX_LENGTH
#define TLV_MAX_LENGTH 1024
#endif

    typedef enum
    {
        GET_SYNC,
        GET_TYPE,
        GET_LENGTH,
        GET_VALUE,
    } TLVParserState_t;

    typedef struct
    {
        TLVParserState_t state;
        uint16_t sync_word;
        uint16_t sync;
        uint32_t count;
    } TLVParser_t;

    typedef struct
    {
        uint16_t type;
        uint32_t length;
        char *value;
    } TLVPacket_t;

    void TLVParser_Init(TLVParser_t *, uint16_t sync_word);
    uint8_t TLVParser_Parse(TLVParser_t *, TLVPacket_t *packet, char c);

#ifdef __cplusplus
}
#endif

#endif
