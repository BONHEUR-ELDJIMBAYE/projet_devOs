/** @file processus.h
 *  @brief Définition des structures et fonctions pour la gestion des processus
 */
#ifndef _PROCESSUS_H
#define _PROCESSUS_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <n7OS/mem.h>


#define NB_PROC 255 // nombre maximum de processus
#define STACK_SIZE PAGE_SIZE // taille de la pile pour chaque processus   

typedef enum{ELU, PRET, BLOQUE} PROCESS_STATE; // état d'un processus

typedef uint32_t process_pid_t;
typedef struct {
    process_pid_t pid; // identifiant du processus
    PROCESS_STATE state; // état du processus
    char* nom; // nom du processus
    uint32_t priorite; // priorité du processus
    uint32_t *stack; // pile d'exécution du processus
    uint32_t regs[5]; // registres du processus ([0]=ebx [1]=esp [2]=ebp [3]=esi [4]=edi)
} process_t;



typedef void* (*fnptr)();

/** @brief Crée un nouveau processus
 *  @param name nom du processus
 *  @param function fonction à exécuter
 *  @return identifiant du processus créé
 */
process_pid_t fork(const char *name, fnptr function);

/**
 * @brief Trouve un identifiant de processus libre
 * @return identifiant de processus libre, ou -1 si aucun n'est disponible
 */
process_pid_t find_free_pid();

/**
 * @brief Libère un identifiant de processus
 * @param pid identifiant de processus à libérer
 */
void clear_pid(process_pid_t pid);

/** @brief Termine le processus courant
 *  @return 0 en cas de succès, -1 en cas d'erreur
 */
int exit();

/** @brief Récupère l'identifiant du processus courant
 *  @return identifiant du processus courant
 */
process_pid_t getpid();

/** @brief Attend la fin d'un processus
 *  @param pid identifiant du processus à attendre
 *  @return 0 en cas de succès, -1 en cas d'erreur
 */
int wait(process_pid_t pid);

/** @brief Met le processus courant en sommeil pendant un certain nombre de secondes
 *  @param seconds nombre de secondes à dormir
 *  @return 0 en cas de succès, -1 en cas d'erreur
 */
int sleep(int seconds);

/**
 * @brief Termine un processus donné
 * @param pid identifiant du processus à terminer
 */
void terminate_process(process_pid_t pid);

/** @brief Arrête un processus donné
 * @param pid identifiant du processus à arrêter
 */
void block_process(process_pid_t pid);

/** @brief Arrête un processus donné
 * @param pid identifiant du processus à arrêter
 */
void stop_process(process_pid_t pid);

/** @brief Débloque un processus donné
 * @param pid identifiant du processus à débloquer
 */
void unblock_process(process_pid_t pid);

/** @brief Démarre un processus donné
 * @param pid identifiant du processus à démarquer
 */
void start_process(process_pid_t pid);
/** @brief Planifie l'exécution des processus
 *  @return 0 en cas de succès, -1 en cas d'erreur
 */
void scheduler();

/** @brief Initialise la gestion des processus
 *  @return 0 en cas de succès, -1 en cas d'erreur
 */
void init_processus();

#endif // _PROCESSUS_H