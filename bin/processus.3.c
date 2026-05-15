#include <stdio.h>
#include <n7OS/processus.h>

void processus3() {
    process_pid_t pid = getpid();
    printf("Hello, world from P3. PID: %d\n", pid);
    scheduler();
    for (;;);
    
}
