#include <n7OS/shell.h>
#include <n7OS/keyboard.h>
#include <n7OS/processus.h>
#include <string.h>

extern uint32_t timer_counter;

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

void play_guess_number() {
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
        
        guess = atoi(guess_buffer);

        if (guess == 0) {
            printf("Tu abandonnes ? Dommage...\n");
            return;
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
        printf("shutdown | off | exit : eteindre le systeme\n");
        printf("history : afficher l'historique des commandes\n");
        printf("history -c : effacer l'historique des commandes\n");
    }else if (strcmp(cmd, "play") == 0){
        play_guess_number();
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
    
