#include <n7OS/processus.h>
#include <stddef.h>
#include <n7OS/cpu.h>


uint32_t process_table_bitmap[NB_PROC / 32];// bitmap pour suivre les PID utilisés (1 = utilisé, 0 = libre)
process_t process_table[NB_PROC]; // table des processus
bool running = false; // determine si le processeur est en train d'executer un processus

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

process_pid_t fork(const char *name, fnptr function){
    process_pid_t pid = find_free_pid();
    if (pid == -1) {
        return -1;// pas de pid libre
    }
    process_table[pid].pid = pid;
    process_table[pid].state = PRET;
    process_table[pid].nom = name;
    process_table[pid].stack = (uint32_t *)malloc(sizeof(uint32_t) * STACK_SIZE);
    for (int i = 0; i < 5; i++) {
        process_table[pid].regs[i] = 0;
    }
    process_table[pid].stack[STACK_SIZE - 1] = (uint32_t)function;
    process_table[pid].regs[1] = (uint32_t)&process_table[pid].stack[STACK_SIZE - 1];
    return pid;
}

bool is_Process(process_pid_t pid){
    if (pid < 0 || pid >= NB_PROC) {
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
    if (pid < 0 || pid >= NB_PROC) {
        return; // pid invalide
    }
    if (pid == 0) {
        return; // ne pas terminer le processus idle
    }
    clear_pid(pid);
    free(process_table[pid].stack);
}

process_pid_t getpid(){
    for (int i = 0; i < NB_PROC; i++) {
        if (process_table[i].state == ELU) {
            return process_table[i].pid;
        }
    }
    return -1; // pas de processus élu
}

void block_process(process_pid_t pid){
    if (pid < 0 || pid >= NB_PROC) {
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
    if (pid < 0 || pid >= NB_PROC) {
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
    if (pid < 0 || pid >= NB_PROC) {
        return; // pid invalide
    }
    if (pid == 0) {
        return; // ne pas arrêter le processus idle
    }
    if (is_Process(pid) && is_process_ELU(pid)) {
        scheduler();
    }    
}

void start_process(process_pid_t pid){
    if (pid < 0 || pid >= NB_PROC) {
        return; // pid invalide
    }
    if(is_Process(pid) && is_process_PRET(pid) && !running){
        process_table[pid].state = ELU;
        running = true;
    }
}


void scheduler(){
    process_pid_t current_pid = getpid();
    for (int i = 0; i < NB_PROC; i++){
        process_pid_t new_pid = (current_pid + i) % NB_PROC;
        if (is_process_PRET(new_pid)) {
            process_table[current_pid].state =PRET;
            process_table[new_pid].state = ELU;
            sti();
            ctx_sw(process_table[current_pid].regs, process_table[new_pid].regs);
            return;
        }
    }
}


void init_processus(){
    for (int i = 0; i < NB_PROC / 32; i++) {
        process_table_bitmap[i] = 0;
    }
    fork("idle", *idle); // processus idle pour éviter les problèmes de scheduler, prend le pid 0
    start_process(0); // on démarre le processus idle
    fork("processus1", *processus1);
    fork("processus2", *processus2);
    fork("processus3", *processus3);
}


void ps() {
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
			default:
				printf("%-8s\n", "UNKNOWN");
				break;
			}
        }
    }
    printf("------------------------------\n");
}
