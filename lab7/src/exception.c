#include "exception.h"
#include "thread.h"
#include "syscall.h"
#include "config.h"


extern task_t* current;
extern int check_reschedule();

unsigned long get_syscall_number(unsigned long trapframe){
	unsigned int x8 = *(unsigned long*)(trapframe+8*8);
	return x8;
}

unsigned long get_syscall_parameter(unsigned long trapframe){
	unsigned long x0 = *(unsigned long*)(trapframe+0*8);
	return x0;
}

int __get_taskid(){
	return current->task_id;
}

void syscall_router(unsigned long trapframe){
    unsigned long x0 = get_syscall_parameter(trapframe);
    unsigned long x8 = get_syscall_number(trapframe);
 	unsigned long syscall_return_value = 0;
    char c;

    switch(x8){
        case SYS_GET_TASKID:
            printf("[info] svc task_id sys_call command \r\n");
            syscall_return_value = __get_taskid();
            break;
        case SYS_UART_READ:
            c = uart_getc();
            printf("get input value is %d\n",c);
            break;
        case SYS_UART_WRITE:
            uart_puts((char *)x0);
            break;
        case SYS_EXEC:
            printf("[info] svc exec sys_call command \r\n");
            void (*func_ptr)(void) = (void (*)(void))x0;
            do_exec(func_ptr);
            break;
        case SYS_FORK:
            printf("[info] svc fork sys_call command \r\n");
            syscall_return_value = do_fork();
            break;
        case SYS_EXIT:
            printf("[info] svc exit sys_call command \r\n");
            do_exit();
            break;
        default:
            printf("syscall not found\n");
            break;
    }
	*(unsigned long*)(trapframe+8*0) = syscall_return_value;
}

void exception_handler(unsigned long trapframe)
{
	// printf("Exception class (EC) 0x%x \r\n",esr >> 26);
	// printf("EInstruction  specific syndrome (ISS) 0x%x \r\n",esr & 0xffffff);

    unsigned long el_level;
    unsigned long esr, elr, spsr, far;
    // unsigned int ec, iss, retaddr;
    unsigned long sp_el0, elr_el1, spsr_el1;
    asm volatile ("mrs %0, CurrentEL" : "=r" (el_level));

    //check el level
    if (el_level == 0x4) {
        asm volatile ("mrs %0, esr_el1" : "=r" (esr));
        asm volatile ("mrs %0, elr_el1" : "=r" (elr));
        asm volatile ("mrs %0, spsr_el1" : "=r" (spsr));
        asm volatile ("mrs %0, far_el1" : "=r" (far));
    } else if (el_level == 0x8) {
        asm volatile ("mrs %0, esr_el2" : "=r" (esr));
        asm volatile ("mrs %0, elr_el2" : "=r" (elr));
        asm volatile ("mrs %0, spsr_el2" : "=r" (spsr));
        asm volatile ("mrs %0, far_el2" : "=r" (far));
    }

    // ec = esr >> (32-6);
    // iss = esr & (0xffffff);
    // retaddr = elr;

    //load register value
    asm volatile ("mrs %0, sp_el0" : "=r" (sp_el0));
    asm volatile ("mrs %0, elr_el1" : "=r" (elr_el1));
    asm volatile ("mrs %0, spsr_el1" : "=r" (spsr_el1));



    // Set trapframe
    task_t *task = get_current();
    printf("In the exception_handler , current task id is %d\n", current->task_id);
    
    
    while(1){

    }


    
    //set user mode conetxt
    task->user_context.sp_el0 = sp_el0;
    task->user_context.elr_el1 = elr_el1;
    task->user_context.spsr_el1 = spsr_el1;
    task->trapframe = trapframe;

    // SVC
    if ((esr>>26) == 0b010101) {
        syscall_router(trapframe);
    }



	// int num = (esr & 0xffffff);
	// check_reschedule();


	// if(num == 0){
	// 	unsigned int c;
	// 	unsigned int x8 = get_syscall_number(trapframe);
	// 	unsigned int x0 = get_syscall_parameter(trapframe);
    //     syscall_router(x8);
	// }
	// if (num == 1){
	// 	printf("[info] svc command \r\n");
	// }
	// else if(num == 2){
	// 	printf("[info] enable timer interrupt \r\n");

	// 	enable_interrupt_controller();
	// 	local_timer_init();

	// 	unsigned int cntfrq;
    //     unsigned int val;
	// 	cntfrq = read_cntfrq();
    //     write_cntp_tval(cntfrq);  
    //     val = read_cntp_tval();    
		
	// 	core_timer_enable();
	// }
	// else if(num == 3){
	// 	disable_all_timer();
	// }
	// else if(num == 4){
	// 	timestamp_handler();
	// }


}