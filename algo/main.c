#include <stdio.h>
#include <numerical.h>
#include <PID.h>
#include <math.h>

#define PI 3.14159

float sinHertz(float freq, float time) {
  return sin(2*PI*freq*time);
}

//tau is about .25*rise time.

int main(int argc, char ** argv) {
  ring_buffer_t r_buff = rb_create(PID_BUFFER_SIZE);

  float *time;
  const float dt = 0.001;

  pid_data_t pid;

  pid_init(&pid, 1.0, 0.1, 0.1, 5000);

  size_t len_time = nm_genRange(0, dt, 100.0, &time);

  printf("t,s,p,c\n");
  for (size_t i = 0; i < len_time; ++i) {
    const float set = 2.0;
    float commanded_setpoint = 0;
    static float sensor_prev = 0;
    static float pid_output = 0;
    float tau = 10; //0.00025

    if (time[i] > 2.0)
      commanded_setpoint = set;

    float sensor = nm_expProcess(sensor_prev, pid_output, dt, tau);

    pid_output = pid_FeedbackCtrl(&pid, commanded_setpoint, sensor, dt,
        pid_velocUpdate);

    printf("%f,%f,%f,%f\n", time[i], commanded_setpoint, sensor, pid_output);

    sensor_prev = sensor;
  }

  free(time);

  return 0;
}
