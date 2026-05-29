#include <n7OS/processus.h>
#include <stddef.h>
#include <stdlib.h>
#include <n7OS/cpu.h>


uint32_t process_table_bitmap[NB_PROC / 32];// bitmap pour suivre les PID utilisés (1 = utilisé, 0 = libre)
process_t process_table[NB_PROC]; // table des processus
bool running = false; // determine si le processeur est en train d'executer un processus

int display_process_test = 0; // Par defaut, on n'affiche pas les processus de test
process_pid_t active_pid = 0; // le pid du processus actuellement ELU

extern void idle();
extern void processus1();
extern void processus2();
extern void processus3();
extern void ctx_sw(uint32_t *ancien, uint32_t *elu);


process_pid_t find_free_pid(){
    for (uint32_t process_idx = 0x0; process_idx < NB_PROC; process_idx++)
	{
        uint32_t index = process_idx / 32;
		uint32_t offset = process_idx % 32;

		if (!(process_table_bitmap[index] & (1 << offset)))
		{
			process_table_bitmap[index] |= 1 << offset;
            return process_idx;
		}
	}
    return -1; // pas de pid libre
}

void clear_pid(process_pid_t pid){
    uint32_t index = pid / 32;
    uint32_t offset = pid % 32;

    process_table_bitmap[index] &= ~(1 << offset);
}

__attribute__((naked)) void start_process_wrapper() {
    __asm__ volatile("sti \n\t ret");
}

process_pid_t kernel_fork(const char *name, fnptr function){
    process_pid_t pid = find_free_pid();
    if (pid == -1) {
        return -1;// pas de pid libre
    }
    process_table[pid].pid = pid;
    process_table[pid].state = PRET;
    process_table[pid].nom = name;
    process_table[pid].parent = kernel_getpid();//le père du processus est le processus courant
    process_table[pid].stack = (uint32_t *)malloc(sizeof(uint32_t) * STACK_SIZE);
    for (int i = 0; i < 5; i++) {
        process_table[pid].regs[i] = 0;
    }
    process_table[pid].stack[STACK_SIZE - 1] = (uint32_t)function;
    process_table[pid].stack[STACK_SIZE - 2] = (uint32_t)start_process_wrapper;
    process_table[pid].regs[1] = (uint32_t)&process_table[pid].stack[STACK_SIZE - 2];
    return pid;
}

process_pid_t kernel_getppid(){
    return process_table[kernel_getpid()].parent;
}

bool is_Process(process_pid_t pid){
    if (pid >= NB_PROC) {
        return false;
    }
    uint32_t index = pid / 32;
    uint32_t offset = pid % 32;
    return process_table_bitmap[index] & (1 << offset);
}

bool is_process_ELU(process_pid_t pid){
    if (!is_Process(pid)) {
        return false;   
    }
    return process_table[pid].state == ELU;
}

bool is_process_BLOQUE(process_pid_t pid){
    if (!is_Process(pid)) {
        return false;
    }
    return process_table[pid].state == BLOQUE;
}

bool is_process_PRET(process_pid_t pid){
    if (!is_Process(pid)) {
        return false;
    }
    return process_table[pid].state == PRET;
}

void terminate_process(process_pid_t pid){
    if (pid >= NB_PROC) {
        return; // pid invalide
    }
    if (pid == 0) {
        return; // ne pas terminer le processus idle
    }
    
    cli();
    process_table[pid].state = TERMINE;

    // Reveiller automatiquement le parent s'il etait en attente (wait)
    unblock_process(process_table[pid].parent);

    if (active_pid == pid) {
        scheduler(); // On se tue nous-meme, on passe la main (on ne reviendra jamais ici)
    } else {
        clear_pid(pid);
        free(process_table[pid].stack);
        kernel_ps();
    }
    sti();
}

int kernel_kill(process_pid_t pid) {
    if (pid == 0 || pid >= NB_PROC || !is_Process(pid)) {
        return -1; // Erreur
    }
    terminate_process(pid);
    return 0; // Succès
}

