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

int main(int argc, char ** argv) {
  ring_buffer_t r_buff = rb_create(PID_BUFFER_SIZE);

  float *time;
  const float dt = 0.001;

  size_t len_time = genRange(-10.0, dt, 10.0, &time);

  printf("t,y,fd,sd\n");
  for (size_t i = 0; i < len_time; ++i) {
    //float y = sinHertz(1.0, time[i]);
    float y = pow(time[i], 2);
    rb_pushFront(r_buff, y);

    float fd = nm_fdFirstDer(r_buff, dt);
    //float sd = nm_sgSecondDer5(r_buff, dt);
    float sd = nm_sgSecondDer(r_buff, dt);
    //float sd = nm_fdSecondDer(r_buff, dt);

    printf("%f,%f,%f,%f\n", time[i], y, fd, sd);
  }

  return 0;
}
