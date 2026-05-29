#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/timer.h>
#include <n7OS/irq.h>
#include <stdio.h>
#include <unistd.h>
#include <n7OS/processus.h>

extern void handler_IT50(void);
extern void idle();
extern void shell_run();

void kernel_start(void)
{
    init_console();
    init_kheap();
    initialise_paging();
    init_timer();
    
    init_syscall();
    init_keyboard();
    init_processus();
    
    //teste de la pagination
    // alloc_page_entry(0xA0000000,1,0); 
    // uint32_t *ptr = (uint32_t *) 0xA0000000;
    // (*ptr)++;

    // //initialisation de l'entrée 50 de la table des interruptions
    // init_irq_entry(50, (uint32_t) handler_IT50);   
    // //teste de l'interruption 50
    // __asm__("int $50"::);
    
    // lancement des interruptions
    sti();
    
    //shutdown(1);
    
    // if ( exemple() == 1 ) {
    //     printf("exemple ok");
    // }
    kernel_fork("shell", *shell_run);
    // on ne doit jamais sortir de kernel_start
    idle();
}
