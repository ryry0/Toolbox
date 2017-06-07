#ifndef PACKET_HYBRID_H_
#define PACKET_HYBRID_H_
#include <packet.h>

typedef enum {
  PKT_SET_PID,
  PKT_DEBUG_STR,
  PKT_DATA_START = 's',
  PKT_DATA_STOP = 'n',
  PKT_EN_MOTOR = 'e',
  PKT_DIS_MOTOR = 'd',
  PKT_STR_MSG,
  PKT_ACK = 0x0A,
  PKT_SYN = 0xFF
} pkt_type_t;

typedef struct {
  float setpoint;
  float Kp;
  float Ki;
  float Kd;
  float current;
  int16_t speed;
} pkt_pid_t;

typedef struct {
  char msg[MAX_PAYLOAD_LENGTH];
} pkt_str_msg_t;

inline void pkt_setTypeNoPayload(pkt_generic_t *packet, pkt_type_t type) {
  packet->type = type;
  packet->type_payload_length = 1;
  packet->total_length = PKT_HEADER_LENGTH + 1;
}

#endif
