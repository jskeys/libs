#include <stdio.h>
#include <string.h>

#include "tlv.h"

const uint32_t k_type_bytes = 2;
const uint32_t k_length_bytes = 4;

int32_t TLVPacket_Copy(const TLVPacket_t *packet, uint8_t *buffer)
{
    int32_t offset = 0;

    memcpy(buffer + offset, &packet->type, sizeof(packet->type));
    offset += sizeof(packet->type);
    memcpy(buffer + offset, &packet->length, sizeof(packet->length));
    offset += sizeof(packet->length);
    memcpy(buffer + offset, packet->value, packet->length);

    return offset + packet->length;
}

void TLVParser_Init(TLVParser_t *this, uint16_t sync_word)
{
    this->state = GET_SYNC;
    this->sync_word = sync_word;
    this->sync = 0;
    this->count = 0;
}

/// Process an incoming byte. Returns 1 when a complete packet has been processed. A
/// packet is considered complete when `packet->length` bytes have been written to
/// `packet->data`.
uint8_t TLVParser_Parse(TLVParser_t *this, TLVPacket_t *packet, char c)
{
    switch (this->state)
    {
        case GET_SYNC:
            this->sync = (this->sync << 8) | c;
            if (this->sync == this->sync_word)
            {
                this->state = GET_TYPE;
                this->count = 0;
            }
            return 0;
        case GET_TYPE:
            packet->type = (packet->type << 8) | c;
            this->count++;
            if (this->count == k_type_bytes)
            {
                this->state = GET_LENGTH;
                this->count = 0;
            }
            return 0;
        case GET_LENGTH:
            // Parse the packet length. In the case of a zero-length value, return
            // 1 and go back to the `GET_SYNC` state. Otherwise, enter the
            // `GET_VALUE` state
            packet->length = (packet->length << 8) | c;
            this->count++;
            if (this->count == k_length_bytes)
            {
                this->count = 0;
                if (packet->length)
                {
                    this->state = GET_VALUE;
                    return 0;
                }
                else
                {
                    this->state = GET_SYNC;
                    return 1;
                }
            }
            else
            {
                return 0;
            }
        case GET_VALUE:
            *(packet->value + this->count) = c;
            this->count++;
            if (this->count == packet->length)
            {
                // Write '\0' after value.
                *(packet->value + packet->length) = '\0';
                this->state = GET_SYNC;
                return 1;
            }
            return 0;
        default:
            return -1;
    }
}
