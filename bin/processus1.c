#include <stdio.h>
#include <n7OS/processus.h>

void processus1() {
    process_pid_t pid = getpid();
    printf("Hello, world from P1. PID: %d\n", pid);
    scheduler();
    for (;;);
}
