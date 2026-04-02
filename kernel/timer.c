#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/timer.h>

extern void handler_IT_timer(void);
long timer_counter = 0;


void init_timer() {
    outb(0x34, 0x43); //Channel 0, accès poids faible/poids fort, générateur d'impulsion, fréquence définie en binaire
    outb(FREQUENCE&0xFF, 0x40); //poids faible de la fréquence
    outb((FREQUENCE>>8)&0xFF, 0x40); //poids fort de la fréquence
    outb(inb(0x21)&0xFE, 0x21);// activation de l’IT du Timer
    init_irq_entry(0x20, (uint32_t) handler_IT_timer); //initialisation de l'entrée 32 de la table des interruptions pour le timer

}

void handler_timer() {
    outb(0x20, 0x20); // ack de l’IT du PIC Master
    timer_counter++;

}

void convert_counter_to_time(int *hours, int *minutes, int *seconds) {
    *hours = timer_counter / 3600;
    printf("heures : %d\n", *hours);
    *minutes = (timer_counter % 3600) / 60;
    *seconds = timer_counter % 60;

}