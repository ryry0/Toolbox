#ifndef PID_H_
#define PID_H_
#include <ring_buffer.h>

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define PID_BUFFER_SIZE 6

//Use this data structure to create a customized PID per motor
typedef struct pid_data_t {
  float proportional_gain;
  float integral_gain;
  float derivative_gain;
  float previous_error;
  float integral_error;
  float integral_guard;
  float pid_output;

  struct ring_buffer_s error_buffer_ring;
  struct ring_buffer_s output_buffer_ring;

  //private
  float error_buffer[PID_BUFFER_SIZE];
  float output_buffer[PID_BUFFER_SIZE];
} pid_data_t;

typedef void (*pid_method_t)(pid_data_t *, const float, const float);

void pid_setConstants(pid_data_t *pid,
    const float proportional_gain,
    const float integral_gain,
    const float derivative_gain,
    const float integral_guard);

void pid_init(pid_data_t *pid,
    const float proportional_gain,
    const float integral_gain,
    const float derivative_gain,
    const float integral_guard);

//traditional PID calculation
void pid_update(pid_data_t *pid,
    const float current_error,
    const float dt);

void pid_fixedUpdate(pid_data_t *pid, const float current_error, const float dt);

//velocity PID implementation
void pid_velocUpdate(pid_data_t *pid, const float current_error,
    const float dt);

//minimal PID implementation
void pid_minUpdate(pid_data_t *pid, const float current_error,
    const float dt);

float pid_FeedbackCtrl(pid_data_t *pid, const float setpoint,
    const float sensor, const float dt, const pid_method_t method);

float pid_feedforwardFeedbackCtrl(pid_data_t *pid, const float setpoint,
    const float sensor, const float dt, const pid_method_t method);

#endif
