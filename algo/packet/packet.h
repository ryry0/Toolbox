#ifndef PACKET_H_
#define PACKET_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_PAYLOAD_LENGTH 251
#define PKT_HEADER_LENGTH 3
#define pkt_interp(type, packet) ((type *) (packet.payload))
#define pkt_interpPtr(type, packet) ((type *) (packet->payload))

typedef struct {
  union {
    struct {
      uint8_t header;
      uint8_t cobs_byte;
      uint8_t type_payload_length; //includes the type, so always nonzero
      uint8_t type;
      uint8_t payload[MAX_PAYLOAD_LENGTH];
    };
    uint8_t data[255];
  };
  size_t index;
  uint8_t total_length;
} pkt_generic_t;

typedef void (*pkt_handler_func_t)(pkt_generic_t *);

void pkt_init(pkt_generic_t *packet);

bool pkt_readByte(pkt_generic_t *packet, uint8_t input);
void pkt_decodePacket(pkt_generic_t *packet);

//encode buffer performs encoding of the packet so that it is ready to send.
//returns a byte pointer to the packet structure.
//assumes total length field has been set.
uint8_t *pkt_encodeBuffer(pkt_generic_t *packet);
void pkt_print(pkt_generic_t *packet);
void pkt_clear(pkt_generic_t *packet);

bool pkt_decodeByteHandler(pkt_generic_t *packet, uint8_t input,
    pkt_handler_func_t callback);

inline size_t pkt_getIndex(const pkt_generic_t *packet) { return packet->index; }

inline size_t pkt_getTotalLength(const pkt_generic_t *packet)
{ return packet->total_length; }

inline void pkt_setTotalLength(pkt_generic_t *packet, size_t total_length)
{ packet->total_length = total_length; }


inline void pkt_setHeader(pkt_generic_t *packet, uint8_t type, size_t length) {
  packet->header = 0x00;
  packet->cobs_byte = 0x00;
  packet->type = type;
  packet->type_payload_length = length;
  packet->total_length = PKT_HEADER_LENGTH + length;
}

#endif
