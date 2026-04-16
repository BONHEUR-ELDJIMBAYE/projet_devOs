/** @file processus.h
 *  @brief Définition des structures et fonctions pour la gestion des processus
 */
#ifndef _PROCESSUS_H
#define _PROCESSUS_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#define NB_PROC 10 // nombre maximum de processus
#define STACK_SIZE 1024 // taille de la pile pour chaque processus   

typedef enum{ELU, PRET_ACTIF, PRET_SUSPENDU, BLOQUE_ACTIF, BLOQUE_SUSPENDU} PROCESS_STATE; // état d'un processus

typedef uint32_t pid_t;
typedef struct {
    pid_t pid; // identifiant du processus
    PROCESS_STATE state; // état du processus
    char* nom; // nom du processus
    uint32_t priorite; // priorité du processus
    uint32_t stack[STACK_SIZE]; // pile d'exécution du processus
    uint32_t context_execution; // contexte d'exécution du processus (registre de base de pile)
} process_t;

typedef void* (*fnptr)();

/** @brief Crée un nouveau processus
 *  @param name nom du processus
 *  @param function fonction à exécuter
 *  @return identifiant du processus créé
 */
pid_t fork(const char *name, fnptr function);

/** @brief Termine le processus courant
 *  @return 0 en cas de succès, -1 en cas d'erreur
 */
int exit();

/** @brief Récupère l'identifiant du processus courant
 *  @return identifiant du processus courant
 */
pid_t getpid();

/** @brief Attend la fin d'un processus
 *  @param pid identifiant du processus à attendre
 *  @return 0 en cas de succès, -1 en cas d'erreur
 */
int wait(pid_t pid);

/** @brief Met le processus courant en sommeil pendant un certain nombre de secondes
 *  @param seconds nombre de secondes à dormir
 *  @return 0 en cas de succès, -1 en cas d'erreur
 */
int sleep(int seconds);

/** @brief Met le processus courant en attente d'un événement
 *  @return 0 en cas de succès, -1 en cas d'erreur
 */
int wait(pid_t pid);

/** @brief Planifie l'exécution des processus
 *  @return 0 en cas de succès, -1 en cas d'erreur
 */
void scheduler();

/** @brief Initialise la gestion des processus
 *  @return 0 en cas de succès, -1 en cas d'erreur
 */
void init_processus();

#endif // _PROCESSUS_H