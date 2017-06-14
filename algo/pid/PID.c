#include <PID.h>

static float fdFirstDer(ring_buffer_t r_buff, float dt) {
  const float A = -1.0/5.0;
  const float B = 5.0/4.0;
  const float C = -10.0/3.0;
  const float D = 5;
  const float E = -5;
  const float F = 137.0/60.0;

  //ring buffer, 0 is first.

  float numerator =
    A*rb_get(r_buff, 5) +
    B*rb_get(r_buff, 4) +
    C*rb_get(r_buff, 3) +
    D*rb_get(r_buff, 2) +
    E*rb_get(r_buff, 1) +
    F*rb_get(r_buff, 0);

  return numerator/dt;
}

static float fdSecondDer(ring_buffer_t r_buff, float dt) {
  const float A = -5.0/6.0;
  const float B = 61.0/12.0;
  const float C = -13.0;
  const float D = 107.0/6.0;
  const float E = -77.0/6.0;
  const float F = 15.0/4.0;

  float numerator =
    A*rb_get(r_buff, 5) +
    B*rb_get(r_buff, 4) +
    C*rb_get(r_buff, 3) +
    D*rb_get(r_buff, 2) +
    E*rb_get(r_buff, 1) +
    F*rb_get(r_buff, 0);

  return numerator/(dt*dt);
}

void pid_init(pid_data_t *pid, const float proportional_gain,
    const float integral_gain, const float derivative_gain,
    const float integral_guard) {
  pid_setConstants(
      pid,
      proportional_gain,
      integral_gain,
      derivative_gain,
      integral_guard);

  //set up the ring buffers
  rb_setBuffer(&pid->error_buffer_ring, pid->error_buffer);
  rb_setBuffer(&pid->output_buffer_ring, pid->output_buffer);

  //set contents of ring buffers to zero
  rb_init(&pid->error_buffer_ring, PID_BUFFER_SIZE);
  rb_init(&pid->output_buffer_ring, PID_BUFFER_SIZE);
  pid->pid_output = 0;
}

void pid_setConstants(pid_data_t *pid, const float proportional_gain,
    const float integral_gain, const float derivative_gain,
    const float integral_guard) {
  pid->proportional_gain = proportional_gain;
  pid->integral_gain = integral_gain;
  pid->derivative_gain = derivative_gain;
  pid->integral_guard = integral_guard;
  pid->previous_error = 0;
  pid->integral_error = 0;
}

void pid_update(pid_data_t *pid, const float *current_error, const float *delta_t) {
  float error_differential = 0;

  pid->integral_error += *current_error * (*delta_t);
  pid->integral_error = constrain(pid->integral_error, -pid->integral_guard,
      pid->integral_guard);
  error_differential = (*current_error - pid->previous_error)/(*delta_t);

  pid->pid_output = (pid->proportional_gain * (*current_error)) +
    (pid->integral_gain    * pid->integral_error) +
    (pid->derivative_gain  * error_differential);

  pid->previous_error = *current_error;
} //end pidControl()

//fixed update PID is meant to be called at constant time intervals,
//therefore it does not need delta_t
void pid_fixedUpdate(pid_data_t *pid, const float *current_error) {
  float error_differential = 0;

  pid->integral_error += *current_error;
  pid->integral_error = constrain(pid->integral_error, -pid->integral_guard,
      pid->integral_guard);

  error_differential = (*current_error - pid->previous_error);

  pid->pid_output = (pid->proportional_gain * (*current_error)) +
    (pid->integral_gain    * pid->integral_error) +
    (pid->derivative_gain  * error_differential);

  pid->previous_error = *current_error;
} //end pidControl()

void pid_velocUpdate(pid_data_t *pid, const float current_error,
    const float dt) {
  const float A = 18.0/11.0;
  const float B = -9.0/11.0;
  const float C = 2.0/11.0;
  const float D = 6.0/11.0;

  float Kp = pid->proportional_gain;
  float Ki = pid->integral_gain;
  float Kd = pid->derivative_gain;
  struct ring_buffer_s *error_buffer = &pid->error_buffer_ring;
  struct ring_buffer_s *output_buffer = &pid->output_buffer_ring;

  rb_pushFront(error_buffer, current_error); //update the error buffer

  float du =
    Kp * fdFirstDer(error_buffer, dt) +
    Ki*current_error +
    Kd*fdSecondDer(error_buffer, dt);

  float u =
    //this is the previous value since current val [n] not pushed
    A*rb_get(output_buffer, 0) + //[n-1]
    B*rb_get(output_buffer, 1) + //[n-2]
    C*rb_get(output_buffer, 2) + //[n-3]
    D*du*dt; //D*du*dt

  rb_pushFront(output_buffer, u); //update the error buffer
  pid->pid_output = u;
}

void pid_minUpdate(pid_data_t *pid, const float current_error,
    const float dt) {
  const float Kp = pid->proportional_gain;
  const float Ki = pid->integral_gain;
  const float Kd = pid->derivative_gain;
  struct ring_buffer_s *error_buffer = &pid->error_buffer_ring;
  struct ring_buffer_s *output_buffer = &pid->output_buffer_ring;

  rb_pushFront(error_buffer, current_error); //update the error buffer

  float u =
    rb_get(output_buffer, 0) + //u[n-1]
    Kp*rb_get(error_buffer, 1) + //Kp*e[n-1]
    (Ki*dt - Kp)*rb_get(error_buffer, 0); //(Ki*dt - Kp) * e[n]

  rb_pushFront(output_buffer, u); //update the error buffer
  pid->pid_output = u;
}
