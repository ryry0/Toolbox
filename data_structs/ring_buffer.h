#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <stdint.h>
#include <stdlib.h>

struct ring_buffer_s {
  float *buffer;
  size_t buffer_size; //full allocated size
  size_t head;
  size_t tail;
  size_t length; //number of items stored
};

typedef struct ring_buffer_s *ring_buffer_t;

ring_buffer_t rb_create(size_t size);
void rb_popAllButBack(ring_buffer_t r_buffer, float *data_buff);
void rb_pushBack(ring_buffer_t r_buffer, float data);
void rb_print(ring_buffer_t r_buffer);
void rb_destroy(ring_buffer_t r_buffer);
void rb_read(ring_buffer_t r_buffer, float *buffer);
float rb_get(ring_buffer_t r_buffer, size_t index);
size_t rb_getLength(ring_buffer_t r_buffer);
void rb_setBuffer(ring_buffer_t r_buffer, float *buffer);
void rb_init(ring_buffer_t r_buffer, size_t size);
void rb_pushFront(ring_buffer_t r_buffer, float data);

#endif
