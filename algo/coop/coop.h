#ifndef COOP_H_
#define COOP_H_

typedef void (*coop_task_func_t)(void);

typedef struct coop_task_s {
  uint32_t period;
  uint32_t countdown;
  bool scheduled;
  coop_task_func_t task_func;
} coop_task_t;

task_t* coop_getTaskList();
void coop_scheduleTasks(coop_task_t *tasks);
void coop_executeTasks(coop_task_t *tasks);
size_t coop_getNumTasks();
void coop_schedExecuteTasks(coop_task_t *tasks);

#endif
