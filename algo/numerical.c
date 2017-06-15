#include <numerical.h>

float nm_fdFirstDer(ring_buffer_t r_buff, float dt) {
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

float nm_fdSecondDer(ring_buffer_t r_buff, float dt) {
  const float A = 5.0/6.0;
  const float B = -61.0/12.0;
  const float C = 13.0;
  const float D = -107.0/6.0;
  const float E = 77.0/6.0;
  const float F = -15.0/4.0;

  float numerator =
    A*rb_get(r_buff, 5) +
    B*rb_get(r_buff, 4) +
    C*rb_get(r_buff, 3) +
    D*rb_get(r_buff, 2) +
    E*rb_get(r_buff, 1) +
    F*rb_get(r_buff, 0);

  return numerator/(dt*dt);
}

float nm_sgSecondDer(ring_buffer_t r_buff, float dt) {
  const float B = 2;
  const float C = -1;
  const float D = -2;
  const float E = -1;
  const float F = 2;

  float numerator =
    B*rb_get(r_buff, 4) +
    C*rb_get(r_buff, 3) +
    D*rb_get(r_buff, 2) +
    E*rb_get(r_buff, 1) +
    F*rb_get(r_buff, 0);

  return numerator/(7*dt*dt);
}

float nm_sgSecondDer5(ring_buffer_t r_buff, float dt) {
  const float A = 5;
  const float C = -3;
  const float D = -4;
  const float E = -3;
  const float G = 5;

  float numerator =
    A*rb_get(r_buff, 6) +
    C*rb_get(r_buff, 4) +
    D*rb_get(r_buff, 3) +
    E*rb_get(r_buff, 2) +
    G*rb_get(r_buff, 0);

  return numerator/(42.0*dt*dt);
}