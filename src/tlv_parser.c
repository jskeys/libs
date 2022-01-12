#include <stdio.h>
#include <string.h>

#include "tlv.h"

const uint32_t k_type_bytes = 4;
const uint32_t k_length_bytes = 4;

/**
 * @brief Print packet contents in the format TTTT LLLL VV VV VV ...
 */
void tlv_print_packet(const TLVPacket_t *packet)
{
    printf("%04X ", packet->type);
    printf("%04X ", packet->length);

    for (int i = 0; i < (int)packet->length; i++)
    {
        printf("%02X ", *(packet->value + i));
    }
    printf("\n");
}

/**
 * @brief Copy raw packet contents to a buffer
 */
int32_t tlv_write_packet_to_buffer(const TLVPacket_t *packet, uint8_t *buffer)
{
    int32_t offset = 0;

    memcpy(buffer + offset, &packet->type, sizeof(packet->type));
    offset += sizeof(packet->type);
    memcpy(buffer + offset, &packet->length, sizeof(packet->length));
    offset += sizeof(packet->length);
    memcpy(buffer + offset, packet->value, packet->length);

    return offset + packet->length;
}

/**
 * @brief Initialize a parser struct to the default state
 */
void tlv_initialize_parser(TLVParser_t *parser, uint16_t sync_word)
{
    parser->state = PARSE_SYNC;
    parser->sync_word = sync_word;
    parser->sync = 0;
    parser->num_state_bytes_parsed = 0;
    parser->num_chars_parsed = 0;
}

/**
 * @brief Process one byte using the specified parser
 */
uint8_t tlv_process_char(TLVParser_t *parser, TLVPacket_t *packet, char c)
{
    parser->num_chars_parsed ++;
    switch (parser->state)
    {
        case PARSE_SYNC:
            parser->sync = (parser->sync << 8) | c;
            if (parser->sync == parser->sync_word)
            {
                parser->state = PARSE_TYPE;
                parser->num_state_bytes_parsed = 0;
            }
            return 0;
        case PARSE_TYPE:
            packet->type = (packet->type << 8) | c;
            parser->num_state_bytes_parsed++;
            if (parser->num_state_bytes_parsed == k_type_bytes)
            {
                parser->state = PARSE_LENGTH;
                parser->num_state_bytes_parsed = 0;
            }
            return 0;
        // Parse the packet length. In the case of a zero-length value, return
        // 1 and go back to the `PARSE_SYNC` state. Otherwise, enter the
        // `PARSE_VALUE` state
        case PARSE_LENGTH:
            packet->length = (packet->length << 8) | c;
            parser->num_state_bytes_parsed++;
            if (parser->num_state_bytes_parsed == k_length_bytes)
            {
                parser->num_state_bytes_parsed = 0;
                if (packet->length)
                {
                    parser->state = PARSE_VALUE;
                    return 0;
                }
                else
                {
                    parser->state = PARSE_SYNC;
                    return 1;
                }
            }
            else
            {
                return 0;
            }
        case PARSE_VALUE:
            *(packet->value + parser->num_state_bytes_parsed) = c;
            parser->num_state_bytes_parsed++;
            if (parser->num_state_bytes_parsed == packet->length)
            {
                parser->state = PARSE_SYNC;
                return 1;
            }
            return 0;
        default:
            return -1;
    }
}
