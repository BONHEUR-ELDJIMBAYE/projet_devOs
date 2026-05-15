#include <stdio.h>
#include <n7OS/cpu.h>
#include <n7OS/processus.h>

void idle(){
    process_pid_t pid = getpid();
    printf("idle. PID: %d\n", pid);
    
    scheduler();
    while (1) {
        hlt();
    }
} 