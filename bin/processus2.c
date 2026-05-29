#include <stdio.h>
#include <n7OS/processus.h>
#include <n7OS/cpu.h>

void processus2() {
    process_pid_t pid = getpid();
    printf("Hello, world from P2. PID: %d\n", pid);
    // scheduler();
    while (1) {
        if (display_process_test) {
            printf("2");
        }
        sleep(1);
    }
}
