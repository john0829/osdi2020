
#include "shell.h"
#include "config.h"
#include "exec.h"


void shell(){
    _memset(buff, '\0', 50); 
    char *buff_ptr = buff;

    uart_puts("Welcome! osdi lab3 interrupts practice img!!!\n#");
    char c = uart_getc();
    while(1) {
        c = uart_getc();
        if(c=='\n'){
            uart_puts("\n");
            if(!_compare_input(buff)){
                uart_puts("Err: Command ");
                _print(buff);
                uart_puts(" not found, try <help>\n");
            }
            uart_puts("#");
            _memset(buff, '\0', 50);    
            buff_ptr = buff;
        }
        else{
            uart_send(c);
            *buff_ptr++ = c;
        }
    }
}




