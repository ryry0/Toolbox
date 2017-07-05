#ifndef PACKET_HYBRID_H_
#define PACKET_HYBRID_H_
#include <packet.h>

//can't use sizeof structs because of struct packing... well you could
//TODO: update all lengths

typedef enum pkt_type_e {
  PKT_ACK         = 0x0A,
  PKT_SET_PID,
  PKT_GET_PID,
  PKT_DBG_PID,
  PKT_ZERO_PID,
  PKT_STR_MSG,
  PKT_LOG_DATA,
  PKT_LOG_PARAMS,
  PKT_SET_PARAMS,
  PKT_SET_STATE,
  PKT_DATA_START  = 's',
  PKT_DATA_STOP   = 'n',
  PKT_EN_MOTOR    = 'e',
  PKT_DIS_MOTOR   = 'd',
  PKT_SYN         = 0xFF
} pkt_type_t;

/*----------------------------------------------------------------------------*/
//TODO: add pid_id
#define PKT_DBG_PID_LEN (11*sizeof(float) + 1*sizeof(uint8_t))
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
  float pwm_command;
  float direction;
} pkt_dbg_pid_t;

/*----------------------------------------------------------------------------*/
//TODO: add white button press
//TODO: add timestamp
#define PKT_LOG_DATA_LEN ()
typedef struct {
  uint16_t an_0_l_fsr_heel;
  uint16_t an_2_r_fsr_heel;
  uint16_t an_4_l_hip_enc;
  uint16_t an_6_l_knee_enc;
  uint16_t an_12_mot1_anout1;
  uint16_t an_14_mot2_anout1;
  uint16_t an_1_l_fsr_ff;
  uint16_t an_3_r_fsr_ff;
  uint16_t an_5_r_hip_enc;
  uint16_t an_7_r_knee_enc;
  uint16_t an_11_bat_volt;
  uint16_t an_13_mot1_anout2;
  uint16_t an_15_mot2_anout2;
  uint8_t state;
} pkt_log_data_t;

/*----------------------------------------------------------------------------*/
typedef struct {
  uint16_t l_fsr_thresh;
  uint16_t r_fsr_thresh;
  uint16_t max_hip_angle;
  uint16_t max_knee_angle;
  uint8_t auto_walk;
} pkt_log_params_t;

typedef pkt_log_params_t pkt_set_params_t;

/*----------------------------------------------------------------------------*/
typedef struct {
  uint8_t state;
} pkt_set_state_t;

/*----------------------------------------------------------------------------*/
//TODO: add pid_id
#define PKT_SET_PID_LEN (6*sizeof(float))
#define PKT_GET_PID_LEN PKT_SET_PID_LEN
typedef struct {
  float setpoint;
  float Kp;
  float Ki;
  float Kd;
  float guard_max;
  float guard_min;
} pkt_set_pid_t;

typedef pkt_set_pid_t pkt_get_pid_t;

/*----------------------------------------------------------------------------*/
typedef struct {
  char msg[MAX_PAYLOAD_LENGTH];
} pkt_str_msg_t;


#endif
