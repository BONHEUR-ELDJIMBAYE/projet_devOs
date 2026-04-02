#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/timer.h>
#include <n7OS/irq.h>
#include <stdio.h>

extern void handler_IT50(void);

int heure = 0;
int minute = 0;
int seconde = 0;

void kernel_start(void)
{
    init_timer();
    init_console();
    init_kheap();
    initialise_paging();

    convert_counter_to_time(&heure, &minute, &seconde);
    printf("%02d:%02d:%02d", heure, minute, seconde);
    //teste de la pagination
    // alloc_page_entry(0xA0000000,1,0); 
    // uint32_t *ptr = (uint32_t *) 0xA0000000;
    // (*ptr)++;

    //initialisation de l'entrée 50 de la table des interruptions
    init_irq_entry(50, (uint32_t) handler_IT50);   

    //teste de l'interruption 50
    __asm__("int $50"::);
    
    // lancement des interruptions
    sti();

    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
