#include <n7OS/processus.h>
#include <stddef.h>
#include <n7OS/cpu.h>


uint32_t process_table_bitmap[NB_PROC / 32];// bitmap pour suivre les PID utilisés (1 = utilisé, 0 = libre)
process_t process_table[NB_PROC]; // table des processus
int index_table_process_pret = 0;
int nbre_process_pret = 0; //nombre de processus prêts à être exécutés
process_t* table_process_pret[NB_PROC];//tableau des pointeurs vers les processus prêts à être exécutés

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
    table_process_pret[nbre_process_pret] = &process_table[pid];
    nbre_process_pret++;
    return pid;
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
    process_table[pid].state = BLOQUE;
}

void unblock_process(process_pid_t pid){
    if (pid < 0 || pid >= NB_PROC) {
        return; // pid invalide
    }
    if (pid == 0) {
        return; // ne pas arrêter le processus idle
    }
    process_table[pid].state = PRET;
}

void stop_process(process_pid_t pid){
    if (pid < 0 || pid >= NB_PROC) {
        return; // pid invalide
    }
    if (pid == 0) {
        return; // ne pas arrêter le processus idle
    }
    process_table[pid].state = PRET;
}

void start_process(process_pid_t pid){
    if (pid < 0 || pid >= NB_PROC) {
        return; // pid invalide
    }
   
    process_table[pid].state = ELU;
}

void scheduler(){
    // process_pid_t current_pid = getpid();
    // process_t *processus_actif = &process_table[current_pid];
    // process_t *elu = NULL;

    // for (int i = 0; i < NB_PROC; i++) {
    //     if (process_table[i].state == PRET) {
    //         elu = &process_table[i];
    //         break;
    //     }
    // }

    // if (elu == NULL) {
    //     elu = &process_table[0];  // idle est toujours pid 0
    // }

    // // printf("processus %s (pid %d) élu\n", elu->nom, elu->pid);
    // // si c'est déjà le même → rien à faire
    // if (elu == processus_actif) {
    //     return;
    // }
    // // transition des états
    // process_t *ancien  = processus_actif;
    // ancien->state      = PRET;
    // elu->state         = ELU;
    index_table_process_pret = (index_table_process_pret + 1) % nbre_process_pret;
    process_pid_t pid = getpid();
    process_t *processus_actif = &process_table[pid];
    process_t *processus_elu = table_process_pret[index_table_process_pret];
    processus_actif->state = PRET;
    processus_elu->state = ELU;
    sti();
    //changement de contexte entre le processus actif et le processus élu
    ctx_sw(processus_actif->regs, processus_elu->regs);
}

// void afficher_table_processus() {
//     printf("PID\tETAT\tNOM\n");
//     for (int i = 0; i < NB_PROC; i++) {
//         if (process_table_bitmap[i / 32] & (1 << (i % 32))) {
//             printf("%d\t%d\t%s\n", process_table[i].pid, process_table[i].state, process_table[i].nom);
//         }
//     }
// }
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