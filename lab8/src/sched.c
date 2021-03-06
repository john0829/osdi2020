#include "sched.h"
#include "thread.h"
#include "config.h"

extern task_t* current;
extern task_manager_t TaskManager;
task_t* task_run_queue[10];
extern task_t* get_current();
extern void set_current(task_t*);

void _schedule2(void)
{
    int next = 1;
    int i = 0;
    int max_c = -1;
    for(i = 1; i < TaskManager.task_num; i++){
        task_t *task = &TaskManager.task_pool[i];
        if(task->state == THREAD_RUNNABLE && task->counter > max_c){
            next = i;
            max_c = task->counter;
        }
    }
    if(max_c == 0){
        for(i = 1; i < TaskManager.task_num; i++){
            task_t *task = &TaskManager.task_pool[i];
            task->counter = 1;
        }
    }
    // if(i == TaskManager.task_num) { next = 0; }
    if(TaskManager.task_num == 1) { next = 0;}
    task_t* new_task = &TaskManager.task_pool[next]; 
    printf("next is %d!\n", next);
    // printf("total thread nunmber is %d!\n", TaskManager.task_num);
	switch_to(new_task);
}

void _schedule(){
    printf("in schedule\n");
    printf("current id is: %d\n", current->task_id);
    int next_task_id = (current->task_id + 1) % 64;
    while(next_task_id != current->task_id) {
        // printf("next task id is %d, state is %d\n", next_task_id, TaskManager.task_pool[next_task_id].state);
        if (TaskManager.task_pool[next_task_id].state != ZOMBIE) {
            if(next_task_id < 0){
                printf("task id is negative, it is weird!!!!!\n");
            }
            task_t* new_task = &TaskManager.task_pool[next_task_id]; 
            printf("next is %d!\n", next_task_id);
            switch_to(new_task);
            return;
        }
        next_task_id = (next_task_id + 1) % 64;
    }
    printf("no change\n");
}

void switch_to(task_t * next) 
{
    if (current == next){
        // printf("the same task!\n");
		return;
    }
    printf("---\n");
    printf("print now current id is: %d\n", current->task_id);
    printf("current number address is %x\n", current);
    printf("current number address is %x\n", get_current());
    set_current(next);
    printf("print now current id is:  %d\n", next->task_id);
    printf("current number address is %x\n", next);
    printf("current number address is %x\n", get_current());
    printf("---\n");

	task_t* prev = current;
	current = next;
    printf("switch start\n");
	cpu_switch_to(prev, next);
    printf("switch finished\n");
}

void check_reschedule(){
    if(current->rescheduled){
        schedule();
    }
}