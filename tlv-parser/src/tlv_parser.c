#include "tlv_parser.h"

const char NULL = '\0';
const uint16_t k_sync_word = 0x5A55;
const uint32_t k_type_bytes = 2;
const uint32_t k_length_bytes = 4;

struct TLVParser {
    TLVParserState_t state;
    uint16_t sync;
    uint32_t count;
};

void TLVParser_Init(TLVParser_t self)
{
    self->state = SYNC;
    self->sync = 0;
    self->count = 0;
}

/// Process an incoming byte. Returns 1 when a complete packet has been processed. A packet is
/// considered complete when `packet->length` bytes have been written to `packet->data`.
uint8_t TLVParser_Parse(TLVParser_t self, TLVPacket_t* packet, char next)
{
  switch (self->state)
  {
    case SYNC:
      self->sync = (self->sync << 8) | next;
      if (self->sync == k_sync_word)
      {
        self->state = TYPE;
        self->count = 0;
      }
      return 0;
    case TYPE:
      packet->type = (packet->type << 8) | next;
      self->count++;
      if (self->count == k_type_bytes)
      {
          self->state = LENGTH;
          self->count = 0;
      }
      return 0;
    case LENGTH:
      packet->length = (packet->length <<8) | next;
      self->count++;
      if (self->count == k_length_bytes)
      {
          self->state = VALUE;
          self->count = 0;
      }
      return 0;
    case VALUE:
      *(packet->value + self->count) = next;
      self->count++;
      if (self->count == packet->length)
      {
        // Write NULL to after message for string selfs.
        *(packet->value + packet->length) = NULL;
        self->state = SYNC;
        return 1;
      }
      return 0;
    default:
        return -1;
  }
}
