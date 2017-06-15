#include <stdio.h>
#include <numerical.h>
#include <PID.h>
#include <math.h>

#define PI 3.14159

float sinHertz(float freq, float time) {
  return sin(2*PI*freq*time);
}

size_t genRange(float start, float increment, float end, float **array) {
  const size_t arr_size = ceil(fabs((end - start)/increment));
  float *range = malloc(arr_size*sizeof(float));

  for (size_t i = 0; i < arr_size; ++i) {
    range[i] = start + i*increment;
  }

  *array = range;
  return arr_size;
}

float expProcess(float prev, float setpoint, float sample_time, float tau) {
  const float A = sample_time/tau;
  return prev + A*setpoint/(A+1);
}

//tau is about .25*rise time.

int main(int argc, char ** argv) {
  ring_buffer_t r_buff = rb_create(PID_BUFFER_SIZE);

  float *time;
  const float dt = 0.001;

  pid_data_t pid;

  pid_init(&pid, 1, 0, 0, 5000);

  size_t len_time = genRange(0, dt, 10.0, &time);

  printf("t,s,p,c\n");
  for (size_t i = 0; i < len_time; ++i) {
    const float set = 2.0;
    float commanded_setpoint = 0;
    static float sensor_prev = 0;
    static float pid_output = 0;

    if (time[i] > 2.0)
      commanded_setpoint = set;

    float sensor = expProcess(sensor_prev, pid_output, dt, 0.00025);

    pid_output = pid_FeedbackCtrl(&pid, commanded_setpoint, sensor, dt,
      pid_minUpdate);

    printf("%f,%f,%f,%f\n", time[i], commanded_setpoint, sensor, pid_output);

    sensor_prev = sensor;
  }

  free(time);

  return 0;
}
