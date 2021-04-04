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
        SYNC,
        TYPE,
        LENGTH,
        VALUE,
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
        char value[TLV_MAX_LENGTH];
    } TLVPacket_t;

    void TLVParser_Init(TLVParser_t *parser, uint16_t sync_word);
    uint8_t TLVParser_Parse(TLVParser_t *parser, TLVPacket_t *packet, char c);

#ifdef __cplusplus
}
#endif

#endif
