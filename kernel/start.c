#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <stdio.h>

void kernel_start(void)
{
    //initialisation de la pagination
    
    // initialisation de la console
    init_console();
    init_kheap();
    initialise_paging();

    printf("Hello world!");
   

    // lancement des interruptions
    sti();

    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
