#ifndef COOP_H_
#define COOP_H_
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct coop_args_s {
    volatile struct isr_struct_s *isr_shared_vars;
    struct thread_struct_s *coop_thread_shared_vars;
} coop_args_t;

typedef void (*coop_task_func_t)(coop_args_t *);

typedef struct coop_task_s {
  uint32_t period;
  uint32_t countdown;
  bool scheduled;
  coop_task_func_t task_func;
} coop_task_t;


typedef struct coop_task_list_s {
  size_t num_tasks;
  coop_task_t *task_arr;
} coop_task_list_t;

//task_t* coop_getTaskList();
void coop_scheduleTasks(coop_task_list_t *tasks);
void coop_executeTasks(coop_task_list_t *tasks, coop_args_t *args);
size_t coop_getNumTasks();
void coop_schedExecuteTasks(coop_task_list_t *tasks, coop_args_t *args);

#endif
