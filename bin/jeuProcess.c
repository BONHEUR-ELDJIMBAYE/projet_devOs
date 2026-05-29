/** le procecuss qui fait tourner le jeu de devinette */

#include <n7OS/cpu.h>
#include <stdio.h>
#include <n7OS/processus.h>
#include <n7OS/timer.h>
#include <stdlib.h>
#include <n7OS/shell.h>

extern uint32_t timer_counter;
void play_guess_number() {
    // pour génerer le nombre "aleatoire" entre 1 et 100    
    // utilisation de timer_counter qui est incrementé à chaque interruption du timer
    int secret = (timer_counter % 100) + 1;
    int guess = -1;
    char guess_buffer[50];
    int tentatives = 0;

    printf("\n=== BIENVENUE AU JEU DU PLUS OU MOINS ===\n");
    printf("J'ai choisi un nombre entre 1 et 100.\n");
    printf("Tape 0 pour quitter.\n\n");

    while (guess != secret) {
        printf("Ton essai > ");
        
        get_input_line(guess_buffer); 
        
        // vérifier que l'entrée ne contient que des chiffres
        int est_valide = 1;
        if (guess_buffer[0] == '\0') {
            est_valide = 0;
        }
        for (int i = 0; guess_buffer[i] != '\0'; i++) {
            if (guess_buffer[i] < '0' || guess_buffer[i] > '9') {
                est_valide = 0;
                break;
            }
        }

        if (!est_valide) {
            printf("Entree invalide : veuillez entrer uniquement des chiffres !\n");
            continue;
        }
        
        guess = atoi(guess_buffer);

        if (guess == 0) {
            printf("Tu abandonnes ? Dommage...\n");
            terminate_process(getpid());//on termine alors le jeu
        }

        tentatives++;

        if (guess < secret) {
            printf("C'est PLUS !\n");
        } else if (guess > secret) {
            printf("C'est MOINS !\n");
        } else {
            printf("\nGagne ! Tu as trouve en %d tentatives !\n\n", tentatives);
        }
    }
    
    // Fin du jeu : on termine ce processus
    terminate_process(getpid());
}