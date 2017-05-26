#include <packet.h>
#include <serial.h>
#include <string.h>

#define PACKET_HEADER 0x00
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
bool pkt_readByte(pkt_generic_t *packet, sr_port_t port) {
  uint8_t input = 0;

  //make sure it is actually reading something.
  if (!sr_ReadPort(port, &input, sizeof(input)))
    return false;

  printf("%d:%x ", packet->index, input);

  if ((packet->index == 0) && (input != PACKET_HEADER))
    return false;

  packet->data[packet->index] = input;
  packet->index++;

  if ((packet->index > LENGTH_INDEX) &&
      (packet->index >= packet->payload_len + 4)) {
    packet->index = 0;
    printf("\n");
    return true;
  }
  return false;
}

/* piecewise read returns true when finished reading */
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

