#include <task.h>
#include <stdbool.h>

typedef enum coop_task_index_e {
  TASK_MAX
} coop_task_index_t;

typedef struct coop_args_s {
} coop_args_t;


static coop_task_t coop_task_list[TASK_MAX] = {

};

coop_task_t *coop_getTaskList() {
  return coop_task_list;
}

size_t coop_getNumTasks() {
  return TASK_MAX;
}

/*
 * tasks countdown so you don't have to deal with overflow
 * Put this in a timer interrupt
 */
void coop_scheduleTasks(coop_task_t *tasks) {
  for (size_t i = 0; i < TASK_MAX; ++i) {
    if (tasks[i].countdown == 0) {
      tasks[i].scheduled = true;
      //always schedule period 0 tasks
      tasks[i].countdown = (tasks[i].period == 0) ? 0 : tasks[i].period - 1;
    }
    else
      tasks[i].countdown--;
  }
}

/*
 * execute tasks that are scheduled to run
 * put this in the main loop
 */
void coop_executeTasks(coop_task_t *tasks) {
  for (size_t i = 0; i < TASK_MAX; ++i) {
    if (tasks[i].scheduled == true) {
      (*tasks[i].task_func)();
      tasks[i].scheduled = false;
    }
  }
}

//function that combines the above into one
void coop_schedExecuteTasks(coop_task_t *tasks) {
  if (tasks[i].countdown == 0) {
    (*tasks[i].task_func)();
    //always schedule period 0 tasks
    tasks[i].countdown = (tasks[i].period == 0) ? 0 : tasks[i].period - 1;
  }
  else
    tasks[i].countdown--;
}
