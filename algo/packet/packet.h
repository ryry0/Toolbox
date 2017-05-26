#ifndef PACKET_H_
#define PACKET_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <serial.h>


#define MAX_PACKET_LENGTH 251

typedef enum {
  PKT_SET_PID,
  PKT_DEBUG_STR,
  PKT_DATA_START = 's',
  PKT_DATA_STOP = 'n',
  PKT_EN_MOTOR = 'e',
  PKT_DIS_MOTOR = 'd',
  PKT_ACK = 0x0A,
  PKT_SYN = 0xFF
} pkt_type_t;

typedef struct {
  union {
    struct {
      uint8_t header;
      uint8_t cobs_byte;
      uint8_t payload_len;
      uint8_t type;
      uint8_t payload[MAX_PACKET_LENGTH];
    };
    uint8_t data[255];
  };
  size_t index;
} pkt_generic_t;

typedef struct {
  float setpoint;
  float Kp;
  float Ki;
  float Kd;
  float current;
  int16_t speed;
} pkt_pid_t;

void pkt_init(pkt_generic_t *packet);
bool pkt_readByte(pkt_generic_t *packet, sr_port_t port);
bool pkt_send(pkt_generic_t *packet, sr_port_t port);
void pkt_print(pkt_generic_t *packet);
void pkt_clear(pkt_generic_t *packet);

#endif