int kernel_sleep(int seconds) {
    extern uint32_t timer_counter;
    cli();
    process_table[active_pid].wake_up_time = timer_counter + (seconds * 1000);
    process_table[active_pid].state = ENDORMI;
    scheduler();
    sti();
    return 0;
}

int kernel_wait(process_pid_t pid) {
    if (pid >= NB_PROC || !is_Process(pid)) {
        return -1; // Processus invalide
    }

    // On se met en pause tant que l'enfant n'est pas mort
    while (process_table[pid].state != TERMINE) {
        cli();
        block_process(active_pid);
        scheduler();
        sti();
    }

    // L'enfant est TERMINE, on nettoie son cadavre (la pile et le PID)
    clear_pid(pid);
    free(process_table[pid].stack);

    return 0;
}

process_pid_t kernel_getpid(){
    return active_pid;
}

void block_process(process_pid_t pid){
    if (pid >= NB_PROC) {
        return; // pid invalide
    }
    if (pid == 0) {
        return; // ne pas arrêter le processus idle
    }
    if (is_Process(pid)) {
        process_table[pid].state = BLOQUE; 
 
    }
}

void unblock_process(process_pid_t pid){
    if (pid >= NB_PROC) {
        return; // pid invalide
    }
    if (pid == 0) {
        return; // ne pas arrêter le processus idle
    }
    if (is_Process(pid) && is_process_BLOQUE(pid)) {
        process_table[pid].state =PRET;    
    }
}

void interrupt_process(process_pid_t pid){
    if (pid >= NB_PROC) {
        return; // pid invalide
    }
    if (pid == 0) {
        return; // ne pas arrêter le processus idle
    }
    if (is_Process(pid) && is_process_ELU(pid)) {
        cli();
        scheduler();
        sti();
    }
}

void start_process(process_pid_t pid){
    if (pid >= NB_PROC) {
        return; // pid invalide
    }
    if(is_Process(pid) && is_process_PRET(pid) && !running){
        process_table[pid].state = ELU;
        active_pid = pid;
        running = true;
    }
}


void scheduler(){
    process_pid_t current_pid = active_pid;
    extern uint32_t timer_counter;
    
    // Reveil des processus endormis
    for (int i = 0; i < NB_PROC; i++) {
        if (is_Process(i) && process_table[i].state == ENDORMI && timer_counter >= process_table[i].wake_up_time) {
            process_table[i].state = PRET;
        }
    }

    for (int i = 1; i <= NB_PROC; i++){
        process_pid_t new_pid = (current_pid + i) % NB_PROC;
        if (is_process_PRET(new_pid)) {
            if (process_table[current_pid].state == ELU) {
                process_table[current_pid].state = PRET;
            }
            process_table[new_pid].state = ELU;
            active_pid = new_pid;
            ctx_sw(process_table[current_pid].regs, process_table[new_pid].regs);
            return;
        }
    }
}


void init_processus(){
    for (int i = 0; i < NB_PROC / 32; i++) {
        process_table_bitmap[i] = 0;
    }
    kernel_fork("idle", *idle); // processus idle pour éviter les problèmes de scheduler, prend le pid 0
    start_process(0); // on démarre le processus idle
    kernel_fork("processus1", *processus1);
    kernel_fork("processus2", *processus2);
    kernel_fork("processus3", *processus3);
}


void kernel_ps() {
    printf("%-5s | %-15s | %-10s\n", "PID", "NOM", "ETAT");
    printf("------------------------------\n");
    
    for(int i = 0; i < NB_PROC; i++) {
        if(is_Process(i)) {
            printf("%-5d | %-15s | ",process_table[i].pid,process_table[i].nom);
            switch (process_table[i].state)
			{
			case PRET:
				printf("%-8s\n", "PRET");
				break;
			case ELU:
				printf("%-8s\n", "ELU");
				break;
			case BLOQUE:
				printf("%-8s\n", "BLOQUE");
				break;
			case TERMINE:
				printf("%-8s\n", "TERMINE");
				break;
			case ENDORMI:
				printf("%-8s\n", "ENDORMI");
				break;
			default:
				printf("%-8s\n", "UNKNOWN");
				break;
			}
        }
    }
    printf("------------------------------\n");
}
