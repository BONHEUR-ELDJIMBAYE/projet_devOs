#include <stdio.h>
#include <n7OS/cpu.h>
#include <n7OS/processus.h>

void idle(){
    while (1) {
        printf("idle\n");
        hlt();
    }
} 