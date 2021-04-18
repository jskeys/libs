#include <stdio.h>

#include "tlv.h"

const uint32_t k_type_bytes = 2;
const uint32_t k_length_bytes = 4;

void TLVParser_Init(TLVParser_t *this, uint16_t sync_word)
{
    this->state = SYNC;
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
        case SYNC:
            this->sync = (this->sync << 8) | c;
            if (this->sync == this->sync_word)
            {
                this->state = TYPE;
                this->count = 0;
            }
            return 0;
        case TYPE:
            packet->type = (packet->type << 8) | c;
            this->count++;
            if (this->count == k_type_bytes)
            {
                this->state = LENGTH;
                this->count = 0;
            }
            return 0;
        case LENGTH:
            packet->length = (packet->length << 8) | c;
            this->count++;
            if (this->count == k_length_bytes)
            {
                this->count = 0;
                if (packet->length)
                {
                    this->state = VALUE;
                    return 0;
                }
                else
                {
                    this->state = SYNC;
                    return 1;
                }
            }
            else
            {
                return 0;
            }
        case VALUE:
            *(packet->value + this->count) = c;
            this->count++;
            if (this->count == packet->length)
            {
                // Write NULL to after message for string thiss.
                *(packet->value + packet->length) = '\0';
                this->state = SYNC;
                return 1;
            }
            return 0;
        default:
            return -1;
    }
}
