#include <stdio.h>
#include <n7OS/cpu.h>
#include <n7OS/processus.h>

void idle(){
    process_pid_t pid = getpid();
    
    // scheduler();
    printf("idle. PID: %d\n", pid);
    while (1) {
        hlt();
    }
} 