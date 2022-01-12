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

    typedef struct
    {
        uint32_t type;
        uint32_t length;
        char *value;
    } TLVPacket_t;

    typedef enum
    {
        PARSE_SYNC,
        PARSE_TYPE,
        PARSE_LENGTH,
        PARSE_VALUE,
    } TLVParserState_t;

    typedef struct
    {
        TLVParserState_t state;
        uint16_t sync_word;
        uint16_t sync;
        uint32_t num_state_bytes_parsed;
        uint32_t num_chars_parsed;
    } TLVParser_t;

    int32_t tlv_write_packet_to_buffer(const TLVPacket_t *packet, uint8_t *buffer);
    uint8_t tlv_process_char(TLVParser_t *, TLVPacket_t *packet, char c);
    void tlv_initialize_parser(TLVParser_t *, uint16_t sync_word);
    void tlv_print_packet(const TLVPacket_t *packet);

#ifdef __cplusplus
}
#endif

#endif
