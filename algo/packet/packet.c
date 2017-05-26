#include <packet.h>
#include <string.h>
#include <cobs.h>

#define PACKET_HEADER 0x00
#define HEADER_LENGTH 3
#define HEADER_INDEX 0
#define COBS_INDEX 1
#define LENGTH_INDEX 2
#define TYPE_INDEX 3

void pkt_init(pkt_generic_t *packet) {
  memset(packet, 0, sizeof(pkt_generic_t));
}


void pkt_clear(pkt_generic_t *packet) {
  memset(packet, 0, sizeof(pkt_generic_t));
}

/* piecewise read returns true when finished reading */
bool pkt_decodeByte(pkt_generic_t *packet, uint8_t input) {
  printf("%d:%x ", packet->index, input);

  if ((packet->index == 0) && (input != PACKET_HEADER))
    return false;

  packet->data[packet->index] = input;
  packet->index++;

  if ((packet->index > LENGTH_INDEX) &&
      (packet->index >= packet->length + HEADER_LENGTH)) {
    packet->index = 0;
    packet->total_length = packet->length + HEADER_LENGTH;

    cobs_decodeInPlace(packet->data, packet->total_length);

    printf("\n");
    return true;
  }
  return false;
}

uint8_t *pkt_encodeBuffer(pkt_generic_t *packet) {
  cobs_encodeInPlace(packet->data, packet->total_length);
  return packet->data;
}

/* piecewise read returns true when finished reading */
/*
static bool pkt_readInputBe(pkt_generic_t *packet, sr_port_t port) {
  uint8_t input = 0;

  sr_ReadPort(port, &input, sizeof(input));
  if ((packet->index == 0) && (input != PACKET_HEADER))
    return false;

  packet->data[MAX_PACKET_LENGTH - 1 - packet->index] = input;
  packet->index++;
  if (packet->index >= MAX_PACKET_LENGTH) {
    packet->index = 0;
    return true;
  }
  return false;
}
*/

void pkt_print(pkt_generic_t *packet) {
  /*
  printf("header: %x\n", packet->header_byte);
  printf("imu_ax: %d\n", packet->imu_ax);
  printf("imu_ay: %d\n", packet->imu_ay);
  printf("imu_az: %d\n", packet->imu_az);
  printf("imu_gx: %d\n", packet->imu_gx);
  printf("imu_gy: %d\n", packet->imu_gy);
  printf("imu_gz: %d\n", packet->imu_gz);
  */
}

