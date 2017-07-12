#ifndef PACKET_HYBRID_H_
#define PACKET_HYBRID_H_
#include <packet.h>

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
  PKT_GET_PARAMS,
  PKT_SET_STATE,
  PKT_CALIBRATE_KNEE,
  PKT_DATA_START  = 's',
  PKT_DATA_STOP   = 'n',
  PKT_EN_MOTOR    = 'e',
  PKT_DIS_MOTOR   = 'd',
  PKT_SYN         = 0xFF
} pkt_type_t;

typedef enum pkt_pid_id_e {
  PKT_PID_L_KNEE_POSITION,
  PKT_PID_L_KNEE_SPEED,

  PKT_PID_R_KNEE_POSITION,
  PKT_PID_R_KNEE_SPEED,
} pkt_pid_id_t;
/*----------------------------------------------------------------------------*/
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
  uint8_t pid_id;
} pkt_dbg_pid_t;

/*----------------------------------------------------------------------------*/
typedef struct {
  float r_hip_angle;

  float r_knee_current;
  float r_knee_velocity;
  float r_knee_angle;

  float r_knee_current_commanded;
  float r_knee_velocity_commanded;
  float r_knee_angle_commanded;

  float l_knee_current;
  float l_knee_velocity;
  float l_knee_angle;

  float l_knee_current_commanded;
  float l_knee_velocity_commanded;
  float l_knee_angle_commanded;

  uint32_t timestamp;

  uint16_t an_0_l_fsr_heel;
  uint16_t an_2_r_fsr_heel;
  uint16_t an_1_l_fsr_ff;
  uint16_t an_3_r_fsr_ff;
  uint16_t an_11_bat_volt;

  uint8_t state;
  uint8_t time_marker;
} pkt_log_data_t;

/*----------------------------------------------------------------------------*/
typedef struct {
  float max_hip_angle;
  float min_hip_angle;
  float max_knee_angle;

  float l_knee_position_pid_kp;
  float l_knee_position_pid_ki;
  float l_knee_position_pid_kd;
  float l_knee_position_pid_aw_ub; //antiwindup upperbound
  float l_knee_position_pid_aw_lb;

  float l_knee_speed_pid_kp;
  float l_knee_speed_pid_ki;
  float l_knee_speed_pid_kd;
  float l_knee_speed_pid_aw_ub; //antiwindup upperbound
  float l_knee_speed_pid_aw_lb;
  float l_knee_fc_alpha;


  float r_knee_position_pid_kp;
  float r_knee_position_pid_ki;
  float r_knee_position_pid_kd;
  float r_knee_position_pid_aw_ub; //antiwindup upperbound
  float r_knee_position_pid_aw_lb;

  float r_knee_speed_pid_kp;
  float r_knee_speed_pid_ki;
  float r_knee_speed_pid_kd;
  float r_knee_speed_pid_aw_ub; //antiwindup upperbound
  float r_knee_speed_pid_aw_lb;
  float r_knee_fc_alpha;

  uint16_t l_fsr_thresh;
  uint16_t r_fsr_thresh;

  uint8_t position_control;
  uint8_t auto_walk;
} pkt_log_params_t;

typedef pkt_log_params_t pkt_set_params_t;
typedef pkt_log_params_t pkt_get_params_t;

/*----------------------------------------------------------------------------*/
typedef struct {
  uint8_t state;
} pkt_set_state_t;

/*----------------------------------------------------------------------------*/
typedef struct {
  float setpoint;
  float Kp;
  float Ki;
  float Kd;
  float guard_max;
  float guard_min;
  uint8_t pid_id;
} pkt_set_pid_t;

typedef pkt_set_pid_t pkt_get_pid_t;

/*----------------------------------------------------------------------------*/
typedef struct {
  char msg[MAX_PAYLOAD_LENGTH];
} pkt_str_msg_t;


#endif
