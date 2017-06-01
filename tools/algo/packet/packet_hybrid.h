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

#endif
