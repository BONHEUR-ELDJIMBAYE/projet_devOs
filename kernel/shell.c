#include <n7OS/shell.h>
#include <n7OS/keyboard.h>
#include <n7OS/processus.h>
#include <string.h>
#include <unistd.h>

extern uint32_t timer_counter;
extern void play_guess_number();

char cmd_buffer[MAX_COMMAND_LENGTH];
char history[10][MAX_COMMAND_LENGTH];//historique des commandes
int history_index = 0;
int index;

void get_input_line(char* buffer) {
    index = 0;
    while (1) {
        char c = kgetch();
        if (c == '\n' || c == KEY_RETURN) {
            buffer[index] = '\0';
            if (index > 0) {
                if (history_index < 10) {
                    // Si l'historique n'est pas plein, on copie à la suite
                    strcpy(history[history_index], buffer);
                    history_index++;
                } else {
                    // Si l'historique est plein, on décale tout vers le haut (on oublie la plus ancienne)
                    for (int i = 1; i < 10; i++) {
                        strcpy(history[i-1], history[i]);
                    }
                    // On place la nouvelle commande à la fin
                    strcpy(history[9], buffer);
                }
            }
            printf("\n");
            break;
        }
        if (c == KEY_BACKSPACE) {
            if (index > 0) {
                index--;
                buffer[index] = '\0';
                printf("\b \b");
            }
            continue;
        }
        buffer[index] = c;
        index++;
        printf("%c", c);
    }
}



void execute_command(char* cmd){
    if (strcmp(cmd, "ps") == 0){
        ps();
    }else if (strcmp(cmd, "clear") == 0){
        clear_screen();
    }else if (strcmp(cmd, "help") == 0){
        printf("Commandes disponibles :\n");
        printf("ps : afficher la liste des processus\n");
        printf("kill <pid> : tuer un processus\n");
        printf("block <pid> : bloquer un processus\n");
        printf("unblock <pid> : debloquer un processus\n");
        printf("interrupt <pid> : interrompre un processus\n");
        printf("start <pid> : demarrer un processus\n");
        printf("clear : effacer l'ecran\n");
        printf("help : afficher l'aide\n");
        printf("play : jouer au jeu du Plus ou Moins\n");
        printf("testproc : activer/desactiver l'affichage des processus de test\n");
        printf("shutdown | off | exit : eteindre le systeme\n");
        printf("history : afficher l'historique des commandes\n");
        printf("history -c : effacer l'historique des commandes\n");
    }else if (strcmp(cmd, "play") == 0){
        int pid = fork("jeu", *play_guess_number);
        if (pid != -1) {
            wait(pid); // Le shell attend simplement la fin du jeu
        } else {
            printf("Erreur: impossible de lancer le jeu.\n");
        }
    }else if (strcmp(cmd, "shutdown") == 0 || strcmp(cmd, "off") == 0 || strcmp(cmd, "exit") == 0){
        shutdown(1);
    }else if (strncmp(cmd, "kill ", 5) == 0) {
        int pid = atoi(cmd + 5);
        terminate_process(pid);
    } else if (strncmp(cmd, "block ", 6) == 0) {
        int pid = atoi(cmd + 6);
        block_process(pid);
    } else if (strncmp(cmd, "unblock ", 8) == 0) {
        int pid = atoi(cmd + 8);
        unblock_process(pid);
    }else if (strncmp(cmd, "interrupt ", 10) == 0) {
        int pid = atoi(cmd + 10);
        interrupt_process(pid);
    }else if (strncmp(cmd, "start ", 6) == 0) {
        int pid = atoi(cmd + 6);
        start_process(pid);

    }else if (strncmp(cmd, "history -c", 10) == 0){
        for (int i = 0; i < history_index; i++) {
            strcpy(history[i], "");
        }
        history_index = 0;
    }else if ((strncmp(cmd, "history", 7) == 0)) {
        for (int i = 0; i < history_index; i++) {
            printf("%d : %s\n", i, history[i]);
        }
    } else if (strcmp(cmd, "testproc") == 0) {
        display_process_test = !display_process_test;
        if (display_process_test) {
            printf("Affichage des processus active. Tapez 'testproc' pour arreter.\n");
        } else {
            printf("Affichage des processus arrete.\n");
        }
    }
    else{
        printf("Commande inconnue\n");
    }
    
}

void shell_run(){
    while(true){
        printf("bonheur-eldjimbaye@devOS> ");
        get_input_line(cmd_buffer);
        execute_command(cmd_buffer);
    }
}
    
