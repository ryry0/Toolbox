#ifndef PACKET_H_
#define PACKET_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_PACKET_LENGTH 251
#define pkt_interp(type, packet) ((type *) (packet.payload))
#define pkt_interpPtr(type, packet) ((type *) (packet->payload))

typedef struct {
  union {
    struct {
      uint8_t header;
      uint8_t cobs_byte;
      uint8_t length; //includes the type, so always nonzero
      uint8_t type;
      uint8_t payload[MAX_PACKET_LENGTH];
    };
    uint8_t data[255];
  };
  size_t index;
  uint8_t total_length;
} pkt_generic_t;

void pkt_init(pkt_generic_t *packet);
bool pkt_decodeByte(pkt_generic_t *packet, uint8_t input);

//encode buffer performs encoding of the packet so that it is ready to send.
//returns a byte pointer to the packet structure.
//assumes total length field has been set.
uint8_t *pkt_encodeBuffer(pkt_generic_t *packet);
void pkt_print(pkt_generic_t *packet);
void pkt_clear(pkt_generic_t *packet);

inline size_t pkt_getIndex(pkt_generic_t *packet) { return packet->index; };

inline size_t pkt_getTotalLength(pkt_generic_t *packet)
{ return packet->total_length };

inline void pkt_setTotalLength(pkt_generic_t *packet, size_t total_length)
{ packet->total_length = total_length };

#endif
