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
    process_pid_t parent; // identifiant du processus parent
    char* nom; // nom du processus
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
 * @brief Récupère l'identifiant du processus parent
 * @return identifiant du processus parent
 */
process_pid_t getppid();

/**
 * @brief Libère un identifiant de processus
 * @param pid identifiant de processus à libérer
 */
void clear_pid(process_pid_t pid);

/**
 * @brief Vérifie si un processus existe
 * @param pid identifiant du processus à vérifier
 * @return true si le processus existe, false sinon
 */
bool is_Process(process_pid_t pid);


/**
 * @brief Vérifie si un processus est élu
 * @param pid identifiant du processus à vérifier
 * @return true si le processus est élu, false sinon
 */
bool is_process_ELU(process_pid_t pid);

/**
 * @brief Vérifie si un processus est bloqué
 * @param pid identifiant du processus à vérifier
 * @return true si le processus est bloqué, false sinon
 */
bool is_process_BLOQUE(process_pid_t pid);

/**
 * @brief Vérifie si un processus est prêt
 * @param pid identifiant du processus à vérifier
 * @return true si le processus est prêt, false sinon
 */
bool is_process_PRET(process_pid_t pid);

/** @brief Termine le processus courant
 *  @return 0 en cas de succès, -1 en cas d'erreur
 */
int exit();

/** @brief Récupère l'identifiant du processus courant
 *  @return identifiant du processus courant
 */
process_pid_t getpid();


/**
 * @brief Termine un processus donné
 * @param pid identifiant du processus à terminer
 */
void terminate_process(process_pid_t pid);

/** @brief bloquer un processus donné
 * le processus bloqué sera retirer de la table des processus prêts
 * @param pid identifiant du processus à bloquer
 */
void block_process(process_pid_t pid);

/** @brief suspendre un processus en cours d'execution
 * @param pid identifiant du processus à arrêter
 */
void interrupt_process(process_pid_t pid);

/** @brief Débloque un processus donné
 * le processus débloqué sera ajouter à la table des processus prêts
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


/**
 * @brief Affiche la table des processus
 */
void ps();

#endif // _PROCESSUS_H