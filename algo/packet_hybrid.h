#ifndef PACKET_HYBRID_H_
#define PACKET_HYBRID_H_
#include <packet.h>

//can't use sizeof structs because of struct packing... well you could
#define PKT_DBG_PID_LEN (1 + 9*sizeof(float) + 1*sizeof(uint16_t) + 1*sizeof(uint8_t))
#define PKT_SET_PID_LEN (1 + 4*sizeof(float))

typedef enum pkt_type_s {
  PKT_SET_PID,
  PKT_DBG_PID,
  PKT_ZERO_PID,
  PKT_DEBUG_STR,
  PKT_SET_SIN_PID,
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
  float speed;
  float position;
  float current_command;
  float speed_command;
  uint16_t pwm_command;
  uint8_t direction;
} pkt_dbg_pid_t;

typedef struct {
  float setpoint;
  float Kp;
  float Ki;
  float Kd;
} pkt_set_pid_t;

typedef struct {
  char msg[MAX_PAYLOAD_LENGTH];
} pkt_str_msg_t;


#endif
