#include "tlv_parser.h"

const char NULL = '\0';

void TLVParser_Init(TLVParser_t* parser)
{
    parser->state = SYNC;
    parser->sync = 0;
    parser->count = 0;
}

/// Process an incoming byte. Returns 1 when a complete packet has been processed. A packet is
/// considered complete when `packet->length` bytes have been written to `packet->data`.
uint8_t TLVParser_Parse(TLVParser_t* parser, TLVPacket_t* packet, char next)
{
  switch (parser -> state)
  {
    case SYNC:
      parser->sync = (parser->sync << 8) | next;
      if (parser->sync == k_sync_word)
      {
        parser->state = TYPE;
        parser->count = 0;
      }
      return 0;
    case TYPE:
      packet->type = (packet->type << 8) | next;
      parser->count++;
      if (parser->count == k_type_bytes)
      {
          parser->state = LENGTH;
          parser->count = 0;
      }
      return 0;
    case LENGTH:
      packet->length = (packet->length <<8) | next;
      parser->count++;
      if (parser->count == k_length_bytes)
      {
          parser->state = VALUE;
          parser->count = 0;
      }
      return 0;
    case VALUE:
      *(packet->value + parser->count) = next;
      parser->count++;
      if (parser->count == packet->length)
      {
        // Write NULL to after message for string parsers.
        *(packet->value + packet->length) = NULL;
        parser->state = SYNC;
        return 1;
      }
      return 0;
    default:
        return -1;
  }
}
